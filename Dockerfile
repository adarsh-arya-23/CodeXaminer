FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools
RUN apt update && apt install -y \
    g++ make curl git bash \
    && rm -rf /var/lib/apt/lists/*

# Install ttyd (web terminal)
RUN curl -L https://github.com/tsl0922/ttyd/releases/download/1.7.7/ttyd.x86_64 \
    -o /usr/local/bin/ttyd && chmod +x /usr/local/bin/ttyd

# Set working directory
WORKDIR /app
COPY . .

# Compile your judge
RUN g++ -std=gnu++17 -O2 -DWEB_TERMINAL -o judge $(find . -name "*.cpp") -pthread

# Render uses this port
EXPOSE 8080

# IMPORTANT: Run inside interactive bash
CMD ["ttyd", "-p", "8080", "-i", "0.0.0.0", "-t", "fontSize=14", "bash", "-i", "-c", "./judge"]
