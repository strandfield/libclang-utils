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

class File;

/*!
 * \class Cursor
 * \brief provides an interface to access a translation unit ast
 */

class LIBCLANGU_API Cursor
{
public:
  LibClang* api;
  CXCursor cursor;

  /*!
   * \fn Cursor() = delete
   */
  Cursor() = delete;

  /*!
   * \fn Cursor(const Cursor&) = default
   */
  Cursor(const Cursor&) = default;

  /*!
   * \fn ~Cursor() = default
   */
  ~Cursor() = default;

  /*!
   * \fn Cursor& operator=(const Cursor&) = default
   */
  Cursor& operator=(const Cursor&) = default;

  Cursor(LibClang& lib, CXCursor c);

  bool isNull() const;

  CXCursorKind kind() const;

  bool isDeclaration() const;
  bool isExpression() const;
  bool isPreprocessing() const;
  bool isStatement() const;
  bool isUnexposed() const;

  bool isReference() const;
  Cursor getReference() const;

  std::string getSpelling() const;
  std::string getCursorKindSpelling() const;
  std::string getUSR() const;
  std::string getMangling() const;
  std::string getDisplayName() const;

  Cursor getLexicalParent() const;
  Cursor getSemanticParent() const;

  bool isDefinition() const;
  Cursor getDefinition() const;

  Cursor getCanonical() const;

  File getIncludedFile() const;

  Type getType() const;
  Type getTypedefDeclUnderlyingType() const;

  int getNumArguments() const;
  Cursor getArgument(int index) const;

  CX_CXXAccessSpecifier getCXXAccessSpecifier() const;

  int getExceptionSpecificationType() const;

  SourceLocation getLocation() const;
  SourceRange getExtent() const;

  bool isConstructor() const;
  bool isDestructor() const;

  bool EnumDecl_isScoped() const;

  bool CXXMethod_isDefaulted() const;
  bool CXXMethod_isConst() const;
  bool CXXMethod_isStatic() const;
  bool CXXMethod_isVirtual() const;
  bool CXXMethod_isPureVirtual() const;

  size_t childCount() const;
  Cursor childAt(size_t index) const;
  std::vector<Cursor> children() const;

  template<typename Func>
  void visitChildren(Func&& f) const;

  operator CXCursor() const;
};

/*!
 * \fn Cursor(LibClang& lib, CXCursor c)
 * \brief constructs a cursor
 */
inline Cursor::Cursor(LibClang& lib, CXCursor c)
  : api(&lib), cursor(c)
{

}

/*!
 * \fn bool isNull() const
 * \brief returns whether this cursor is the null cursor
 */
inline bool Cursor::isNull() const
{
  return api->clang_Cursor_isNull(this->cursor);
}

/*!
 * \fn CXCursorKind kind() const
 */
inline CXCursorKind Cursor::kind() const
{
  return api->clang_getCursorKind(this->cursor);
}

/*!
 * \fn bool isDeclaration() const
 * \brief returns whether the cursor is a declaration
 */
inline bool Cursor::isDeclaration() const
{
  return api->clang_isDeclaration(kind());
}

/*!
 * \fn bool isExpression() const
 * \brief returns whether the cursor is an expression
 */
inline bool Cursor::isExpression() const
{
  return api->clang_isExpression(kind());
}

/*!
 * \fn bool isPreprocessing() const
 */
inline bool Cursor::isPreprocessing() const
{
  return api->clang_isPreprocessing(kind());
}

/*!
 * \fn bool isStatement() const
 * \brief returns whether the cursor is a statement
 */
inline bool Cursor::isStatement() const
{
  return api->clang_isStatement(kind());
}

/*!
 * \fn bool isUnexposed() const
 */
inline bool Cursor::isUnexposed() const
{
  return api->clang_isUnexposed(kind());
}

/*!
 * \fn bool isReference() const
 * \brief returns whether the cursor is a reference
 */
inline bool Cursor::isReference() const
{
  return api->clang_isReference(kind());
}

/*!
 * \fn Cursor getReference() const
 * \brief returns the cursor that this cursor is a reference to
 */
inline Cursor Cursor::getReference() const
{
  return Cursor(*api, api->clang_getCursorReferenced(*this));
}

/*!
 * \fn std::string getSpelling() const
 */
inline std::string Cursor::getSpelling() const
{
  CXString str = api->clang_getCursorSpelling(this->cursor);
  std::string result = api->clang_getCString(str);
  api->clang_disposeString(str);
  return result;
}

/*!
 * \fn std::string getCursorKindSpelling() const
 */
