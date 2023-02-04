// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_CURSOR_H
#define LIBCLANGUTILS_CLANG_CURSOR_H

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-source-location.h"
#include "libclang-utils/clang-source-range.h"
#include "libclang-utils/clang-type.h"

#include <cassert>
#include <functional>
#include <utility>
#include <vector>

/*!
 * \namespace libclang
 */

namespace libclang
{

template<CXCursorKind K>
struct CursorKind 
{
  operator CXCursorKind() const
  {
    return K;
  }
};

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

  Cursor getLexicalParent() const;
  Cursor getSemanticParent() const;

  bool isDefinition() const;
  Cursor getDefinition() const;

  Cursor getCanonical() const;

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
 * \class TCursor
 * \tparam the CXCursorKind of the cursor
 * \brief provides a type for a specific kind of cursor
 */
template<CXCursorKind K>
class TCursor : public Cursor
{
public:
  using Cursor::Cursor;

  explicit TCursor(const Cursor& c)
    : Cursor(c)
  {
    assert(c.kind() == K);
  }

  CursorKind<K> kind() const { return {}; }
};

/*!
 * \endclass
 */

/*!
 * \fn void visit(const Cursor& c, F&& f)
 * \tparam F functor type
 * \brief dynamic dispatch of the cursor based on its kind
 */
template<typename F>
void visit(const Cursor& c, F&& f)
{
  switch (c.kind())
  {
  case CXCursor_UnexposedDecl:                                    f(TCursor<CXCursor_UnexposedDecl>(c));                                    break;
  case CXCursor_StructDecl:                                       f(TCursor<CXCursor_StructDecl>(c));                                       break;
  case CXCursor_UnionDecl:                                        f(TCursor<CXCursor_UnionDecl>(c));                                        break;
  case CXCursor_ClassDecl:                                        f(TCursor<CXCursor_ClassDecl>(c));                                        break;
  case CXCursor_EnumDecl:                                         f(TCursor<CXCursor_EnumDecl>(c));                                         break;
  case CXCursor_FieldDecl:                                        f(TCursor<CXCursor_FieldDecl>(c));                                        break;
  case CXCursor_EnumConstantDecl:                                 f(TCursor<CXCursor_EnumConstantDecl>(c));                                 break;
  case CXCursor_FunctionDecl:                                     f(TCursor<CXCursor_FunctionDecl>(c));                                     break;
  case CXCursor_VarDecl:                                          f(TCursor<CXCursor_VarDecl>(c));                                          break;
  case CXCursor_ParmDecl:                                         f(TCursor<CXCursor_ParmDecl>(c));                                         break;
  case CXCursor_ObjCInterfaceDecl:                                f(TCursor<CXCursor_ObjCInterfaceDecl>(c));                                break;
  case CXCursor_ObjCCategoryDecl:                                 f(TCursor<CXCursor_ObjCCategoryDecl>(c));                                 break;
  case CXCursor_ObjCProtocolDecl:                                 f(TCursor<CXCursor_ObjCProtocolDecl>(c));                                 break;
  case CXCursor_ObjCPropertyDecl:                                 f(TCursor<CXCursor_ObjCPropertyDecl>(c));                                 break;
  case CXCursor_ObjCIvarDecl:                                     f(TCursor<CXCursor_ObjCIvarDecl>(c));                                     break;
  case CXCursor_ObjCInstanceMethodDecl:                           f(TCursor<CXCursor_ObjCInstanceMethodDecl>(c));                           break;
  case CXCursor_ObjCClassMethodDecl:                              f(TCursor<CXCursor_ObjCClassMethodDecl>(c));                              break;
  case CXCursor_ObjCImplementationDecl:                           f(TCursor<CXCursor_ObjCImplementationDecl>(c));                           break;
  case CXCursor_ObjCCategoryImplDecl:                             f(TCursor<CXCursor_ObjCCategoryImplDecl>(c));                             break;
  case CXCursor_TypedefDecl:                                      f(TCursor<CXCursor_TypedefDecl>(c));                                      break;
  case CXCursor_CXXMethod:                                        f(TCursor<CXCursor_CXXMethod>(c));                                        break;
  case CXCursor_Namespace:                                        f(TCursor<CXCursor_Namespace>(c));                                        break;
  case CXCursor_LinkageSpec:                                      f(TCursor<CXCursor_LinkageSpec>(c));                                      break;
  case CXCursor_Constructor:                                      f(TCursor<CXCursor_Constructor>(c));                                      break;
  case CXCursor_Destructor:                                       f(TCursor<CXCursor_Destructor>(c));                                       break;
  case CXCursor_ConversionFunction:                               f(TCursor<CXCursor_ConversionFunction>(c));                               break;
  case CXCursor_TemplateTypeParameter:                            f(TCursor<CXCursor_TemplateTypeParameter>(c));                            break;
  case CXCursor_NonTypeTemplateParameter:                         f(TCursor<CXCursor_NonTypeTemplateParameter>(c));                         break;
  case CXCursor_TemplateTemplateParameter:                        f(TCursor<CXCursor_TemplateTemplateParameter>(c));                        break;
  case CXCursor_FunctionTemplate:                                 f(TCursor<CXCursor_FunctionTemplate>(c));                                 break;
  case CXCursor_ClassTemplate:                                    f(TCursor<CXCursor_ClassTemplate>(c));                                    break;
  case CXCursor_ClassTemplatePartialSpecialization:               f(TCursor<CXCursor_ClassTemplatePartialSpecialization>(c));               break;
  case CXCursor_NamespaceAlias:                                   f(TCursor<CXCursor_NamespaceAlias>(c));                                   break;
  case CXCursor_UsingDirective:                                   f(TCursor<CXCursor_UsingDirective>(c));                                   break;
  case CXCursor_UsingDeclaration:                                 f(TCursor<CXCursor_UsingDeclaration>(c));                                 break;
  case CXCursor_TypeAliasDecl:                                    f(TCursor<CXCursor_TypeAliasDecl>(c));                                    break;
  case CXCursor_ObjCSynthesizeDecl:                               f(TCursor<CXCursor_ObjCSynthesizeDecl>(c));                               break;
  case CXCursor_ObjCDynamicDecl:                                  f(TCursor<CXCursor_ObjCDynamicDecl>(c));                                  break;
  case CXCursor_CXXAccessSpecifier:                               f(TCursor<CXCursor_CXXAccessSpecifier>(c));                               break;
  case CXCursor_ObjCSuperClassRef:                                f(TCursor<CXCursor_ObjCSuperClassRef>(c));                                break;
  case CXCursor_ObjCProtocolRef:                                  f(TCursor<CXCursor_ObjCProtocolRef>(c));                                  break;
  case CXCursor_ObjCClassRef:                                     f(TCursor<CXCursor_ObjCClassRef>(c));                                     break;
  case CXCursor_TypeRef:                                          f(TCursor<CXCursor_TypeRef>(c));                                          break;
  case CXCursor_CXXBaseSpecifier:                                 f(TCursor<CXCursor_CXXBaseSpecifier>(c));                                 break;
  case CXCursor_TemplateRef:                                      f(TCursor<CXCursor_TemplateRef>(c));                                      break;
  case CXCursor_NamespaceRef:                                     f(TCursor<CXCursor_NamespaceRef>(c));                                     break;
  case CXCursor_MemberRef:                                        f(TCursor<CXCursor_MemberRef>(c));                                        break;
  case CXCursor_LabelRef:                                         f(TCursor<CXCursor_LabelRef>(c));                                         break;
  case CXCursor_OverloadedDeclRef:                                f(TCursor<CXCursor_OverloadedDeclRef>(c));                                break;
  case CXCursor_VariableRef:                                      f(TCursor<CXCursor_VariableRef>(c));                                      break;
  case CXCursor_InvalidFile:                                      f(TCursor<CXCursor_InvalidFile>(c));                                      break;
  case CXCursor_NoDeclFound:                                      f(TCursor<CXCursor_NoDeclFound>(c));                                      break;
  case CXCursor_NotImplemented:                                   f(TCursor<CXCursor_NotImplemented>(c));                                   break;
  case CXCursor_InvalidCode:                                      f(TCursor<CXCursor_InvalidCode>(c));                                      break;
  case CXCursor_UnexposedExpr:                                    f(TCursor<CXCursor_UnexposedExpr>(c));                                    break;
  case CXCursor_DeclRefExpr:                                      f(TCursor<CXCursor_DeclRefExpr>(c));                                      break;
  case CXCursor_MemberRefExpr:                                    f(TCursor<CXCursor_MemberRefExpr>(c));                                    break;
  case CXCursor_CallExpr:                                         f(TCursor<CXCursor_CallExpr>(c));                                         break;
  case CXCursor_ObjCMessageExpr:                                  f(TCursor<CXCursor_ObjCMessageExpr>(c));                                  break;
  case CXCursor_BlockExpr:                                        f(TCursor<CXCursor_BlockExpr>(c));                                        break;
  case CXCursor_IntegerLiteral:                                   f(TCursor<CXCursor_IntegerLiteral>(c));                                   break;
  case CXCursor_FloatingLiteral:                                  f(TCursor<CXCursor_FloatingLiteral>(c));                                  break;
  case CXCursor_ImaginaryLiteral:                                 f(TCursor<CXCursor_ImaginaryLiteral>(c));                                 break;
  case CXCursor_StringLiteral:                                    f(TCursor<CXCursor_StringLiteral>(c));                                    break;
  case CXCursor_CharacterLiteral:                                 f(TCursor<CXCursor_CharacterLiteral>(c));                                 break;
  case CXCursor_ParenExpr:                                        f(TCursor<CXCursor_ParenExpr>(c));                                        break;
  case CXCursor_UnaryOperator:                                    f(TCursor<CXCursor_UnaryOperator>(c));                                    break;
  case CXCursor_ArraySubscriptExpr:                               f(TCursor<CXCursor_ArraySubscriptExpr>(c));                               break;
  case CXCursor_BinaryOperator:                                   f(TCursor<CXCursor_BinaryOperator>(c));                                   break;
  case CXCursor_CompoundAssignOperator:                           f(TCursor<CXCursor_CompoundAssignOperator>(c));                           break;
  case CXCursor_ConditionalOperator:                              f(TCursor<CXCursor_ConditionalOperator>(c));                              break;
  case CXCursor_CStyleCastExpr:                                   f(TCursor<CXCursor_CStyleCastExpr>(c));                                   break;
  case CXCursor_CompoundLiteralExpr:                              f(TCursor<CXCursor_CompoundLiteralExpr>(c));                              break;
  case CXCursor_InitListExpr:                                     f(TCursor<CXCursor_InitListExpr>(c));                                     break;
  case CXCursor_AddrLabelExpr:                                    f(TCursor<CXCursor_AddrLabelExpr>(c));                                    break;
  case CXCursor_StmtExpr:                                         f(TCursor<CXCursor_StmtExpr>(c));                                         break;
  case CXCursor_GenericSelectionExpr:                             f(TCursor<CXCursor_GenericSelectionExpr>(c));                             break;
  case CXCursor_GNUNullExpr:                                      f(TCursor<CXCursor_GNUNullExpr>(c));                                      break;
  case CXCursor_CXXStaticCastExpr:                                f(TCursor<CXCursor_CXXStaticCastExpr>(c));                                break;
  case CXCursor_CXXDynamicCastExpr:                               f(TCursor<CXCursor_CXXDynamicCastExpr>(c));                               break;
  case CXCursor_CXXReinterpretCastExpr:                           f(TCursor<CXCursor_CXXReinterpretCastExpr>(c));                           break;
  case CXCursor_CXXConstCastExpr:                                 f(TCursor<CXCursor_CXXConstCastExpr>(c));                                 break;
  case CXCursor_CXXFunctionalCastExpr:                            f(TCursor<CXCursor_CXXFunctionalCastExpr>(c));                            break;
  case CXCursor_CXXTypeidExpr:                                    f(TCursor<CXCursor_CXXTypeidExpr>(c));                                    break;
  case CXCursor_CXXBoolLiteralExpr:                               f(TCursor<CXCursor_CXXBoolLiteralExpr>(c));                               break;
  case CXCursor_CXXNullPtrLiteralExpr:                            f(TCursor<CXCursor_CXXNullPtrLiteralExpr>(c));                            break;
  case CXCursor_CXXThisExpr:                                      f(TCursor<CXCursor_CXXThisExpr>(c));                                      break;
  case CXCursor_CXXThrowExpr:                                     f(TCursor<CXCursor_CXXThrowExpr>(c));                                     break;
  case CXCursor_CXXNewExpr:                                       f(TCursor<CXCursor_CXXNewExpr>(c));                                       break;
  case CXCursor_CXXDeleteExpr:                                    f(TCursor<CXCursor_CXXDeleteExpr>(c));                                    break;
  case CXCursor_UnaryExpr:                                        f(TCursor<CXCursor_UnaryExpr>(c));                                        break;
  case CXCursor_ObjCStringLiteral:                                f(TCursor<CXCursor_ObjCStringLiteral>(c));                                break;
  case CXCursor_ObjCEncodeExpr:                                   f(TCursor<CXCursor_ObjCEncodeExpr>(c));                                   break;
  case CXCursor_ObjCSelectorExpr:                                 f(TCursor<CXCursor_ObjCSelectorExpr>(c));                                 break;
  case CXCursor_ObjCProtocolExpr:                                 f(TCursor<CXCursor_ObjCProtocolExpr>(c));                                 break;
  case CXCursor_ObjCBridgedCastExpr:                              f(TCursor<CXCursor_ObjCBridgedCastExpr>(c));                              break;
  case CXCursor_PackExpansionExpr:                                f(TCursor<CXCursor_PackExpansionExpr>(c));                                break;
  case CXCursor_SizeOfPackExpr:                                   f(TCursor<CXCursor_SizeOfPackExpr>(c));                                   break;
  case CXCursor_LambdaExpr:                                       f(TCursor<CXCursor_LambdaExpr>(c));                                       break;
  case CXCursor_ObjCBoolLiteralExpr:                              f(TCursor<CXCursor_ObjCBoolLiteralExpr>(c));                              break;
  case CXCursor_ObjCSelfExpr:                                     f(TCursor<CXCursor_ObjCSelfExpr>(c));                                     break;
  case CXCursor_OMPArraySectionExpr:                              f(TCursor<CXCursor_OMPArraySectionExpr>(c));                              break;
  case CXCursor_ObjCAvailabilityCheckExpr:                        f(TCursor<CXCursor_ObjCAvailabilityCheckExpr>(c));                        break;
  case CXCursor_FixedPointLiteral:                                f(TCursor<CXCursor_FixedPointLiteral>(c));                                break;
  case CXCursor_UnexposedStmt:                                    f(TCursor<CXCursor_UnexposedStmt>(c));                                    break;
  case CXCursor_LabelStmt:                                        f(TCursor<CXCursor_LabelStmt>(c));                                        break;
  case CXCursor_CompoundStmt:                                     f(TCursor<CXCursor_CompoundStmt>(c));                                     break;
  case CXCursor_CaseStmt:                                         f(TCursor<CXCursor_CaseStmt>(c));                                         break;
  case CXCursor_DefaultStmt:                                      f(TCursor<CXCursor_DefaultStmt>(c));                                      break;
  case CXCursor_IfStmt:                                           f(TCursor<CXCursor_IfStmt>(c));                                           break;
  case CXCursor_SwitchStmt:                                       f(TCursor<CXCursor_SwitchStmt>(c));                                       break;
  case CXCursor_WhileStmt:                                        f(TCursor<CXCursor_WhileStmt>(c));                                        break;
  case CXCursor_DoStmt:                                           f(TCursor<CXCursor_DoStmt>(c));                                           break;
  case CXCursor_ForStmt:                                          f(TCursor<CXCursor_ForStmt>(c));                                          break;
  case CXCursor_GotoStmt:                                         f(TCursor<CXCursor_GotoStmt>(c));                                         break;
  case CXCursor_IndirectGotoStmt:                                 f(TCursor<CXCursor_IndirectGotoStmt>(c));                                 break;
  case CXCursor_ContinueStmt:                                     f(TCursor<CXCursor_ContinueStmt>(c));                                     break;
  case CXCursor_BreakStmt:                                        f(TCursor<CXCursor_BreakStmt>(c));                                        break;
  case CXCursor_ReturnStmt:                                       f(TCursor<CXCursor_ReturnStmt>(c));                                       break;
  case CXCursor_GCCAsmStmt:                                       f(TCursor<CXCursor_GCCAsmStmt>(c));                                       break;
  case CXCursor_ObjCAtTryStmt:                                    f(TCursor<CXCursor_ObjCAtTryStmt>(c));                                    break;
  case CXCursor_ObjCAtCatchStmt:                                  f(TCursor<CXCursor_ObjCAtCatchStmt>(c));                                  break;
  case CXCursor_ObjCAtFinallyStmt:                                f(TCursor<CXCursor_ObjCAtFinallyStmt>(c));                                break;
  case CXCursor_ObjCAtThrowStmt:                                  f(TCursor<CXCursor_ObjCAtThrowStmt>(c));                                  break;
  case CXCursor_ObjCAtSynchronizedStmt:                           f(TCursor<CXCursor_ObjCAtSynchronizedStmt>(c));                           break;
  case CXCursor_ObjCAutoreleasePoolStmt:                          f(TCursor<CXCursor_ObjCAutoreleasePoolStmt>(c));                          break;
  case CXCursor_ObjCForCollectionStmt:                            f(TCursor<CXCursor_ObjCForCollectionStmt>(c));                            break;
  case CXCursor_CXXCatchStmt:                                     f(TCursor<CXCursor_CXXCatchStmt>(c));                                     break;
  case CXCursor_CXXTryStmt:                                       f(TCursor<CXCursor_CXXTryStmt>(c));                                       break;
  case CXCursor_CXXForRangeStmt:                                  f(TCursor<CXCursor_CXXForRangeStmt>(c));                                  break;
  case CXCursor_SEHTryStmt:                                       f(TCursor<CXCursor_SEHTryStmt>(c));                                       break;
  case CXCursor_SEHExceptStmt:                                    f(TCursor<CXCursor_SEHExceptStmt>(c));                                    break;
  case CXCursor_SEHFinallyStmt:                                   f(TCursor<CXCursor_SEHFinallyStmt>(c));                                   break;
  case CXCursor_MSAsmStmt:                                        f(TCursor<CXCursor_MSAsmStmt>(c));                                        break;
  case CXCursor_NullStmt:                                         f(TCursor<CXCursor_NullStmt>(c));                                         break;
  case CXCursor_DeclStmt:                                         f(TCursor<CXCursor_DeclStmt>(c));                                         break;
  case CXCursor_OMPParallelDirective:                             f(TCursor<CXCursor_OMPParallelDirective>(c));                             break;
  case CXCursor_OMPSimdDirective:                                 f(TCursor<CXCursor_OMPSimdDirective>(c));                                 break;
  case CXCursor_OMPForDirective:                                  f(TCursor<CXCursor_OMPForDirective>(c));                                  break;
  case CXCursor_OMPSectionsDirective:                             f(TCursor<CXCursor_OMPSectionsDirective>(c));                             break;
  case CXCursor_OMPSectionDirective:                              f(TCursor<CXCursor_OMPSectionDirective>(c));                              break;
  case CXCursor_OMPSingleDirective:                               f(TCursor<CXCursor_OMPSingleDirective>(c));                               break;
  case CXCursor_OMPParallelForDirective:                          f(TCursor<CXCursor_OMPParallelForDirective>(c));                          break;
  case CXCursor_OMPParallelSectionsDirective:                     f(TCursor<CXCursor_OMPParallelSectionsDirective>(c));                     break;
  case CXCursor_OMPTaskDirective:                                 f(TCursor<CXCursor_OMPTaskDirective>(c));                                 break;
  case CXCursor_OMPMasterDirective:                               f(TCursor<CXCursor_OMPMasterDirective>(c));                               break;
  case CXCursor_OMPCriticalDirective:                             f(TCursor<CXCursor_OMPCriticalDirective>(c));                             break;
  case CXCursor_OMPTaskyieldDirective:                            f(TCursor<CXCursor_OMPTaskyieldDirective>(c));                            break;
  case CXCursor_OMPBarrierDirective:                              f(TCursor<CXCursor_OMPBarrierDirective>(c));                              break;
  case CXCursor_OMPTaskwaitDirective:                             f(TCursor<CXCursor_OMPTaskwaitDirective>(c));                             break;
  case CXCursor_OMPFlushDirective:                                f(TCursor<CXCursor_OMPFlushDirective>(c));                                break;
  case CXCursor_SEHLeaveStmt:                                     f(TCursor<CXCursor_SEHLeaveStmt>(c));                                     break;
  case CXCursor_OMPOrderedDirective:                              f(TCursor<CXCursor_OMPOrderedDirective>(c));                              break;
  case CXCursor_OMPAtomicDirective:                               f(TCursor<CXCursor_OMPAtomicDirective>(c));                               break;
  case CXCursor_OMPForSimdDirective:                              f(TCursor<CXCursor_OMPForSimdDirective>(c));                              break;
  case CXCursor_OMPParallelForSimdDirective:                      f(TCursor<CXCursor_OMPParallelForSimdDirective>(c));                      break;
  case CXCursor_OMPTargetDirective:                               f(TCursor<CXCursor_OMPTargetDirective>(c));                               break;
  case CXCursor_OMPTeamsDirective:                                f(TCursor<CXCursor_OMPTeamsDirective>(c));                                break;
  case CXCursor_OMPTaskgroupDirective:                            f(TCursor<CXCursor_OMPTaskgroupDirective>(c));                            break;
  case CXCursor_OMPCancellationPointDirective:                    f(TCursor<CXCursor_OMPCancellationPointDirective>(c));                    break;
  case CXCursor_OMPCancelDirective:                               f(TCursor<CXCursor_OMPCancelDirective>(c));                               break;
  case CXCursor_OMPTargetDataDirective:                           f(TCursor<CXCursor_OMPTargetDataDirective>(c));                           break;
  case CXCursor_OMPTaskLoopDirective:                             f(TCursor<CXCursor_OMPTaskLoopDirective>(c));                             break;
  case CXCursor_OMPTaskLoopSimdDirective:                         f(TCursor<CXCursor_OMPTaskLoopSimdDirective>(c));                         break;
  case CXCursor_OMPDistributeDirective:                           f(TCursor<CXCursor_OMPDistributeDirective>(c));                           break;
  case CXCursor_OMPTargetEnterDataDirective:                      f(TCursor<CXCursor_OMPTargetEnterDataDirective>(c));                      break;
  case CXCursor_OMPTargetExitDataDirective:                       f(TCursor<CXCursor_OMPTargetExitDataDirective>(c));                       break;
  case CXCursor_OMPTargetParallelDirective:                       f(TCursor<CXCursor_OMPTargetParallelDirective>(c));                       break;
  case CXCursor_OMPTargetParallelForDirective:                    f(TCursor<CXCursor_OMPTargetParallelForDirective>(c));                    break;
  case CXCursor_OMPTargetUpdateDirective:                         f(TCursor<CXCursor_OMPTargetUpdateDirective>(c));                         break;
  case CXCursor_OMPDistributeParallelForDirective:                f(TCursor<CXCursor_OMPDistributeParallelForDirective>(c));                break;
  case CXCursor_OMPDistributeParallelForSimdDirective:            f(TCursor<CXCursor_OMPDistributeParallelForSimdDirective>(c));            break;
  case CXCursor_OMPDistributeSimdDirective:                       f(TCursor<CXCursor_OMPDistributeSimdDirective>(c));                       break;
  case CXCursor_OMPTargetParallelForSimdDirective:                f(TCursor<CXCursor_OMPTargetParallelForSimdDirective>(c));                break;
  case CXCursor_OMPTargetSimdDirective:                           f(TCursor<CXCursor_OMPTargetSimdDirective>(c));                           break;
  case CXCursor_OMPTeamsDistributeDirective:                      f(TCursor<CXCursor_OMPTeamsDistributeDirective>(c));                      break;
  case CXCursor_OMPTeamsDistributeSimdDirective:                  f(TCursor<CXCursor_OMPTeamsDistributeSimdDirective>(c));                  break;
  case CXCursor_OMPTeamsDistributeParallelForSimdDirective:       f(TCursor<CXCursor_OMPTeamsDistributeParallelForSimdDirective>(c));       break;
  case CXCursor_OMPTeamsDistributeParallelForDirective:           f(TCursor<CXCursor_OMPTeamsDistributeParallelForDirective>(c));           break;
  case CXCursor_OMPTargetTeamsDirective:                          f(TCursor<CXCursor_OMPTargetTeamsDirective>(c));                          break;
  case CXCursor_OMPTargetTeamsDistributeDirective:                f(TCursor<CXCursor_OMPTargetTeamsDistributeDirective>(c));                break;
  case CXCursor_OMPTargetTeamsDistributeParallelForDirective:     f(TCursor<CXCursor_OMPTargetTeamsDistributeParallelForDirective>(c));     break;
  case CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective: f(TCursor<CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective>(c)); break;
  case CXCursor_OMPTargetTeamsDistributeSimdDirective:            f(TCursor<CXCursor_OMPTargetTeamsDistributeSimdDirective>(c));            break;
  case CXCursor_TranslationUnit:                                  f(TCursor<CXCursor_TranslationUnit>(c));                                  break;
  case CXCursor_UnexposedAttr:                                    f(TCursor<CXCursor_UnexposedAttr>(c));                                    break;
  case CXCursor_IBActionAttr:                                     f(TCursor<CXCursor_IBActionAttr>(c));                                     break;
  case CXCursor_IBOutletAttr:                                     f(TCursor<CXCursor_IBOutletAttr>(c));                                     break;
  case CXCursor_IBOutletCollectionAttr:                           f(TCursor<CXCursor_IBOutletCollectionAttr>(c));                           break;
  case CXCursor_CXXFinalAttr:                                     f(TCursor<CXCursor_CXXFinalAttr>(c));                                     break;
  case CXCursor_CXXOverrideAttr:                                  f(TCursor<CXCursor_CXXOverrideAttr>(c));                                  break;
  case CXCursor_AnnotateAttr:                                     f(TCursor<CXCursor_AnnotateAttr>(c));                                     break;
  case CXCursor_AsmLabelAttr:                                     f(TCursor<CXCursor_AsmLabelAttr>(c));                                     break;
  case CXCursor_PackedAttr:                                       f(TCursor<CXCursor_PackedAttr>(c));                                       break;
  case CXCursor_PureAttr:                                         f(TCursor<CXCursor_PureAttr>(c));                                         break;
  case CXCursor_ConstAttr:                                        f(TCursor<CXCursor_ConstAttr>(c));                                        break;
  case CXCursor_NoDuplicateAttr:                                  f(TCursor<CXCursor_NoDuplicateAttr>(c));                                  break;
  case CXCursor_CUDAConstantAttr:                                 f(TCursor<CXCursor_CUDAConstantAttr>(c));                                 break;
  case CXCursor_CUDADeviceAttr:                                   f(TCursor<CXCursor_CUDADeviceAttr>(c));                                   break;
  case CXCursor_CUDAGlobalAttr:                                   f(TCursor<CXCursor_CUDAGlobalAttr>(c));                                   break;
  case CXCursor_CUDAHostAttr:                                     f(TCursor<CXCursor_CUDAHostAttr>(c));                                     break;
  case CXCursor_CUDASharedAttr:                                   f(TCursor<CXCursor_CUDASharedAttr>(c));                                   break;
  case CXCursor_VisibilityAttr:                                   f(TCursor<CXCursor_VisibilityAttr>(c));                                   break;
  case CXCursor_DLLExport:                                        f(TCursor<CXCursor_DLLExport>(c));                                        break;
  case CXCursor_DLLImport:                                        f(TCursor<CXCursor_DLLImport>(c));                                        break;
  case CXCursor_PreprocessingDirective:                           f(TCursor<CXCursor_PreprocessingDirective>(c));                           break;
  case CXCursor_MacroDefinition:                                  f(TCursor<CXCursor_MacroDefinition>(c));                                  break;
  case CXCursor_MacroExpansion:                                   f(TCursor<CXCursor_MacroExpansion>(c));                                   break;
  case CXCursor_InclusionDirective:                               f(TCursor<CXCursor_InclusionDirective>(c));                               break;
  case CXCursor_ModuleImportDecl:                                 f(TCursor<CXCursor_ModuleImportDecl>(c));                                 break;
  case CXCursor_TypeAliasTemplateDecl:                            f(TCursor<CXCursor_TypeAliasTemplateDecl>(c));                            break;
  case CXCursor_StaticAssert:                                     f(TCursor<CXCursor_StaticAssert>(c));                                     break;
  case CXCursor_FriendDecl:                                       f(TCursor<CXCursor_FriendDecl>(c));                                       break;
  case CXCursor_OverloadCandidate:                                f(TCursor<CXCursor_OverloadCandidate>(c));                                break;
  default: f(c); break;
  }
}

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
