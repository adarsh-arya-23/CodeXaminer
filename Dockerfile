FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y \
    g++ make curl git bash \
    && rm -rf /var/lib/apt/lists/*

RUN curl -L https://github.com/tsl0922/ttyd/releases/download/1.7.7/ttyd.x86_64 \
    -o /usr/local/bin/ttyd && chmod +x /usr/local/bin/ttyd

WORKDIR /app
COPY . .

RUN g++ -std=gnu++17 -O2 -DWEB_TERMINAL -o judge $(find . -name "*.cpp") -pthread

EXPOSE 8080

CMD ["ttyd", "-p", "8080", "bash", "-lc", "./judge"]
