const std = @import("std");
const Self = @This();
const CrossTarget = std.zig.CrossTarget;

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    var executable = b.addExecutable(.{
        .name = "fbv",
        .target = target,
        .optimize = mode,
    });

    executable.linkLibC();

    executable.addIncludePath(b.path("miniz/"));
    executable.addIncludePath(b.path("libspng/spng"));

    executable.addCSourceFiles(.{ .files = srcs, .flags = &.{ "-std=c99", "-Wno-implicit-function-declaration", "-DSPNG_USE_MINIZ" } });

    const config_header = b.addConfigHeader(.{ .include_path = "config.h" }, .{
        // .FBV_SUPPORT_JPEG = 1,
        .FBV_SUPPORT_PNG = 1,
        .FBV_SUPPORT_BMP = 1,
        .DEFAULT_FRAMEBUFFER = "/dev/fb0",
    });

    executable.addConfigHeader(config_header);

    b.installArtifact(executable);
}

const srcs = &.{
    "main.c",
    "vt.c",
    "bmp.c",
    "miniz/miniz.c",
    "miniz/miniz_tinfl.c",
    "miniz/miniz_tdef.c",
    "libspng/spng/spng.c",
    "png.c",
    "jpeg.c",
    "fb_display.c",
    "transforms.c",
};
