#include "feeder_cli.hpp"
#include <iostream>
#include <cstring>

namespace feeder {

FeederConfig ParseArgs(int argc, char** argv)
{
    FeederConfig config;
    config.publisher_options.address = "localhost:55555";
    config.publisher_options.use_ssl = false;

    // Positional arguments: [can_interface] [kuksa_address]
    if (argc >= 2) {
        config.can_interface = argv[1];
    }
    if (argc >= 3) {
        config.publisher_options.address = argv[2];
    }

    // Optional flags: --insecure, --tls, --ca, --cert, --key, --token
    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--insecure") {
            config.publisher_options.use_ssl = false;
        } 
        else if (arg == "--tls") {
            config.publisher_options.use_ssl = true;
        } 
        else if (arg == "--ca" && i + 1 < argc) {
            config.publisher_options.root_ca_path = argv[++i];
        }
        else if (arg == "--cert" && i + 1 < argc) {
            config.publisher_options.client_cert_path = argv[++i];
        }
        else if (arg == "--key" && i + 1 < argc) {
            config.publisher_options.client_key_path = argv[++i];
        }
        else if (arg == "--token" && i + 1 < argc) {
            config.publisher_options.token = argv[++i];
        } 
        else if (arg == "--help" || arg == "-h") {
            PrintUsage(argv[0]);
            exit(0);
        }
        else {
            std::cerr << "[CLI] Unknown option: " << arg << std::endl;
        }
    }

    return config;
}

void PrintConfig(const FeederConfig& config)
{
    std::cout << "========================================" << std::endl;
    std::cout << "  KUKSA CAN Feeder Configuration" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "CAN Interface:    " << config.can_interface << std::endl;
    std::cout << "KUKSA Address:    " << config.publisher_options.address << std::endl;
    std::cout << "Security Mode:    " << (config.publisher_options.use_ssl ? "TLS" : "Insecure") << std::endl;
    if (!config.publisher_options.root_ca_path.empty()) {
        std::cout << "Root CA:          " << config.publisher_options.root_ca_path << std::endl;
    }
    if (!config.publisher_options.client_cert_path.empty()) {
        std::cout << "Client Cert:      " << config.publisher_options.client_cert_path << std::endl;
    }
    if (!config.publisher_options.client_key_path.empty()) {
        std::cout << "Client Key:       " << config.publisher_options.client_key_path << std::endl;
    }
    if (!config.publisher_options.token.empty()) {
        std::cout << "Authorization:    Bearer <token>" << std::endl;
    }
    std::cout << "========================================" << std::endl;
}

void PrintUsage(const std::string& program_name)
{
    std::cout << "\nUsage: " << program_name 
              << " [can_interface] [kuksa_address] [options]\n" 
              << "\nPositional Arguments:\n"
              << "  can_interface     CAN interface name (default: vcan0)\n"
              << "  kuksa_address     KUKSA databroker host:port (default: localhost:55555)\n"
              << "\nOptions:\n"
              << "  --insecure        Use insecure channel (default)\n"
              << "  --tls             Use TLS encrypted channel\n"
              << "  --ca <path>       Root CA certificate path (for TLS)\n"
              << "  --cert <path>     Client certificate path (for mTLS)\n"
              << "  --key <path>      Client private key path (for mTLS)\n"
              << "  --token <jwt>     Authorization token (JWT)\n"
              << "  --help, -h        Show this help message\n"
              << "\nExamples:\n"
              << "  " << program_name << " vcan0 localhost:55555\n"
              << "  " << program_name << " can0 databroker.local:55555 --tls --ca ca.crt --token mytoken\n"
              << std::endl;
}

} // namespace feeder
