set(srcs
    "led_cube_main.c"
    "contrl.c"
    "heart.c"
    "move.c"
    "mycube.c"
    "water.c"
)

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