inline std::string Cursor::getCursorKindSpelling() const
{
  CXString str = api->clang_getCursorKindSpelling(kind());
  std::string result = api->clang_getCString(str);
  api->clang_disposeString(str);
  return result;
}

/*!
 * \fn std::string getUSR() const
 */
inline std::string Cursor::getUSR() const
{
  CXString str = api->clang_getCursorUSR(*this);
  std::string result = api->clang_getCString(str);
  api->clang_disposeString(str);
  return result;
}

/*!
 * \fn std::string getMangling() const
 */
inline std::string Cursor::getMangling() const
{
  CXString str = api->clang_Cursor_getMangling(this->cursor);
  std::string result = api->clang_getCString(str);
  api->clang_disposeString(str);
  return result;
}

/*!
 * \fn std::string Cursor::getDisplayName() const
 */
inline std::string Cursor::getDisplayName() const
{
  return api->toStdString(api->clang_getCursorDisplayName(this->cursor));
}

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
 * \fn bool isDefinition() const
 */
inline bool Cursor::isDefinition() const
{
  return  api->clang_isCursorDefinition(*this);
}

/*!
 * \fn Cursor getDefinition() const
 * \brief returns the cursor's definition
 */
inline Cursor Cursor::getDefinition() const
{
  return Cursor(*api, api->clang_getCursorDefinition(this->cursor));
}

/*!
 * \fn Cursor getCanonical() const
 * 
 * If this cursor is declaration, returns the canonical cursor representing the 
 * entity. 
 * This is useful for entities that can be declared multiple times (i.e. via 
 * forward declarations).
 */
inline Cursor Cursor::getCanonical() const
{
  return Cursor(*api, api->clang_getCanonicalCursor(*this));
}

/*!
 * \fn Type getType() const
 */
inline Type Cursor::getType() const
{
  return Type(*api, api->clang_getCursorType(this->cursor));
}

/*!
 * \fn Type getTypedefDeclUnderlyingType() const
 */
inline Type Cursor::getTypedefDeclUnderlyingType() const
{
  return Type(*api, api->clang_getTypedefDeclUnderlyingType(*this));
}

/*!
 * \fn int getNumArguments() const
 */
inline int Cursor::getNumArguments() const
{
  return api->clang_Cursor_getNumArguments(this->cursor);
}

/*!
 * \fn Cursor getArgument(int index) const
 */
inline Cursor Cursor::getArgument(int index) const
{
  return Cursor(*api, api->clang_Cursor_getArgument(this->cursor, index));
}

/*!
 * \fn CX_CXXAccessSpecifier getCXXAccessSpecifier() const
 */
inline CX_CXXAccessSpecifier Cursor::getCXXAccessSpecifier() const
{
  return api->clang_getCXXAccessSpecifier(this->cursor);
}

/*!
 * \fn int getExceptionSpecificationType() const
 */
inline int Cursor::getExceptionSpecificationType() const
{
  return api->clang_getCursorExceptionSpecificationType(this->cursor);
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
 * \fn bool CXXMethod_isDefaulted() const
 * \brief returns whether the cursor is a defaulted function
 */
inline bool Cursor::CXXMethod_isDefaulted() const
{
  return api->clang_CXXMethod_isDefaulted(*this);
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

namespace details
{

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
void visitor_invoker(F&& func, bool& stop_token, const Cursor& cursor, VisitorSelector1)
{
  func(cursor);
}

template<typename F, typename = decltype(std::declval<F>()(std::declval<bool&>(), std::declval<const Cursor&>()))>
void visitor_invoker(F&& func, bool& stop_token, const Cursor& cursor, VisitorSelector2)
{
  func(stop_token, cursor);
}

template<typename T>
CXChildVisitResult generic_visit_callback(CXCursor c, CXCursor p, CXClientData client_data)
{
  VisitorData<T>& data = *static_cast<VisitorData<T>*>(client_data);
  visitor_invoker(data.functor, data.should_break, Cursor{ data.libclang, c }, VisitorSelector2{});
  return data.should_break ? CXChildVisit_Break : CXChildVisit_Continue;
}

} // namespace details

/*!
 * \fn void visitChildren(Func&& f) const
 */
template<typename Func>
inline void Cursor::visitChildren(Func&& f) const
{
  details::VisitorData<Func> data{ *api, f, false };
  api->clang_visitChildren(this->cursor, details::generic_visit_callback<Func>, &data);
}

/*!
 * \fn operator CXCursor() const
 */
inline Cursor::operator CXCursor() const
{
  return cursor;
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
