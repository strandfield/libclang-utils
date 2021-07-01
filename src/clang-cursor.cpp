// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-cursor.h"

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class Cursor
 */

/*!
 * \fn size_t childCount() const
 * \brief returns the number of children of this cursor
 */
size_t Cursor::childCount() const
{
  size_t counter = 0;

  visitChildren([&counter](const Cursor&) {
    ++counter;
    });

  return counter;
}

/*!
 * \fn Cursor childAt(size_t index) const
 * \brief access a child by index
 */
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

/*!
 * \fn std::vector<Cursor> children() const
 * \brief returns the cursor's children
 */
std::vector<Cursor> Cursor::children() const
{
  std::vector<Cursor> result;

  visitChildren([&](const Cursor& c) {
    result.push_back(c);
    });

  return result;
}

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang
