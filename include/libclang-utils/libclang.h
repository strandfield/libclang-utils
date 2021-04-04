// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_LIBCLANG_H
#define LIBCLANGUTILS_LIBCLANG_H

#include "libclang-utils/libclang-utils-defs.h"
#include "libclang-utils/cindex.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace dynlib
{
class Library;  
} // namespace dynlib

namespace cxx
{

class ClangIndex;

class LibClangError : public std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};

class LIBCLANGU_API LibClang
{
public:
  std::unique_ptr<dynlib::Library> libclang;

private:
  std::string m_printable_version;
  CXVersion m_version;

public:

  /* libclang functions */

  ClangGetCString clang_getCString = nullptr;
  ClangDisposeString clang_disposeString = nullptr;
  ClangDisposeStringSet clang_disposeStringSet = nullptr;

  ClangCreateIndex clang_createIndex = nullptr;
  ClangDisposeIndex clang_disposeIndex = nullptr;
  ClangCXIndexSetGlobalOptions clang_CXIndex_setGlobalOptions = nullptr;
  ClangCXIndexGetGlobalOptions clang_CXIndex_getGlobalOptions = nullptr;
  ClangCXIndexSetInvocationEmissionPathOption clang_CXIndex_setInvocationEmissionPathOption = nullptr;
 
  ClangGetFileName clang_getFileName = nullptr;
  ClangGetFileUniqueID clang_getFileUniqueID = nullptr;
  ClangIsFileMultipleIncludeGuard clang_isFileMultipleIncludeGuarded = nullptr;
  ClangGetFile clang_getFile = nullptr;
  ClangGetFileContents clang_getFileContents = nullptr;
  ClangFileIsEqual clang_File_isEqual = nullptr;
  ClangFileTryGetRealPathName clang_File_tryGetRealPathName = nullptr;

  ClangGetNullLocation clang_getNullLocation = nullptr;
  ClangEqualLocations clang_equalLocations = nullptr;
  ClangGetLocation clang_getLocation = nullptr;
  ClangGetLocationForOffset clang_getLocationForOffset = nullptr;
  ClangLocationIsInSystemHeader clang_Location_isInSystemHeader = nullptr;
  ClangLocationIsFromMainFile clang_Location_isFromMainFile = nullptr;
  ClangGetNullRange clang_getNullRange = nullptr;
  ClangGetRange clang_getRange = nullptr;
  ClangEqualRanges clang_equalRanges = nullptr;
  ClangRangeIsNull clang_Range_isNull = nullptr;
  ClangGetExpansionLocation clang_getExpansionLocation = nullptr;
  ClangGetPresumedLocation clang_getPresumedLocation = nullptr;
  ClangGetInstantiationLocation clang_getInstantiationLocation = nullptr;
  ClangGetSpellingLocation clang_getSpellingLocation = nullptr;
  ClangGetFileLocation clang_getFileLocation = nullptr;
  ClangGetRangeStart clang_getRangeStart = nullptr;
  ClangGetRangeEnd clang_getRangeEnd = nullptr;
  ClangGetSkippedRanges clang_getSkippedRanges = nullptr;
  ClangGetAllSkippedRanges clang_getAllSkippedRanges = nullptr;
  ClangDisposeSourceRangeList clang_disposeSourceRangeList = nullptr;

