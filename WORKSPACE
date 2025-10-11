load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# --- Protobuf ta gRPC (stable combination) ---
http_archive(
    name = "com_google_protobuf",
    sha256 = "930c2c3b5ecc6c9c12615cf5ad93f1cd6e12d0aba862b572e076259970ac3a53",
    strip_prefix = "protobuf-3.21.12",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/v3.21.12.tar.gz"],
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

http_archive(
    name = "com_github_grpc_grpc",
    sha256 = "0c3faa83e39d4f1ab55fe1476362b9ac3b81632a46dce7fd4d50271bce816b53",
    strip_prefix = "grpc-1.48.4",
    urls = ["https://github.com/grpc/grpc/archive/v1.48.4.tar.gz"],
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()