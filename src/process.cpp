#include "process.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

Process::Process(const std::string& path) {
    if (pipe(fd.data()) < 0) {
        throw ProcessException{"Pipes failed"};
    }

    pid = fork();
    if (pid < 0) {
        throw ProcessException{"Fork failed"};
    } else if (!pid) {
        if (dup2(fd[0], 0) < 0 || dup2(fd[1], 1) < 0) {
            throw ProcessException{"Dup2 failed"};
        }

        if (execl(path.data(), path.data(), nullptr) < 0) {
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
    return ::write(fd[1], data, len);
}

void Process::writeExact(const void* data, size_t len) {
    size_t recorded = 0;
    while (true) {
        ssize_t write_out = ::write(fd[1], static_cast<const int8_t*>(data) + recorded, len - recorded);
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
    return ::read(fd[0], data, len);
}

void Process::readExact(void* data, size_t len) {
    size_t readed = 0;
    while (true) {
        ssize_t read_out = ::read(fd[0], static_cast<int8_t*>(data) + readed, len - readed);
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
    fstat(fd[0], &buff);
    return buff.st_blksize;
}

void Process::closeStdin() {
    ::close(fd[1]);
}

void Process::close() {
    ::close(fd[0]);
    ::close(fd[1]);
}

pid_t Process::getPid() const noexcept {
    return pid;
}
