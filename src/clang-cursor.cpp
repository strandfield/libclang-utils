// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-cursor.h"

namespace libclang
{

size_t Cursor::childCount() const
{
  size_t counter = 0;

  visitChildren([&counter](const Cursor&) {
    ++counter;
    });

  return counter;
}

Cursor Cursor::childAt(size_t index) const
{
  Cursor result{ *api, this->cursor };

  visitChildren([&result, &index](bool& stop_token, const Cursor& c) {
    if (index == 0)
    {
      result = c;
      stop_token = true;
    }
    --index;
    });

  return result;
}

std::vector<Cursor> Cursor::children() const
{
  std::vector<Cursor> result;

  visitChildren([&](const Cursor& c) {
    result.push_back(c);
    });

  return result;
}

} // namespace libclang

