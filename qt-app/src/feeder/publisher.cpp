/**
 * @file publisher.cpp
 * @brief KUKSA VAL v2 publisher implementation
 */

#include "publisher.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <google/protobuf/timestamp.pb.h>

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
        const std::string root = LoadFile(opts_.root_ca_path);
        if (!root.empty()) ssl_opts.pem_root_certs = root;
        const std::string cert = LoadFile(opts_.client_cert_path);
        const std::string key  = LoadFile(opts_.client_key_path);
        if (!cert.empty() && !key.empty()) {
            ssl_opts.pem_cert_chain = cert;
            ssl_opts.pem_private_key = key;
        }
        creds = grpc::SslCredentials(ssl_opts);
    }

    channel_ = grpc::CreateChannel(opts_.address, creds);
    stub_ = val::v2::VAL::NewStub(channel_);
    // Verify channel connectivity (blocks briefly)
    if (!channel_->WaitForConnected(std::chrono::system_clock::now() + std::chrono::seconds(2))) {
        std::cerr << "[Publisher] Warning: Channel not connected to " << opts_.address 
                  << " (broker may be unreachable)" << std::endl;
    }
    std::cout << "[Publisher] Connected to KUKSA databroker at " << opts_.address
              << (opts_.use_ssl ? " (TLS)" : " (insecure)") << std::endl;
    if (!opts_.token.empty()) {
        std::cout << "[Publisher] Using Authorization token" << std::endl;
    }
}

Publisher::~Publisher() {
    // Signal reader thread to stop and close provider stream cleanly
    stream_stop_.store(true);
    {
        std::lock_guard<std::mutex> lg(stream_mutex_);
        if (stream_) {
            stream_->WritesDone();
        }
        if (stream_ctx_) {
            stream_ctx_->TryCancel();
        }
    }
    if (stream_reader_thread_.joinable()) {
        stream_reader_thread_.join();
    }
}

