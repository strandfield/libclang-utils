// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-token.h"

namespace libclang
{

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

