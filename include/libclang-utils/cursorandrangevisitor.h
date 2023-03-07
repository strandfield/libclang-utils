// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CURSORANDRANGEVISITOR_H
#define LIBCLANGUTILS_CURSORANDRANGEVISITOR_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-source-range.h"

namespace libclang
{

namespace details
{

template<typename T>
struct CursorAndRangeVisitor
{
  LibClang& libclang;
  T& functor;
};

template<typename T>
CXVisitorResult cursor_and_range_visit_proc(void* contextptr, CXCursor cursor, CXSourceRange source_range)
{
  auto& context = *static_cast<CursorAndRangeVisitor<T>*>(contextptr);
  context.functor(Cursor(context.libclang, cursor), SourceRange(context.libclang, source_range));
  return CXVisit_Continue;
}

} // namespace details

} // namespace libclang

#endif // LIBCLANGUTILS_CURSORANDRANGEVISITOR_H
