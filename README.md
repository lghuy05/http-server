
# HTTP Server (C++)

A minimal concurrent HTTP/1.1 server implemented from scratch in C++ using POSIX sockets.
The project is Dockerized for reproducible builds and easy execution.

## Run with Docker

### 1. Clone the repository
```bash
git clone https://github.com/lghuy05/http-server.git
cd http-server


docker build -t http-server .

docker run --rm -p 8080:8080 http-server

curl http://localhost:8080
```

