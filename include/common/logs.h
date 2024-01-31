#pragma once
#include <stdio.h>
#include "colors.h"

/**
 * @brief The log levels.
 *
 * The log levels are used to specify the severity of a log message.
 * The higher the level, the more severe the message is.
 */
typedef enum log_level_t
{
    DEFAULT,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
} LogLevel;

/**
 * @brief Prints a log message with the specified log level.
 *
 * @param level The log level of the message.
 * @param message The message to be printed.
 */
void print_log(LogLevel level, const char *message);

/**
 * @brief Prints a log message with a specified color.
 *
 * This function is used to print log messages with different colors.
 *
 * @param level The log level of the message.
 * @param message The log message to be printed.
 * @param color The color code to be used for printing the log message.
 */
void print_log_by_color(LogLevel level, const char *message, int color);

/**
 * @brief Prints a log message with parameters.
 *
 * This function prints a log message with the specified log level and message.
 * It supports one string variable argument.
 *
 * @param level The log level of the message.
 * @param message The log message to be printed.
 * @param params Additional parameters to be included in the log message.
 */
void print_log_with_params(LogLevel level, const char *message, char *params);

/**
 * @brief Prints a log message with parameters in a specified color.
 *
 * This function prints a log message with additional parameters in a specified color.
 *
 * @param level The log level of the message.
 * @param message The log message to be printed.
 * @param params Additional parameters to be included in the log message.
 * @param color The color in which the log message should be printed.
 */
void print_log_with_params_by_color(LogLevel level, const char *message, char *params, int color);


/**
 * @brief Prints a log message with bold formatting and colored parameters.
 *
 * This function is used to print log messages with bold formatting and colored parameters.
 * The log level, message, parameters, and color are specified as arguments.
 *
 * @param level The log level of the message.
 * @param message The log message to be printed.
 * @param params The parameters to be included in the log message.
 * @param color The color of the parameters in the log message.
 */
void print_bold_log_with_params_by_color(LogLevel level, const char *message, char *params, int color);

/**
 * @brief Prints a message with a specified color.
 *
 * This function prints the given message with the specified color.
 *
 * @param message The message to be printed.
 * @param color The color code to be used for printing.
 */
void print_by_color(const char *message, int color);

/**
 * @brief Prints a message in bold using a specified color.
 *
 * This function prints the given message in bold using the specified color.
 *
 * @param message The message to be printed.
 * @param color The color to be used for printing.
 */
void print_bold_by_color(const char *message, int color);

/**
 * @brief Prints a log message with a specified color.
 *
 * This function prints a log message with a specified color.
 *
 * @param level The log level of the message.
 * @param message The log message to be printed.
 * @param color The color to be used for printing.
 */
void print_bold_log_by_color(LogLevel level, const char *message, int color);
