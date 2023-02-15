// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-translation-unit.h"

#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-file.h"
#include "libclang-utils/clang-token.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class TranslationUnit
 */

/*!
 * \fn ~TranslationUnit()
 */
TranslationUnit::~TranslationUnit()
{
  if(translation_unit)
    api->clang_disposeTranslationUnit(translation_unit);
}

/*!
 * \fn TranslationUnit& operator=(TranslationUnit&& other)
 */
TranslationUnit& TranslationUnit::operator=(TranslationUnit&& other)
{
  if (this->api && this->translation_unit)
    this->api->clang_disposeTranslationUnit(this->translation_unit);

  this->api = other.api;
  this->translation_unit = other.translation_unit;
  other.translation_unit = nullptr;

  return *(this);
}

/*!
 * \fn Cursor getCursor() const
 */
Cursor TranslationUnit::getCursor() const
{
  CXCursor c = api->clang_getTranslationUnitCursor(this->translation_unit);
  return Cursor{ *api, c };
}

/*!
 * \fn CXSaveError saveTranslationUnit(const std::string& filename, unsigned options = CXSaveTranslationUnit_None) const
 * \brief saves the translation unit
 */
CXSaveError TranslationUnit::saveTranslationUnit(const std::string& filename, unsigned options) const
{
  int r = api->clang_saveTranslationUnit(this->translation_unit, filename.c_str(), options);
  return static_cast<CXSaveError>(r);
}

/*!
 * \fn void suspendTranslationUnit()
 * \brief suspends a translation unit
 * 
 * Use \m reparseTranslationUnit() to resume the translation unit.
 */
void TranslationUnit::suspendTranslationUnit()
{
  api->clang_suspendTranslationUnit(*this);
}

/*!
 * \fn CXErrorCode reparseTranslationUnit()
 * \brief reparse a translation unit
 */
CXErrorCode TranslationUnit::reparseTranslationUnit()
{
  unsigned options = api->clang_defaultReparseOptions(*this);
  int err = api->clang_reparseTranslationUnit(*this, 0, nullptr, options);
  return static_cast<CXErrorCode>(err);
}

/*!
 * \fn TokenSet tokenize(CXSourceRange range) const
 */
TokenSet TranslationUnit::tokenize(CXSourceRange range) const
{
  CXToken* tokens = nullptr;
  unsigned int size = 0;
  api->clang_tokenize(translation_unit, range, &tokens, &size);
  return TokenSet{ *api, translation_unit, tokens, size };
}

/*!
 * \fn File getFile(const char* path) const
 */
File TranslationUnit::getFile(const char* path) const
{
  return File(*api, api->clang_getFile(*this, path));
}

/*!
 * \fn File getFile(const std::string& path) const
 */
File TranslationUnit::getFile(const std::string& path) const
{
  return File(*api, api->clang_getFile(*this, path.c_str()));
}

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang

