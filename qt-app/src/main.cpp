#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScopedPointer>
#include <QWindow>
#include <QThread>
#include <QPointer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "vehicle_data.hpp"
#ifdef ENABLE_CAN_MODE
#include "can_reader.hpp"
#endif
#include "kuksa_reader.hpp"

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
    
    // Check if the user passed the argument
    bool useKuksa = true;
#ifdef ENABLE_CAN_MODE
    useKuksa = !parser.isSet(canModeOption);
#endif

    // UI and Engine setup
    QQmlApplicationEngine engine;
    QScopedPointer<VehicleData> vehicleData(new VehicleData());
    // Expose VehicleData to QML (keep ownership in C++)
    engine.rootContext()->setContextProperty("vehicleData", vehicleData.data());

    // Worker thread setup for data reading
    QThread *workerThread = new QThread(&app);

    // Pointers to readers for cleanup
    KUKSAReader *kuksaReader = nullptr;
#ifdef ENABLE_CAN_MODE
    CANReader *canReader = nullptr;
#endif

    if (useKuksa)
    {
        qInfo() << "Starting in KUKSA mode (default)";
        // KUKSA Reader setup with options
        KuksaOptions ko;
        ko.address = parser.value(kuksaAddrOption).isEmpty()
            ? QStringLiteral("localhost:55555")
            : parser.value(kuksaAddrOption);
        ko.use_ssl = parser.isSet(kuksaTlsOption) && !parser.isSet(kuksaInsecureOption);
        ko.root_ca_path = parser.value(kuksaCaOption);
        ko.client_cert_path = parser.value(kuksaCertOption);
        ko.client_key_path = parser.value(kuksaKeyOption);
        ko.token = parser.value(kuksaTokenOption);

        kuksaReader = new KUKSAReader(ko);
        kuksaReader->moveToThread(workerThread);
        // Start KUKSAReader when thread starts
        QObject::connect(workerThread, &QThread::started, kuksaReader, &KUKSAReader::start);
        // Ensure worker object is deleted when thread finishes (safe because it's a QObject)
        QObject::connect(workerThread, &QThread::finished, kuksaReader, &KUKSAReader::deleteLater);
        // Forward speed data from KUKSAReader to VehicleData (thread-safe)
        QObject::connect(kuksaReader, &KUKSAReader::speedReceived,
                         vehicleData.data(), &VehicleData::handleSpeedUpdate);
    } else {
#ifdef ENABLE_CAN_MODE
        const QString canInterface = parser.value(canIfOption);
        qInfo() << "Starting in CAN mode on" << canInterface << "(--can)";
        // CAN Reader setup
        canReader = new CANReader(canInterface);
        canReader->moveToThread(workerThread);
        // Start CANReader when thread starts
        QObject::connect(workerThread, &QThread::started, canReader, &CANReader::start);
        // Ensure worker object is deleted when thread finishes (safe because it's a QObject)
        QObject::connect(workerThread, &QThread::finished, canReader, &CANReader::deleteLater);

        // Forward CAN messages from thread worker to UI (thread-safe)
        QObject::connect(canReader, &CANReader::canMessageReceived,
                        vehicleData.data(), &VehicleData::handleCanMessage,
                        Qt::QueuedConnection);
#else
        qWarning() << "CAN mode requested but CAN support is disabled at build time (ENABLE_CAN_MODE=OFF). Falling back to KUKSA.";
        useKuksa = true;
#endif
    }

    workerThread->start();

    // CLEANUP HANDLING
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [workerThread,
#ifdef ENABLE_CAN_MODE
        canReader,
#endif
        kuksaReader]() {
#ifdef ENABLE_CAN_MODE
        if (canReader)
        {
            // ask CAN reader to stop
            QMetaObject::invokeMethod(canReader, "stop", Qt::QueuedConnection);
        }
#endif
        if (workerThread)
        {
            workerThread->quit();
            // wait for thread to finish
            if (!workerThread->wait(2000)) {
                qWarning() << "Worker thread did not quit in 2 seconds, terminating";
                workerThread->terminate();
                workerThread->wait();
            }
            // delete the thread (worker will be deleted via deleteLater)
            delete workerThread;
        }
    });

    // Load main QML file
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

    engine.load(url);

    if (!engine.rootObjects().isEmpty()) {
        QWindow *window = qobject_cast<QWindow*>(engine.rootObjects().first());
        if (window) {
            window->showFullScreen();  // ✅ Hides title bar & taskbar
        }
    }

    // Keep running until app quits
    return app.exec();

    // vehicleData will be automatically destroyed when going out of scope
}