bool Publisher::PublishDouble(const std::string& path, double value) {
    grpc::ClientContext context;
    AttachAuth(context);
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_double_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value
                  << ") failed: code=" << status.error_code()
                  << " msg='" << status.error_message() << "'" << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::PublishFloat(const std::string& path, float value) {
    // Try provider-stream publishing first
    int32_t sigid = LookupSignalId(path);
    if (sigid >= 0) {
        if (EnsureProviderStream()) {
            std::shared_ptr<grpc::ClientReaderWriter<kuksa::val::v2::OpenProviderStreamRequest,
                                                     kuksa::val::v2::OpenProviderStreamResponse>> local_stream;
            {
                std::lock_guard<std::mutex> lg(stream_mutex_);
                local_stream = stream_;
            }

            if (local_stream) {
                    stream_stop_.store(false);
                    std::cerr << "[Publisher] Provider stream opened" << std::endl;
                // Send ProvideSignalRequest once per signal
                bool need_provide = false;
                {
                    std::lock_guard<std::mutex> lg(stream_mutex_);
                    if (provided_signals_.find(sigid) == provided_signals_.end()) {
                        need_provide = true;
                    }
                }
                if (need_provide) {
                    kuksa::val::v2::OpenProviderStreamRequest provideReq;
                    auto* psr = provideReq.mutable_provide_signal_request();
                    auto& mapRef = *psr->mutable_signals_sample_intervals();
                    kuksa::val::v2::SampleInterval si;
                    si.set_interval_ms(0);
                    (*psr->mutable_signals_sample_intervals())[sigid] = si;

                    try {
                        if (!local_stream->Write(provideReq)) {
                            std::cerr << "[Publisher] Failed to send ProvideSignalRequest for id=" << sigid << std::endl;
                        } else {
                            std::lock_guard<std::mutex> lg(stream_mutex_);
                            provided_signals_.insert(sigid);
                                std::cerr << "[Publisher] Sent ProvideSignalRequest for id=" << sigid << std::endl;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "[Publisher] Exception writing ProvideSignalRequest: " << e.what() << std::endl;
                    } catch (...) {
                        std::cerr << "[Publisher] Unknown exception writing ProvideSignalRequest" << std::endl;
                    }
                }

                // Build PublishValuesRequest
                kuksa::val::v2::OpenProviderStreamRequest req;
                auto* pvr = req.mutable_publish_values_request();
                pvr->set_request_id(next_request_id_++);

                kuksa::val::v2::Datapoint dp;
                auto now = std::chrono::system_clock::now();
                auto secs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
                auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() - secs * 1000000000LL;
                google::protobuf::Timestamp* ts = dp.mutable_timestamp();
                ts->set_seconds(secs);
                ts->set_nanos(static_cast<int>(nanos));
                dp.mutable_value()->set_float_(value);

                (*pvr->mutable_data_points())[sigid] = dp;

                try {
                    if (local_stream->Write(req)) {
                            std::cerr << "[Publisher] Sent PublishValuesRequest for id=" << sigid << std::endl;
                        return true;
                    } else {
                        std::cerr << "[Publisher] Provider stream write failed for " << path << std::endl;
                        // fallthrough to unary fallback
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[Publisher] Exception writing PublishValuesRequest: " << e.what() << std::endl;
                } catch (...) {
                    std::cerr << "[Publisher] Unknown exception writing PublishValuesRequest" << std::endl;
                }
            }
        }
    }

    // Fallback: unary PublishValue
    grpc::ClientContext context;
    AttachAuth(context);
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_float_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value
                  << ") failed: code=" << status.error_code()
                  << " msg='" << status.error_message() << "'" << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::PublishInt32(const std::string& path, int32_t value) {
    grpc::ClientContext context;
    AttachAuth(context);
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_int32(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << value
                  << ") failed: code=" << status.error_code()
                  << " msg='" << status.error_message() << "'" << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::PublishUint32(const std::string& path, uint32_t value) {
    grpc::ClientContext context;
    AttachAuth(context);
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

bool Publisher::PublishBool(const std::string& path, bool value) {
    grpc::ClientContext context;
    AttachAuth(context);
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_bool_(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", " << (value ? "true" : "false")
                  << ") failed: code=" << status.error_code()
                  << " msg='" << status.error_message() << "'" << std::endl;
        return false;
    }
    
    return true;
}

bool Publisher::PublishString(const std::string& path, const std::string& value) {
    grpc::ClientContext context;
    AttachAuth(context);
    val::v2::PublishValueRequest request;
    val::v2::PublishValueResponse response;

    request.mutable_signal_id()->set_path(path);
    request.mutable_data_point()->mutable_value()->set_string(value);

    grpc::Status status = stub_->PublishValue(&context, request, &response);
    
    if (!status.ok()) {
        std::cerr << "[Publisher] PublishValue(" << path << ", \"" << value
                  << "\") failed: code=" << status.error_code()
                  << " msg='" << status.error_message() << "'" << std::endl;
        return false;
    }
    
    return true;
}

// Ensure a provider stream is open and start a reader thread to consume responses
bool Publisher::EnsureProviderStream() {
    std::lock_guard<std::mutex> lg(stream_mutex_);
    if (stream_) return true;

    stream_ctx_ = std::make_unique<grpc::ClientContext>();
    AttachAuth(*stream_ctx_);
    // Open stream as unique_ptr then convert to shared_ptr to allow safe local copies
    auto raw_uptr = stub_->OpenProviderStream(stream_ctx_.get());
    if (!raw_uptr) {
        std::cerr << "[Publisher] Failed to open provider stream" << std::endl;
        stream_ctx_.reset();
        return false;
    }
    stream_ = std::shared_ptr<grpc::ClientReaderWriter<kuksa::val::v2::OpenProviderStreamRequest,
                                                      kuksa::val::v2::OpenProviderStreamResponse>>(raw_uptr.release(), [](auto*p){ delete p; });

    // No reader thread: we perform writes only and rely on permissive server behavior
    stream_stop_.store(false);

    return true;
}

int32_t Publisher::LookupSignalId(const std::string& path) {
    // Cache lookup
    auto it = signal_id_cache_.find(path);
    if (it != signal_id_cache_.end()) return it->second;

    // Try ListMetadata with the full path first
    grpc::ClientContext ctx;
    AttachAuth(ctx);
    val::v2::ListMetadataRequest req;
    req.set_root(path);
    val::v2::ListMetadataResponse resp;
    grpc::Status st = stub_->ListMetadata(&ctx, req, &resp);
    if (st.ok()) {
        for (const auto& md : resp.metadata()) {
            if (md.path() == path) {
                signal_id_cache_[path] = md.id();
                return md.id();
            }
        }
        // If exact match not found, try to find an entry that equals the path
        for (const auto& md : resp.metadata()) {
            if (md.path().size() >= path.size() && md.path().find(path) != std::string::npos) {
                signal_id_cache_[path] = md.id();
                return md.id();
            }
        }
    }

    // Try parent branch search
    auto pos = path.rfind('.');
    if (pos != std::string::npos) {
        std::string parent = path.substr(0, pos);
        grpc::ClientContext ctx2;
        AttachAuth(ctx2);
        val::v2::ListMetadataRequest req2;
        req2.set_root(parent);
        val::v2::ListMetadataResponse resp2;
        grpc::Status st2 = stub_->ListMetadata(&ctx2, req2, &resp2);
        if (st2.ok()) {
            for (const auto& md : resp2.metadata()) {
                if (md.path() == path) {
                    signal_id_cache_[path] = md.id();
                    return md.id();
                }
            }
        }
    }

    // Not found
    return -1;
}

void Publisher::AttachAuth(grpc::ClientContext& ctx) {
    if (!opts_.token.empty()) {
        ctx.AddMetadata("authorization", std::string("Bearer ") + opts_.token);
    }
}

std::string Publisher::LoadFile(const std::string& path) {
    if (path.empty()) return {};
    
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (!ifs) return {};
    
    // Check file size to prevent excessive memory consumption
    ifs.seekg(0, std::ios::end);
    std::streamsize file_size = ifs.tellg();
    const std::streamsize MAX_FILE_SIZE = 10 * 1024 * 1024;  // 10 MB limit for certificates
    
    if (file_size > MAX_FILE_SIZE) {
        std::cerr << "[Publisher] File '" << path << "' exceeds maximum allowed size (" 
                  << file_size << " > " << MAX_FILE_SIZE << " bytes)" << std::endl;
        return {};
    }
    
    ifs.seekg(0, std::ios::beg);
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

} // namespace kuksa
