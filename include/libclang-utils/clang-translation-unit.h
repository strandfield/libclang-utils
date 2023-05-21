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
class Token;
class TokenSet;
class File;
class SourceLocation;
class SourceRange;

/*!
 * \class TranslationUnit
 * \brief represents a C++ translation unit
 */

class LIBCLANGU_API TranslationUnit
{
public:
  LibClang* api = nullptr;
  CXTranslationUnit translation_unit = nullptr;

  /*!
   * \fn TranslationUnit() = default
   */
  TranslationUnit() = default;

  /*!
   * \fn TranslationUnit(const TranslationUnit&) = delete
   */
  TranslationUnit(const TranslationUnit&) = delete;

  TranslationUnit(LibClang& lib, CXTranslationUnit tu);
  TranslationUnit(TranslationUnit&& other) noexcept;

  ~TranslationUnit();

  /*!
   * \fn TranslationUnit& operator=(const TranslationUnit&) = delete
   */
  TranslationUnit& operator=(const TranslationUnit&) = delete;

  TranslationUnit& operator=(TranslationUnit&& other);

  CXSaveError saveTranslationUnit(const std::string& filename, unsigned options = CXSaveTranslationUnit_None) const;

  void suspendTranslationUnit();
  CXErrorCode reparseTranslationUnit();

  std::string getTranslationUnitSpelling() const;

  Cursor getCursor() const;
  Cursor getCursor(const SourceLocation& loc) const;

  Token getToken(const SourceLocation& loc) const;
  [[deprecated("use SourceRange overload")]] TokenSet tokenize(CXSourceRange range) const;
  TokenSet tokenize(const SourceRange& range) const;

  File getFile(const char* path) const;
  File getFile(const std::string& path) const;
  SourceLocation getLocation(const File& file, int line, int column) const;

  bool isFileMultipleIncludeGuarded(const File& f) const;
  const char* getFileContents(const File& f, size_t* bufsize = nullptr) const;

  operator CXTranslationUnit() const;
};

/*!
 * \fn TranslationUnit(LibClang& lib, CXTranslationUnit tu)
 */
inline TranslationUnit::TranslationUnit(LibClang& lib, CXTranslationUnit tu)
  : api(&lib), translation_unit(tu)
{

}

/*!
 * \fn TranslationUnit(TranslationUnit&& other) noexcept
 */
inline TranslationUnit::TranslationUnit(TranslationUnit&& other) noexcept
  : api(other.api), translation_unit(other.translation_unit)
{
  other.translation_unit = nullptr;
}

/*!
 * \fn operator CXTranslationUnit() const
 */
inline TranslationUnit::operator CXTranslationUnit() const
{
  return translation_unit;
}

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_TRANSLATION_UNIT_H
