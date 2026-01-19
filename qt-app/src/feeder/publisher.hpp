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
struct PublisherOptions {
    std::string address = "localhost:55555";
    bool use_ssl = false;                 // false = insecure
    std::string root_ca_path;             // optional
    std::string client_cert_path;         // optional (for mTLS)
    std::string client_key_path;          // optional (for mTLS)
    std::string token;                    // optional JWT token ("Bearer <token>")
};

class Publisher {
public:
    /**
     * @brief Construct a publisher connected to the specified databroker
     * @param address Databroker address (e.g., "localhost:55555")
     */
    explicit Publisher(const std::string& address);

    /**
     * @brief Construct with options (TLS and auth)
     */
    explicit Publisher(const PublisherOptions& opts);
    ~Publisher();

    /**
     * @brief Publish a double value to a VSS path
     * @param path VSS signal path (e.g., "Vehicle.Speed")
     * @param value The value to publish
     * @return true if publish succeeded, false otherwise
     */
    bool PublishDouble(const std::string& path, double value);

    /**
     * @brief Publish a float value to a VSS path
     */
    bool PublishFloat(const std::string& path, float value);

    /**
     * @brief Publish an int32 value to a VSS path
     */
    bool PublishInt32(const std::string& path, int32_t value);

    /**
     * @brief Publish a uint32 value to a VSS path
     */
    bool PublishUint32(const std::string& path, uint32_t value);

    /**
     * @brief Publish a boolean value to a VSS path
     */
    bool PublishBool(const std::string& path, bool value);

    /**
     * @brief Publish a string value to a VSS path
     */
    bool PublishString(const std::string& path, const std::string& value);

private:
    // Attach authorization metadata if token present
    void AttachAuth(grpc::ClientContext& ctx);

    // Load file contents into string (returns empty if path empty or read fails)
    static std::string LoadFile(const std::string& path);

    PublisherOptions opts_;
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<kuksa::val::v2::VAL::Stub> stub_;
};

} // namespace kuksa
