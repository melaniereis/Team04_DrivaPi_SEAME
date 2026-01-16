/**
 * @file handlers.hpp
 * @brief CAN frame handlers for converting CAN data to VSS signals
 */

#pragma once

#include <linux/can.h>

namespace kuksa {
class Publisher;
}

namespace handlers {

/**
 * @brief Handle speed CAN frame (ID 0x100)
 * 
 * Payload: 4-byte float (little-endian) representing speed in m/s
 * Publishes to: Vehicle.Speed
 * 
 * @param frame The CAN frame
 * @param publisher KUKSA publisher instance
 */
void handleSpeed(const can_frame& frame, kuksa::Publisher& publisher);

// Additional handlers can be added here:
// void handleTemperature(const can_frame& frame, kuksa::Publisher& publisher);
// void handleBattery(const can_frame& frame, kuksa::Publisher& publisher);

} // namespace handlers
