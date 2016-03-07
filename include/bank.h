#pragma once

#include <3ds/types.h>

/**
 * @brief Update a bankbuffer to the latest version.
 * @param[in/out] bank The bankbuffer.
 * @param bytesRead The total of read bytes.
 * @return Whether the bankbuffer is the final version.
 */
bool bankUpdate(uint8_t* bank, uint16_t bytesRead);
