// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/libclang.h"

#include "dynlib/dynlib.h"

#include "libclang-utils/clang-index.h"

namespace libclang
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
  lib.reset(new dynlib::Library("libclang"));

  if (!lib->load())
    throw LibClangError{ "could not load libclang" };

  resolve(*lib, "clang_getClangVersion", clang_getClangVersion);
  resolve(*lib, "clang_getCString", clang_getCString);
  resolve(*lib, "clang_disposeString", clang_disposeString);

  m_printable_version = toStdString(clang_getClangVersion());
  m_version = parse_clang_version(m_printable_version);

  resolve(*lib, "clang_disposeStringSet", clang_disposeStringSet);

  resolve(*lib, "clang_createIndex", clang_createIndex);
  resolve(*lib, "clang_disposeIndex", clang_disposeIndex);
  resolve(*lib, "clang_CXIndex_setGlobalOptions", clang_CXIndex_setGlobalOptions);
  resolve(*lib, "clang_CXIndex_getGlobalOptions", clang_CXIndex_getGlobalOptions);
  resolve(*lib, "clang_CXIndex_setInvocationEmissionPathOption", clang_CXIndex_setInvocationEmissionPathOption);

  resolve(*lib, "clang_getFileName", clang_getFileName);
  resolve(*lib, "clang_getFileUniqueID", clang_getFileUniqueID);
  resolve(*lib, "clang_isFileMultipleIncludeGuarded", clang_isFileMultipleIncludeGuarded);
  resolve(*lib, "clang_getFile", clang_getFile);
  resolve(*lib, "clang_getFileContents", clang_getFileContents);
  resolve(*lib, "clang_File_isEqual", clang_File_isEqual);
  resolve(*lib, "clang_File_tryGetRealPathName", clang_File_tryGetRealPathName);

  resolve(*lib, "clang_getNullLocation", clang_getNullLocation);
  resolve(*lib, "clang_equalLocations", clang_equalLocations);
  resolve(*lib, "clang_getLocation", clang_getLocation);
  resolve(*lib, "clang_getLocationForOffset", clang_getLocationForOffset);
  resolve(*lib, "clang_Location_isInSystemHeader", clang_Location_isInSystemHeader);
  resolve(*lib, "clang_Location_isFromMainFile", clang_Location_isFromMainFile);
  resolve(*lib, "clang_getNullRange", clang_getNullRange);
  resolve(*lib, "clang_getRange", clang_getRange);
  resolve(*lib, "clang_equalRanges", clang_equalRanges);
  resolve(*lib, "clang_Range_isNull", clang_Range_isNull);
  resolve(*lib, "clang_getExpansionLocation", clang_getExpansionLocation);
  resolve(*lib, "clang_getPresumedLocation", clang_getPresumedLocation);
  resolve(*lib, "clang_getInstantiationLocation", clang_getInstantiationLocation);
  resolve(*lib, "clang_getSpellingLocation", clang_getSpellingLocation);
  resolve(*lib, "clang_getFileLocation", clang_getFileLocation);
  resolve(*lib, "clang_getRangeStart", clang_getRangeStart);
  resolve(*lib, "clang_getRangeEnd", clang_getRangeEnd);
  resolve(*lib, "clang_getSkippedRanges", clang_getSkippedRanges);
  resolve(*lib, "clang_getAllSkippedRanges", clang_getAllSkippedRanges);
  resolve(*lib, "clang_disposeSourceRangeList", clang_disposeSourceRangeList);

  resolve(*lib, "clang_getNumDiagnosticsInSet", clang_getNumDiagnosticsInSet);
  resolve(*lib, "clang_getDiagnosticInSet", clang_getDiagnosticInSet);
  resolve(*lib, "clang_loadDiagnostics", clang_loadDiagnostics);
  resolve(*lib, "clang_disposeDiagnosticSet", clang_disposeDiagnosticSet);
  resolve(*lib, "clang_getChildDiagnostics", clang_getChildDiagnostics);
  resolve(*lib, "clang_getNumDiagnostics", clang_getNumDiagnostics);
  resolve(*lib, "clang_getDiagnostic", clang_getDiagnostic);
  resolve(*lib, "clang_getDiagnosticSetFromTU", clang_getDiagnosticSetFromTU);
  resolve(*lib, "clang_disposeDiagnostic", clang_disposeDiagnostic);
  resolve(*lib, "clang_formatDiagnostic", clang_formatDiagnostic);
  resolve(*lib, "clang_defaultDiagnosticDisplayOptions", clang_defaultDiagnosticDisplayOptions);
  resolve(*lib, "clang_getDiagnosticSeverity", clang_getDiagnosticSeverity);
  resolve(*lib, "clang_getDiagnosticLocation", clang_getDiagnosticLocation);
  resolve(*lib, "clang_getDiagnosticSpelling", clang_getDiagnosticSpelling);
  resolve(*lib, "clang_getDiagnosticOption", clang_getDiagnosticOption);
  resolve(*lib, "clang_getDiagnosticCategory", clang_getDiagnosticCategory);
  resolve(*lib, "clang_getDiagnosticCategoryText", clang_getDiagnosticCategoryText);
  resolve(*lib, "clang_getDiagnosticNumRanges", clang_getDiagnosticNumRanges);
  resolve(*lib, "clang_getDiagnosticRange", clang_getDiagnosticRange);
  resolve(*lib, "clang_getDiagnosticNumFixIts", clang_getDiagnosticNumFixIts);
  resolve(*lib, "clang_getDiagnosticFixIt", clang_getDiagnosticFixIt);
  resolve(*lib, "clang_getTranslationUnitSpelling", clang_getTranslationUnitSpelling);
  resolve(*lib, "clang_createTranslationUnitFromSourceFile", clang_createTranslationUnitFromSourceFile);
  resolve(*lib, "clang_createTranslationUnit", clang_createTranslationUnit);
  resolve(*lib, "clang_createTranslationUnit2", clang_createTranslationUnit2);
  resolve(*lib, "clang_defaultEditingTranslationUnitOptions", clang_defaultEditingTranslationUnitOptions);
  resolve(*lib, "clang_parseTranslationUnit", clang_parseTranslationUnit);
  resolve(*lib, "clang_parseTranslationUnit2", clang_parseTranslationUnit2);
  resolve(*lib, "clang_parseTranslationUnit2FullArgv", clang_parseTranslationUnit2FullArgv);
  resolve(*lib, "clang_defaultSaveOptions", clang_defaultSaveOptions);
  resolve(*lib, "clang_saveTranslationUnit", clang_saveTranslationUnit);
  resolve(*lib, "clang_suspendTranslationUnit", clang_suspendTranslationUnit);
  resolve(*lib, "clang_disposeTranslationUnit", clang_disposeTranslationUnit);
  resolve(*lib, "clang_defaultReparseOptions", clang_defaultReparseOptions);
  resolve(*lib, "clang_reparseTranslationUnit", clang_reparseTranslationUnit);
  resolve(*lib, "clang_getTUResourceUsageName", clang_getTUResourceUsageName);
  resolve(*lib, "clang_getCXTUResourceUsage", clang_getCXTUResourceUsage);
  resolve(*lib, "clang_disposeCXTUResourceUsage", clang_disposeCXTUResourceUsage);
  resolve(*lib, "clang_getTranslationUnitTargetInfo", clang_getTranslationUnitTargetInfo);
  resolve(*lib, "clang_TargetInfo_dispose", clang_TargetInfo_dispose);
  resolve(*lib, "clang_TargetInfo_getTriple", clang_TargetInfo_getTriple);
  resolve(*lib, "clang_TargetInfo_getPointerWidth", clang_TargetInfo_getPointerWidth);
  resolve(*lib, "clang_getNullCursor", clang_getNullCursor);
  resolve(*lib, "clang_getTranslationUnitCursor", clang_getTranslationUnitCursor);
  resolve(*lib, "clang_equalCursors", clang_equalCursors);
  resolve(*lib, "clang_Cursor_isNull", clang_Cursor_isNull);
  resolve(*lib, "clang_hashCursor", clang_hashCursor);
  resolve(*lib, "clang_getCursorKind", clang_getCursorKind);
  resolve(*lib, "clang_isDeclaration", clang_isDeclaration);
  resolve(*lib, "clang_isInvalidDeclaration", clang_isInvalidDeclaration);
  resolve(*lib, "clang_isReference", clang_isReference);
  resolve(*lib, "clang_isExpression", clang_isExpression);
  resolve(*lib, "clang_isStatement", clang_isStatement);
  resolve(*lib, "clang_isAttribute", clang_isAttribute);
  resolve(*lib, "clang_Cursor_hasAttrs", clang_Cursor_hasAttrs);
  resolve(*lib, "clang_isInvalid", clang_isInvalid);
  resolve(*lib, "clang_isTranslationUnit", clang_isTranslationUnit);
  resolve(*lib, "clang_isPreprocessing", clang_isPreprocessing);
  resolve(*lib, "clang_isUnexposed", clang_isUnexposed);
  resolve(*lib, "clang_getCursorLinkage", clang_getCursorLinkage);
  resolve(*lib, "clang_getCursorVisibility", clang_getCursorVisibility);
  resolve(*lib, "clang_getCursorAvailability", clang_getCursorAvailability);
  resolve(*lib, "clang_getCursorPlatformAvailability", clang_getCursorPlatformAvailability);
  resolve(*lib, "clang_disposeCXPlatformAvailability", clang_disposeCXPlatformAvailability);
  resolve(*lib, "clang_getCursorLanguage", clang_getCursorLanguage);
  resolve(*lib, "clang_getCursorTLSKind", clang_getCursorTLSKind);
  resolve(*lib, "clang_Cursor_getTranslationUnit", clang_Cursor_getTranslationUnit);
  resolve(*lib, "clang_createCXCursorSet", clang_createCXCursorSet);
  resolve(*lib, "clang_disposeCXCursorSet", clang_disposeCXCursorSet);
  resolve(*lib, "clang_CXCursorSet_contains", clang_CXCursorSet_contains);
  resolve(*lib, "clang_CXCursorSet_insert", clang_CXCursorSet_insert);
  resolve(*lib, "clang_getCursorSemanticParent", clang_getCursorSemanticParent);
  resolve(*lib, "clang_getCursorLexicalParent", clang_getCursorLexicalParent);
  resolve(*lib, "clang_getOverriddenCursors", clang_getOverriddenCursors);
  resolve(*lib, "clang_disposeOverriddenCursors", clang_disposeOverriddenCursors);
  resolve(*lib, "clang_getIncludedFile", clang_getIncludedFile);
  resolve(*lib, "clang_getCursor", clang_getCursor);
  resolve(*lib, "clang_getCursorLocation", clang_getCursorLocation);
  resolve(*lib, "clang_getCursorExtent", clang_getCursorExtent);
  resolve(*lib, "clang_getCursorType", clang_getCursorType);
  resolve(*lib, "clang_getTypeSpelling", clang_getTypeSpelling);
  resolve(*lib, "clang_getTypedefDeclUnderlyingType", clang_getTypedefDeclUnderlyingType);
  resolve(*lib, "clang_getEnumDeclIntegerType", clang_getEnumDeclIntegerType);
  resolve(*lib, "clang_getEnumConstantDeclValue", clang_getEnumConstantDeclValue);
  resolve(*lib, "clang_getEnumConstantDeclUnsignedValue", clang_getEnumConstantDeclUnsignedValue);
  resolve(*lib, "clang_getFieldDeclBitWidth", clang_getFieldDeclBitWidth);
  resolve(*lib, "clang_Cursor_getNumArguments", clang_Cursor_getNumArguments);
  resolve(*lib, "clang_Cursor_getArgument", clang_Cursor_getArgument);
  resolve(*lib, "clang_Cursor_getNumTemplateArguments", clang_Cursor_getNumTemplateArguments);
  resolve(*lib, "clang_Cursor_getTemplateArgumentKind", clang_Cursor_getTemplateArgumentKind);
  resolve(*lib, "clang_Cursor_getTemplateArgumentType", clang_Cursor_getTemplateArgumentType);
  resolve(*lib, "clang_Cursor_getTemplateArgumentValue", clang_Cursor_getTemplateArgumentValue);
  resolve(*lib, "clang_Cursor_getTemplateArgumentUnsignedValue", clang_Cursor_getTemplateArgumentUnsignedValue);
  resolve(*lib, "clang_equalTypes", clang_equalTypes);
  resolve(*lib, "clang_getCanonicalType", clang_getCanonicalType);
  resolve(*lib, "clang_isConstQualifiedType", clang_isConstQualifiedType);
  resolve(*lib, "clang_Cursor_isMacroFunctionLike", clang_Cursor_isMacroFunctionLike);
  resolve(*lib, "clang_Cursor_isMacroBuiltin", clang_Cursor_isMacroBuiltin);
  resolve(*lib, "clang_Cursor_isFunctionInlined", clang_Cursor_isFunctionInlined);
  resolve(*lib, "clang_isVolatileQualifiedType", clang_isVolatileQualifiedType);
  resolve(*lib, "clang_isRestrictQualifiedType", clang_isRestrictQualifiedType);
  resolve(*lib, "clang_getAddressSpace", clang_getAddressSpace);
  resolve(*lib, "clang_getTypedefName", clang_getTypedefName);
  resolve(*lib, "clang_getPointeeType", clang_getPointeeType);
  resolve(*lib, "clang_getTypeDeclaration", clang_getTypeDeclaration);
  resolve(*lib, "clang_getDeclObjCTypeEncoding", clang_getDeclObjCTypeEncoding);
  resolve(*lib, "clang_Type_getObjCEncoding", clang_Type_getObjCEncoding);
  resolve(*lib, "clang_getTypeKindSpelling", clang_getTypeKindSpelling);
  resolve(*lib, "clang_getFunctionTypeCallingConv", clang_getFunctionTypeCallingConv);
  resolve(*lib, "clang_getResultType", clang_getResultType);
  resolve(*lib, "clang_getExceptionSpecificationType", clang_getExceptionSpecificationType);
  resolve(*lib, "clang_getNumArgTypes", clang_getNumArgTypes);
  resolve(*lib, "clang_getArgType", clang_getArgType);
  resolve(*lib, "clang_isFunctionTypeVariadic", clang_isFunctionTypeVariadic);
  resolve(*lib, "clang_getCursorResultType", clang_getCursorResultType);
  resolve(*lib, "clang_getCursorExceptionSpecificationType", clang_getCursorExceptionSpecificationType);
  resolve(*lib, "clang_isPODType", clang_isPODType);
  resolve(*lib, "clang_getElementType", clang_getElementType);
  resolve(*lib, "clang_getNumElements", clang_getNumElements);
  resolve(*lib, "clang_getArrayElementType", clang_getArrayElementType);
  resolve(*lib, "clang_getArraySize", clang_getArraySize);
  resolve(*lib, "clang_Type_getNamedType", clang_Type_getNamedType);
  resolve(*lib, "clang_Type_isTransparentTagTypedef", clang_Type_isTransparentTagTypedef);
  resolve(*lib, "clang_Type_getAlignOf", clang_Type_getAlignOf);
  resolve(*lib, "clang_Type_getClassType", clang_Type_getClassType);
  resolve(*lib, "clang_Type_getSizeOf", clang_Type_getSizeOf);
  resolve(*lib, "clang_Type_getOffsetOf", clang_Type_getOffsetOf);
  resolve(*lib, "clang_Cursor_getOffsetOfField", clang_Cursor_getOffsetOfField);
  resolve(*lib, "clang_Cursor_isAnonymous", clang_Cursor_isAnonymous);
  resolve(*lib, "clang_Type_getNumTemplateArguments", clang_Type_getNumTemplateArguments);
  resolve(*lib, "clang_Type_getTemplateArgumentAsType", clang_Type_getTemplateArgumentAsType);
  resolve(*lib, "clang_Type_getCXXRefQualifier", clang_Type_getCXXRefQualifier);
  resolve(*lib, "clang_Cursor_isBitField", clang_Cursor_isBitField);
  resolve(*lib, "clang_isVirtualBase", clang_isVirtualBase);
  resolve(*lib, "clang_getCXXAccessSpecifier", clang_getCXXAccessSpecifier);
  resolve(*lib, "clang_Cursor_getStorageClass", clang_Cursor_getStorageClass);
  resolve(*lib, "clang_getNumOverloadedDecls", clang_getNumOverloadedDecls);
  resolve(*lib, "clang_getOverloadedDecl", clang_getOverloadedDecl);
  resolve(*lib, "clang_getIBOutletCollectionType", clang_getIBOutletCollectionType);
  resolve(*lib, "clang_visitChildren", clang_visitChildren);
  resolve(*lib, "clang_getCursorUSR", clang_getCursorUSR);
  resolve(*lib, "clang_constructUSR_ObjCClass", clang_constructUSR_ObjCClass);
  resolve(*lib, "clang_constructUSR_ObjCCategory", clang_constructUSR_ObjCCategory);
  resolve(*lib, "clang_constructUSR_ObjCProtocol", clang_constructUSR_ObjCProtocol);
  resolve(*lib, "clang_constructUSR_ObjCIvar", clang_constructUSR_ObjCIvar);
  resolve(*lib, "clang_constructUSR_ObjCMethod", clang_constructUSR_ObjCMethod);
  resolve(*lib, "clang_constructUSR_ObjCProperty", clang_constructUSR_ObjCProperty);
  resolve(*lib, "clang_getCursorSpelling", clang_getCursorSpelling);
  resolve(*lib, "clang_Cursor_getSpellingNameRange", clang_Cursor_getSpellingNameRange);
  resolve(*lib, "clang_PrintingPolicy_getProperty", clang_PrintingPolicy_getProperty);
  resolve(*lib, "clang_PrintingPolicy_setProperty", clang_PrintingPolicy_setProperty);
  resolve(*lib, "clang_getCursorPrintingPolicy", clang_getCursorPrintingPolicy);
  resolve(*lib, "clang_PrintingPolicy_dispose", clang_PrintingPolicy_dispose);
  resolve(*lib, "clang_getCursorPrettyPrinted", clang_getCursorPrettyPrinted);
  resolve(*lib, "clang_getCursorDisplayName", clang_getCursorDisplayName);
  resolve(*lib, "clang_getCursorReferenced", clang_getCursorReferenced);
  resolve(*lib, "clang_getCursorDefinition", clang_getCursorDefinition);
  resolve(*lib, "clang_isCursorDefinition", clang_isCursorDefinition);
  resolve(*lib, "clang_getCanonicalCursor", clang_getCanonicalCursor);
  resolve(*lib, "clang_Cursor_getObjCSelectorIndex", clang_Cursor_getObjCSelectorIndex);
  resolve(*lib, "clang_Cursor_isDynamicCall", clang_Cursor_isDynamicCall);
  resolve(*lib, "clang_Cursor_getReceiverType", clang_Cursor_getReceiverType);
  resolve(*lib, "clang_Cursor_getObjCPropertyAttributes", clang_Cursor_getObjCPropertyAttributes);
  resolve(*lib, "clang_Cursor_getObjCDeclQualifiers", clang_Cursor_getObjCDeclQualifiers);
  resolve(*lib, "clang_Cursor_isObjCOptional", clang_Cursor_isObjCOptional);
  resolve(*lib, "clang_Cursor_isVariadic", clang_Cursor_isVariadic);
  resolve(*lib, "clang_Cursor_isExternalSymbol", clang_Cursor_isExternalSymbol);
  resolve(*lib, "clang_Cursor_getCommentRange", clang_Cursor_getCommentRange);
  resolve(*lib, "clang_Cursor_getRawCommentText", clang_Cursor_getRawCommentText);
  resolve(*lib, "clang_Cursor_getBriefCommentText", clang_Cursor_getBriefCommentText);
  resolve(*lib, "clang_Cursor_getMangling", clang_Cursor_getMangling);
  resolve(*lib, "clang_Cursor_getCXXManglings", clang_Cursor_getCXXManglings);
  resolve(*lib, "clang_Cursor_getObjCManglings", clang_Cursor_getObjCManglings);
  resolve(*lib, "clang_Cursor_getModule", clang_Cursor_getModule);
  resolve(*lib, "clang_getModuleForFile", clang_getModuleForFile);
  resolve(*lib, "clang_Module_getASTFile", clang_Module_getASTFile);
  resolve(*lib, "clang_Module_getParent", clang_Module_getParent);
  resolve(*lib, "clang_Module_getName", clang_Module_getName);
  resolve(*lib, "clang_Module_getFullName", clang_Module_getFullName);
  resolve(*lib, "clang_Module_isSystem", clang_Module_isSystem);
  resolve(*lib, "clang_Module_getNumTopLevelHeaders", clang_Module_getNumTopLevelHeaders);
  resolve(*lib, "clang_Module_getTopLevelHeader", clang_Module_getTopLevelHeader);
  resolve(*lib, "clang_CXXConstructor_isConvertingConstructor", clang_CXXConstructor_isConvertingConstructor);
  resolve(*lib, "clang_CXXConstructor_isCopyConstructor", clang_CXXConstructor_isCopyConstructor);
  resolve(*lib, "clang_CXXConstructor_isDefaultConstructor", clang_CXXConstructor_isDefaultConstructor);
  resolve(*lib, "clang_CXXConstructor_isMoveConstructor", clang_CXXConstructor_isMoveConstructor);
  resolve(*lib, "clang_CXXField_isMutable", clang_CXXField_isMutable);
  resolve(*lib, "clang_CXXMethod_isDefaulted", clang_CXXMethod_isDefaulted);
  resolve(*lib, "clang_CXXMethod_isPureVirtual", clang_CXXMethod_isPureVirtual);
  resolve(*lib, "clang_CXXMethod_isVirtual", clang_CXXMethod_isVirtual);
  resolve(*lib, "clang_CXXMethod_isStatic", clang_CXXMethod_isStatic);
  resolve(*lib, "clang_CXXRecord_isAbstract", clang_CXXRecord_isAbstract);
  resolve(*lib, "clang_EnumDecl_isScoped", clang_EnumDecl_isScoped);
  resolve(*lib, "clang_CXXMethod_isConst", clang_CXXMethod_isConst);
  resolve(*lib, "clang_getTemplateCursorKind", clang_getTemplateCursorKind);
  resolve(*lib, "clang_getSpecializedCursorTemplate", clang_getSpecializedCursorTemplate);
  resolve(*lib, "clang_getCursorReferenceNameRange", clang_getCursorReferenceNameRange);

  resolve(*lib, "clang_getToken", clang_getToken);
  resolve(*lib, "clang_getTokenKind", clang_getTokenKind);
  resolve(*lib, "clang_getTokenSpelling", clang_getTokenSpelling);
  resolve(*lib, "clang_getTokenLocation", clang_getTokenLocation);
  resolve(*lib, "clang_getTokenExtent", clang_getTokenExtent);
  resolve(*lib, "clang_tokenize", clang_tokenize);
  resolve(*lib, "clang_annotateTokens", clang_annotateTokens);
  resolve(*lib, "clang_disposeTokens", clang_disposeTokens);
  resolve(*lib, "clang_getCursorKindSpelling", clang_getCursorKindSpelling);
  resolve(*lib, "clang_getDefinitionSpellingAndExtent", clang_getDefinitionSpellingAndExtent);
  resolve(*lib, "clang_enableStackTraces", clang_enableStackTraces);
  resolve(*lib, "clang_executeOnThread", clang_executeOnThread);
  resolve(*lib, "clang_toggleCrashRecovery", clang_toggleCrashRecovery);

  resolve(*lib, "clang_getInclusions", clang_getInclusions);

  resolve(*lib, "clang_Cursor_Evaluate", clang_Cursor_Evaluate);
  resolve(*lib, "clang_EvalResult_getKind", clang_EvalResult_getKind);
  resolve(*lib, "clang_EvalResult_getAsInt", clang_EvalResult_getAsInt);
  resolve(*lib, "clang_EvalResult_getAsLongLong", clang_EvalResult_getAsLongLong);
  resolve(*lib, "clang_EvalResult_isUnsignedInt", clang_EvalResult_isUnsignedInt);
  resolve(*lib, "clang_EvalResult_getAsUnsigned", clang_EvalResult_getAsUnsigned);
  resolve(*lib, "clang_EvalResult_getAsDouble", clang_EvalResult_getAsDouble);
  resolve(*lib, "clang_EvalResult_getAsStr", clang_EvalResult_getAsStr);
  resolve(*lib, "clang_EvalResult_dispose", clang_EvalResult_dispose);

  resolve(*lib, "clang_getRemappings", clang_getRemappings);
  resolve(*lib, "clang_getRemappingsFromFileList", clang_getRemappingsFromFileList);
  resolve(*lib, "clang_remap_getNumFiles", clang_remap_getNumFiles);
  resolve(*lib, "clang_remap_getFilenames", clang_remap_getFilenames);
  resolve(*lib, "clang_remap_dispose", clang_remap_dispose);

  resolve(*lib, "clang_findReferencesInFile", clang_findReferencesInFile);
  resolve(*lib, "clang_findIncludesInFile", clang_findIncludesInFile);

  resolve(*lib, "clang_index_isEntityObjCContainerKind", clang_index_isEntityObjCContainerKind);
  resolve(*lib, "clang_index_getObjCContainerDeclInfo", clang_index_getObjCContainerDeclInfo);
  resolve(*lib, "clang_index_getObjCInterfaceDeclInfo", clang_index_getObjCInterfaceDeclInfo);
  resolve(*lib, "clang_index_getObjCCategoryDeclInfo", clang_index_getObjCCategoryDeclInfo);
  resolve(*lib, "clang_index_getObjCProtocolRefListInfo", clang_index_getObjCProtocolRefListInfo);
  resolve(*lib, "clang_index_getObjCPropertyDeclInfo", clang_index_getObjCPropertyDeclInfo);
  resolve(*lib, "clang_index_getIBOutletCollectionAttrInfo", clang_index_getIBOutletCollectionAttrInfo);
  resolve(*lib, "clang_index_getCXXClassDeclInfo", clang_index_getCXXClassDeclInfo);
  resolve(*lib, "clang_index_getClientContainer", clang_index_getClientContainer);
  resolve(*lib, "clang_index_setClientContainer", clang_index_setClientContainer);
  resolve(*lib, "clang_index_getClientEntity", clang_index_getClientEntity);
  resolve(*lib, "clang_index_setClientEntity", clang_index_setClientEntity);

  resolve(*lib, "clang_IndexAction_create", clang_IndexAction_create);
  resolve(*lib, "clang_IndexAction_dispose", clang_IndexAction_dispose);
  resolve(*lib, "clang_indexSourceFile", clang_indexSourceFile);
  resolve(*lib, "clang_indexSourceFileFullArgv", clang_indexSourceFileFullArgv);
  resolve(*lib, "clang_indexTranslationUnit", clang_indexTranslationUnit);
  resolve(*lib, "clang_indexLoc_getFileLocation", clang_indexLoc_getFileLocation);
  resolve(*lib, "clang_indexLoc_getCXSourceLocation", clang_indexLoc_getCXSourceLocation);
  resolve(*lib, "clang_Type_visitFields", clang_Type_visitFields);
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

Index LibClang::createIndex()
{
  return Index{ *this };
}

std::string LibClang::toStdString(CXString str)
{
  if (!str.data)
    return std::string();

  std::string result = clang_getCString(str);
  clang_disposeString(str);
  return result;
}

} // namespace libclang
