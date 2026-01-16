/**
 * @file publisher.hpp
 * @brief KUKSA VAL v2 publisher client wrapper
 * 
 * Provides typed publish methods for sending VSS signals to KUKSA databroker
 */

#pragma once

#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "kuksa/val/v2/val.grpc.pb.h"

namespace kuksa {

/**
 * @brief Simple KUKSA VAL v2 publisher client
 */
class Publisher {
public:
    /**
     * @brief Construct a publisher connected to the specified databroker
     * @param address Databroker address (e.g., "localhost:55555")
     */
    explicit Publisher(const std::string& address);
    ~Publisher();

    /**
     * @brief Publish a double value to a VSS path
     * @param path VSS signal path (e.g., "Vehicle.Speed")
     * @param value The value to publish
     * @return true if publish succeeded, false otherwise
     */
    bool publishDouble(const std::string& path, double value);

    /**
     * @brief Publish a float value to a VSS path
     */
    bool publishFloat(const std::string& path, float value);

    /**
     * @brief Publish an int32 value to a VSS path
     */
    bool publishInt32(const std::string& path, int32_t value);

    /**
     * @brief Publish a uint32 value to a VSS path
     */
    bool publishUint32(const std::string& path, uint32_t value);

    /**
     * @brief Publish a boolean value to a VSS path
     */
    bool publishBool(const std::string& path, bool value);

    /**
     * @brief Publish a string value to a VSS path
     */
    bool publishString(const std::string& path, const std::string& value);

private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<kuksa::val::v2::VAL::Stub> stub_;
};

} // namespace kuksa