  ClangGetNumDiagnosticsInSet clang_getNumDiagnosticsInSet = nullptr;
  ClangGetDiagnosticInSet clang_getDiagnosticInSet = nullptr;
  ClangLoadDiagnostics clang_loadDiagnostics = nullptr;
  ClangDisposeDiagnosticSet clang_disposeDiagnosticSet = nullptr;
  ClangGetChildDiagnostics clang_getChildDiagnostics = nullptr;
  ClangGetNumDiagnostics clang_getNumDiagnostics = nullptr;
  ClangGetDiagnostic clang_getDiagnostic = nullptr;
  ClangGetDiagnosticSetFromTU clang_getDiagnosticSetFromTU = nullptr;
  ClangDisposeDiagnostic clang_disposeDiagnostic = nullptr;
  ClangFormatDiagnostic clang_formatDiagnostic = nullptr;
  ClangDefaultDiagnosticDisplayOptions clang_defaultDiagnosticDisplayOptions = nullptr;
  ClangGetDiagnosticSeverity clang_getDiagnosticSeverity = nullptr;
  ClangGetDiagnosticLocation clang_getDiagnosticLocation = nullptr;
  ClangGetDiagnosticSpelling clang_getDiagnosticSpelling = nullptr;
  ClangGetDiagnosticOption clang_getDiagnosticOption = nullptr;
  ClangGetDiagnosticCategory clang_getDiagnosticCategory = nullptr;
  ClangGetDiagnosticCategoryText clang_getDiagnosticCategoryText = nullptr;
  ClangGetDiagnosticNumRanges clang_getDiagnosticNumRanges = nullptr;
  ClangGetDiagnosticRange clang_getDiagnosticRange = nullptr;
  ClangGetDiagnosticNumFixIts clang_getDiagnosticNumFixIts = nullptr;
  ClangGetDiagnosticFixIt clang_getDiagnosticFixIt = nullptr;
  ClangGetTranslationUnitSpelling clang_getTranslationUnitSpelling = nullptr;
  ClangCreateTranslationUnitFromSourceFile clang_createTranslationUnitFromSourceFile = nullptr;
  ClangCreateTranslationUnit clang_createTranslationUnit = nullptr;
  ClangCreateTranslationUnit2 clang_createTranslationUnit2 = nullptr;
  ClangDefaultEditingTranslationUnitOptions clang_defaultEditingTranslationUnitOptions = nullptr;
  ClangParseTranslationUnit clang_parseTranslationUnit = nullptr;
  ClangParseTranslationUnit2 clang_parseTranslationUnit2 = nullptr;
  ClangParseTranslationUnit2FullArgv clang_parseTranslationUnit2FullArgv = nullptr;
  ClangDefaultSaveOptions clang_defaultSaveOptions = nullptr;
  ClangSaveTranslationUnit clang_saveTranslationUnit = nullptr;
  ClangSuspendTranslationUnit clang_suspendTranslationUnit = nullptr;
  ClangDisposeTranslationUnit clang_disposeTranslationUnit = nullptr;
  ClangDefaultReparseOptions clang_defaultReparseOptions = nullptr;
  ClangReparseTranslationUnit clang_reparseTranslationUnit = nullptr;
  ClangGetTUResourceUsageName clang_getTUResourceUsageName = nullptr;
  ClangGetCXTUResourceUsage clang_getCXTUResourceUsage = nullptr;
  ClangDisposeCXTUResourceUsage clang_disposeCXTUResourceUsage = nullptr;
  ClangGetTranslationUnitTargetInfo clang_getTranslationUnitTargetInfo = nullptr;
  ClangTargetInfoDispose clang_TargetInfo_dispose = nullptr;
  ClangTargetInfoGetTriple clang_TargetInfo_getTriple = nullptr;
  ClangTargetInfoGetPointerWidth clang_TargetInfo_getPointerWidth = nullptr;
  ClangGetNullCursor clang_getNullCursor = nullptr;
  ClangGetTranslationUnitCursor clang_getTranslationUnitCursor = nullptr;
  ClangEqualCursors clang_equalCursors = nullptr;
  ClangCursorIsNull clang_Cursor_isNull = nullptr;
  ClangHashCursor clang_hashCursor = nullptr;
  ClangGetCursorKind clang_getCursorKind = nullptr;
  ClangIsDeclaration clang_isDeclaration = nullptr;
  ClangIsInvalidDeclaration clang_isInvalidDeclaration = nullptr;
  ClangIsReference clang_isReference = nullptr;
  ClangIsExpression clang_isExpression = nullptr;
  ClangIsStatement clang_isStatement = nullptr;
  ClangIsAttribute clang_isAttribute = nullptr;
  ClangCursorHasAttrs clang_Cursor_hasAttrs = nullptr;
  ClangIsInvalid clang_isInvalid = nullptr;
  ClangDisposeTranslationUnit clang_isTranslationUnit = nullptr;
  ClangIsPreprocessing clang_isPreprocessing = nullptr;
  ClangIsUnexposed clang_isUnexposed = nullptr;
  ClangGetCursorLinkage clang_getCursorLinkage = nullptr;
  ClangGetCursorVisibility clang_getCursorVisibility = nullptr;
  ClangGetCursorAvailability clang_getCursorAvailability = nullptr;
  ClangGetCursorPlatformAvailability clang_getCursorPlatformAvailability = nullptr;
  ClangDisposeCXPlatformAvailability clang_disposeCXPlatformAvailability = nullptr;
  ClangGetCursorLanguage clang_getCursorLanguage = nullptr;
  ClangGetCursorTLSKind clang_getCursorTLSKind = nullptr;
  ClangCursorGetTranslationUnit clang_Cursor_getTranslationUnit = nullptr;
  ClangCreateCXCursorSet clang_createCXCursorSet = nullptr;
  ClangDisposeCXCursorSet clang_disposeCXCursorSet = nullptr;
  ClangCXCursorSetContains clang_CXCursorSet_contains = nullptr;
  ClangCXCursorSetInsert clang_CXCursorSet_insert = nullptr;
  ClangGetCursorSemanticParent clang_getCursorSemanticParent = nullptr;
  ClangGetCursorLexicalParent clang_getCursorLexicalParent = nullptr;
  ClangGetOverriddenCursors clang_getOverriddenCursors = nullptr;
  ClangDisposeOverriddenCursors clang_disposeOverriddenCursors = nullptr;
  ClangGetIncludedFile clang_getIncludedFile = nullptr;
  ClangGetCursor clang_getCursor = nullptr;
  ClangGetCursorLocation clang_getCursorLocation = nullptr;
  ClangGetCursorExtent clang_getCursorExtent = nullptr;
  ClangGetCursorType clang_getCursorType = nullptr;
  ClangGetTypeSpelling clang_getTypeSpelling = nullptr;
  ClangGetTypedefDeclUnderlyingType clang_getTypedefDeclUnderlyingType = nullptr;
  ClangGetEnumDeclIntegerType clang_getEnumDeclIntegerType = nullptr;
  ClangGetEnumConstantDeclValue clang_getEnumConstantDeclValue = nullptr;
  ClangGetEnumConstantDeclUnsignedValue clang_getEnumConstantDeclUnsignedValue = nullptr;
  ClangGetFieldDeclBitWidth clang_getFieldDeclBitWidth = nullptr;
  ClangCursorGetNumArguments clang_Cursor_getNumArguments = nullptr;
  ClangCursorGetArgument clang_Cursor_getArgument = nullptr;
  ClangCursorGetNumTemplateArguments clang_Cursor_getNumTemplateArguments = nullptr;
  ClangCursorGetTemplateArgumentKind clang_Cursor_getTemplateArgumentKind = nullptr;
  ClangCursorGetTemplateArgumentType clang_Cursor_getTemplateArgumentType = nullptr;
  ClangCursorGetTemplateArgumentValue clang_Cursor_getTemplateArgumentValue = nullptr;
  ClangCursorGetTemplateArgumentUnsignedValue clang_Cursor_getTemplateArgumentUnsignedValue = nullptr;
  ClangEqualTypes clang_equalTypes = nullptr;
  ClangGetCanonicalType clang_getCanonicalType = nullptr;
  ClangIsConstQualifiedType clang_isConstQualifiedType = nullptr;
  ClangCursorIsMacroFunctionLike clang_Cursor_isMacroFunctionLike = nullptr;
  ClangCursorIsMacroBuiltin clang_Cursor_isMacroBuiltin = nullptr;
  ClangCursorIsFunctionInlined clang_Cursor_isFunctionInlined = nullptr;
  ClangIsVolatileQualifiedType clang_isVolatileQualifiedType = nullptr;
  ClangIsRestrictQualifiedType clang_isRestrictQualifiedType = nullptr;
  ClangGetAddressSpace clang_getAddressSpace = nullptr;
  ClangGetTypedefName clang_getTypedefName = nullptr;
  ClangGetPointeeType clang_getPointeeType = nullptr;
  ClangGetTypeDeclaration clang_getTypeDeclaration = nullptr;
  ClangGetDeclObjCTypeEncoding clang_getDeclObjCTypeEncoding = nullptr;
  ClangTypeGetObjCEncoding clang_Type_getObjCEncoding = nullptr;
  ClangGetTypeKindSpelling clang_getTypeKindSpelling = nullptr;
  ClangGetFunctionTypeCallingConv clang_getFunctionTypeCallingConv = nullptr;
  ClangGetResultType clang_getResultType = nullptr;
  ClangGetExceptionSpecificationType clang_getExceptionSpecificationType = nullptr;
  ClangGetNumArgTypes clang_getNumArgTypes = nullptr;
  ClangGetArgType clang_getArgType = nullptr;
  ClangIsFunctionTypeVariadic clang_isFunctionTypeVariadic = nullptr;
  ClangGetCursorResultType clang_getCursorResultType = nullptr;
  ClangGetCursorExceptionSpecificationType clang_getCursorExceptionSpecificationType = nullptr;
  ClangIsPODType clang_isPODType = nullptr;
  ClangGetElementType clang_getElementType = nullptr;
  ClangGetNumElements clang_getNumElements = nullptr;
  ClangGetArrayElementType clang_getArrayElementType = nullptr;
  ClangGetArraySize clang_getArraySize = nullptr;
  ClangTypeGetNamedType clang_Type_getNamedType = nullptr;
  ClangTypeIsTransparentTagTypedef clang_Type_isTransparentTagTypedef = nullptr;
  ClangTypeGetAlignOf clang_Type_getAlignOf = nullptr;
  ClangTypeGetClassType clang_Type_getClassType = nullptr;
  ClangTypeGetSizeOf clang_Type_getSizeOf = nullptr;
  ClangTypeGetOffsetOf clang_Type_getOffsetOf = nullptr;
  ClangCursorGetOffsetOfField clang_Cursor_getOffsetOfField = nullptr;
  ClangCursorIsAnonymous clang_Cursor_isAnonymous = nullptr;
  ClangTypeGetNumTemplateArguments clang_Type_getNumTemplateArguments = nullptr;
  ClangTypeGetTemplateArgumentAsType clang_Type_getTemplateArgumentAsType = nullptr;
  ClangTypeGetCXXRefQualifier clang_Type_getCXXRefQualifier = nullptr;
  ClangCursorIsBitField clang_Cursor_isBitField = nullptr;
  ClangIsVirtualBase clang_isVirtualBase = nullptr;
  ClangGetCXXAccessSpecifier clang_getCXXAccessSpecifier = nullptr;
  ClangCursorGetStorageClass clang_Cursor_getStorageClass = nullptr;
  ClangGetNumOverloadedDecls clang_getNumOverloadedDecls = nullptr;
  ClangGetOverloadedDecl clang_getOverloadedDecl = nullptr;
  ClangGetIBOutletCollectionType clang_getIBOutletCollectionType = nullptr;
  ClangVisitChildren clang_visitChildren = nullptr;
  ClangGetCursorUSR clang_getCursorUSR = nullptr;
  ClangConstructUSRObjCClass clang_constructUSR_ObjCClass = nullptr;
  ClangConstructUSRObjCCategory clang_constructUSR_ObjCCategory = nullptr;
  ClangConstructUSRObjCProtocol clang_constructUSR_ObjCProtocol = nullptr;
  ClangConstructUSRObjCIvar clang_constructUSR_ObjCIvar = nullptr;
  ClangConstructUSRObjCMethod clang_constructUSR_ObjCMethod = nullptr;
  ClangConstructUSRObjCProperty clang_constructUSR_ObjCProperty = nullptr;
  ClangGetCursorSpelling clang_getCursorSpelling = nullptr;
  ClangCursorGetSpellingNameRange clang_Cursor_getSpellingNameRange = nullptr;
  ClangPrintingPolicyGetProperty clang_PrintingPolicy_getProperty = nullptr;
  ClangPrintingPolicySetProperty clang_PrintingPolicy_setProperty = nullptr;
  ClangGetCursorPrintingPolicy clang_getCursorPrintingPolicy = nullptr;
  ClangPrintingPolicyDispose clang_PrintingPolicy_dispose = nullptr;
  ClangGetCursorPrettyPrinted clang_getCursorPrettyPrinted = nullptr;
  ClangGetCursorDisplayName clang_getCursorDisplayName = nullptr;
  ClangGetCursorReferenced clang_getCursorReferenced = nullptr;
  ClangGetCursorDefinition clang_getCursorDefinition = nullptr;
  ClangIsCursorDefinition clang_isCursorDefinition = nullptr;
  ClangGetCanonicalCursor clang_getCanonicalCursor = nullptr;
  ClangCursorGetObjCSelectorIndex clang_Cursor_getObjCSelectorIndex = nullptr;
  ClangCursorIsDynamicCall clang_Cursor_isDynamicCall = nullptr;
  ClangCursorGetReceiverType clang_Cursor_getReceiverType = nullptr;
  ClangCursorGetObjCPropertyAttributes clang_Cursor_getObjCPropertyAttributes = nullptr;
  ClangCursorGetObjCDeclQualifiers clang_Cursor_getObjCDeclQualifiers = nullptr;
  ClangCursorIsObjCOptional clang_Cursor_isObjCOptional = nullptr;
  ClangCursorIsVariadic clang_Cursor_isVariadic = nullptr;
  ClangCursorIsExternalSymbol clang_Cursor_isExternalSymbol = nullptr;
  ClangCursorGetCommentRange clang_Cursor_getCommentRange = nullptr;
  ClangCursorGetRawCommentText clang_Cursor_getRawCommentText = nullptr;
  ClangCursorGetBriefCommentText clang_Cursor_getBriefCommentText = nullptr;
  ClangCursorGetMangling clang_Cursor_getMangling = nullptr;
  ClangCursorGetCXXManglings clang_Cursor_getCXXManglings = nullptr;
  ClangCursorGetObjCManglings clang_Cursor_getObjCManglings = nullptr;
  ClangCursorGetModule clang_Cursor_getModule = nullptr;
  ClangGetModuleForFile clang_getModuleForFile = nullptr;
  ClangModuleGetASTFile clang_Module_getASTFile = nullptr;
  ClangModuleGetParent clang_Module_getParent = nullptr;
  ClangModuleGetName clang_Module_getName = nullptr;
  ClangModuleGetFullName clang_Module_getFullName = nullptr;
  ClangModuleIsSystem clang_Module_isSystem = nullptr;
  ClangModuleGetNumTopLevelHeaders clang_Module_getNumTopLevelHeaders = nullptr;
  ClangModuleGetTopLevelHeader clang_Module_getTopLevelHeader = nullptr;
  ClangCXXConstructorIsConvertingConstructor clang_CXXConstructor_isConvertingConstructor = nullptr;
  ClangCXXConstructorIsCopyConstructor clang_CXXConstructor_isCopyConstructor = nullptr;
  ClangCXXConstructorIsDefaultConstructor clang_CXXConstructor_isDefaultConstructor = nullptr;
  ClangCXXConstructorIsMoveConstructor clang_CXXConstructor_isMoveConstructor = nullptr;
  ClangCXXFieldIsMutable clang_CXXField_isMutable = nullptr;
  ClangCXXMethodIsDefaulted clang_CXXMethod_isDefaulted = nullptr;
  ClangCXXMethodIsPureVirtual clang_CXXMethod_isPureVirtual = nullptr;
  ClangCXXMethodIsVirtual clang_CXXMethod_isVirtual = nullptr;
  ClangCXXMethodIsStatic clang_CXXMethod_isStatic = nullptr;
  ClangCXXRecordIsAbstract clang_CXXRecord_isAbstract = nullptr;
  ClangEnumDeclIsScoped clang_EnumDecl_isScoped = nullptr;
  ClangCXXMethodIsConst clang_CXXMethod_isConst = nullptr;
  ClangGetTemplateCursorKind clang_getTemplateCursorKind = nullptr;
  ClangGetSpecializedCursorTemplate clang_getSpecializedCursorTemplate = nullptr;
  ClangGetCursorReferenceNameRange clang_getCursorReferenceNameRange = nullptr;

