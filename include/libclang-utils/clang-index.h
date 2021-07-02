// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_INDEX_H
#define LIBCLANGUTILS_CLANG_INDEX_H

#include "libclang-utils/libclang.h"

#include <set>

/*!
 * \namespace libclang
 */

namespace libclang
{

class TranslationUnit;

/*!
 * \class Index
 */

class LIBCLANGU_API Index
{
public:
  LibClang& api;
  CXIndex index;

  Index(const Index&) = delete;

  explicit Index(LibClang& lib);
  Index(Index&& other) noexcept;
  ~Index();

  TranslationUnit parseTranslationUnit(const std::string& file, const std::set<std::string>& includedirs, int options = 0);
};

/*! 
 * \fn explicit Index(LibClang& lib)
 */
inline Index::Index(LibClang& lib)
  : api(lib)
{
  index = api.clang_createIndex(0, 0);
}

/*!
 * \fn Index(Index&& other) noexcept
 */
inline Index::Index(Index&& other) noexcept
  : api(other.api),
  index(other.index)
{
  other.index = nullptr;
}

/*!
 * \fn ~Index()
 */
inline Index::~Index()
{
  if (this->index)
  {
    api.clang_disposeIndex(this->index);
  }
}

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_INDEX_H
