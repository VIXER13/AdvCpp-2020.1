#include <unistd.h>
#include <signal.h>
#include <iostream>
#include "simple_dialog.hpp"

int main(int argc, char **argv) {
    if (argc > 1) {
        try {
            process_lib::Process proc(argv[1]);
            printMenu();

            while (true) {
                try {
                    if (!simpleDialog(proc))
                        break;
                } catch(const process_lib::ProcessException& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        } catch(const process_lib::ProcessException& e) {
            std::cerr << e.what() << std::endl;
            kill(getppid(), SIGTERM);
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Argc < 2." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
