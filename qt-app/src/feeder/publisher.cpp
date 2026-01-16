/**
 * @file publisher.cpp
 * @brief KUKSA VAL v2 publisher implementation
 */

#include "publisher.hpp"
#include <iostream>

namespace kuksa {

Publisher::Publisher(const std::string& address) {
    channel_ = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    stub_ = val::v2::VAL::NewStub(channel_);
    std::cout << "[Publisher] Connected to KUKSA databroker at " << address << std::endl;
}

Publisher::~Publisher() = default;

bool Publisher::publishDouble(const std::string& path, double value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_double_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value 
                  << ") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::publishFloat(const std::string& path, float value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_float_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value 
                  << ") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::publishInt32(const std::string& path, int32_t value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_int32(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value 
                  << ") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::publishUint32(const std::string& path, uint32_t value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_uint32(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value 
                  << ") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::publishBool(const std::string& path, bool value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_bool_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << (value ? "true" : "false")
                  << ") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::publishString(const std::string& path, const std::string& value) {
    grpc::ClientContext context;
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_string(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", \"" << value 
                  << "\") failed: " << status.error_message() << std::endl;
        return false;
    }
    
    return true;
}

} // namespace kuksa
