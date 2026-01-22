#include <QGuiApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "app_controller.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("DrivaPi Dashboard");

    // --- 1. ARGUMENT PARSING ---
    QCommandLineParser parser;
    parser.setApplicationDescription("Hybrid Dashboard (KUKSA default, CAN optional)");
    parser.addHelpOption();
    
    // Define options
#ifdef ENABLE_CAN_MODE
    QCommandLineOption canModeOption(QStringList() << "can",
        "Force CAN mode (default is KUKSA)");
#endif

    QCommandLineOption kuksaAddrOption(QStringList() << "kuksa-addr",
        "Kuksa databroker address (host:port)", "addr");
    QCommandLineOption kuksaTlsOption(QStringList() << "kuksa-tls",
        "Use TLS for Kuksa connection");
    QCommandLineOption kuksaInsecureOption(QStringList() << "kuksa-insecure",
        "Use insecure connection (default)");
    QCommandLineOption kuksaCaOption(QStringList() << "kuksa-ca",
        "Root CA certificate path", "path");
    QCommandLineOption kuksaCertOption(QStringList() << "kuksa-cert",
        "Client certificate path (mTLS)", "path");
    QCommandLineOption kuksaKeyOption(QStringList() << "kuksa-key",
        "Client private key path (mTLS)", "path");
    QCommandLineOption kuksaTokenOption(QStringList() << "kuksa-token",
        "Authorization token (JWT)", "token");
#ifdef ENABLE_CAN_MODE
    QCommandLineOption canIfOption(QStringList() << "c" << "can-if",
        "CAN interface name", "ifname", "can0");
#endif
#ifdef ENABLE_CAN_MODE
    parser.addOption(canModeOption);
#endif
    parser.addOption(kuksaAddrOption);
    parser.addOption(kuksaTlsOption);
    parser.addOption(kuksaInsecureOption);
    parser.addOption(kuksaCaOption);
    parser.addOption(kuksaCertOption);
    parser.addOption(kuksaKeyOption);
    parser.addOption(kuksaTokenOption);
#ifdef ENABLE_CAN_MODE
    parser.addOption(canIfOption);
#endif
    
    parser.process(app);
    
    // Populate run config
    RunConfig config;
#ifdef ENABLE_CAN_MODE
    config.useKuksa = !parser.isSet(canModeOption);
    config.canInterface = parser.value(canIfOption);
#else
    config.useKuksa = true;
#endif
    config.kuksa.address = parser.value(kuksaAddrOption).isEmpty()
        ? QStringLiteral("localhost:55555")
        : parser.value(kuksaAddrOption);
    config.kuksa.use_ssl = parser.isSet(kuksaTlsOption) && !parser.isSet(kuksaInsecureOption);
    config.kuksa.root_ca_path = parser.value(kuksaCaOption);
    config.kuksa.client_cert_path = parser.value(kuksaCertOption);
    config.kuksa.client_key_path = parser.value(kuksaKeyOption);
    config.kuksa.token = parser.value(kuksaTokenOption);

    AppController controller(config);
    return controller.run(app);
}
