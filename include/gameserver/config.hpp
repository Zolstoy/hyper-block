#pragma once

#if defined(_WIN32)
 #define GAMESERVER_SYSTEM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
 #include "TargetConditionals.h"
 #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
  #define GAMESERVER_SYSTEM_IOS
 #elif TARGET_OS_MAC
  #define GAMESERVER_SYSTEM_MACOS
 #else
  #error This Apple operating system is not supported by GAMESERVER library
 #endif
#elif defined(__unix__)
 #if defined(__ANDROID__)
  #define GAMESERVER_SYSTEM_ANDROID
 #elif defined(__linux__)
  #define GAMESERVER_SYSTEM_LINUX
 #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
  #define GAMESERVER_SYSTEM_FREEBSD
 #else
  #error This UNIX operating system is not supported by GAMESERVER library
 #endif
#else
 #error This operating system is not supported by GAMESERVER library
#endif

#if !defined(GAMESERVER_STATIC)
 #if defined(GAMESERVER_SYSTEM_WINDOWS)
  #define GAMESERVER_SYMBOL_EXPORT __declspec(dllexport)
  #define GAMESERVER_SYMBOL_IMPORT __declspec(dllimport)
  #ifdef _MSC_VER
   #pragma warning(disable : 4251)
  #endif
 #else   // Linux, FreeBSD, Mac OS X
  #if __GNUC__ >= 4
   #define GAMESERVER_SYMBOL_EXPORT __attribute__((__visibility__("default")))
   #define GAMESERVER_SYMBOL_IMPORT __attribute__((__visibility__("default")))
  #else
   #define GAMESERVER_SYMBOL_EXPORT
   #define GAMESERVER_SYMBOL_IMPORT
  #endif
 #endif
#else
 #define GAMESERVER_SYMBOL_EXPORT
 #define GAMESERVER_SYMBOL_IMPORT
#endif

#if defined(GAMESERVER_BUILDING_THE_LIB)
 #define GAMESERVER_API GAMESERVER_SYMBOL_EXPORT
#else
 #define GAMESERVER_API GAMESERVER_SYMBOL_IMPORT
#endif
