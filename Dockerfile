
FROM gcc:13

# Install cmake
RUN apt-get update && apt-get install -y cmake

WORKDIR /app

COPY . .

RUN cmake -S . -B build \
    && cmake --build build

EXPOSE 8080

CMD ["./build/httpserver"]
