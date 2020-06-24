load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary","cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "demo_proto",
	srcs = ["demo.proto"],
)

cc_proto_library(
    name = "demo_cc_proto",
    deps = [":demo_proto"],
)

cc_grpc_library(
    name = "demo_cc_grpc",
    srcs = [":demo_proto"],
    grpc_only = True,
    deps = [":demo_cc_proto"],
)

cc_library(
    name="grpc_server_test",
    srcs=["token.cc"],
    hdrs=["token.h"],
    defines = ["BAZEL_BUILD"],
    visibility = ["//visibility:public"],
)


cc_binary(
    name = 'grpc_client',
    srcs = ['grpc_client.cc'],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":demo_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = 'grpc_server',
    srcs = ['grpc_server.cc'],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":demo_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
    visibility = ["//visibility:public"],
)
