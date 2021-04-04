// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_INDEX_H
#define LIBCLANGUTILS_CLANG_INDEX_H

#include "libclang-utils/libclang.h"

#include <set>

namespace cxx
{

class ClangTranslationUnit;

class LIBCLANGU_API ClangIndex
{
public:
  LibClang& libclang;
  CXIndex index;

  explicit ClangIndex(LibClang& lib)
    : libclang(lib)
  {
    index = libclang.clang_createIndex(0, 0);
  }

  ClangIndex(const ClangIndex&) = delete;
  
  ClangIndex(ClangIndex&& other) noexcept
    : libclang(other.libclang),
      index(other.index)
  {
    other.index = nullptr;
  }

  ~ClangIndex()
  {
    if (this->index)
    {
      libclang.clang_disposeIndex(this->index);
    }
  }

  ClangTranslationUnit parseTranslationUnit(const std::string& file, const std::set<std::string>& includedirs, int options = 0);
};

} // namespace cxx

#endif // LIBCLANGUTILS_CLANG_INDEX_H
