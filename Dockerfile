# 1. Use a lightweight Linux image with a modern C++ compiler
FROM alpine:latest

# 2. Install necessary build tools and the C++ compiler
# 'musl-dev' is required for standard C++ headers on Alpine Linux
RUN apk add --no-cache g++ make wget bash musl-dev

# 3. Download and install Gotty (The bridge that puts your terminal on a website)
RUN wget https://github.com && \
    tar -C /usr/local/bin -xvf gotty_linux_amd64.tar.gz && \
    rm gotty_linux_amd64.tar.gz

# 4. Set the working directory inside the container
WORKDIR /app

# 5. Copy all your project files from GitHub into the container
COPY . .

# 6. Compile the application
# We use 'find' to make sure we catch every .cpp file, even in subfolders.
# '-pthread' is added in case your judge uses threading for timeouts.
RUN g++ -O3 -o judge $(find . -name "*.cpp") -pthread

# 7. Tell Render which port to use (matches your Environment Variable)
EXPOSE 8080

# 8. Start Gotty to serve your 'judge' executable on the web
# -w: Allows user input (typing)
# -r: Randomizes the URL session for security
CMD ["gotty", "-r", "-w", "--port", "8080", "./judge"]
