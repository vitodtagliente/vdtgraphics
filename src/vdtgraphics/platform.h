/// Copyright (c) Vito Domenico Tagliente

#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
#define PLATFORM_WINDOWS 1
#elif defined( __linux__ )
#define PLATFORM_LINUX 1
#elif defined( __APPLE__ )
#define PLATFORM_OSX 1
#endif

#define USE_OPENGL PLATFORM_WINDOWS | PLATFORM_LINUX | PLATFORM_OSX