  ClangGetToken clang_getToken = nullptr;
  ClangGetTokenKind clang_getTokenKind = nullptr;
  ClangGetTokenSpelling clang_getTokenSpelling = nullptr;
  ClangGetTokenLocation clang_getTokenLocation = nullptr;
  ClangGetTokenExtent clang_getTokenExtent = nullptr;
  ClangTokenize clang_tokenize = nullptr;
  ClangAnnotateTokens clang_annotateTokens = nullptr;
  ClangDisposeTokens clang_disposeTokens = nullptr;
  ClangGetCursorKindSpelling clang_getCursorKindSpelling = nullptr;
  ClangGetDefinitionSpellingAndExtent clang_getDefinitionSpellingAndExtent = nullptr;
  ClangEnableStackTraces clang_enableStackTraces = nullptr;
  ClangExecuteOnThread clang_executeOnThread = nullptr;
  ClangGetClangVersion clang_getClangVersion = nullptr;
  ClangToggleCrashRecovery clang_toggleCrashRecovery = nullptr;

  ClangGetInclusions clang_getInclusions = nullptr;

  ClangCursorEvaluate clang_Cursor_Evaluate = nullptr;
  ClangEvalResultGetKind clang_EvalResult_getKind = nullptr;
  ClangEvalResultGetAsInt clang_EvalResult_getAsInt = nullptr;
  ClangEvalResultGetAsLongLong clang_EvalResult_getAsLongLong = nullptr;
  ClangEvalResultIsUnsignedInt clang_EvalResult_isUnsignedInt = nullptr;
  ClangEvalResultGetAsUnsigned clang_EvalResult_getAsUnsigned = nullptr;
  ClangEvalResultGetAsDouble clang_EvalResult_getAsDouble = nullptr;
  ClangEvalResultGetAsStr clang_EvalResult_getAsStr = nullptr;
  ClangEvalResultDispose clang_EvalResult_dispose = nullptr;

