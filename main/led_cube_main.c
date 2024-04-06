/* SPI Master example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "contrl.h"
#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"
#include "esp_private/esp_int_wdt.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "heart.h"
#include "move.h"
#include "mycube.h"
#include "water.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIN_GPIO_KEY 23
#define PIN_INFRARED 39 // ADC1_CH3
#define PIN_BAT_ADC 36  // ADC1_CH0

#define EXAMPLE_ADC_UNIT ADC_UNIT_1
#define _EXAMPLE_ADC_UNIT_STR(unit) #unit
#define EXAMPLE_ADC_UNIT_STR(unit) _EXAMPLE_ADC_UNIT_STR(unit)
#define EXAMPLE_ADC_CONV_MODE ADC_CONV_SINGLE_UNIT_1
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_0
#define EXAMPLE_ADC_BIT_WIDTH SOC_ADC_DIGI_MAX_BITWIDTH

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2
#define EXAMPLE_ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_ADC_GET_CHANNEL(p_data) ((p_data)->type1.channel)
#define EXAMPLE_ADC_GET_DATA(p_data) ((p_data)->type1.data)
#else
#define EXAMPLE_ADC_OUTPUT_TYPE ADC_DIGI_OUTPUT_FORMAT_TYPE2
#define EXAMPLE_ADC_GET_CHANNEL(p_data) ((p_data)->type2.channel)
#define EXAMPLE_ADC_GET_DATA(p_data) ((p_data)->type2.data)
#endif

#define EXAMPLE_READ_LEN 256

static adc_channel_t channel[2] = {ADC_CHANNEL_0, ADC_CHANNEL_3};

static TaskHandle_t s_task_handle;
static const char *TAG = "EXAMPLE";

#define CUBE_MAX_MODE 2
uint8_t g_mode = 0;
bool g_light_switch = false;

static bool IRAM_ATTR s_conv_done_cb(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data)
{
    BaseType_t mustYield = pdFALSE;
    // Notify that ADC continuous driver has done enough number of conversions
    vTaskNotifyGiveFromISR(s_task_handle, &mustYield);

    return (mustYield == pdTRUE);
}

static void continuous_adc_init(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle)
{
    adc_continuous_handle_t handle = NULL;

    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size    = EXAMPLE_READ_LEN,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode      = EXAMPLE_ADC_CONV_MODE,
        .format         = EXAMPLE_ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num                                         = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten     = EXAMPLE_ADC_ATTEN;
        adc_pattern[i].channel   = channel[i] & 0x7;
        adc_pattern[i].unit      = EXAMPLE_ADC_UNIT;
        adc_pattern[i].bit_width = EXAMPLE_ADC_BIT_WIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%" PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%" PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%" PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));

    *out_handle = handle;
}

void _display(u_int16_t time, u_int8_t dat)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(dat);
                }
                hc595out();
                // cen_on(y);
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

void light_on(u_int16_t time)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    if (x == 0 || x == 7) {
                        hc595_write(0xFF);
                    } else {
                        hc595_write(0x81);
                    }
                }
                hc595out();
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

void light_off(u_int16_t time)
{
    unsigned char x, y, z;
    int times = 0;
    for (z = 0; z < 1; z++) {
        while (times < time) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(0x00);
                }
                hc595out();
                usleep(500);
                cen_on(9);
            }
            times++;
        }
    }
}

const unsigned char test[][8][8] = {
    {{0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}, // 8
     {0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00},
     {0x00, 0x00, 0x3C, 0x24, 0x24, 0x3C, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
     {0x00, 0x00, 0x3C, 0x24, 0x24, 0x3C, 0x00, 0x00},
     {0x00, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x00},
     {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF}}};

void general(const unsigned char po[][8][8], unsigned int cnt, int tv)
{
    unsigned int x, y, z;
    int times = tv;
    for (z = 0; z < cnt; z++) {
        while (times--) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(po[z][y][x]);
                }
                hc595out();
                cen_on(y);
                usleep(500);
                cen_on(8);
            }
        }
        times = tv;
        // printf("Z=%d\n",z);
    }
}

void _hourglass(const unsigned char po[][8][8], unsigned int cnt, int tv)
{
    unsigned int x, y, z;
    int times = tv;
    for (z = 0; z < cnt; z++) {
        while (times--) {
            for (y = 0; y < 8; y++) {
                for (x = 0; x < 8; x++) {
                    hc595_write(po[z][7 - y][x]);
                }
                hc595out();
                cen_on(y);
                usleep(500);
                cen_on(8);
            }
        }
        times = tv;
        // printf("Z=%d\n",z);
    }
}

void displayking(int tv)
{
    uint8_t x, y, z, i = 0, j = 0, num;
    int times          = tv;
    uint8_t Cube[8][8] = {0};
    uint8_t cen[8]     = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    for (i = 0; i < 8; i++) {
        if (i < 4)
            z = -8 * i + 28;
        else
            z = 8 * i - 28;

        j = i;
        for (num = 0; num < z; num++) {
            if (i < 4) {
                switch (num / (z / 4)) {
                case 0: {
                    Cube[i][j] |= Cube[i][j] >> 1 | cen[7 - i];
                } break;
                case 1: {
                    Cube[i][j++] |= cen[i];
                } break;
                case 2: {
                    Cube[i][7 - i] |= Cube[i][7 - i] << 1 | cen[i];
                } break;
                case 3: {
                    Cube[i][j] |= Cube[i][j] | cen[7 - i];
                    j--;
                } break;
                }
            } else {
                switch (num / (z / 4)) {
                case 0: {
                    Cube[i][7 - j] |= Cube[i][7 - i] >> 1 | cen[i];
                } break;
                case 1: {
                    Cube[i][7 - j] |= cen[7 - i];
                    j--;
                } break;
                case 2: {
                    Cube[i][i] |= Cube[i][i] << 1 | cen[7 - i];
                } break;
                case 3: {
                    Cube[i][7 - j] |= Cube[i][7 - j] | cen[i];
                    j++;
                } break;
                }
            }

            while (times--) {
                for (y = 0; y < 8; y++) {
                    for (x = 0; x < 8; x++) {
                        hc595_write(Cube[y][x]);
                    }
                    hc595out();
                    cen_on(y);
                    usleep(500);
                    cen_on(8);
                }
            }
            times = tv;
        }
    }
}

static void adc_read_task(void *param)
{
    esp_err_t ret;
    uint32_t ret_num                 = 0;
    uint8_t last_key = 0;
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);

    s_task_handle = xTaskGetCurrentTaskHandle();

    adc_continuous_handle_t handle = NULL;
    continuous_adc_init(channel, sizeof(channel) / sizeof(adc_channel_t), &handle);

    adc_continuous_evt_cbs_t cbs = {
        .on_conv_done = s_conv_done_cb,
    };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));
    uint32_t last_ch0_data = 0;
    uint32_t last_ch3_data = 0;

    while (1) {
        /**
         * This is to show you the way to use the ADC continuous mode driver event callback.
         * This `ulTaskNotifyTake` will block when the data processing in the task is fast.
         * However in this example, the data processing (print) is slow, so you barely block here.
         *
         * Without using this event callback (to notify this task), you can still just call
         * `adc_continuous_read()` here in a loop, with/without a certain block timeout.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        char unit[] = EXAMPLE_ADC_UNIT_STR(EXAMPLE_ADC_UNIT);
        while (1) {
            uint8_t gpio_key = gpio_get_level(PIN_GPIO_KEY);
            if (gpio_key != last_key) {
                last_key = gpio_key;
                if (gpio_key == 0) {
                    g_mode++;
                    if (g_mode > CUBE_MAX_MODE) {
                        g_mode = 0;
                    }
                    ESP_LOGI(TAG, "SHOW MODE %d", g_mode);
                }   
            }
            ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
            if (ret == ESP_OK) {
                // ESP_LOGI("TASK", "ret is %x, ret_num is %" PRIu32 " bytes", ret, ret_num);
                for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
                    adc_digi_output_data_t *p = (adc_digi_output_data_t *)&result[i];
                    uint32_t chan_num         = EXAMPLE_ADC_GET_CHANNEL(p);
                    uint32_t data             = EXAMPLE_ADC_GET_DATA(p);
                    /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                    if (chan_num < SOC_ADC_CHANNEL_NUM(EXAMPLE_ADC_UNIT)) {
                        // ESP_LOGI(TAG, "Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32, unit, chan_num, data);
                        if (chan_num == 0) {
                            if (last_ch0_data != data) {
                                last_ch0_data = data;
                                ESP_LOGI(TAG, "Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32, unit, chan_num, data);
                            }
                        } else if (chan_num == 3) {
                            if (last_ch3_data != data) {
                                last_ch3_data = data;
                                if (last_ch3_data == 0) {
                                    g_light_switch = false;
                                } else {
                                    g_light_switch = true;
                                }
                                ESP_LOGI(TAG, "Unit: %s, Channel: %" PRIu32 ", Value: %" PRIx32, unit, chan_num, data);
                            }
                        }
                    } else {
                        ESP_LOGW(TAG, "Invalid data [%s_%" PRIu32 "_%" PRIx32 "]", unit, chan_num, data);
                    }
                }
                /**
                 * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
                 * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
                 * usually you don't need this delay (as this task will block for a while).
                 */
                vTaskDelay(10);
            } else if (ret == ESP_ERR_TIMEOUT) {
                // We try to read `EXAMPLE_READ_LEN` until API returns timeout, which means there's no available data
                break;
            }
        }
    }

    ESP_ERROR_CHECK(adc_continuous_stop(handle));
    ESP_ERROR_CHECK(adc_continuous_deinit(handle));
}

