// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_FINDREFERENCESINFILE_H
#define LIBCLANGUTILS_FINDREFERENCESINFILE_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-cursor.h"
#include "libclang-utils/clang-source-range.h"

namespace libclang
{

/*!
 * \fn CXResult findReferencesInFile(const Cursor& cursor, const File& file, Func&& callback)
 */
template<typename Func>
CXResult findReferencesInFile(const Cursor& cursor, const File& file, Func&& callback);

namespace details
{

template<typename T>
struct FindReferencesInFileVisitor
{
  LibClang& libclang;
  T& functor;
};

template<typename T>
CXVisitorResult find_references_in_file_visit_proc(void* contextptr, CXCursor cursor, CXSourceRange source_range)
{
  FindReferencesInFileVisitor<T>& context = *static_cast<FindReferencesInFileVisitor<T>*>(contextptr);
  context.functor(Cursor(context.libclang, cursor), SourceRange(context.libclang, source_range));
  return CXVisit_Continue;
}

} // namespace details

template<typename Func>
inline CXResult findReferencesInFile(const Cursor& cursor, const File& file, Func&& callback)
{
  details::FindReferencesInFileVisitor<Func> context{ *cursor.api, callback };
  CXCursorAndRangeVisitor data;
  data.context = &context;
  data.visit = &details::find_references_in_file_visit_proc<Func>;
  return cursor.api->clang_findReferencesInFile(cursor, file, data);
}

} // namespace libclang


#endif // LIBCLANGUTILS_FINDREFERENCESINFILE_H
