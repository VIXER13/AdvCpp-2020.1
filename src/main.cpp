#include <iostream>
#include "simple_dialog.hpp"

int main(int argc, char **argv) {
    if (argc > 1) {
        Process proc(argv[1]);
        printMenu();

        while (true) {
            try {
            if (!simpleDialog(proc))
                break;
            } catch(const ProcessException& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    } else {
        std::cerr << "Argc < 2." << std::endl;
    }
    return 0;
}
