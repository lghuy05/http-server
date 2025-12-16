#include "socket.hpp"
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

void timeout(int fd, int seconds) {
  timeval tv{};
  tv.tv_sec = seconds;
  tv.tv_usec = 0;
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
};
void handle_request(Socket client) {
  timeout(client.fd(), 5);
  char buffer[2048];
  ssize_t byte_read = read(client.fd(), buffer, sizeof(buffer) - 1);
  if (byte_read < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      std::cout << "Client timeout \n";
    } else {
      perror("read");
    }
    return;
  }
  if (byte_read == 0) {
    std::cout << "Disconnected\n";
    return;
  }
  buffer[byte_read] = '\0';
  std::cout << "Received request: " << buffer << std::endl;
  std::string response = "HTTP/1.1 200 OK\r\n"
                         "Content-Length: 2\r\n"
                         "\r\n"
                         "OK\n";
  write(client.fd(), response.c_str(), response.size());
};

int main() {
  int raw_fd = socket(AF_INET, SOCK_STREAM, 0);
  Socket listen_socket(raw_fd);

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(listen_socket.fd(), (sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 1;
  }
  if (listen(listen_socket.fd(), 5) == -1) {
    perror("listen");
    return 1;
  }
  std::cout << "Listening ....\n";
  while (true) {
    std::cout << "Waiting for connection .....\n";
    int client_fd = accept(listen_socket.fd(), nullptr, nullptr);
    if (client_fd == -1) {
      perror("accept");
      return 1;
    }
    Socket client_socket(client_fd);
    std::thread(handle_request, std::move(client_socket)).detach();
    std::cout << "Client handle, waiting for response";
  }
  return 0;
}
