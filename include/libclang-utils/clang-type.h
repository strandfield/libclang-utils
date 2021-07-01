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

  CXTypeKind kind() const;

  std::string getSpelling() const;
  Type getResultType() const;
  Type getPointeeType() const;

  bool isConstQualified() const;
  bool isVolatileQualified() const;

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
 * \fn CXTypeKind kind() const
 * \brief returns the type's kind
 */
inline CXTypeKind Type::kind() const
{
  return data.kind;
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
 * \fn Type getPointeeType() const
 * \brief returns the type pointed to
 */
inline Type Type::getPointeeType() const
{
  return Type(*api, api->clang_getPointeeType(data));
}

/*!
 * \fn bool isConstQualified() const
 * \brief returns whether the type is const qualified
 */
inline bool Type::isConstQualified() const
{
  return api->clang_isConstQualifiedType(*this);
}

/*!
 * \fn bool isVolatileQualified() const
 * \brief returns whether the type is volatile qualified
 */
inline bool Type::isVolatileQualified() const
{
  return api->clang_isVolatileQualifiedType(*this);
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
