#pragma once
#include <stdio.h>
#include <stdlib.h>

#define COLOR_END "\033[m"
#define COLOR_BOLD  "\e[1m"
#define COLOR_UNBOLD "\e[1m"

typedef enum color_t {
    RED,
    YELLOW,
    CYAN,
    PURPLE,
    GREEN,
    WHITE
} Color;

/**
 * @brief Retrieves the color code based on the enum value.
 *
 * This function takes an integer representing the enum value and returns
 * the corresponding color code as a string.
 *
 * @param color The index of the color.
 * @return The color code as a string.
 */
char* getColorCode(int color);