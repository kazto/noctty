const Proxy = @This();

const std = @import("std");
const Config = @import("Config.zig");

const name = "noctty-terminal-handoff-proxy";
const source_root = "src/apprt/win32_terminal_handoff_proxy";

library: *std.Build.Step.Compile,
install_step: *std.Build.Step.InstallFile,

pub fn init(b: *std.Build, cfg: *const Config) !Proxy {
    const arch_dir = switch (cfg.target.result.cpu.arch) {
        .x86_64 => "x64",
        .aarch64 => "arm64",
        else => return error.UnsupportedTerminalHandoffProxyArchitecture,
    };
    const generated_root = b.path(b.fmt("{s}/{s}", .{ source_root, arch_dir }));
    const module = b.createModule(.{
        .target = cfg.target,
        .optimize = cfg.optimize,
    });
    // mingw-w64's <rpcproxy.h>/<ndr64types.h> are missing a few
    // declarations that current MIDL-generated stubless proxy code (this
    // directory) relies on. This shim directory backfills them via
    // `#include_next` without touching the MSVC ABI build. See
    // src/apprt/win32_terminal_handoff_proxy/mingw_compat/ for details.
    if (cfg.target.result.abi == .gnu) {
        module.addIncludePath(b.path(source_root ++ "/mingw_compat"));
    }
    module.addIncludePath(generated_root);
    module.addCMacro("REGISTER_PROXY_DLL", "1");
    module.addCMacro("WIN32", "1");
    // The MIDL-generated ITerminalHandoff_p.c guards its entire body
    // (including the ITerminalHandoff_ProxyFileInfo definition dlldata.c
    // needs) behind the MSVC-only _M_AMD64/_M_ARM64 macros. clang targeting
    // the -windows-gnu ABI never defines these, so without this the file
    // compiles to nothing and the link fails with an undefined symbol.
    switch (cfg.target.result.cpu.arch) {
        .x86_64 => module.addCMacro("_M_AMD64", "100"),
        .aarch64 => module.addCMacro("_M_ARM64", "1"),
        else => {},
    }
    module.addCMacro(
        "PROXY_CLSID_IS",
        "{0x1d349824,0x21fb,0x46c7,{0xac,0xf3,0x74,0x6e,0xdc,0x99,0x1d,0x52}}",
    );
    module.addCSourceFiles(.{
        .root = generated_root,
        .files = &.{
            "dlldata.c",
            "ITerminalHandoff_i.c",
            "ITerminalHandoff_p.c",
        },
        // ITerminalHandoff_p.c forward-declares NDR64 format fragments as
        // `extern const` and later defines them `static const` (MSVC emits
        // and then suppresses warning C4211 for exactly this pattern).
        // clang treats it as a hard linkage-conflict error unless
        // -fms-extensions relaxes it to match MSVC's behavior.
        .flags = &.{"-fms-extensions"},
    });
    module.addCSourceFile(.{
        .file = b.path(source_root ++ "/exports.c"),
    });
    module.linkSystemLibrary("rpcrt4", .{});
    module.linkSystemLibrary("ole32", .{});
    module.linkSystemLibrary("oleaut32", .{});

    const library = b.addLibrary(.{
        .name = name,
        .root_module = module,
        .linkage = .dynamic,
        .use_llvm = true,
    });
    library.linkLibC();
    const install_step = b.addInstallBinFile(
        library.getEmittedBin(),
        name ++ ".dll",
    );
    return .{ .library = library, .install_step = install_step };
}

pub fn install(self: *const Proxy) void {
    const b = self.install_step.step.owner;
    b.getInstallStep().dependOn(&self.install_step.step);
}
