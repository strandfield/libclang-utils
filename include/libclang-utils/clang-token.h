// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_TOKEN_H
#define LIBCLANGUTILS_CLANG_TOKEN_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-source-range.h"

#include <functional>

namespace libclang
{

LIBCLANGU_API const std::string& getSpelling(CXTokenKind k);

class LIBCLANGU_API Token
{
public:
  LibClang* api;
  CXTranslationUnit translation_unit;
  CXToken token;

public:
  Token() = delete;
  Token(const Token&) = default;
  ~Token() = default;
  Token& operator=(const Token&) = default;

  Token(LibClang& lib, CXTranslationUnit tu, CXToken tok);

  operator const CXToken& () const;

  CXTokenKind getKind() const;
  const std::string& getKindSpelling() const;

  std::string getSpelling() const;

  SourceLocation getLocation() const;
  SourceRange getExtent() const;
};

inline Token::Token(LibClang& lib, CXTranslationUnit tu, CXToken tok)
  : api(&lib), translation_unit(tu), token(tok)
{

}

inline Token::operator const CXToken& () const
{
  return this->token;
}

/**
 * \brief determine the kind of the token
 */
inline CXTokenKind Token::getKind() const
{
  return api->clang_getTokenKind(*this);
}

class LIBCLANGU_API TokenSet
{
public:
  LibClang* api;
  CXTranslationUnit translation_unit;

protected:
  CXToken* tokens;
  size_t m_size;

public:
  TokenSet();
  TokenSet(LibClang& lib, CXTranslationUnit tu, CXToken* toks, size_t size);
  TokenSet(const TokenSet&) = delete;
  TokenSet(TokenSet&& other) noexcept;
  ~TokenSet();

  TokenSet& operator=(const TokenSet&) = delete;
  TokenSet& operator=(TokenSet&& other) noexcept;

  CXToken* data() const;

  bool empty() const;
  size_t size() const;
  Token at(size_t i) const;

  std::string getSpelling() const;
};

/**
 * \brief constructs an empty token set
 */
inline TokenSet::TokenSet()
  : api(nullptr), translation_unit(nullptr), tokens(nullptr), m_size(0)
{

}

inline TokenSet::TokenSet(LibClang& lib, CXTranslationUnit tu, CXToken* toks, size_t size)
  : api(&lib), translation_unit(tu), tokens(toks), m_size(size)
{

}

inline TokenSet::TokenSet(TokenSet&& other) noexcept
  : api(other.api), translation_unit(other.translation_unit), tokens(other.tokens), m_size(other.m_size)
{
  other.api = nullptr;
  other.tokens = nullptr;
  other.m_size = 0;
}

inline TokenSet::~TokenSet()
{
  if (api && m_size)
  {
    api->clang_disposeTokens(translation_unit, tokens, static_cast<unsigned>(m_size));
  }
}

inline TokenSet& TokenSet::operator=(TokenSet&& other) noexcept
{
  api = other.api;
  tokens = other.tokens;
  m_size = other.m_size;

  other.api = nullptr;
  other.tokens = nullptr;
  other.m_size = 0;

  return *this;
}

inline CXToken* TokenSet::data() const
{
  return tokens;
}

/**
 * \brief returns whether the set is empty
 */
inline bool TokenSet::empty() const
{
  return size() == 0;
}

/**
 * \brief returns the number of token in the set
 */
inline size_t TokenSet::size() const
{
  return m_size;
}

/**
 * \brief retrieves a token from the set
 */
inline Token TokenSet::at(size_t i) const
{
  return Token(*api, translation_unit, this->tokens[i]);
}

} // namespace libclang

#endif // LIBCLANGUTILS_CLANG_TOKEN_H
