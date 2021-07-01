// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_CURSOR_H
#define LIBCLANGUTILS_CLANG_CURSOR_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-source-location.h"
#include "libclang-utils/clang-source-range.h"
#include "libclang-utils/clang-type.h"

#include <functional>
#include <utility>
#include <vector>

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class Cursor
 */

class LIBCLANGU_API Cursor
{
public:
  LibClang* api;
  CXCursor cursor;

  Cursor(LibClang& lib, CXCursor c)
    : api(&lib), cursor(c)
  {

  }

  Cursor(const Cursor&) = default;

  ~Cursor()
  {

  }

  Cursor& operator=(const Cursor&) = default;

  operator CXCursor() const
  {
    return cursor;
  }

  CXCursorKind kind() const
  {
    return api->clang_getCursorKind(this->cursor);
  }

  bool isDeclaration() const
  {
    return api->clang_isDeclaration(kind());
  }

  bool isExpression() const
  {
    return api->clang_isExpression(kind());
  }

  bool isPreprocessing() const
  {
    return api->clang_isPreprocessing(kind());
  }

  bool isReference() const
  {
    return api->clang_isReference(kind());
  }

  bool isStatement() const
  {
    return api->clang_isStatement(kind());
  }

  bool isUnexposed() const
  {
    return api->clang_isUnexposed(kind());
  }

  std::string getSpelling() const
  {
    CXString str = api->clang_getCursorSpelling(this->cursor);
    std::string result = api->clang_getCString(str);
    api->clang_disposeString(str);
    return result;
  }

  std::string getCursorKindSpelling() const
  {
    CXString str = api->clang_getCursorKindSpelling(kind());
    std::string result = api->clang_getCString(str);
    api->clang_disposeString(str);
    return result;
  }

  Cursor getLexicalParent() const;
  Cursor getSemanticParent() const;

  Type getType() const
  {
    return Type(*api, api->clang_getCursorType(this->cursor));
  }

  int getNumArguments() const
  {
    return api->clang_Cursor_getNumArguments(this->cursor);
  }

  Cursor getArgument(int index) const
  {
    return Cursor(*api, api->clang_Cursor_getArgument(this->cursor, index));
  }

  CX_CXXAccessSpecifier getCXXAccessSpecifier() const
  {
    return api->clang_getCXXAccessSpecifier(this->cursor);
  }

  int getExceptionSpecificationType() const
  {
    return api->clang_getCursorExceptionSpecificationType(this->cursor);
  }

  SourceLocation getLocation() const;
  SourceRange getExtent() const;

  bool isConstructor() const;
  bool isDestructor() const;

  bool EnumDecl_isScoped() const;

  bool CXXMethod_isConst() const;
  bool CXXMethod_isStatic() const;
  bool CXXMethod_isVirtual() const;
  bool CXXMethod_isPureVirtual() const;

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
  static void visitor_invoker(F&& func, bool& stop_token, const Cursor& cursor, VisitorSelector1)
  {
    func(cursor);
  }

  template<typename F, typename = decltype(std::declval<F>()(std::declval<bool&>(), std::declval<const Cursor&>()))>
  static void visitor_invoker(F&& func, bool& stop_token, const Cursor& cursor, VisitorSelector2)
  {
    func(stop_token, cursor);
  }

  template<typename T>
  static CXChildVisitResult generic_visit_callback(CXCursor c, CXCursor p, CXClientData client_data)
  {
    VisitorData<T>& data = *static_cast<VisitorData<T>*>(client_data);
    visitor_invoker(data.functor, data.should_break, Cursor{ data.libclang, c }, VisitorSelector2{});
    return data.should_break ? CXChildVisit_Break : CXChildVisit_Continue;
  }

  template<typename Func>
  void visitChildren(Func&& f) const
  {
    VisitorData<Func> data{ *api, f, false };
    api->clang_visitChildren(this->cursor, generic_visit_callback<Func>, &data);
  }

  size_t childCount() const;
  Cursor childAt(size_t index) const;
  std::vector<Cursor> children() const;
};

/*!
 * \fn Cursor getLexicalParent() const
 * \brief returns the cursor's lexical parent
 */
inline Cursor Cursor::getLexicalParent() const
{
  return Cursor(*api, api->clang_getCursorLexicalParent(*this));
}

/*!
 * \fn Cursor getSemanticParent() const
 * \brief returns the cursor's semantic parent
 */
inline Cursor Cursor::getSemanticParent() const
{
  return Cursor(*api, api->clang_getCursorSemanticParent(this->cursor));
}

/*!
 * \fn SourceLocation getLocation() const
 * \brief returns the cursor's location
 */
inline SourceLocation Cursor::getLocation() const
{
  return SourceLocation(*api, api->clang_getCursorLocation(cursor));
}

/*!
 * \fn SourceRange getExtent() const
 * \brief returns the cursor's source range
 */
inline SourceRange Cursor::getExtent() const
{
  return SourceRange(*api, api->clang_getCursorExtent(*this));
}

/*!
 * \fn bool isConstructor() const
 * \brief convenience function that returns whether this cursor is a constructor
 * 
 * This checks the cursor kind against CXCursor_Constructor.
 */
inline bool Cursor::isConstructor() const
{
  return kind() == CXCursor_Constructor;
}

/*!
 * \fn bool isDestructor() const
 * \brief convenience function that returns whether this cursor is a destructor
 * 
 * This checks the cursor kind against CXCursor_Destructor.
 */
inline bool Cursor::isDestructor() const
{
  return kind() == CXCursor_Destructor;
}

/*!
 * \fn bool EnumDecl_isScoped() const
 * \brief returns whether the cursor is an enum class
 */
inline bool Cursor::EnumDecl_isScoped() const
{
  return api->clang_EnumDecl_isScoped(*this);
}

/*!
 * \fn bool CXXMethod_isConst() const
 * \brief returns whether the cursor is a const method
 */
inline bool Cursor::CXXMethod_isConst() const
{
  return api->clang_CXXMethod_isConst(cursor);
}

/*!
 * \fn bool CXXMethod_isStatic() const
 * \brief returns whether the cursor is a static method
 */
inline bool Cursor::CXXMethod_isStatic() const
{
  return api->clang_CXXMethod_isStatic(cursor);
}

/*!
 * \fn bool CXXMethod_isVirtual() const
 * \brief returns whether the cursor is a virtual method
 */
inline bool Cursor::CXXMethod_isVirtual() const
{
  return api->clang_CXXMethod_isVirtual(*this);
}

/*!
 * \fn bool CXXMethod_isPureVirtual() const
 * \brief returns whether the cursor is a pure virtual method
 */
inline bool Cursor::CXXMethod_isPureVirtual() const
{
  return api->clang_CXXMethod_isPureVirtual(*this);
}

inline bool operator==(const Cursor& lhs, const Cursor& rhs)
{
  return lhs.api->clang_equalCursors(lhs.cursor, rhs.cursor);
}

inline bool operator!=(const Cursor& lhs, const Cursor& rhs)
{
  return !(lhs == rhs);
}

/*!
 * \endclass
 */

/*!
 * \endnamespace
 */

} // namespace libclang

namespace std
{
template<> struct hash<libclang::Cursor>
{
  std::size_t operator()(const libclang::Cursor& c) const noexcept
  {
    return c.api != nullptr ? c.api->clang_hashCursor(c.cursor) : 0;
  }
};
}

#endif // LIBCLANGUTILS_CLANG_CURSOR_H
