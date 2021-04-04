// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_DEFS_H
#define LIBCLANGUTILS_DEFS_H

#if (defined(WIN32) || defined(_WIN32)) && !defined(LIBCLANGUTILS_LIBRARY_STATIC)

#if defined(LIBCLANGUTILS_BUILD_LIBRARY_SHARED)
#  define LIBCLANGU_API __declspec(dllexport)
#else
#  define LIBCLANGU_API __declspec(dllimport)
#endif

#else

#  define LIBCLANGU_API

#endif

#endif // LIBCLANGUTILS_DEFS_H
