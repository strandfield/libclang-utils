// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_FINDINCLUDESINFILE_H
#define LIBCLANGUTILS_FINDINCLUDESINFILE_H

#include "libclang-utils/cursorandrangevisitor.h"

#include "libclang-utils/clang-translation-unit.h"

namespace libclang
{

/*!
 * \fn CXResult findIncludesInFile(const Cursor& cursor, const File& file, Func&& callback)
 */
template<typename Func>
CXResult findIncludesInFile(const TranslationUnit& tu, const File& file, Func&& callback)
{
  details::CursorAndRangeVisitor<Func> context{ *tu.api, callback };
  CXCursorAndRangeVisitor data;
  data.context = &context;
  data.visit = &details::cursor_and_range_visit_proc<Func>;
  return tu.api->clang_findIncludesInFile(tu, file, data);
}

} // namespace libclang

#endif // LIBCLANGUTILS_FINDINCLUDESINFILE_H
