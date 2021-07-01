// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_SOURCERANGE_H
#define LIBCLANGUTILS_CLANG_SOURCERANGE_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-source-location.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class SourceRange
 */

class LIBCLANGU_API SourceRange
{
public:
  LibClang* api;
  CXSourceRange data;

public:

  /*!
   * \fn SourceRange() = delete 
   */
  SourceRange() = delete;

  /*!
   * \fn SourceRange(const SourceRange&) = default
   */
  SourceRange(const SourceRange&) = default;

  /*!
   * \fn ~SourceRange() = default
   */
  ~SourceRange() = default;

  /*!
   * \fn SourceRange& operator=(const SourceRange&) = default
   */
  SourceRange& operator=(const SourceRange&) = default;

  SourceRange(LibClang& lib, CXSourceRange sourcerange);

  bool isNull() const;

  SourceLocation getRangeStart() const;
  SourceLocation getRangeEnd() const;

  operator CXSourceRange() const;
};

/*!
 * \fn SourceRange(LibClang& lib, CXSourceRange sourcerange)
 */
inline SourceRange::SourceRange(LibClang& lib, CXSourceRange sourcerange)
  : api(&lib), data(sourcerange)
{

}

/*!
 * \fn operator CXSourceRange() const
 */
inline SourceRange::operator CXSourceRange() const
{
  return data;
}

/*!
 * \fn bool SourceRange::isNull() const
 */
inline bool SourceRange::isNull() const
{
  return api->clang_Range_isNull(*this);
}

/*!
 * \fn SourceLocation SourceRange::getRangeStart() const
 * \brief returns the range start
 */
inline SourceLocation SourceRange::getRangeStart() const
{
  return SourceLocation(*api, api->clang_getRangeStart(*this));
}

/*!
 * \fn SourceLocation SourceRange::getRangeEnd() const
 * \brief returns the range end
 */
inline SourceLocation SourceRange::getRangeEnd() const
{
  return SourceLocation(*api, api->clang_getRangeEnd(*this));
}

/*!
 * \endclass
 */

/*!
 * \endnamespace
 */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_SOURCERANGE_H
