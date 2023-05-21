// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-token.h"

namespace libclang
{

/**
 * \brief returns a string representation of a token kind
 */
const std::string& getSpelling(CXTokenKind k)
{
  static const std::string Punctuation = "Punctuation";
  static const std::string Keyword = "Keyword";
  static const std::string Identifier = "Identifier";
  static const std::string Literal = "Literal";
  static const std::string Comment = "Comment";
  static const std::string Invalid = "Invalid";

  switch (k)
  {
  case CXToken_Punctuation:
    return Punctuation;
  case CXToken_Keyword:
    return Keyword;
  case CXToken_Identifier:
    return Identifier;
  case CXToken_Literal:
    return Literal;
  case CXToken_Comment:
    return Comment;
  default:
    return Invalid;
  }
}

const std::string& Token::getKindSpelling() const
{
  return libclang::getSpelling(getKind());
}

/**
 * \brief determine the spelling of the token
 */
std::string Token::getSpelling() const
{
  CXString str = api->clang_getTokenSpelling(this->translation_unit, this->token);
  return api->toStdString(str);
}

/**
 * \brief retrieve the source location of the token
 */
SourceLocation Token::getLocation() const
{
  return SourceLocation(*api, api->clang_getTokenLocation(translation_unit, token));
}

/**
 * \brief retrieve a source range that covers the token
 */
SourceRange Token::getExtent() const
{
  return SourceRange(*api, api->clang_getTokenExtent(this->translation_unit, this->token));
}

std::string TokenSet::getSpelling() const
{
  if (size() == 0)
    return std::string();

  std::string result = at(0).getSpelling();

  libclang::SourceRange range = at(0).getExtent();
  libclang::SpellingLocation loc = range.getRangeEnd().getSpellingLocation();

  for (size_t i = 1; i < size(); i++)
  {
    range = at(i).getExtent();
    libclang::SpellingLocation tokloc = range.getRangeStart().getSpellingLocation();

    if (tokloc.line != loc.line || tokloc.col != loc.col)
      result.push_back(' ');

    result += at(i).getSpelling();

    loc = range.getRangeEnd().getSpellingLocation();
  }

  return result;
}

} // namespace libclang

