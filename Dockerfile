FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install compiler & tools
RUN apt update && apt install -y \
    g++ make curl git \
    && rm -rf /var/lib/apt/lists/*

# Install ttyd (web terminal)
RUN curl -L https://github.com/tsl0922/ttyd/releases/download/1.7.7/ttyd.x86_64 \
    -o /usr/local/bin/ttyd && chmod +x /usr/local/bin/ttyd

WORKDIR /app
COPY . .

# Compile your judge
RUN g++ -std=gnu++17 -O2 -o judge $(find . -name "*.cpp") -pthread

EXPOSE 8080

# Start web terminal
CMD ["ttyd", "-p", "8080", "./judge"]
