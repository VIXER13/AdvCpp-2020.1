#include <unistd.h>
#include <signal.h>
#include <iostream>
#include "simple_dialog.hpp"
#include "process_exception.hpp"
#include "file_descriptor_exception.hpp"

int main(int argc, char **argv) {
    if (argc > 1) {
        try {
            process::Process proc(argv[1]);
            printMenu();
            signal(SIGPIPE, SIG_IGN);
            while (true) {
                try {
                    if (!simpleDialog(proc))
                        break;
                } catch(const process::ProcessException& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        } catch(const process::ProcessException& e) {
            std::cerr << e.what() << std::endl;
            kill(getppid(), SIGTERM);
            return EXIT_FAILURE;
        } catch(const file_descriptor::FileDescriptorException& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "Argc < 2." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