static void cube_task(void *param)
{
    init_gpio();

    while (1) {
        if (g_mode == 1) {
            // 照明模式
            _display(10, 0xFF);
        } else if (g_mode == 2) {
            // 感应模式
            if (g_light_switch) {
                light_on(10);
            } else {
                light_off(10);
            }
        } else {
            // 动画模式
            // rotating_mycube_(1);
            for(int i = 0; i < 5; i++) {
                blew_heart(30);
                if (g_mode != 0) {
                    continue;
                }
            }
            // mycube(30); // 上善若水
            for (int i = 0; i < 3; i++) {
                cube_water1(18);
                if (g_mode != 0) {
                    continue;
                }
            }
            for (int i = 0; i < 5; i++) {
                rain_cube(20);
                if (g_mode != 0) {
                    continue;
                }
            }
            rotating_mycube_(20);
            if (g_mode != 0) {
                continue;
            }
            for (int i = 0; i < 3; i++) {
                general(warping, 15, 20);
                if (g_mode != 0) {
                    continue;
                }
            }
            general(IVU_1, 21, 20);
            if (g_mode != 0) {
                continue;
            }
            for(int i = 0; i < 3; i++) {
                _sin_cube(sin_cube_table, 14, 20);
                if (g_mode != 0) {
                    continue;
                }
            }
            displayking(20);
            if (g_mode != 0) {
                continue;
            }
            _hourglass( king,8,20);
            if (g_mode != 0) {
                continue;
            }
            general(shandian, 22, 20);
            if (g_mode != 0) {
                continue;
            }
            general(cube, 26, 20);
            if (g_mode != 0) {
                continue;
            }
            general(cube2, 8, 20);
            if (g_mode != 0) {
                continue;
            }
        }
    }
}

void app_main(void)
{
    gpio_set_direction(PIN_GPIO_KEY, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PIN_GPIO_KEY, GPIO_PULLUP_ONLY);
    xTaskCreate(adc_read_task, "adc_read_task", 4 * 1024, NULL, 5, NULL);
    xTaskCreate(cube_task, "cube_task", 4 * 1024, NULL, 5, NULL);
    // 启动调度器
    vTaskStartScheduler();
}
