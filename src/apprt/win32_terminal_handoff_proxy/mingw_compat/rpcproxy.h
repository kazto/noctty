/*
 * Compatibility shim for the x86_64-windows-gnu (mingw-w64) target.
 *
 * mingw-w64's <rpcproxy.h> tracks an older MIDL/Windows SDK revision and is
 * missing two things that current MIDL-generated stubless proxy code (like
 * ITerminalHandoff_p.c) relies on:
 *
 *   - ObjectStublessClient3..32: forwarders implemented in ole32.dll. The
 *     mingw-w64 import library (ole32.def.in) already exports these symbols,
 *     only the header declaration is missing.
 *   - CStdStubBuffer_METHODS_OPT: identical in content to the already
 *     present CStdStubBuffer_METHODS, just under the name newer MIDL output
 *     expects.
 *
 * Declarations below are copied from the Windows SDK's um/rpcproxy.h
 * (10.0.26100.0), minus the ARM64EC-only DECLSPEC_CHPE_GUEST annotation
 * which doesn't apply to x86_64.
 */
#pragma once

#include_next <rpcproxy.h>

#ifndef __NOCTTY_RPCPROXY_OBJECTSTUBLESSCLIENT_DEFINED
#define __NOCTTY_RPCPROXY_OBJECTSTUBLESSCLIENT_DEFINED

#if defined(__cplusplus)
extern "C" {
#endif

void WINAPI ObjectStublessClient3(void);
void WINAPI ObjectStublessClient4(void);
void WINAPI ObjectStublessClient5(void);
void WINAPI ObjectStublessClient6(void);
void WINAPI ObjectStublessClient7(void);
void WINAPI ObjectStublessClient8(void);
void WINAPI ObjectStublessClient9(void);
void WINAPI ObjectStublessClient10(void);
void WINAPI ObjectStublessClient11(void);
void WINAPI ObjectStublessClient12(void);
void WINAPI ObjectStublessClient13(void);
void WINAPI ObjectStublessClient14(void);
void WINAPI ObjectStublessClient15(void);
void WINAPI ObjectStublessClient16(void);
void WINAPI ObjectStublessClient17(void);
void WINAPI ObjectStublessClient18(void);
void WINAPI ObjectStublessClient19(void);
void WINAPI ObjectStublessClient20(void);
void WINAPI ObjectStublessClient21(void);
void WINAPI ObjectStublessClient22(void);
void WINAPI ObjectStublessClient23(void);
void WINAPI ObjectStublessClient24(void);
void WINAPI ObjectStublessClient25(void);
void WINAPI ObjectStublessClient26(void);
void WINAPI ObjectStublessClient27(void);
void WINAPI ObjectStublessClient28(void);
void WINAPI ObjectStublessClient29(void);
void WINAPI ObjectStublessClient30(void);
void WINAPI ObjectStublessClient31(void);
void WINAPI ObjectStublessClient32(void);

#if defined(__cplusplus)
}
#endif

#endif /* __NOCTTY_RPCPROXY_OBJECTSTUBLESSCLIENT_DEFINED */

#ifndef CStdStubBuffer_METHODS_OPT
#define CStdStubBuffer_METHODS_OPT \
    CStdStubBuffer_QueryInterface, \
    CStdStubBuffer_AddRef, \
    CStdStubBuffer_Release, \
    CStdStubBuffer_Connect, \
    CStdStubBuffer_Disconnect, \
    CStdStubBuffer_Invoke, \
    CStdStubBuffer_IsIIDSupported, \
    CStdStubBuffer_CountRefs, \
    CStdStubBuffer_DebugServerQueryInterface, \
    CStdStubBuffer_DebugServerRelease
#endif
