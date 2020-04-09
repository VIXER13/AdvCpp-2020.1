#include "LoggerInit.hpp"
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <iostream>
#include "Logger.hpp"

void loggerInit(int argc, char *const *argv) {
    static const std::array<std::string, 4> levels = {"debug", "info", "warning", "error"};
    static constexpr struct option long_options[] = {
            {.name = "verbose"      , .has_arg = no_argument       , .flag = nullptr, .val = 'v'},
            {.name = "quiet"        , .has_arg = no_argument       , .flag = nullptr, .val = 'q'},
            {.name = "verbose_level", .has_arg = optional_argument , .flag = nullptr, .val = 'l'},
            {.name = "log-to-file"  , .has_arg = optional_argument , .flag = nullptr, .val = 'f'},
            {.name = "stderr"       , .has_arg = optional_argument , .flag = nullptr, .val = 'e'},
            {}
        };

    auto lvl = logger::Level::INFO;
    bool stderr_quest = false;  // false - stdout, true - stderr
    std::string path;  // Если не пуст, печать в файл, несмотря на stderr_quest

    int short_opt = -1;
    while ((short_opt = getopt_long(argc, argv, "-vqe", long_options, nullptr)) != -1) {
        if (short_opt == 'v') {
            lvl = logger::Level::DEBUG;
        } else if (short_opt == 'q') {
            lvl = logger::Level::ERROR;
        } else if (short_opt == 'l') {
            if (!strcmp(optarg, levels[0].data())) {
                lvl = logger::Level::DEBUG;
            } else if (!strcmp(optarg, levels[1].data())) {
                lvl = logger::Level::INFO;
            } else if (!strcmp(optarg, levels[2].data())) {
                lvl = logger::Level::WARNING;
            } else if (!strcmp(optarg, levels[3].data())) {
                lvl = logger::Level::ERROR;
            } else {
                std::cerr << "Error verbose_level option. Set default level - INFO." << std::endl;
            }
        } else if (short_opt == 'f') {
            path = optarg;
        } else if (short_opt == 'e') {
            stderr_quest = true;
        }
    }

    if (!path.empty()) {
        logger::Logger::getInstance().setGlobalLogger(logger::createFileLogger(path, lvl));
    } else if (stderr_quest) {
        logger::Logger::getInstance().setGlobalLogger(logger::createStderrLogger(lvl));
    } else {
        logger::Logger::getInstance().setGlobalLogger(logger::createStdoutLogger(lvl));
    }
}