  ClangGetRemappings clang_getRemappings = nullptr;
  ClangGetRemappingsFromFileList clang_getRemappingsFromFileList = nullptr;
  ClangRemapGetNumFiles clang_remap_getNumFiles = nullptr;
  ClangRemapGetFilenames clang_remap_getFilenames = nullptr;
  ClangRemapDispose clang_remap_dispose = nullptr;

  ClangFindReferencesInFile clang_findReferencesInFile = nullptr;
  ClangFindIncludesInFile clang_findIncludesInFile = nullptr;

  ClangIndexIsEntityObjCContainerKind clang_index_isEntityObjCContainerKind = nullptr;
  ClangIndexGetObjCContainerDeclInfo clang_index_getObjCContainerDeclInfo = nullptr;
  ClangIndexGetObjCInterfaceDeclInfo clang_index_getObjCInterfaceDeclInfo = nullptr;
  ClangIndexGetObjCCategoryDeclInfo clang_index_getObjCCategoryDeclInfo = nullptr;
  ClangIndexGetObjCProtocolRefListInfo clang_index_getObjCProtocolRefListInfo = nullptr;
  ClangIndexGetObjCPropertyDeclInfo clang_index_getObjCPropertyDeclInfo = nullptr;
  ClangIndexGetIBOutletCollectionAttrInfo clang_index_getIBOutletCollectionAttrInfo = nullptr;
  ClangIndexGetCXXClassDeclInfo clang_index_getCXXClassDeclInfo = nullptr;
  ClangIndexGetClientContainer clang_index_getClientContainer = nullptr;
  ClangIndexSetClientContainer clang_index_setClientContainer = nullptr;
  ClangIndexGetClientEntity clang_index_getClientEntity = nullptr;
  ClangIndexSetClientEntity clang_index_setClientEntity = nullptr;

