// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/libclang.h"

#include "dynlib/dynlib.h"

#include "libclang-utils/clang-index.h"

namespace cxx
{

template<typename T>
void resolve(dynlib::Library& libclang, const char* name, T& callback)
{
  callback = (T) libclang.resolve(name);

  if (!callback)
    throw LibClangError{ ("could not resolve libclang function : " + std::string(name)).c_str() };
}

static CXVersion parse_clang_version(std::string str)
{
  CXVersion result;

  auto is_digit = [](char c) { return c >= '0' && c <= '9'; };

  auto it = str.begin();

  while (!is_digit(*it)) ++it;

  auto major_begin = it;

  while (is_digit(*it)) ++it;

  result.Major = std::stoi(std::string(major_begin, it));

  while (!is_digit(*it)) ++it;

  auto minor_begin = it;

  while (is_digit(*it)) ++it;

  result.Minor = std::stoi(std::string(minor_begin, it));

  while (!is_digit(*it)) ++it;

  auto subminor_begin = it;

  while (it != str.end() && is_digit(*it)) ++it;

  result.Subminor = std::stoi(std::string(subminor_begin, it));


  return result;
}

LibClang::LibClang()
{
  libclang.reset(new dynlib::Library("libclang"));

  if (!libclang->load())
    throw LibClangError{ "could not load libclang" };

  resolve(*libclang, "clang_getClangVersion", clang_getClangVersion);
  resolve(*libclang, "clang_getCString", clang_getCString);
  resolve(*libclang, "clang_disposeString", clang_disposeString);

  m_printable_version = toStdString(clang_getClangVersion());
  m_version = parse_clang_version(m_printable_version);

  resolve(*libclang, "clang_disposeStringSet", clang_disposeStringSet);

  resolve(*libclang, "clang_createIndex", clang_createIndex);
  resolve(*libclang, "clang_disposeIndex", clang_disposeIndex);
  resolve(*libclang, "clang_CXIndex_setGlobalOptions", clang_CXIndex_setGlobalOptions);
  resolve(*libclang, "clang_CXIndex_getGlobalOptions", clang_CXIndex_getGlobalOptions);
  resolve(*libclang, "clang_CXIndex_setInvocationEmissionPathOption", clang_CXIndex_setInvocationEmissionPathOption);

  resolve(*libclang, "clang_getFileName", clang_getFileName);
  resolve(*libclang, "clang_getFileUniqueID", clang_getFileUniqueID);
  resolve(*libclang, "clang_isFileMultipleIncludeGuarded", clang_isFileMultipleIncludeGuarded);
  resolve(*libclang, "clang_getFile", clang_getFile);
  resolve(*libclang, "clang_getFileContents", clang_getFileContents);
  resolve(*libclang, "clang_File_isEqual", clang_File_isEqual);
  resolve(*libclang, "clang_File_tryGetRealPathName", clang_File_tryGetRealPathName);

  resolve(*libclang, "clang_getNullLocation", clang_getNullLocation);
  resolve(*libclang, "clang_equalLocations", clang_equalLocations);
  resolve(*libclang, "clang_getLocation", clang_getLocation);
  resolve(*libclang, "clang_getLocationForOffset", clang_getLocationForOffset);
  resolve(*libclang, "clang_Location_isInSystemHeader", clang_Location_isInSystemHeader);
  resolve(*libclang, "clang_Location_isFromMainFile", clang_Location_isFromMainFile);
  resolve(*libclang, "clang_getNullRange", clang_getNullRange);
  resolve(*libclang, "clang_getRange", clang_getRange);
  resolve(*libclang, "clang_equalRanges", clang_equalRanges);
  resolve(*libclang, "clang_Range_isNull", clang_Range_isNull);
  resolve(*libclang, "clang_getExpansionLocation", clang_getExpansionLocation);
  resolve(*libclang, "clang_getPresumedLocation", clang_getPresumedLocation);
  resolve(*libclang, "clang_getInstantiationLocation", clang_getInstantiationLocation);
  resolve(*libclang, "clang_getSpellingLocation", clang_getSpellingLocation);
  resolve(*libclang, "clang_getFileLocation", clang_getFileLocation);
  resolve(*libclang, "clang_getRangeStart", clang_getRangeStart);
  resolve(*libclang, "clang_getRangeEnd", clang_getRangeEnd);
  resolve(*libclang, "clang_getSkippedRanges", clang_getSkippedRanges);
  resolve(*libclang, "clang_getAllSkippedRanges", clang_getAllSkippedRanges);
  resolve(*libclang, "clang_disposeSourceRangeList", clang_disposeSourceRangeList);

  resolve(*libclang, "clang_getNumDiagnosticsInSet", clang_getNumDiagnosticsInSet);
  resolve(*libclang, "clang_getDiagnosticInSet", clang_getDiagnosticInSet);
  resolve(*libclang, "clang_loadDiagnostics", clang_loadDiagnostics);
  resolve(*libclang, "clang_disposeDiagnosticSet", clang_disposeDiagnosticSet);
  resolve(*libclang, "clang_getChildDiagnostics", clang_getChildDiagnostics);
  resolve(*libclang, "clang_getNumDiagnostics", clang_getNumDiagnostics);
  resolve(*libclang, "clang_getDiagnostic", clang_getDiagnostic);
  resolve(*libclang, "clang_getDiagnosticSetFromTU", clang_getDiagnosticSetFromTU);
  resolve(*libclang, "clang_disposeDiagnostic", clang_disposeDiagnostic);
  resolve(*libclang, "clang_formatDiagnostic", clang_formatDiagnostic);
  resolve(*libclang, "clang_defaultDiagnosticDisplayOptions", clang_defaultDiagnosticDisplayOptions);
  resolve(*libclang, "clang_getDiagnosticSeverity", clang_getDiagnosticSeverity);
  resolve(*libclang, "clang_getDiagnosticLocation", clang_getDiagnosticLocation);
  resolve(*libclang, "clang_getDiagnosticSpelling", clang_getDiagnosticSpelling);
  resolve(*libclang, "clang_getDiagnosticOption", clang_getDiagnosticOption);
  resolve(*libclang, "clang_getDiagnosticCategory", clang_getDiagnosticCategory);
  resolve(*libclang, "clang_getDiagnosticCategoryText", clang_getDiagnosticCategoryText);
  resolve(*libclang, "clang_getDiagnosticNumRanges", clang_getDiagnosticNumRanges);
  resolve(*libclang, "clang_getDiagnosticRange", clang_getDiagnosticRange);
  resolve(*libclang, "clang_getDiagnosticNumFixIts", clang_getDiagnosticNumFixIts);
  resolve(*libclang, "clang_getDiagnosticFixIt", clang_getDiagnosticFixIt);
  resolve(*libclang, "clang_getTranslationUnitSpelling", clang_getTranslationUnitSpelling);
  resolve(*libclang, "clang_createTranslationUnitFromSourceFile", clang_createTranslationUnitFromSourceFile);
  resolve(*libclang, "clang_createTranslationUnit", clang_createTranslationUnit);
  resolve(*libclang, "clang_createTranslationUnit2", clang_createTranslationUnit2);
  resolve(*libclang, "clang_defaultEditingTranslationUnitOptions", clang_defaultEditingTranslationUnitOptions);
  resolve(*libclang, "clang_parseTranslationUnit", clang_parseTranslationUnit);
  resolve(*libclang, "clang_parseTranslationUnit2", clang_parseTranslationUnit2);
  resolve(*libclang, "clang_parseTranslationUnit2FullArgv", clang_parseTranslationUnit2FullArgv);
  resolve(*libclang, "clang_defaultSaveOptions", clang_defaultSaveOptions);
  resolve(*libclang, "clang_saveTranslationUnit", clang_saveTranslationUnit);
  resolve(*libclang, "clang_suspendTranslationUnit", clang_suspendTranslationUnit);
  resolve(*libclang, "clang_disposeTranslationUnit", clang_disposeTranslationUnit);
  resolve(*libclang, "clang_defaultReparseOptions", clang_defaultReparseOptions);
  resolve(*libclang, "clang_reparseTranslationUnit", clang_reparseTranslationUnit);
  resolve(*libclang, "clang_getTUResourceUsageName", clang_getTUResourceUsageName);
  resolve(*libclang, "clang_getCXTUResourceUsage", clang_getCXTUResourceUsage);
  resolve(*libclang, "clang_disposeCXTUResourceUsage", clang_disposeCXTUResourceUsage);
  resolve(*libclang, "clang_getTranslationUnitTargetInfo", clang_getTranslationUnitTargetInfo);
  resolve(*libclang, "clang_TargetInfo_dispose", clang_TargetInfo_dispose);
  resolve(*libclang, "clang_TargetInfo_getTriple", clang_TargetInfo_getTriple);
  resolve(*libclang, "clang_TargetInfo_getPointerWidth", clang_TargetInfo_getPointerWidth);
  resolve(*libclang, "clang_getNullCursor", clang_getNullCursor);
  resolve(*libclang, "clang_getTranslationUnitCursor", clang_getTranslationUnitCursor);
  resolve(*libclang, "clang_equalCursors", clang_equalCursors);
  resolve(*libclang, "clang_Cursor_isNull", clang_Cursor_isNull);
  resolve(*libclang, "clang_hashCursor", clang_hashCursor);
  resolve(*libclang, "clang_getCursorKind", clang_getCursorKind);
  resolve(*libclang, "clang_isDeclaration", clang_isDeclaration);
  resolve(*libclang, "clang_isInvalidDeclaration", clang_isInvalidDeclaration);
  resolve(*libclang, "clang_isReference", clang_isReference);
  resolve(*libclang, "clang_isExpression", clang_isExpression);
  resolve(*libclang, "clang_isStatement", clang_isStatement);
  resolve(*libclang, "clang_isAttribute", clang_isAttribute);
  resolve(*libclang, "clang_Cursor_hasAttrs", clang_Cursor_hasAttrs);
  resolve(*libclang, "clang_isInvalid", clang_isInvalid);
  resolve(*libclang, "clang_isTranslationUnit", clang_isTranslationUnit);
  resolve(*libclang, "clang_isPreprocessing", clang_isPreprocessing);
  resolve(*libclang, "clang_isUnexposed", clang_isUnexposed);
  resolve(*libclang, "clang_getCursorLinkage", clang_getCursorLinkage);
  resolve(*libclang, "clang_getCursorVisibility", clang_getCursorVisibility);
  resolve(*libclang, "clang_getCursorAvailability", clang_getCursorAvailability);
  resolve(*libclang, "clang_getCursorPlatformAvailability", clang_getCursorPlatformAvailability);
  resolve(*libclang, "clang_disposeCXPlatformAvailability", clang_disposeCXPlatformAvailability);
  resolve(*libclang, "clang_getCursorLanguage", clang_getCursorLanguage);
  resolve(*libclang, "clang_getCursorTLSKind", clang_getCursorTLSKind);
  resolve(*libclang, "clang_Cursor_getTranslationUnit", clang_Cursor_getTranslationUnit);
  resolve(*libclang, "clang_createCXCursorSet", clang_createCXCursorSet);
  resolve(*libclang, "clang_disposeCXCursorSet", clang_disposeCXCursorSet);
  resolve(*libclang, "clang_CXCursorSet_contains", clang_CXCursorSet_contains);
  resolve(*libclang, "clang_CXCursorSet_insert", clang_CXCursorSet_insert);
  resolve(*libclang, "clang_getCursorSemanticParent", clang_getCursorSemanticParent);
  resolve(*libclang, "clang_getCursorLexicalParent", clang_getCursorLexicalParent);
  resolve(*libclang, "clang_getOverriddenCursors", clang_getOverriddenCursors);
  resolve(*libclang, "clang_disposeOverriddenCursors", clang_disposeOverriddenCursors);
  resolve(*libclang, "clang_getIncludedFile", clang_getIncludedFile);
  resolve(*libclang, "clang_getCursor", clang_getCursor);
  resolve(*libclang, "clang_getCursorLocation", clang_getCursorLocation);
  resolve(*libclang, "clang_getCursorExtent", clang_getCursorExtent);
  resolve(*libclang, "clang_getCursorType", clang_getCursorType);
  resolve(*libclang, "clang_getTypeSpelling", clang_getTypeSpelling);
  resolve(*libclang, "clang_getTypedefDeclUnderlyingType", clang_getTypedefDeclUnderlyingType);
  resolve(*libclang, "clang_getEnumDeclIntegerType", clang_getEnumDeclIntegerType);
  resolve(*libclang, "clang_getEnumConstantDeclValue", clang_getEnumConstantDeclValue);
  resolve(*libclang, "clang_getEnumConstantDeclUnsignedValue", clang_getEnumConstantDeclUnsignedValue);
  resolve(*libclang, "clang_getFieldDeclBitWidth", clang_getFieldDeclBitWidth);
  resolve(*libclang, "clang_Cursor_getNumArguments", clang_Cursor_getNumArguments);
  resolve(*libclang, "clang_Cursor_getArgument", clang_Cursor_getArgument);
  resolve(*libclang, "clang_Cursor_getNumTemplateArguments", clang_Cursor_getNumTemplateArguments);
  resolve(*libclang, "clang_Cursor_getTemplateArgumentKind", clang_Cursor_getTemplateArgumentKind);
  resolve(*libclang, "clang_Cursor_getTemplateArgumentType", clang_Cursor_getTemplateArgumentType);
  resolve(*libclang, "clang_Cursor_getTemplateArgumentValue", clang_Cursor_getTemplateArgumentValue);
  resolve(*libclang, "clang_Cursor_getTemplateArgumentUnsignedValue", clang_Cursor_getTemplateArgumentUnsignedValue);
  resolve(*libclang, "clang_equalTypes", clang_equalTypes);
  resolve(*libclang, "clang_getCanonicalType", clang_getCanonicalType);
  resolve(*libclang, "clang_isConstQualifiedType", clang_isConstQualifiedType);
  resolve(*libclang, "clang_Cursor_isMacroFunctionLike", clang_Cursor_isMacroFunctionLike);
  resolve(*libclang, "clang_Cursor_isMacroBuiltin", clang_Cursor_isMacroBuiltin);
  resolve(*libclang, "clang_Cursor_isFunctionInlined", clang_Cursor_isFunctionInlined);
  resolve(*libclang, "clang_isVolatileQualifiedType", clang_isVolatileQualifiedType);
  resolve(*libclang, "clang_isRestrictQualifiedType", clang_isRestrictQualifiedType);
  resolve(*libclang, "clang_getAddressSpace", clang_getAddressSpace);
  resolve(*libclang, "clang_getTypedefName", clang_getTypedefName);
  resolve(*libclang, "clang_getPointeeType", clang_getPointeeType);
  resolve(*libclang, "clang_getTypeDeclaration", clang_getTypeDeclaration);
  resolve(*libclang, "clang_getDeclObjCTypeEncoding", clang_getDeclObjCTypeEncoding);
  resolve(*libclang, "clang_Type_getObjCEncoding", clang_Type_getObjCEncoding);
  resolve(*libclang, "clang_getTypeKindSpelling", clang_getTypeKindSpelling);
  resolve(*libclang, "clang_getFunctionTypeCallingConv", clang_getFunctionTypeCallingConv);
  resolve(*libclang, "clang_getResultType", clang_getResultType);
  resolve(*libclang, "clang_getExceptionSpecificationType", clang_getExceptionSpecificationType);
  resolve(*libclang, "clang_getNumArgTypes", clang_getNumArgTypes);
  resolve(*libclang, "clang_getArgType", clang_getArgType);
  resolve(*libclang, "clang_isFunctionTypeVariadic", clang_isFunctionTypeVariadic);
  resolve(*libclang, "clang_getCursorResultType", clang_getCursorResultType);
  resolve(*libclang, "clang_getCursorExceptionSpecificationType", clang_getCursorExceptionSpecificationType);
  resolve(*libclang, "clang_isPODType", clang_isPODType);
  resolve(*libclang, "clang_getElementType", clang_getElementType);
  resolve(*libclang, "clang_getNumElements", clang_getNumElements);
  resolve(*libclang, "clang_getArrayElementType", clang_getArrayElementType);
  resolve(*libclang, "clang_getArraySize", clang_getArraySize);
  resolve(*libclang, "clang_Type_getNamedType", clang_Type_getNamedType);
  resolve(*libclang, "clang_Type_isTransparentTagTypedef", clang_Type_isTransparentTagTypedef);
  resolve(*libclang, "clang_Type_getAlignOf", clang_Type_getAlignOf);
  resolve(*libclang, "clang_Type_getClassType", clang_Type_getClassType);
  resolve(*libclang, "clang_Type_getSizeOf", clang_Type_getSizeOf);
  resolve(*libclang, "clang_Type_getOffsetOf", clang_Type_getOffsetOf);
  resolve(*libclang, "clang_Cursor_getOffsetOfField", clang_Cursor_getOffsetOfField);
  resolve(*libclang, "clang_Cursor_isAnonymous", clang_Cursor_isAnonymous);
  resolve(*libclang, "clang_Type_getNumTemplateArguments", clang_Type_getNumTemplateArguments);
  resolve(*libclang, "clang_Type_getTemplateArgumentAsType", clang_Type_getTemplateArgumentAsType);
  resolve(*libclang, "clang_Type_getCXXRefQualifier", clang_Type_getCXXRefQualifier);
  resolve(*libclang, "clang_Cursor_isBitField", clang_Cursor_isBitField);
  resolve(*libclang, "clang_isVirtualBase", clang_isVirtualBase);
  resolve(*libclang, "clang_getCXXAccessSpecifier", clang_getCXXAccessSpecifier);
  resolve(*libclang, "clang_Cursor_getStorageClass", clang_Cursor_getStorageClass);
  resolve(*libclang, "clang_getNumOverloadedDecls", clang_getNumOverloadedDecls);
  resolve(*libclang, "clang_getOverloadedDecl", clang_getOverloadedDecl);
  resolve(*libclang, "clang_getIBOutletCollectionType", clang_getIBOutletCollectionType);
  resolve(*libclang, "clang_visitChildren", clang_visitChildren);
  resolve(*libclang, "clang_getCursorUSR", clang_getCursorUSR);
  resolve(*libclang, "clang_constructUSR_ObjCClass", clang_constructUSR_ObjCClass);
  resolve(*libclang, "clang_constructUSR_ObjCCategory", clang_constructUSR_ObjCCategory);
  resolve(*libclang, "clang_constructUSR_ObjCProtocol", clang_constructUSR_ObjCProtocol);
  resolve(*libclang, "clang_constructUSR_ObjCIvar", clang_constructUSR_ObjCIvar);
  resolve(*libclang, "clang_constructUSR_ObjCMethod", clang_constructUSR_ObjCMethod);
  resolve(*libclang, "clang_constructUSR_ObjCProperty", clang_constructUSR_ObjCProperty);
  resolve(*libclang, "clang_getCursorSpelling", clang_getCursorSpelling);
  resolve(*libclang, "clang_Cursor_getSpellingNameRange", clang_Cursor_getSpellingNameRange);
  resolve(*libclang, "clang_PrintingPolicy_getProperty", clang_PrintingPolicy_getProperty);
  resolve(*libclang, "clang_PrintingPolicy_setProperty", clang_PrintingPolicy_setProperty);
  resolve(*libclang, "clang_getCursorPrintingPolicy", clang_getCursorPrintingPolicy);
  resolve(*libclang, "clang_PrintingPolicy_dispose", clang_PrintingPolicy_dispose);
  resolve(*libclang, "clang_getCursorPrettyPrinted", clang_getCursorPrettyPrinted);
  resolve(*libclang, "clang_getCursorDisplayName", clang_getCursorDisplayName);
  resolve(*libclang, "clang_getCursorReferenced", clang_getCursorReferenced);
  resolve(*libclang, "clang_getCursorDefinition", clang_getCursorDefinition);
  resolve(*libclang, "clang_isCursorDefinition", clang_isCursorDefinition);
  resolve(*libclang, "clang_getCanonicalCursor", clang_getCanonicalCursor);
  resolve(*libclang, "clang_Cursor_getObjCSelectorIndex", clang_Cursor_getObjCSelectorIndex);
  resolve(*libclang, "clang_Cursor_isDynamicCall", clang_Cursor_isDynamicCall);
  resolve(*libclang, "clang_Cursor_getReceiverType", clang_Cursor_getReceiverType);
  resolve(*libclang, "clang_Cursor_getObjCPropertyAttributes", clang_Cursor_getObjCPropertyAttributes);
  resolve(*libclang, "clang_Cursor_getObjCDeclQualifiers", clang_Cursor_getObjCDeclQualifiers);
  resolve(*libclang, "clang_Cursor_isObjCOptional", clang_Cursor_isObjCOptional);
  resolve(*libclang, "clang_Cursor_isVariadic", clang_Cursor_isVariadic);
  resolve(*libclang, "clang_Cursor_isExternalSymbol", clang_Cursor_isExternalSymbol);
  resolve(*libclang, "clang_Cursor_getCommentRange", clang_Cursor_getCommentRange);
  resolve(*libclang, "clang_Cursor_getRawCommentText", clang_Cursor_getRawCommentText);
  resolve(*libclang, "clang_Cursor_getBriefCommentText", clang_Cursor_getBriefCommentText);
  resolve(*libclang, "clang_Cursor_getMangling", clang_Cursor_getMangling);
  resolve(*libclang, "clang_Cursor_getCXXManglings", clang_Cursor_getCXXManglings);
  resolve(*libclang, "clang_Cursor_getObjCManglings", clang_Cursor_getObjCManglings);
  resolve(*libclang, "clang_Cursor_getModule", clang_Cursor_getModule);
  resolve(*libclang, "clang_getModuleForFile", clang_getModuleForFile);
  resolve(*libclang, "clang_Module_getASTFile", clang_Module_getASTFile);
  resolve(*libclang, "clang_Module_getParent", clang_Module_getParent);
  resolve(*libclang, "clang_Module_getName", clang_Module_getName);
  resolve(*libclang, "clang_Module_getFullName", clang_Module_getFullName);
  resolve(*libclang, "clang_Module_isSystem", clang_Module_isSystem);
  resolve(*libclang, "clang_Module_getNumTopLevelHeaders", clang_Module_getNumTopLevelHeaders);
  resolve(*libclang, "clang_Module_getTopLevelHeader", clang_Module_getTopLevelHeader);
  resolve(*libclang, "clang_CXXConstructor_isConvertingConstructor", clang_CXXConstructor_isConvertingConstructor);
  resolve(*libclang, "clang_CXXConstructor_isCopyConstructor", clang_CXXConstructor_isCopyConstructor);
  resolve(*libclang, "clang_CXXConstructor_isDefaultConstructor", clang_CXXConstructor_isDefaultConstructor);
  resolve(*libclang, "clang_CXXConstructor_isMoveConstructor", clang_CXXConstructor_isMoveConstructor);
  resolve(*libclang, "clang_CXXField_isMutable", clang_CXXField_isMutable);
  resolve(*libclang, "clang_CXXMethod_isDefaulted", clang_CXXMethod_isDefaulted);
  resolve(*libclang, "clang_CXXMethod_isPureVirtual", clang_CXXMethod_isPureVirtual);
  resolve(*libclang, "clang_CXXMethod_isVirtual", clang_CXXMethod_isVirtual);
  resolve(*libclang, "clang_CXXMethod_isStatic", clang_CXXMethod_isStatic);
  resolve(*libclang, "clang_CXXRecord_isAbstract", clang_CXXRecord_isAbstract);
  resolve(*libclang, "clang_EnumDecl_isScoped", clang_EnumDecl_isScoped);
  resolve(*libclang, "clang_CXXMethod_isConst", clang_CXXMethod_isConst);
  resolve(*libclang, "clang_getTemplateCursorKind", clang_getTemplateCursorKind);
  resolve(*libclang, "clang_getSpecializedCursorTemplate", clang_getSpecializedCursorTemplate);
  resolve(*libclang, "clang_getCursorReferenceNameRange", clang_getCursorReferenceNameRange);

  resolve(*libclang, "clang_getToken", clang_getToken);
  resolve(*libclang, "clang_getTokenKind", clang_getTokenKind);
  resolve(*libclang, "clang_getTokenSpelling", clang_getTokenSpelling);
  resolve(*libclang, "clang_getTokenLocation", clang_getTokenLocation);
  resolve(*libclang, "clang_getTokenExtent", clang_getTokenExtent);
  resolve(*libclang, "clang_tokenize", clang_tokenize);
  resolve(*libclang, "clang_annotateTokens", clang_annotateTokens);
  resolve(*libclang, "clang_disposeTokens", clang_disposeTokens);
  resolve(*libclang, "clang_getCursorKindSpelling", clang_getCursorKindSpelling);
  resolve(*libclang, "clang_getDefinitionSpellingAndExtent", clang_getDefinitionSpellingAndExtent);
  resolve(*libclang, "clang_enableStackTraces", clang_enableStackTraces);
  resolve(*libclang, "clang_executeOnThread", clang_executeOnThread);
  resolve(*libclang, "clang_toggleCrashRecovery", clang_toggleCrashRecovery);

  resolve(*libclang, "clang_getInclusions", clang_getInclusions);

  resolve(*libclang, "clang_Cursor_Evaluate", clang_Cursor_Evaluate);
  resolve(*libclang, "clang_EvalResult_getKind", clang_EvalResult_getKind);
  resolve(*libclang, "clang_EvalResult_getAsInt", clang_EvalResult_getAsInt);
  resolve(*libclang, "clang_EvalResult_getAsLongLong", clang_EvalResult_getAsLongLong);
  resolve(*libclang, "clang_EvalResult_isUnsignedInt", clang_EvalResult_isUnsignedInt);
  resolve(*libclang, "clang_EvalResult_getAsUnsigned", clang_EvalResult_getAsUnsigned);
  resolve(*libclang, "clang_EvalResult_getAsDouble", clang_EvalResult_getAsDouble);
  resolve(*libclang, "clang_EvalResult_getAsStr", clang_EvalResult_getAsStr);
  resolve(*libclang, "clang_EvalResult_dispose", clang_EvalResult_dispose);

  resolve(*libclang, "clang_getRemappings", clang_getRemappings);
  resolve(*libclang, "clang_getRemappingsFromFileList", clang_getRemappingsFromFileList);
  resolve(*libclang, "clang_remap_getNumFiles", clang_remap_getNumFiles);
  resolve(*libclang, "clang_remap_getFilenames", clang_remap_getFilenames);
  resolve(*libclang, "clang_remap_dispose", clang_remap_dispose);

  resolve(*libclang, "clang_findReferencesInFile", clang_findReferencesInFile);
  resolve(*libclang, "clang_findIncludesInFile", clang_findIncludesInFile);

  resolve(*libclang, "clang_index_isEntityObjCContainerKind", clang_index_isEntityObjCContainerKind);
  resolve(*libclang, "clang_index_getObjCContainerDeclInfo", clang_index_getObjCContainerDeclInfo);
  resolve(*libclang, "clang_index_getObjCInterfaceDeclInfo", clang_index_getObjCInterfaceDeclInfo);
  resolve(*libclang, "clang_index_getObjCCategoryDeclInfo", clang_index_getObjCCategoryDeclInfo);
  resolve(*libclang, "clang_index_getObjCProtocolRefListInfo", clang_index_getObjCProtocolRefListInfo);
  resolve(*libclang, "clang_index_getObjCPropertyDeclInfo", clang_index_getObjCPropertyDeclInfo);
  resolve(*libclang, "clang_index_getIBOutletCollectionAttrInfo", clang_index_getIBOutletCollectionAttrInfo);
  resolve(*libclang, "clang_index_getCXXClassDeclInfo", clang_index_getCXXClassDeclInfo);
  resolve(*libclang, "clang_index_getClientContainer", clang_index_getClientContainer);
  resolve(*libclang, "clang_index_setClientContainer", clang_index_setClientContainer);
  resolve(*libclang, "clang_index_getClientEntity", clang_index_getClientEntity);
  resolve(*libclang, "clang_index_setClientEntity", clang_index_setClientEntity);

  resolve(*libclang, "clang_IndexAction_create", clang_IndexAction_create);
  resolve(*libclang, "clang_IndexAction_dispose", clang_IndexAction_dispose);
  resolve(*libclang, "clang_indexSourceFile", clang_indexSourceFile);
  resolve(*libclang, "clang_indexSourceFileFullArgv", clang_indexSourceFileFullArgv);
  resolve(*libclang, "clang_indexTranslationUnit", clang_indexTranslationUnit);
  resolve(*libclang, "clang_indexLoc_getFileLocation", clang_indexLoc_getFileLocation);
  resolve(*libclang, "clang_indexLoc_getCXSourceLocation", clang_indexLoc_getCXSourceLocation);
  resolve(*libclang, "clang_Type_visitFields", clang_Type_visitFields);
}

LibClang::~LibClang()
{

}

CXVersion LibClang::version() const
{
  return m_version;
}

const std::string& LibClang::printableVersion() const
{
  return m_printable_version;
}

ClangIndex LibClang::createIndex()
{
  return ClangIndex{ *this };
}

std::string LibClang::toStdString(CXString str)
{
  std::string result = clang_getCString(str);
  clang_disposeString(str);
  return result;
}

CXFile LibClang::getCursorFile(CXCursor cursor)
{
  CXSourceLocation location = clang_getCursorLocation(cursor);

  CXFile file;
  unsigned int line, col, offset;
  clang_getSpellingLocation(location, &file, &line, &col, &offset);

  return file;
}

std::string LibClang::getCursorFilePath(CXCursor cursor)
{
  CXFile file = getCursorFile(cursor);

  std::string result = toStdString(clang_getFileName(file));

  for (char& c : result)
  {
    if (c == '\\')
      c = '/';
  }

  return result;
}

std::string LibClang::getCursorSpelling(CXCursor cursor)
{
  return toStdString(clang_getCursorSpelling(cursor));
}

std::string LibClang::getTypeSpelling(CXType type)
{
  return toStdString(clang_getTypeSpelling(type));
}

std::string LibClang::getTokenSpelling(CXTranslationUnit tu, CXToken tok)
{
  return toStdString(clang_getTokenSpelling(tu, tok));
}

bool LibClang::isForwardDeclaration(CXCursor cursor)
{
  return !clang_equalCursors(clang_getCursorDefinition(cursor), cursor);
}

} // namespace cxx
