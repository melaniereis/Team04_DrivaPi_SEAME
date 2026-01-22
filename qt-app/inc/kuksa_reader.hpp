#ifndef KUKSAREADER_HPP
#define KUKSAREADER_HPP

#include <QObject>
#include <QDebug>
#include <grpcpp/grpcpp.h>
#include "kuksa/val/v2/val.grpc.pb.h"

// KUKSA Val v2 gRPC namespace
using namespace kuksa::val::v2;

struct KuksaOptions {
    QString address = QStringLiteral("localhost:55555");
    bool use_ssl = false;                 // false = insecure
    QString root_ca_path;                 // optional
    QString client_cert_path;             // optional (for mTLS)
    QString client_key_path;              // optional (for mTLS)
    QString token;                        // optional JWT token
};

class KUKSAReader : public QObject
{
    Q_OBJECT
public:
    explicit KUKSAReader(QObject *parent = nullptr);
    explicit KUKSAReader(const KuksaOptions& opts, QObject *parent = nullptr);
    ~KUKSAReader() override;
public slots:
    // Starts the data fetching process
    void start();
    // Gracefully stops the subscription stream
    void stop();

signals:
    // Emitted when new speed data is received
    void speedReceived(float speed);
    // Emitted on fatal connection or configuration errors
    void errorOccurred(const QString& message);

private:
    // Attach Authorization metadata if token present
    void attachAuth(grpc::ClientContext& ctx);

    // Helper to load file contents with optional warning on failure
    static std::string loadFile(const QString& path, bool warnOnMissing = false);

    // Helper to validate and encode bearer token
    static std::string encodeBearerToken(const QString& token);

    KuksaOptions m_opts_;
    // gRPC client stub -> used to communicate with KUKSA Val server
    std::unique_ptr<VAL::Stub> m_stub_;
    // Flag to signal graceful shutdown of subscription stream
    std::atomic<bool> m_stop_requested_{false};
};

#endif // KUKSAREADER_HPP