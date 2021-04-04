// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H
#define LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H

#include "libclang-utils/libclang.h"

namespace cxx
{

class ClangCursor;
class ClangTokenSet;

class LIBCLANGU_API ClangTranslationUnit
{
public:
  LibClang* libclang = nullptr;
  CXTranslationUnit translation_unit = nullptr;

  ClangTranslationUnit() = default;

  ClangTranslationUnit(LibClang& lib, CXTranslationUnit tu)
    : libclang(&lib), translation_unit(tu)
  {

  }

  ClangTranslationUnit(const ClangTranslationUnit&) = delete;

  ClangTranslationUnit(ClangTranslationUnit&& other) noexcept
    : libclang(other.libclang), translation_unit(other.translation_unit)
  {
    other.translation_unit = nullptr;
  }

  ~ClangTranslationUnit();

  ClangTranslationUnit& operator=(const ClangTranslationUnit&) = delete;

  ClangTranslationUnit& operator=(ClangTranslationUnit&& other);

  operator CXTranslationUnit() const
  {
    return translation_unit;
  }

  ClangCursor getCursor() const;

  ClangTokenSet tokenize(CXSourceRange range) const;
};

} // namespace cxx

#endif // LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H
