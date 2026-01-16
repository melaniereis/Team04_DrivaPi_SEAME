#include "kuksa_reader.hpp"

KUKSAReader::KUKSAReader(QObject *parent)
    : QObject(parent)
{}

KUKSAReader::~KUKSAReader()
{}

void KUKSAReader::start()
{
    // 1. Connect to Broker
    auto channel = grpc::CreateChannel("localhost:55555", grpc::InsecureChannelCredentials());
    m_stub_ = VAL::NewStub(channel);

    // 2. Subscribe to speed data using VAL v2 API
    grpc::ClientContext context;
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

