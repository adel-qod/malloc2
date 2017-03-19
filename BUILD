cc_library(
	name = "malloc",
	srcs = ["my_malloc.c"],
	hdrs = ["my_malloc.h"],
	visibility = ["//main:__pkg__"],
)

cc_binary(
	name = "main",
	srcs = ["main.c"],
	deps = [":malloc"],
)
