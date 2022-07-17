// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_FILE_H
#define LIBCLANGUTILS_CLANG_FILE_H

#include "libclang-utils/libclang.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class File
 * \brief represents a file
 */

class LIBCLANGU_API File
{
public:
  LibClang* api;
  CXFile data;

public:

  /*!
   * \fn File() = delete 
   */
  File() = delete;

  /*!
   * \fn File(const File&) = default
   */
  File(const File&) = default;

  /*!
   * \fn ~File() = default
   */
  ~File() = default;

  /*!
   * \fn File& operator=(const File&) = default
   */
  File& operator=(const File&) = default;

  File(LibClang& lib, CXFile file);

  std::string getFileName() const;

  operator CXFile() const;
};

/*!
 * \fn File(LibClang& lib, CXFile file)
 */
inline File::File(LibClang& lib, CXFile file)
  : api(&lib), data(file)
{

}

/*!
 * \fn std::string getFileName() const
 * \brief returns the file's name
 */
inline std::string File::getFileName() const
{
  return api->toStdString(api->clang_getFileName(*this));
}

/*!
 * \fn operator CXFile() const
 */
inline File::operator CXFile() const
{
  return data;
}

/*!
 * \endclass
 */

inline bool operator==(const File& lhs, const File& rhs)
{
  return lhs.api->clang_File_isEqual(lhs, rhs);
}

inline bool operator!=(const File& lhs, const File& rhs)
{
  return !(lhs == rhs);
}

/*!
 * \endnamespace
 */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_FILE_H
