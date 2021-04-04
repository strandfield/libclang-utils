// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-cursor.h"

namespace cxx
{

size_t ClangCursor::childCount() const
{
  size_t counter = 0;

  visitChildren([&counter](const ClangCursor&) {
    ++counter;
    });

  return counter;
}

ClangCursor ClangCursor::childAt(size_t index) const
{
  ClangCursor result{ *libclang, this->cursor };

  visitChildren([&result, &index](bool& stop_token, const ClangCursor& c) {
    if (index == 0)
    {
      result = c;
      stop_token = true;
    }
    --index;
    });

  return result;
}

std::vector<ClangCursor> ClangCursor::children() const
{
  std::vector<ClangCursor> result;

  visitChildren([&](const ClangCursor& c) {
    result.push_back(c);
    });

  return result;
}

} // namespace cxx

