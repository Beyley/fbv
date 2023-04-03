const std = @import("std");
const Self = @This();
const CrossTarget = std.zig.CrossTarget;

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const executable_options: std.build.ExecutableOptions = .{
        .name = "fbv",
        .target = target,
        .optimize = mode,
    };

    var executable: *std.build.CompileStep = b.addExecutable(executable_options);

    executable.linkLibC();

    executable.addIncludePath(root_path ++ "miniz/");
    executable.addIncludePath(root_path ++ "libspng/spng");

    executable.addCSourceFiles(srcs, &.{ "-std=c99", "-DSPNG_USE_MINIZ" });

    var config_header = b.addConfigHeader(.{ .include_path = "config.h" }, .{
        // .FBV_SUPPORT_JPEG = 1,
        .FBV_SUPPORT_PNG = 1,
        .FBV_SUPPORT_BMP = 1,
        .DEFAULT_FRAMEBUFFER = "/dev/fb0",
    });

    executable.addConfigHeader(config_header);

    executable.install();
}

fn root() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}

const root_path = root() ++ "/";

const srcs = &.{
    root_path ++ "main.c",
    root_path ++ "vt.c",
    root_path ++ "bmp.c",
    root_path ++ "miniz/miniz.c",
    root_path ++ "miniz/miniz_tinfl.c",
    root_path ++ "miniz/miniz_tdef.c",
    root_path ++ "libspng/spng/spng.c",
    root_path ++ "png.c",
    root_path ++ "jpeg.c",
    root_path ++ "fb_display.c",
    root_path ++ "transforms.c",
};
