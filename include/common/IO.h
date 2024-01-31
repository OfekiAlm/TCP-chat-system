#pragma once

/**
 * Sets the given socket file descriptor to non-blocking mode.
 *
 * @param sockfd The socket file descriptor to set to non-blocking mode.
 */
void set_non_blocking(int sockfd);