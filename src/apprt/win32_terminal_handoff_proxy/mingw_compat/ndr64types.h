/*
 * Compatibility shim for the x86_64-windows-gnu (mingw-w64) target.
 *
 * mingw-w64's <ndr64types.h> defines every other NDR64_* format struct but
 * omits NDR64_SYSTEM_HANDLE_FORMAT, which MIDL emits for any IDL parameter
 * using the `system_handle(...)` attribute (ITerminalHandoff.idl uses it on
 * every HANDLE parameter). The layout below is copied verbatim from the
 * Windows SDK's um/ndr64types.h (10.0.26100.0) so the static format tables
 * MIDL generates are byte-for-byte identical to what MSVC would produce.
 */
#pragma once

#include_next <ndr64types.h>

#ifndef __NOCTTY_NDR64_SYSTEM_HANDLE_FORMAT_DEFINED
#define __NOCTTY_NDR64_SYSTEM_HANDLE_FORMAT_DEFINED

#include <pshpack8.h>

typedef struct _NDR64_SYSTEM_HANDLE_FORMAT {
    NDR64_FORMAT_CHAR FormatCode;
    NDR64_UINT8 HandleType;
    NDR64_UINT32 DesiredAccess;
} NDR64_SYSTEM_HANDLE_FORMAT;

#include <poppack.h>

#endif /* __NOCTTY_NDR64_SYSTEM_HANDLE_FORMAT_DEFINED */