  ClangIndexActionCreate clang_IndexAction_create = nullptr;
  ClangIndexActionDispose clang_IndexAction_dispose = nullptr;
  ClangIndexSourceFile clang_indexSourceFile = nullptr;
  ClangIndexSourceFileFullArgv clang_indexSourceFileFullArgv = nullptr;
  ClangIndexTranslationUnit clang_indexTranslationUnit = nullptr;
  ClangIndexLocGetFileLocation clang_indexLoc_getFileLocation = nullptr;
  ClangIndexLocGetCXSourceLocation clang_indexLoc_getCXSourceLocation = nullptr;
  ClangTypeVisitFields clang_Type_visitFields = nullptr;

public:

  ClangIndex createIndex();

  /* libclang helpers */
  
  std::string toStdString(CXString str);
  CXFile getCursorFile(CXCursor cursor);
  std::string getCursorFilePath(CXCursor cursor);
  std::string getCursorSpelling(CXCursor cursor);
  std::string getTypeSpelling(CXType type);
  std::string getTokenSpelling(CXTranslationUnit tu, CXToken tok);
  bool isForwardDeclaration(CXCursor cursor);

public:
  LibClang();
  ~LibClang();

  CXVersion version() const;
  const std::string& printableVersion() const;
};

} // namespace cxx

#endif // LIBCLANGUTILS_LIBCLANG_H
