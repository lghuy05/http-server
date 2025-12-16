#pragma once
#include <unistd.h>

class Socket {
public:
  explicit Socket(int fd) : fd_(fd) {};
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  Socket(Socket &&other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
  Socket &operator=(Socket &&other) noexcept {
    if (this != &other) {
      close();
      fd_ = other.fd_;
      other.fd_ = -1;
    }
    return *this;
  }
  ~Socket() { close(); }
  int fd() { return fd_; }

private:
  int fd_;
  void close() {
    if (fd_ != -1) {
      ::close(fd_);
      fd_ = -1;
    }
  }
};
