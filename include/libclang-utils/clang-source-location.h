// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_SOURCELOCATION_H
#define LIBCLANGUTILS_CLANG_SOURCELOCATION_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-file.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

struct SpellingLocation
{
  File file;
  unsigned int line, col, offset;

  SpellingLocation(File f, unsigned int l, unsigned int c, unsigned int off)
    : file(std::move(f)),
      line(l),
      col(c),
      offset(off)
  {

  }
};

/*!
 * \class SourceLocation
 */

class LIBCLANGU_API SourceLocation
{
public:
  LibClang* api;
  CXSourceLocation data;

public:

  /*!
   * \fn SourceLocation() = delete 
   */
  SourceLocation() = delete;

  /*!
   * \fn SourceLocation(const SourceLocation&) = default
   */
  SourceLocation(const SourceLocation&) = default;

  /*!
   * \fn ~SourceLocation() = default
   */
  ~SourceLocation() = default;

  /*!
   * \fn SourceLocation& operator=(const SourceLocation&) = default
   */
  SourceLocation& operator=(const SourceLocation&) = default;


  SourceLocation(LibClang& lib, CXSourceLocation sourceloc);

  operator CXSourceLocation() const;

  SpellingLocation getSpellingLocation() const;
};

/*!
 * \fn SourceLocation(LibClang& lib, CXSourceLocation sourceloc)
 */
inline SourceLocation::SourceLocation(LibClang& lib, CXSourceLocation sourceloc)
  : api(&lib), data(sourceloc)
{

}

/*!
 * \fn operator CXSourceLocation() const
 */
inline SourceLocation::operator CXSourceLocation() const
{
  return data;
}

/*!
 * \fn SpellingLocation getSpellingLocation() const
 * \brief maps the location to the file
 */
inline SpellingLocation SourceLocation::getSpellingLocation() const
{
  CXFile file;
  unsigned int line, col, offset;

  api->clang_getSpellingLocation(data, &file, &line, &col, &offset);

  return SpellingLocation(File(*api, file), line, col, offset);
}

/*!
 * \endclass
 */

/*!
 * \endnamespace
 */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_SOURCELOCATION_H
