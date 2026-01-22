#pragma once

#include <QString>
#include "kuksa_reader.hpp"

class QGuiApplication;

namespace drivaui {

struct RunConfig {
    bool useKuksa{true};
    QString canInterface{QStringLiteral("can0")};
    kuksa::KuksaOptions kuksa{};
};

class AppController {
public:
    explicit AppController(const RunConfig& config);
    int run(QGuiApplication& app);

private:
    RunConfig config_;
};

}  // namespace drivaui
