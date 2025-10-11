# bazel/deps.bzl
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def _non_module_deps_impl(_ctx):
    http_archive(
        name = "fmt",
        build_file_content = """
load("@rules_cc//cc:defs.bzl", "cc_library")
cc_library(
    name = "fmt",
    hdrs = glob(["include/fmt/*.h"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    copts = ["-DFMT_HEADER_ONLY=1"],
)""",
        sha256 = "f2aad1a340d27c0b22cf23268c1cbdffb8472a242b95daf2d2311eed8d4948fc",
        strip_prefix = "fmt-8.1.1",
        urls = ["https://github.com/fmtlib/fmt/archive/refs/tags/8.1.1.zip"],
    )

    http_archive(
        name = "libenvpp",
        build_file_content = """
load("@rules_cc//cc:defs.bzl", "cc_library")
cc_library(
    name = "libenvpp",
    hdrs = glob(["include/**/*.hpp"]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
    deps = ["@fmt//:fmt"],
)""",
        sha256 = "94023b6c93eff0b7fdb6dbd56dc5ee0e7b0d8af8ee12bebc94b638083bac7ac7",
        strip_prefix = "libenvpp-1.5.1",
        urls = ["https://github.com/ph3at/libenvpp/archive/refs/tags/v1.5.1.zip"],
    )

    http_archive(
        name = "nlohmann_json",
        build_file_content = """
load("@rules_cc//cc:defs.bzl", "cc_library")
cc_library(
    name = "json",
    hdrs = ["single_include/nlohmann/json.hpp"],
    strip_include_prefix = "single_include",
    visibility = ["//visibility:public"],
)""",
        sha256 = "a32a67e3b1c67825b448a972a91f4a5a51c91104e747053ffca3264420a7b45c",
        strip_prefix = "json-3.11.3",
        urls = ["https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip"],
    )

non_module_deps = module_extension(
    implementation = _non_module_deps_impl,
)