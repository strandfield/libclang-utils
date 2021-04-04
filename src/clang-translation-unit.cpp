// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-translation-unit.h"

#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-token.h"

namespace libclang
{

TranslationUnit::~TranslationUnit()
{
  if(translation_unit)
    api->clang_disposeTranslationUnit(translation_unit);
}

TranslationUnit& TranslationUnit::operator=(TranslationUnit&& other)
{
  this->api = other.api;

  if (this->translation_unit)
    api->clang_disposeTranslationUnit(this->translation_unit);

  this->translation_unit = other.translation_unit;
  other.translation_unit = nullptr;

  return *(this);
}

Cursor TranslationUnit::getCursor() const
{
  CXCursor c = api->clang_getTranslationUnitCursor(this->translation_unit);
  return Cursor{ *api, c };
}

TokenSet TranslationUnit::tokenize(CXSourceRange range) const
{
  CXToken* tokens = nullptr;
  unsigned int size = 0;
  api->clang_tokenize(translation_unit, range, &tokens, &size);
  return TokenSet{ *api, translation_unit, tokens, size };
}

} // namespace libclang

