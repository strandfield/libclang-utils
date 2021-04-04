// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_CURSOR_H
#define LIBCLANGUTILS_CLANG_CURSOR_H

#include "libclang-utils/libclang.h"

#include <functional>
#include <utility>
#include <vector>

namespace cxx
{

class LIBCLANGU_API ClangCursor
{
public:
  LibClang* libclang;
  CXCursor cursor;

  ClangCursor(LibClang& lib, CXCursor c)
    : libclang(&lib), cursor(c)
  {

  }

  ClangCursor(const ClangCursor&) = default;

  ~ClangCursor()
  {

  }

  ClangCursor& operator=(const ClangCursor&) = default;

  operator CXCursor() const
  {
    return cursor;
  }

  CXCursorKind kind() const
  {
    return libclang->clang_getCursorKind(this->cursor);
  }

  bool isDeclaration() const
  {
    return libclang->clang_isDeclaration(kind());
  }

  bool isExpression() const
  {
    return libclang->clang_isExpression(kind());
  }

  bool isPreprocessing() const
  {
    return libclang->clang_isPreprocessing(kind());
  }

  bool isReference() const
  {
    return libclang->clang_isReference(kind());
  }

  bool isStatement() const
  {
    return libclang->clang_isStatement(kind());
  }

  bool isUnexposed() const
  {
    return libclang->clang_isUnexposed(kind());
  }

  std::string getSpelling() const
  {
    CXString str = libclang->clang_getCursorSpelling(this->cursor);
    std::string result = libclang->clang_getCString(str);
    libclang->clang_disposeString(str);
    return result;
  }

  std::string getCursorKindSpelling() const
  {
    CXString str = libclang->clang_getCursorKindSpelling(kind());
    std::string result = libclang->clang_getCString(str);
    libclang->clang_disposeString(str);
    return result;
  }

  ClangCursor getLexicalParent() const
  {
    CXCursor c = libclang->clang_getCursorLexicalParent(this->cursor);
    return ClangCursor{ *libclang, c };
  }

  ClangCursor getSemanticParent() const
  {
    CXCursor c = libclang->clang_getCursorSemanticParent(this->cursor);
    return ClangCursor{ *libclang, c };
  }

  CXSourceRange getExtent() const
  {
    return libclang->clang_getCursorExtent(this->cursor);
  }

  CXType getType() const
  {
    return libclang->clang_getCursorType(this->cursor);
  }

  CX_CXXAccessSpecifier getCXXAccessSpecifier() const
  {
    return libclang->clang_getCXXAccessSpecifier(this->cursor);
  }

  int getExceptionSpecificationType() const
  {
    return libclang->clang_getCursorExceptionSpecificationType(this->cursor);
  }

  template<typename T>
  struct VisitorData
  {
    LibClang& libclang;
    T& functor;
    bool should_break = false;
  };

  struct VisitorSelector1 {};
  struct VisitorSelector2 : VisitorSelector1 {};

  template<typename F>
  static void visitor_invoker(F&& func, bool& stop_token, const ClangCursor& cursor, VisitorSelector1)
  {
    func(cursor);
  }

  template<typename F, typename = decltype(std::declval<F>()(std::declval<bool&>(), std::declval<const ClangCursor&>()))>
  static void visitor_invoker(F&& func, bool& stop_token, const ClangCursor& cursor, VisitorSelector2)
  {
    func(stop_token, cursor);
  }

  template<typename T>
  static CXChildVisitResult generic_visit_callback(CXCursor c, CXCursor p, CXClientData client_data)
  {
    VisitorData<T>& data = *static_cast<VisitorData<T>*>(client_data);
    visitor_invoker(data.functor, data.should_break, ClangCursor{ data.libclang, c }, VisitorSelector2{});
    return data.should_break ? CXChildVisit_Break : CXChildVisit_Continue;
  }

  template<typename Func>
  void visitChildren(Func&& f) const
  {
    VisitorData<Func> data{ *libclang, f, false };
    libclang->clang_visitChildren(this->cursor, generic_visit_callback<Func>, &data);
  }

  size_t childCount() const;
  ClangCursor childAt(size_t index) const;
  std::vector<ClangCursor> children() const;
};

inline bool operator==(const ClangCursor& lhs, const ClangCursor& rhs)
{
  return lhs.libclang->clang_equalCursors(lhs.cursor, rhs.cursor);
}

inline bool operator!=(const ClangCursor& lhs, const ClangCursor& rhs)
{
  return !(lhs == rhs);
}

} // namespace cxx

namespace std
{
template<> struct hash<cxx::ClangCursor>
{
  std::size_t operator()(const cxx::ClangCursor& c) const noexcept
  {
    return c.libclang != nullptr ? c.libclang->clang_hashCursor(c.cursor) : 0;
  }
};
}

#endif // LIBCLANGUTILS_CLANG_CURSOR_H
