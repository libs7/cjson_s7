load("@cc_config//:CONFIG.bzl",
     _BASE_COPTS    = "BASE_COPTS",
     _BASE_DEFINES  = "BASE_DEFINES",
     _BASE_LINKOPTS = "BASE_LINKOPTS")

BASE_COPTS = _BASE_COPTS
BASE_LINKOPTS = _BASE_LINKOPTS
BASE_DEFINES = _BASE_DEFINES

BASE_SRCS = ["@libs7//src:libs7.h"]

BASE_DEPS = ["@libs7//src:s7", "@liblogc//src:logc"]

BASE_INCLUDE_PATHS = [
    "-I$(@libs7)/src",
    "-I$(GENDIR)/$(@libs7)/src",
    "-I$(@liblogc)/src"
]

