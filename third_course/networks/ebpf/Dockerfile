# Dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    clang \
    llvm \
    libbpf-dev \
    linux-headers-generic \
    tcpdump \
    netcat-openbsd \
    curl

WORKDIR /app
COPY *.bpf.c ./
COPY *.sh ./

CMD ["/bin/bash"]
