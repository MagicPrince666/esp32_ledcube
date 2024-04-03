#include "contrl.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

static int hc595_dat     = -1;
static int hc595_st      = -1;
static int hc595_sh      = -1;
static int gpio_layer[8] = {-1};

#define PIN_NUM_MISO 22
#define PIN_NUM_MOSI 18
#define PIN_NUM_CLK 21
#define PIN_NUM_CS 19

#define PIN_FLOW1 25
#define PIN_FLOW2 26
#define PIN_FLOW3 27
#define PIN_FLOW4 2
#define PIN_FLOW5 4
#define PIN_FLOW6 16
#define PIN_FLOW7 17
#define PIN_FLOW8 5

int init_gpio(void)
{
    hc595_dat = PIN_NUM_MOSI;
    hc595_sh  = PIN_NUM_CLK;
    hc595_st  = PIN_NUM_CS;

    gpio_layer[0] = PIN_FLOW1;
    gpio_layer[1] = PIN_FLOW2;
    gpio_layer[2] = PIN_FLOW3;
    gpio_layer[3] = PIN_FLOW4;
    gpio_layer[4] = PIN_FLOW5;
    gpio_layer[5] = PIN_FLOW6;
    gpio_layer[6] = PIN_FLOW7;
    gpio_layer[7] = PIN_FLOW8;

    // Initialize GPIOs
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask =
        ((1ULL << PIN_FLOW1) | (1ULL << PIN_FLOW2) | (1ULL << PIN_FLOW3) | (1ULL << PIN_FLOW4) |
         (1ULL << PIN_FLOW5) | (1ULL << PIN_FLOW6) | (1ULL << PIN_FLOW7) | (1ULL << PIN_FLOW8) |
         (1ULL << PIN_NUM_MOSI) | (1ULL << PIN_NUM_CLK) | (1ULL << PIN_NUM_CS));
    io_conf.mode       = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = true;

    gpio_config(&io_conf);

    // esp_err_t ret;
    // spi_device_handle_t spi;
    // spi_bus_config_t buscfg = {
    //     .miso_io_num = PIN_NUM_MISO,
    //     .mosi_io_num = PIN_NUM_MOSI,
    //     .sclk_io_num = PIN_NUM_CLK,
    //     .quadwp_io_num = -1,
    //     .quadhd_io_num = -1,
    //     .max_transfer_sz = 64
    // };
    // spi_device_interface_config_t devcfg = {
    //     .clock_speed_hz = 10 * 1000 * 1000,     //Clock out at 10 MHz
    //     .mode = 0,                              //SPI mode 0
    //     .spics_io_num = PIN_NUM_CS,             //CS pin
    //     .queue_size = 7,                        //We want to be able to queue 7 transactions at a time
    //     // .pre_cb = lcd_spi_pre_transfer_callback, //Specify pre-transfer callback to handle D/C line
    // };
    // //Initialize the SPI bus
    // ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    // ESP_ERROR_CHECK(ret);
    // //Attach the LCD to the SPI bus
    // ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi);
    // ESP_ERROR_CHECK(ret);

    return 0;
}

void hc595_write(uint8_t dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        if ((dat << i) & 0x80) {
            gpio_set_level(hc595_dat, 0);
        } else {
            gpio_set_level(hc595_dat, 1);
        }

        gpio_set_level(hc595_sh, 1);
        gpio_set_level(hc595_sh, 0);
    }
}

void hc595out()
{
    gpio_set_level(hc595_st, 1);
    gpio_set_level(hc595_st, 0);
}

void cen_on(u_int8_t y)
{
    switch (y) {
    case 0x00: {
        gpio_set_level(gpio_layer[7], 1);
        gpio_set_level(gpio_layer[0], 0);
    } break;
    case 0x01: {
        gpio_set_level(gpio_layer[0], 1);
        gpio_set_level(gpio_layer[1], 0);
    } break;
    case 0x02: {
        gpio_set_level(gpio_layer[1], 1);
        gpio_set_level(gpio_layer[2], 0);
    } break;
    case 0x03: {
        gpio_set_level(gpio_layer[2], 1);
        gpio_set_level(gpio_layer[3], 0);
    } break;
    case 0x04: {
        gpio_set_level(gpio_layer[3], 1);
        gpio_set_level(gpio_layer[4], 0);
    } break;
    case 0x05: {
        gpio_set_level(gpio_layer[4], 1);
        gpio_set_level(gpio_layer[5], 0);
    } break;
    case 0x06: {
        gpio_set_level(gpio_layer[5], 1);
        gpio_set_level(gpio_layer[6], 0);
    } break;
    case 0x07: {
        gpio_set_level(gpio_layer[6], 1);
        gpio_set_level(gpio_layer[7], 0);
    } break;
    case 0x08: {
        for (int i = 0; i < 8; i++) {
            gpio_set_level(gpio_layer[i], 1);
        }
    } break;
    default:
        break;
    }
}

int cube_close(void)
{
    cen_on(8);
    return 0;
}
