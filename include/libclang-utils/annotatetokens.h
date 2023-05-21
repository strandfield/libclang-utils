// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_ANNOTATETOKENS_H
#define LIBCLANGUTILS_ANNOTATETOKENS_H

#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-token.h"
#include "libclang-utils/clang-translation-unit.h"

#include <vector>

namespace libclang
{

template<typename F>
void annotateTokens(const TranslationUnit& tu, const TokenSet& tokens, F&& fun)
{
  std::vector<CXCursor> cursors;
  cursors.resize(tokens.size());

  tu.api->clang_annotateTokens(tu, tokens.data(), static_cast<unsigned int>(tokens.size()), cursors.data());

  for (size_t i(0); i < tokens.size(); ++i)
  {
    fun(tokens.at(i), Cursor(*tu.api, cursors.at(i)));
  }
}

/**
 * \brief exposes clang_annotateTokens()
 */
inline std::vector<Cursor> annotateTokens(const TranslationUnit& tu, const TokenSet& tokens)
{
  std::vector<Cursor> result;
  result.reserve(tokens.size());

  annotateTokens(tu, tokens, [&result](const Token&, const Cursor& c) {
    result.push_back(c);
    });

  return result;
}

} // namespace libclang

#endif // LIBCLANGUTILS_ANNOTATETOKENS_H
