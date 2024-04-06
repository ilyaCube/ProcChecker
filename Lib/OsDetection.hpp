#pragma once

# if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#   if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define OS_DARWIN
#     define OS_BSD4
#     ifdef __LP64__
#       define OS_DARWIN64
#     else
#       define OS_DARWIN32
#     endif
#     if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#       define PLATFORM_UIKIT
#       if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
#         define OS_WATCHOS
#       elif defined(TARGET_OS_TV) && TARGET_OS_TV
#         define OS_TVOS
#       else
#         define OS_IOS
#       endif
#     else
#       define OS_MACOS
#     endif
#   else
#     error "This Apple platform is not supported"
#   endif
# elif defined(__ANDROID__) || defined(ANDROID)
#   define OS_ANDROID
#   define OS_LINUX
# elif defined(__CYGWIN__)
#   define OS_CYGWIN
# elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#   define OS_WIN32
#   define OS_WIN64
# elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#   if defined(WINAPI_FAMILY)
#     ifndef WINAPI_FAMILY_PC_APP
#       define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#     endif
#     if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#       define OS_WINRT
#     elif WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
#       define OS_WINRT
#     else
#       define OS_WIN32
#     endif
#   else
#     define OS_WIN32
#   endif
# elif defined(__sun) || defined(sun)
#   define OS_SOLARIS
# elif defined(hpux) || defined(__hpux)
#   define OS_HPUX
# elif defined(__native_client__)
#   define OS_NACL
# elif defined(__EMSCRIPTEN__)
#   define OS_WASM
# elif defined(__linux__) || defined(__linux)
#   define OS_LINUX
# elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#   ifndef __FreeBSD_kernel__
#     define OS_FREEBSD
#   endif
#   define OS_FREEBSD_KERNEL
#   define OS_BSD4
# elif defined(__NetBSD__)
#   define OS_NETBSD
#   define OS_BSD4
# elif defined(__OpenBSD__)
#   define OS_OPENBSD
#   define OS_BSD4
# elif defined(__INTERIX)
#   define OS_INTERIX
#   define OS_BSD4
# elif defined(_AIX)
#   define OS_AIX
# elif defined(__Lynx__)
#   define OS_LYNX
# elif defined(__GNU__)
#   define OS_HURD
# elif defined(__QNXNTO__)
#   define OS_QNX
# elif defined(__INTEGRITY)
#   define OS_INTEGRITY
# elif defined(__rtems__)
#   define OS_RTEMS
# elif defined(VXWORKS)
#   define OS_VXWORKS
# elif defined(__HAIKU__)
#   define OS_HAIKU
# elif defined(__MAKEDEPEND__)
# else
#   error "This OS is not supported"
# endif

# if defined(OS_WIN32) || defined(OS_WIN64) || defined(OS_WINRT)
#   define OS_WINDOWS
#   define OS_WIN
# endif

# if defined(OS_WIN)
#   undef OS_UNIX
# elif !defined(OS_UNIX)
#   define OS_UNIX
# endif

# ifdef OS_DARWIN
# define OS_MAC
# endif
# ifdef OS_DARWIN32
# define OS_MAC32
# endif
# ifdef OS_DARWIN64
# define OS_MAC64
# endif
# ifdef OS_MACOS
# define OS_MACX
# define OS_OSX
# endif

# ifdef OS_DARWIN
#   include <Availability.h>
#   include <AvailabilityMacros.h>
# 
#   ifdef OS_MACOS
#     if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
#        undef __MAC_OS_X_VERSION_MIN_REQUIRED
#        define __MAC_OS_X_VERSION_MIN_REQUIRED __MAC_10_6
#     endif
#     if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#        undef MAC_OS_X_VERSION_MIN_REQUIRED
#        define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
#     endif
#   endif

#   if !defined(__MAC_10_11)
#        define __MAC_10_11 101100
#   endif
#   if !defined(__MAC_10_12)
#        define __MAC_10_12 101200
#   endif
#   if !defined(__MAC_10_13)
#        define __MAC_10_13 101300
#   endif
#   if !defined(__MAC_10_14)
#        define __MAC_10_14 101400
#   endif
#   if !defined(__MAC_10_15)
#        define __MAC_10_15 101500
#   endif
#   if !defined(MAC_OS_X_VERSION_10_11)
#        define MAC_OS_X_VERSION_10_11 __MAC_10_11
#   endif
#   if !defined(MAC_OS_X_VERSION_10_12)
#        define MAC_OS_X_VERSION_10_12 __MAC_10_12
#   endif
#   if !defined(MAC_OS_X_VERSION_10_13)
#        define MAC_OS_X_VERSION_10_13 __MAC_10_13
#   endif
#   if !defined(MAC_OS_X_VERSION_10_14)
#        define MAC_OS_X_VERSION_10_14 __MAC_10_14
#   endif
#   if !defined(MAC_OS_X_VERSION_10_15)
#        define MAC_OS_X_VERSION_10_15 __MAC_10_15
#   endif
# 
#   if !defined(__IPHONE_10_0)
#        define __IPHONE_10_0 100000
#   endif
#   if !defined(__IPHONE_10_1)
#        define __IPHONE_10_1 100100
#   endif
#   if !defined(__IPHONE_10_2)
#        define __IPHONE_10_2 100200
#   endif
#   if !defined(__IPHONE_10_3)
#        define __IPHONE_10_3 100300
#   endif
#   if !defined(__IPHONE_11_0)
#        define __IPHONE_11_0 110000
#   endif
#   if !defined(__IPHONE_12_0)
#        define __IPHONE_12_0 120000
#   endif
# endif

# ifdef __LSB_VERSION__
#   if __LSB_VERSION__ < 40
#     error "This version of the Linux Standard Base is unsupported"
#   endif

# ifndef _LINUXBASE
#   define _LINUXBASE
# endif
# endif
