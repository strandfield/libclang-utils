// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-translation-unit.h"

#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-token.h"

namespace cxx
{

ClangTranslationUnit::~ClangTranslationUnit()
{
  if(translation_unit)
    libclang->clang_disposeTranslationUnit(translation_unit);
}

ClangTranslationUnit& ClangTranslationUnit::operator=(ClangTranslationUnit&& other)
{
  this->libclang = other.libclang;

  if (this->translation_unit)
    libclang->clang_disposeTranslationUnit(this->translation_unit);

  this->translation_unit = other.translation_unit;
  other.translation_unit = nullptr;

  return *(this);
}

ClangCursor ClangTranslationUnit::getCursor() const
{
  CXCursor c = libclang->clang_getTranslationUnitCursor(this->translation_unit);
  return ClangCursor{ *libclang, c };
}

ClangTokenSet ClangTranslationUnit::tokenize(CXSourceRange range) const
{
  CXToken* tokens = nullptr;
  unsigned int size = 0;
  libclang->clang_tokenize(translation_unit, range, &tokens, &size);
  return ClangTokenSet{ *libclang, translation_unit, tokens, size };
}

} // namespace cxx

