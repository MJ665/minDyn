
# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies: build tools, git, and LLVM 14
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    llvm-14-dev \
    libclang-14-dev \
    clang-14 && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the project files into the container
COPY . .

# Create a build directory and run CMake and Make
RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

# The default command when the container starts
CMD ["/bin/bash"]