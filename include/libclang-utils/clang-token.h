// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TOKEN_H
#define LIBCLANGUTILS_CLANG_TOKEN_H

#include "libclang-utils/libclang.h"

#include <functional>

namespace cxx
{

class LIBCLANGU_API ClangToken
{
public:
  LibClang* libclang;
  CXTranslationUnit translation_unit;
  CXToken token;

  ClangToken(LibClang& lib, CXTranslationUnit tu, CXToken tok)
    : libclang(&lib), translation_unit(tu), token(tok)
  {

  }

  ClangToken(const ClangToken&) = default;

  ~ClangToken()
  {

  }

  ClangToken& operator=(const ClangToken&) = default;

  operator const CXToken&() const
  {
    return this->token;
  }

  std::string getSpelling() const
  {
    CXString str = libclang->clang_getTokenSpelling(this->translation_unit, this->token);
    std::string result = libclang->clang_getCString(str);
    libclang->clang_disposeString(str);
    return result;
  }

  CXSourceRange getExtent() const
  {
    return libclang->clang_getTokenExtent(this->translation_unit, this->token);
  }
};

class LIBCLANGU_API ClangTokenSet
{
public:
  LibClang* libclang;
  CXTranslationUnit translation_unit;

protected:
  CXToken* tokens;
  size_t m_size;

public:
  ClangTokenSet()
    : libclang(nullptr), translation_unit(nullptr), tokens(nullptr), m_size(0)
  {

  }

  ClangTokenSet(LibClang& lib, CXTranslationUnit tu, CXToken* toks, size_t size)
    : libclang(&lib), translation_unit(tu), tokens(toks), m_size(size)
  {

  }

  ClangTokenSet(const ClangTokenSet&) = delete;

  ClangTokenSet(ClangTokenSet&& other) noexcept
    : libclang(other.libclang), translation_unit(other.translation_unit), tokens(other.tokens), m_size(other.m_size)
  {
    other.libclang = nullptr;
    other.tokens = nullptr;
    other.m_size = 0;
  }

  ~ClangTokenSet()
  {
    if (libclang && m_size)
    {
      libclang->clang_disposeTokens(translation_unit, tokens, static_cast<unsigned>(m_size));
    }
  }

  ClangTokenSet& operator=(const ClangTokenSet&) = delete;

  ClangTokenSet& operator=(ClangTokenSet&& other) noexcept
  {

    libclang = other.libclang;
    tokens = other.tokens;
    m_size = other.m_size;

    other.libclang = nullptr;
    other.tokens = nullptr;
    other.m_size = 0;

    return *this;
  }

  size_t size() const
  {
    return m_size;
  }

  ClangToken at(size_t i) const
  {
    return ClangToken{ *libclang, translation_unit, this->tokens[i] };
  }
};

} // namespace cxx

#endif // LIBCLANGUTILS_CLANG_TOKEN_H
