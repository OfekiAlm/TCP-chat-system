#include <stdio.h>
#include "logs.h"

void print_log(LogLevel level, const char *message)
{
    switch (level)
    {
    case LOG_ERROR:
        printf("[E] %s\n", message);
        break;
    case LOG_WARN:
        printf("[W] %s\n", message);
        break;
    case LOG_INFO:
        printf("[I] %s\n", message);
        break;
    case LOG_DEBUG:
        printf("[D] %s\n", message);
        break;
    case DEFAULT:
        printf("%s\n", message);
        break;
    }
}

void print_log_with_params(LogLevel level, const char *message, char *params)
{

    switch (level)
    {
    case LOG_ERROR:
        printf("[E] ");
        break;
    case LOG_WARN:
        printf("[W] ");
        break;
    case LOG_INFO:
        printf("[I] ");
        break;
    case LOG_DEBUG:
        printf("[D] ");
        break;
    case DEFAULT:
        break;
    }
    printf(message, params);
    printf("\n");
}

void print_log_with_params_by_color(LogLevel level, const char *message, char *params, int color)
{
    printf("%s", getColorCode(color));
    print_log_with_params(level, message, params);
    printf(COLOR_END);
}

void print_bold_log_with_params_by_color(LogLevel level, const char *message, char *params, int color)
{
    printf("%s", getColorCode(color));
    printf(COLOR_BOLD);
    print_log_with_params(level, message, params);
    printf(COLOR_UNBOLD);
    printf(COLOR_END);
}

void print_log_by_color(LogLevel level, const char *message, int color)
{
    printf("%s", getColorCode(color));
    print_log(level, message);
    printf(COLOR_END);
}

void print_bold_log_by_color(LogLevel level, const char *message, int color)
{
    printf(COLOR_BOLD);
    print_log_by_color(level, message, color);
    printf(COLOR_UNBOLD);
    printf(COLOR_END);
}

void print_by_color(const char *message, int color)
{
    printf("%s", getColorCode(color));
    printf("%s", message);
    printf(COLOR_END);
}

void print_bold_by_color(const char *message, int color)
{
    printf("%s", getColorCode(color));
    printf(COLOR_BOLD);
    printf("%s", message);
    printf(COLOR_UNBOLD);
    printf(COLOR_END);
}