// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TOKEN_H
#define LIBCLANGUTILS_CLANG_TOKEN_H

#include "libclang-utils/libclang.h"

#include <functional>

namespace libclang
{

class LIBCLANGU_API Token
{
public:
  LibClang* api;
  CXTranslationUnit translation_unit;
  CXToken token;

  Token(LibClang& lib, CXTranslationUnit tu, CXToken tok)
    : api(&lib), translation_unit(tu), token(tok)
  {

  }

  Token(const Token&) = default;

  ~Token()
  {

  }

  Token& operator=(const Token&) = default;

  operator const CXToken&() const
  {
    return this->token;
  }

  std::string getSpelling() const
  {
    CXString str = api->clang_getTokenSpelling(this->translation_unit, this->token);
    std::string result = api->clang_getCString(str);
    api->clang_disposeString(str);
    return result;
  }

  CXSourceRange getExtent() const
  {
    return api->clang_getTokenExtent(this->translation_unit, this->token);
  }
};

class LIBCLANGU_API TokenSet
{
public:
  LibClang* api;
  CXTranslationUnit translation_unit;

protected:
  CXToken* tokens;
  size_t m_size;

public:
  TokenSet()
    : api(nullptr), translation_unit(nullptr), tokens(nullptr), m_size(0)
  {

  }

  TokenSet(LibClang& lib, CXTranslationUnit tu, CXToken* toks, size_t size)
    : api(&lib), translation_unit(tu), tokens(toks), m_size(size)
  {

  }

  TokenSet(const TokenSet&) = delete;

  TokenSet(TokenSet&& other) noexcept
    : api(other.api), translation_unit(other.translation_unit), tokens(other.tokens), m_size(other.m_size)
  {
    other.api = nullptr;
    other.tokens = nullptr;
    other.m_size = 0;
  }

  ~TokenSet()
  {
    if (api && m_size)
    {
      api->clang_disposeTokens(translation_unit, tokens, static_cast<unsigned>(m_size));
    }
  }

  TokenSet& operator=(const TokenSet&) = delete;

  TokenSet& operator=(TokenSet&& other) noexcept
  {

    api = other.api;
    tokens = other.tokens;
    m_size = other.m_size;

    other.api = nullptr;
    other.tokens = nullptr;
    other.m_size = 0;

    return *this;
  }

  size_t size() const
  {
    return m_size;
  }

  Token at(size_t i) const
  {
    return Token{ *api, translation_unit, this->tokens[i] };
  }
};

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_TOKEN_H
