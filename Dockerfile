FROM ubuntu:22.04

ENV TZ=Europe/Berlin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y \
    sudo \
    gnupg \
    curl \
    unzip \
    git \
    build-essential \
    libssl-dev \
    pkg-config \
    clang \
    lld \
    libboost-dev \
    nlohmann-json3-dev \
    libsdbus-c++-dev \
    libsdbus-c++-bin \
    zlib1g-dev \
    qtbase5-dev \
    qtdeclarative5-dev \
    libqt5dbus5 \
    dbus \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y curl build-essential && \
    BAZEL_VERSION=8.4.1 && \
    curl -fLO "https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/bazel-${BAZEL_VERSION}-linux-arm64" && \
    chmod +x "bazel-${BAZEL_VERSION}-linux-arm64" && \
    mv "bazel-${BAZEL_VERSION}-linux-arm64" /usr/local/bin/bazel && \
    rm -rf /var/lib/apt/lists/*

RUN BUILDTOOLS_VERSION=v6.3.3 && \
    curl -fLo /usr/local/bin/buildifier "https://github.com/bazelbuild/buildtools/releases/download/${BUILDTOOLS_VERSION}/buildifier-linux-arm64" && \
    chmod +x /usr/local/bin/buildifier

RUN useradd -ms /bin/bash developer

WORKDIR /home/developer/project
