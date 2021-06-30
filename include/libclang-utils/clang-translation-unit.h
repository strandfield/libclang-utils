// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H
#define LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H

#include "libclang-utils/libclang.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

class Cursor;
class TokenSet;
class File;

/*!
 * \class TranslationUnit
 * \brief represents a C++ translation unit
 */

class LIBCLANGU_API TranslationUnit
{
public:
  LibClang* api = nullptr;
  CXTranslationUnit translation_unit = nullptr;

  TranslationUnit() = default;

  TranslationUnit(LibClang& lib, CXTranslationUnit tu)
    : api(&lib), translation_unit(tu)
  {

  }

  TranslationUnit(const TranslationUnit&) = delete;

  TranslationUnit(TranslationUnit&& other) noexcept
    : api(other.api), translation_unit(other.translation_unit)
  {
    other.translation_unit = nullptr;
  }

  ~TranslationUnit();

  TranslationUnit& operator=(const TranslationUnit&) = delete;

  TranslationUnit& operator=(TranslationUnit&& other);

  operator CXTranslationUnit() const
  {
    return translation_unit;
  }

  Cursor getCursor() const;

  TokenSet tokenize(CXSourceRange range) const;

  File getFile(const char* path) const;
  File getFile(const std::string& path) const;
};

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H
