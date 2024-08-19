# Use an official Debian or Ubuntu as a parent image
# FROM debian:bullseye
FROM ubuntu:20.04

# Set the working directory in the container
WORKDIR /home/tung/git/kernel-drivers

# Update the package repository and install GCC
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    g++ \
    make \
    cmake \
    kmod \
    linux-headers-5.15.0-117-generic
    # linux-headers-$(uname -r)

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Build your project (if you have a Makefile or CMakeLists.txt)
# RUN make
# WORKDIR /home/tung/git/kernel-drivers
RUN make

# Optional
# RUN make -C user

# Set the default command to run when starting the container
CMD ["bash"]

# Specify the default command to run your application (optional)
# CMD ["./user/main"]

