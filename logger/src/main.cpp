#include <iostream>
#include "logger_init.hpp"
#include "Logger.hpp"

int main(int argc, char **argv) {
    logger_init(argc, argv);
    logger::debug("test");
    logger::info("test");
    logger::warning("test");
    logger::error("test");
    logger::flush();
    return 0;
}
