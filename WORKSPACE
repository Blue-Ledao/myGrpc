#采用本地仓库的方式引入依赖
local_repository(
    name = "com_github_grpc_grpc",
    path = "{你的路径}/grpc-1.28.1",
)

local_repository(
    name = "rules_proto",
    path = "{你的路径}/rules_proto",
)

local_repository(
    name = "googletest",
    path = "{你的路径}/googletest-release-1.10.0",
)


load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()


load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")

grpc_deps()

load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")

grpc_extra_deps()




