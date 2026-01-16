/**
 * @file publisher.cpp
 * @brief KUKSA VAL v2 publisher implementation
 */

#include "publisher.hpp"
#include <iostream>
#include <fstream>

namespace kuksa {

Publisher::Publisher(const std::string& address) {
    channel_ = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    stub_ = val::v2::VAL::NewStub(channel_);
    std::cout << "[Publisher] Connected to KUKSA databroker at " << address << std::endl;
}

Publisher::Publisher(const PublisherOptions& opts)
    : opts_(opts)
{
    std::shared_ptr<grpc::ChannelCredentials> creds;
    if (!opts_.use_ssl) {
        creds = grpc::InsecureChannelCredentials();
    } else {
        grpc::SslCredentialsOptions ssl_opts;
        const std::string root = loadFile(opts_.root_ca_path);
        if (!root.empty()) ssl_opts.pem_root_certs = root;
        const std::string cert = loadFile(opts_.client_cert_path);
        const std::string key  = loadFile(opts_.client_key_path);
        if (!cert.empty() && !key.empty()) {
            ssl_opts.pem_cert_chain = cert;
            ssl_opts.pem_private_key = key;
        }
        creds = grpc::SslCredentials(ssl_opts);
    }

    channel_ = grpc::CreateChannel(opts_.address, creds);
    stub_ = val::v2::VAL::NewStub(channel_);
    std::cout << "[Publisher] Connected to KUKSA databroker at " << opts_.address
              << (opts_.use_ssl ? " (TLS)" : " (insecure)") << std::endl;
    if (!opts_.token.empty()) {
        std::cout << "[Publisher] Using Authorization token" << std::endl;
    }
}

Publisher::~Publisher() = default;

bool Publisher::publishDouble(const std::string& path, double value) {
    grpc::ClientContext context;
    attachAuth(context);
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
    attachAuth(context);
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
    attachAuth(context);
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
    attachAuth(context);
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
    attachAuth(context);
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
    attachAuth(context);
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

void Publisher::attachAuth(grpc::ClientContext& ctx) {
    if (!opts_.token.empty()) {
        ctx.AddMetadata("authorization", std::string("Bearer ") + opts_.token);
    }
}

std::string Publisher::loadFile(const std::string& path) {
    if (path.empty()) return {};
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs) return {};
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

} // namespace kuksa
