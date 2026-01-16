/**
 * @file handlers.cpp
 * @brief CAN frame handler implementations
 */

#include "handlers.hpp"
#include "publisher.hpp"
#include "can_decode.hpp"
#include "signals.hpp"
#include <iostream>

namespace handlers {

void handleSpeed(const can_frame& frame, kuksa::Publisher& publisher) {
    // Validate payload size (expect 4 bytes for float)
    if (frame.can_dlc < 4) {
        std::cerr << "[Handler] Speed frame too short: " << static_cast<int>(frame.can_dlc) 
                  << " bytes" << std::endl;
        return;
    }

    // Decode float from CAN payload (little-endian)
    float speed_mps = can_decode::float_le(frame.data);

    // Publish to KUKSA
    if (publisher.publishFloat(vss::VEHICLE_SPEED, speed_mps)) {
        std::cout << "[Handler] Published Vehicle.Speed = " << speed_mps << " m/s" << std::endl;
    }
}

} // namespace handlers
