#include <iostream>
#include "LoggerInit.hpp"
#include "Logger.hpp"

int main(int argc, char **argv) {
    loggerInit(argc, argv);
    logger::debug("test");
    logger::info("test");
    logger::warning("test");
    logger::error("test");
    logger::flush();
    return 0;
}
