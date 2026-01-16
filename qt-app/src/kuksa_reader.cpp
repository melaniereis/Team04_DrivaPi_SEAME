#include "kuksa_reader.hpp"
#include <fstream>

KUKSAReader::KUKSAReader(QObject *parent)
    : QObject(parent)
{}

KUKSAReader::KUKSAReader(const KuksaOptions& opts, QObject *parent)
    : QObject(parent), m_opts_(opts)
{}

KUKSAReader::~KUKSAReader()
{}

void KUKSAReader::start()
{
    // 1. Connect to Broker (TLS or insecure)
    std::shared_ptr<grpc::ChannelCredentials> creds;
    if (!m_opts_.use_ssl) {
        creds = grpc::InsecureChannelCredentials();
    } else {
        grpc::SslCredentialsOptions ssl_opts;
        const std::string root = loadFile(m_opts_.root_ca_path);
        if (!root.empty()) ssl_opts.pem_root_certs = root;
        const std::string cert = loadFile(m_opts_.client_cert_path);
        const std::string key  = loadFile(m_opts_.client_key_path);
        if (!cert.empty() && !key.empty()) {
            ssl_opts.pem_cert_chain = cert;
            ssl_opts.pem_private_key = key;
        }
        creds = grpc::SslCredentials(ssl_opts);
    }
    const std::string addr = m_opts_.address.isEmpty() ? std::string("localhost:55555")
                                                       : m_opts_.address.toStdString();
    auto channel = grpc::CreateChannel(addr, creds);
    m_stub_ = VAL::NewStub(channel);

    // 2. Subscribe to speed data using VAL v2 API
    grpc::ClientContext context;
    attachAuth(context);
    SubscribeRequest request;
    
    // Add Vehicle.Speed to subscription paths
    request.add_signal_paths("Vehicle.Speed");

    std::unique_ptr<grpc::ClientReader<SubscribeResponse>> reader(
        m_stub_->Subscribe(&context, request));

    SubscribeResponse response;
    qDebug() << "KuksaReader: Connected and Subscribed to Vehicle.Speed";

    // 3. Loop to read incoming speed data (blocking call - runs in QThread)
    while(reader->Read(&response))
    {
        // Iterate over the map entries in the response
        const auto& entries = response.entries();
        auto it = entries.find("Vehicle.Speed");
        
        if (it != entries.end()) {
            const auto& datapoint = it->second;
            
            if (datapoint.has_value()) {
                const auto& value = datapoint.value();
                
                // Handle both float and double types from VAL v2
                float speed = 0.0f;
                if (value.has_float_()) {
                    speed = value.float_();
                } else if (value.has_double_()) {
                    speed = static_cast<float>(value.double_());
                }
                
                qDebug() << "KuksaReader: Received Vehicle.Speed =" << speed << "m/s";
                emit speedReceived(speed);
            }
        }
    }
    
    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        qWarning() << "KuksaReader: Subscribe stream ended with error:" 
                   << QString::fromStdString(status.error_message());
    }
}

void KUKSAReader::attachAuth(grpc::ClientContext& ctx)
{
    if (!m_opts_.token.isEmpty()) {
        const std::string bearer = std::string("Bearer ") + m_opts_.token.toStdString();
        ctx.AddMetadata("authorization", bearer);
    }
}

std::string KUKSAReader::loadFile(const QString& path)
{
    if (path.isEmpty()) return {};
    std::ifstream ifs(path.toStdString(), std::ios::in | std::ios::binary);
    if (!ifs) return {};
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

