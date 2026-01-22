#include <QGuiApplication>
#include "app_controller.hpp"
#include "cli_parser.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("DrivaPi Dashboard");

    CliOptions opts;
    QCommandLineParser parser;
    configureParser(parser, opts);
    parser.process(app);

    RunConfig config = buildRunConfig(parser, opts);

    if (!validateOptions(parser, opts, config, app.arguments())) {
        return 1;
    }

    AppController controller(config);
    return controller.run(app);
}
