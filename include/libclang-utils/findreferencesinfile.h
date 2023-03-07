// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_FINDREFERENCESINFILE_H
#define LIBCLANGUTILS_FINDREFERENCESINFILE_H

#include "libclang-utils/cursorandrangevisitor.h"

namespace libclang
{

/*!
 * \fn CXResult findReferencesInFile(const Cursor& cursor, const File& file, Func&& callback)
 */
template<typename Func>
CXResult findReferencesInFile(const Cursor& cursor, const File& file, Func&& callback)
{
  details::CursorAndRangeVisitor<Func> context{ *cursor.api, callback };
  CXCursorAndRangeVisitor data;
  data.context = &context;
  data.visit = &details::cursor_and_range_visit_proc<Func>;
  return cursor.api->clang_findReferencesInFile(cursor, file, data);
}

} // namespace libclang

#endif // LIBCLANGUTILS_FINDREFERENCESINFILE_H
