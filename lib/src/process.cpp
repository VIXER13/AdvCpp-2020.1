#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

namespace process_lib {

Process::Process(const std::string& path) {
    if (pipe(fd_in.data()) < 0 || pipe(fd_out.data()) < 0) {
        close();
        throw ProcessException{"Pipes failed"};
    }

    pid = fork();
    if (pid < 0) {
        close();
        throw ProcessException{"Fork failed"};
    } else if (!pid) {
        if (dup2(fd_in[READ], STDIN_FILENO) < 0 || dup2(fd_out[WRITE], STDOUT_FILENO) < 0) {
            close();
            throw ProcessException{"Dup2 failed"};
        }

        if (execl(path.c_str(), path.c_str(), nullptr) < 0) {
            close();
            throw ProcessException{"Execl failed"};
        }
    }
}

Process::~Process() {
    close();
    if (pid) {
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
    }
}

size_t Process::write(const void* data, size_t len) {
    return ::write(fd_in[WRITE], data, len);
}

void Process::writeExact(const void* data, size_t len) {
    size_t recorded = 0;
    while (true) {
        auto write_out = ssize_t(write(static_cast<const int8_t*>(data) + recorded, len - recorded));
        if (write_out < 0) {
            throw ProcessException{"WriteExact error"};
        }
        recorded += write_out;
        if (recorded == len) {
            break;
        }
    }
}

size_t Process::read(void* data, size_t len) {
    return ::read(fd_out[READ], data, len);
}

void Process::readExact(void* data, size_t len) {
    size_t readed = 0;
    while (true) {
        auto read_out = ssize_t(read(static_cast<int8_t*>(data) + readed, len - readed));
        if (read_out == -1) {
            throw ProcessException{"ReadExact error"};
        }
        readed += read_out;
        if (readed == len) {
            break;
        }
    }
}

bool Process::isReadable() const {
    
    struct stat buff = {};
    fstat(fd_out[READ], &buff);
    return buff.st_blksize;
}

void Process::closeStdin() {
    ::close(fd_in[READ]);
    ::close(fd_in[WRITE]);
}

void Process::close() {
    closeStdin();
    ::close(fd_out[READ]);
    ::close(fd_out[WRITE]);
}

}
