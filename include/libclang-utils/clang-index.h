// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_INDEX_H
#define LIBCLANGUTILS_CLANG_INDEX_H

#include "libclang-utils/libclang.h"

#include <set>

namespace libclang
{

class TranslationUnit;

class LIBCLANGU_API Index
{
public:
  LibClang& api;
  CXIndex index;

  explicit Index(LibClang& lib)
    : api(lib)
  {
    index = api.clang_createIndex(0, 0);
  }

  Index(const Index&) = delete;
  
  Index(Index&& other) noexcept
    : api(other.api),
      index(other.index)
  {
    other.index = nullptr;
  }

  ~Index()
  {
    if (this->index)
    {
      api.clang_disposeIndex(this->index);
    }
  }

  TranslationUnit parseTranslationUnit(const std::string& file, const std::set<std::string>& includedirs, int options = 0);
};

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_INDEX_H
