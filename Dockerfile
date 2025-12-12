# Use a lightweight Linux
FROM alpine:latest

# Install C++ compilers and Go (for the web terminal)
RUN apk add --no-cache g++ git go

# Install GoTTY (this turns your C++ app into a website)
RUN go install github.com/sorenisanerd/gotty@latest
ENV PATH="$PATH:/root/go/bin"

# Set up the app folder
WORKDIR /app
COPY . .

# Compile the modernized C++ code
RUN g++ -o library_app main.cpp

# Open port 8080
EXPOSE 8080

# Run the app inside the web terminal
# -w allows writing (typing) in the terminal
CMD ["gotty", "-w", "-p", "8080", "./library_app"]