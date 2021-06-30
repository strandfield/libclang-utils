// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TYPE_H
#define LIBCLANGUTILS_CLANG_TYPE_H

#include "libclang-utils/libclang.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class Type
 * \brief represents a C++ type
 */

class LIBCLANGU_API Type
{
public:
  LibClang* api;
  CXType data;

public:

  /*!
   * \fn Type() = delete 
   */
  Type() = delete;

  /*!
   * \fn Type(const Type&) = default
   */
  Type(const Type&) = default;

  /*!
   * \fn ~Type() = default
   */
  ~Type() = default;

  /*!
   * \fn Type& operator=(const Type&) = default
   */
  Type& operator=(const Type&) = default;

  Type(LibClang& lib, CXType type);

  std::string getSpelling() const;
  Type getResultType() const;

  operator CXType() const;

};

/*!
 * \fn Type(LibClang& lib, CXType type)
 */
inline Type::Type(LibClang& lib, CXType type)
  : api(&lib), data(type)
{

}

/*!
 * \fn std::string getSpelling() const
 * \brief returns a string representation of the type
 */
inline std::string Type::getSpelling() const
{
  return api->toStdString(api->clang_getTypeSpelling(data));
}

/*!
 * \fn Type getResultType() const
 * \brief returns the result type of a function type
 */
inline Type Type::getResultType() const
{
  return Type(*api, api->clang_getResultType(data));
}

/*!
 * \fn operator CXType() const
 */
inline Type::operator CXType() const
{
  return data;
}

/*!
 * \endclass
 */

/*!
 * \endnamespace
 */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_TYPE_H
