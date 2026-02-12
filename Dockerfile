# Use a lightweight C++ base image
FROM alpine:latest

# Install C++ compiler and Gotty (Web-to-Terminal bridge)
RUN apk add --no-cache g++ make wget && \
    wget https://github.com/yudai/gotty/releases/download/v1.0.1/gotty_linux_amd64.tar.gz && \
    tar -C /usr/local/bin -xvf gotty_linux_amd64.tar.gz

# Copy your source code
COPY . /app
WORKDIR /app

# Compile your application for Linux
RUN g++ -o judge main.cpp # Replace 'main.cpp' with your actual file names

# Expose the port
EXPOSE 8080

# Run Gotty to serve your app on the web
CMD ["gotty", "-w", "./judge"]
