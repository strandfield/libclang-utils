// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_CLANG_CINDEX_H
#define LIBCLANGUTILS_CLANG_CINDEX_H

#include <cstddef>

/****************************************************************
For now this is limited to clang 7.0.0
****************************************************************/

/****************************************************************
clang-c/CXErrorCode.h
****************************************************************/

enum CXErrorCode
{
  CXError_Success = 0,
  CXError_Failure = 1,
  CXError_Crashed = 2,
  CXError_InvalidArguments = 3,
  CXError_ASTReadError = 4
};

/****************************************************************
clang-c/CXString.h
****************************************************************/

struct CXString
{
  const void* data;
  unsigned private_flags;
};

struct CXStringSet
{
  CXString* Strings;
  unsigned Count;
};

using ClangGetCString = const char* (*)(CXString);
using ClangDisposeString = void(*)(CXString);
using ClangDisposeStringSet = void(*)(CXStringSet*);

/****************************************************************
clang-c/Index.h
****************************************************************/

typedef void* CXIndex;
typedef struct CXTargetInfoImpl* CXTargetInfo;
typedef struct CXTranslationUnitImpl* CXTranslationUnit;
typedef void* CXClientData;

struct CXUnsavedFile
{
  const char* Filename;
  const char* Contents;
  unsigned long Length;
};

enum CXAvailabilityKind {
  CXAvailability_Available,
  CXAvailability_Deprecated,
  CXAvailability_NotAvailable,
  CXAvailability_NotAccessible
};

typedef struct CXVersion {
  int Major;
  int Minor;
  int Subminor;
} CXVersion;

enum CXCursor_ExceptionSpecificationKind {
  CXCursor_ExceptionSpecificationKind_None,
  CXCursor_ExceptionSpecificationKind_DynamicNone,
  CXCursor_ExceptionSpecificationKind_Dynamic,
  CXCursor_ExceptionSpecificationKind_MSAny,
  CXCursor_ExceptionSpecificationKind_BasicNoexcept,
  CXCursor_ExceptionSpecificationKind_ComputedNoexcept,
  CXCursor_ExceptionSpecificationKind_Unevaluated,
  CXCursor_ExceptionSpecificationKind_Uninstantiated,
  CXCursor_ExceptionSpecificationKind_Unparsed
};

using ClangCreateIndex = CXIndex(*)(int, int);
using ClangDisposeIndex = void(*)(CXIndex);

typedef enum {
  CXGlobalOpt_None = 0x0,
  CXGlobalOpt_ThreadBackgroundPriorityForIndexing = 0x1,
  CXGlobalOpt_ThreadBackgroundPriorityForEditing = 0x2,
  CXGlobalOpt_ThreadBackgroundPriorityForAll = CXGlobalOpt_ThreadBackgroundPriorityForIndexing | CXGlobalOpt_ThreadBackgroundPriorityForEditing
} CXGlobalOptFlags;

using ClangCXIndexSetGlobalOptions = void(*)(CXIndex, unsigned);
using ClangCXIndexGetGlobalOptions = unsigned(*)(CXIndex);
using ClangCXIndexSetInvocationEmissionPathOption = void(*)(CXIndex, const char*);

typedef void* CXFile;

using ClangGetFileName = CXString(*)(CXFile);

typedef struct {
  unsigned long long data[3];
} CXFileUniqueID;

using ClangGetFileUniqueID = int(*)(CXFile, CXFileUniqueID*);
using ClangIsFileMultipleIncludeGuard = unsigned(*)(CXTranslationUnit, CXFile);
using ClangGetFile = CXFile(*)(CXTranslationUnit, const char*);
using ClangGetFileContents = const char* (*)(CXTranslationUnit, CXFile, size_t*);
using ClangFileIsEqual = int(*)(CXFile, CXFile);
using ClangFileTryGetRealPathName = CXString(*)(CXFile);

typedef struct {
  const void* ptr_data[2];
  unsigned int_data;
} CXSourceLocation;

typedef struct {
  const void* ptr_data[2];
  unsigned begin_int_data;
  unsigned end_int_data;
} CXSourceRange;

using ClangGetNullLocation = CXSourceLocation(*)(void);
using ClangEqualLocations = unsigned(*)(CXSourceLocation, CXSourceLocation);
using ClangGetLocation = CXSourceLocation(*)(CXTranslationUnit, CXFile, unsigned, unsigned);
using ClangGetLocationForOffset = CXSourceLocation(*)(CXTranslationUnit, CXFile, unsigned);
using ClangLocationIsInSystemHeader = int(*)(CXSourceLocation);
using ClangLocationIsFromMainFile = int(*)(CXSourceLocation);
using ClangGetNullRange = CXSourceRange(*)(void);
using ClangGetRange = CXSourceRange(*)(CXSourceLocation, CXSourceLocation);
using ClangEqualRanges = unsigned(*)(CXSourceRange, CXSourceRange);
using ClangRangeIsNull = int(*)(CXSourceRange);
using ClangGetExpansionLocation = void(*)(CXSourceLocation, CXFile*, unsigned*, unsigned*, unsigned*);
using ClangGetPresumedLocation = void(*)(CXSourceLocation, CXString*, unsigned*, unsigned*);
using ClangGetInstantiationLocation = void(*)(CXSourceLocation, CXFile*, unsigned*, unsigned*, unsigned*);
using ClangGetSpellingLocation = void(*)(CXSourceLocation, CXFile*, unsigned*, unsigned*, unsigned*);
using ClangGetFileLocation = void(*)(CXSourceLocation, CXFile*, unsigned*, unsigned*, unsigned*);
using ClangGetRangeStart = CXSourceLocation(*)(CXSourceRange);
using ClangGetRangeEnd = CXSourceLocation(*)(CXSourceRange);

typedef struct {
  unsigned count;
  CXSourceRange* ranges;
} CXSourceRangeList;

using ClangGetSkippedRanges = CXSourceRangeList * (*)(CXTranslationUnit, CXFile);
using ClangGetAllSkippedRanges = CXSourceRangeList * (*)(CXTranslationUnit);
using ClangDisposeSourceRangeList = void(*)(CXSourceRangeList*);

enum CXDiagnosticSeverity {
  CXDiagnostic_Ignored = 0,
  CXDiagnostic_Note = 1,
  CXDiagnostic_Warning = 2,
  CXDiagnostic_Error = 3,
  CXDiagnostic_Fatal = 4
};

typedef void* CXDiagnostic;
typedef void* CXDiagnosticSet;

using ClangGetNumDiagnosticsInSet = unsigned(*)(CXDiagnosticSet);
using ClangGetDiagnosticInSet = CXDiagnostic(*)(CXDiagnosticSet, unsigned);

enum CXLoadDiag_Error {
  CXLoadDiag_None = 0,
  CXLoadDiag_Unknown = 1,
  CXLoadDiag_CannotLoad = 2,
  CXLoadDiag_InvalidFile = 3
};

using ClangLoadDiagnostics = CXDiagnosticSet(*)(const char*, enum CXLoadDiag_Error*, CXString*);
using ClangDisposeDiagnosticSet = void(*)(CXDiagnosticSet);
using ClangGetChildDiagnostics = CXDiagnosticSet(*)(CXDiagnostic);
using ClangGetNumDiagnostics = unsigned(*)(CXTranslationUnit);
using ClangGetDiagnostic = CXDiagnostic(*)(CXTranslationUnit, unsigned);
using ClangGetDiagnosticSetFromTU = CXDiagnosticSet(*)(CXTranslationUnit);
using ClangDisposeDiagnostic = void(*)(CXDiagnostic);

enum CXDiagnosticDisplayOptions {
  CXDiagnostic_DisplaySourceLocation = 0x01,
  CXDiagnostic_DisplayColumn = 0x02,
  CXDiagnostic_DisplaySourceRanges = 0x04,
  CXDiagnostic_DisplayOption = 0x08,
  CXDiagnostic_DisplayCategoryId = 0x10,
  CXDiagnostic_DisplayCategoryName = 0x20
};

using ClangFormatDiagnostic = CXString(*)(CXDiagnostic, unsigned);
using ClangDefaultDiagnosticDisplayOptions = unsigned (*)(void);
using ClangGetDiagnosticSeverity = CXDiagnosticSeverity (*)(CXDiagnostic);
using ClangGetDiagnosticLocation = CXSourceLocation (*)(CXDiagnostic);
using ClangGetDiagnosticSpelling = CXString(*) (CXDiagnostic);
using ClangGetDiagnosticOption = CXString (*)(CXDiagnostic, CXString*);
using ClangGetDiagnosticCategory = unsigned (*)(CXDiagnostic);
using ClangGetDiagnosticCategoryText = CXString (*)(CXDiagnostic);
using ClangGetDiagnosticNumRanges = unsigned(*)(CXDiagnostic);
using ClangGetDiagnosticRange = CXSourceRange(*) (CXDiagnostic, unsigned);
using ClangGetDiagnosticNumFixIts= unsigned (*)(CXDiagnostic);
using ClangGetDiagnosticFixIt = CXString (*)(CXDiagnostic, unsigned, CXSourceRange*);
using ClangGetTranslationUnitSpelling = CXString (*)(CXTranslationUnit);
using ClangCreateTranslationUnitFromSourceFile = CXTranslationUnit (*)(CXIndex, const char*, int, const char* const*, unsigned, struct CXUnsavedFile*);
using ClangCreateTranslationUnit = CXTranslationUnit (*)(CXIndex, const char*);
using ClangCreateTranslationUnit2 =  CXErrorCode (*)(CXIndex, const char*, CXTranslationUnit*);

enum CXTranslationUnit_Flags {
  CXTranslationUnit_None = 0x0,
  CXTranslationUnit_DetailedPreprocessingRecord = 0x01,
  CXTranslationUnit_Incomplete = 0x02,
  CXTranslationUnit_PrecompiledPreamble = 0x04,
  CXTranslationUnit_CacheCompletionResults = 0x08,
  CXTranslationUnit_ForSerialization = 0x10,
  CXTranslationUnit_CXXChainedPCH = 0x20,
  CXTranslationUnit_SkipFunctionBodies = 0x40,
  CXTranslationUnit_IncludeBriefCommentsInCodeCompletion = 0x80,
  CXTranslationUnit_CreatePreambleOnFirstParse = 0x100,
  CXTranslationUnit_KeepGoing = 0x200,
  CXTranslationUnit_SingleFileParse = 0x400,
  CXTranslationUnit_LimitSkipFunctionBodiesToPreamble = 0x800
};

using ClangDefaultEditingTranslationUnitOptions = unsigned (*)(void);
using ClangParseTranslationUnit = CXTranslationUnit (*)(CXIndex, const char*, const char* const*, int, struct CXUnsavedFile*, unsigned, unsigned);
using ClangParseTranslationUnit2 = CXErrorCode(*)(CXIndex, const char*, const char* const*, int, struct CXUnsavedFile*, unsigned, unsigned, CXTranslationUnit*);
using ClangParseTranslationUnit2FullArgv = enum CXErrorCode (*)(CXIndex, const char*, const char* const*, int, struct CXUnsavedFile*, unsigned, unsigned, CXTranslationUnit*);

enum CXSaveTranslationUnit_Flags {
  CXSaveTranslationUnit_None = 0x0
};

using ClangDefaultSaveOptions = unsigned (*)(CXTranslationUnit);

enum CXSaveError {
  CXSaveError_None = 0,
  CXSaveError_Unknown = 1,
  CXSaveError_TranslationErrors = 2,
  CXSaveError_InvalidTU = 3
};

using ClangSaveTranslationUnit = int (*)(CXTranslationUnit, const char*, unsigned);
using ClangSuspendTranslationUnit = unsigned (*)(CXTranslationUnit);
using ClangDisposeTranslationUnit = void(*)(CXTranslationUnit);

enum CXReparse_Flags {
  CXReparse_None = 0x0
};

using ClangDefaultReparseOptions = unsigned (*)(CXTranslationUnit);
using ClangReparseTranslationUnit = int (*)(CXTranslationUnit, unsigned, struct CXUnsavedFile*, unsigned);

enum CXTUResourceUsageKind {
  CXTUResourceUsage_AST = 1,
  CXTUResourceUsage_Identifiers = 2,
  CXTUResourceUsage_Selectors = 3,
  CXTUResourceUsage_GlobalCompletionResults = 4,
  CXTUResourceUsage_SourceManagerContentCache = 5,
  CXTUResourceUsage_AST_SideTables = 6,
  CXTUResourceUsage_SourceManager_Membuffer_Malloc = 7,
  CXTUResourceUsage_SourceManager_Membuffer_MMap = 8,
  CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc = 9,
  CXTUResourceUsage_ExternalASTSource_Membuffer_MMap = 10,
  CXTUResourceUsage_Preprocessor = 11,
  CXTUResourceUsage_PreprocessingRecord = 12,
  CXTUResourceUsage_SourceManager_DataStructures = 13,
  CXTUResourceUsage_Preprocessor_HeaderSearch = 14,
  CXTUResourceUsage_MEMORY_IN_BYTES_BEGIN = CXTUResourceUsage_AST,
  CXTUResourceUsage_MEMORY_IN_BYTES_END =
  CXTUResourceUsage_Preprocessor_HeaderSearch,
  CXTUResourceUsage_First = CXTUResourceUsage_AST,
  CXTUResourceUsage_Last = CXTUResourceUsage_Preprocessor_HeaderSearch
};

using ClangGetTUResourceUsageName = const char* (*)(enum CXTUResourceUsageKind);

typedef struct CXTUResourceUsageEntry {
  enum CXTUResourceUsageKind kind;
  unsigned long amount;
} CXTUResourceUsageEntry;

typedef struct CXTUResourceUsage {
  void* data;
  unsigned numEntries;
  CXTUResourceUsageEntry* entries;
} CXTUResourceUsage;

using ClangGetCXTUResourceUsage = CXTUResourceUsage (*)(CXTranslationUnit);
using ClangDisposeCXTUResourceUsage = void (*)(CXTUResourceUsage);
using ClangGetTranslationUnitTargetInfo = CXTargetInfo (*)(CXTranslationUnit);
using ClangTargetInfoDispose = void (*)(CXTargetInfo);
using ClangTargetInfoGetTriple = CXString (*)(CXTargetInfo);
using ClangTargetInfoGetPointerWidth = int (*)(CXTargetInfo);

enum CXCursorKind {
  CXCursor_UnexposedDecl = 1,
  CXCursor_StructDecl = 2,
  CXCursor_UnionDecl = 3,
  CXCursor_ClassDecl = 4,
  CXCursor_EnumDecl = 5,
  CXCursor_FieldDecl = 6,
  CXCursor_EnumConstantDecl = 7,
  CXCursor_FunctionDecl = 8,
  CXCursor_VarDecl = 9,
  CXCursor_ParmDecl = 10,
  CXCursor_ObjCInterfaceDecl = 11,
  CXCursor_ObjCCategoryDecl = 12,
  CXCursor_ObjCProtocolDecl = 13,
  CXCursor_ObjCPropertyDecl = 14,
  CXCursor_ObjCIvarDecl = 15,
  CXCursor_ObjCInstanceMethodDecl = 16,
  CXCursor_ObjCClassMethodDecl = 17,
  CXCursor_ObjCImplementationDecl = 18,
  CXCursor_ObjCCategoryImplDecl = 19,
  CXCursor_TypedefDecl = 20,
  CXCursor_CXXMethod = 21,
  CXCursor_Namespace = 22,
  CXCursor_LinkageSpec = 23,
  CXCursor_Constructor = 24,
  CXCursor_Destructor = 25,
  CXCursor_ConversionFunction = 26,
  CXCursor_TemplateTypeParameter = 27,
  CXCursor_NonTypeTemplateParameter = 28,
  CXCursor_TemplateTemplateParameter = 29,
  CXCursor_FunctionTemplate = 30,
  CXCursor_ClassTemplate = 31,
  CXCursor_ClassTemplatePartialSpecialization = 32,
  CXCursor_NamespaceAlias = 33,
  CXCursor_UsingDirective = 34,
  CXCursor_UsingDeclaration = 35,
  CXCursor_TypeAliasDecl = 36,
  CXCursor_ObjCSynthesizeDecl = 37,
  CXCursor_ObjCDynamicDecl = 38,
  CXCursor_CXXAccessSpecifier = 39,
  CXCursor_FirstDecl = CXCursor_UnexposedDecl,
  CXCursor_LastDecl = CXCursor_CXXAccessSpecifier,
  CXCursor_FirstRef = 40,
  CXCursor_ObjCSuperClassRef = 40,
  CXCursor_ObjCProtocolRef = 41,
  CXCursor_ObjCClassRef = 42,
  CXCursor_TypeRef = 43,
  CXCursor_CXXBaseSpecifier = 44,
  CXCursor_TemplateRef = 45,
  CXCursor_NamespaceRef = 46,
  CXCursor_MemberRef = 47,
  CXCursor_LabelRef = 48,
  CXCursor_OverloadedDeclRef = 49,
  CXCursor_VariableRef = 50,
  CXCursor_LastRef = CXCursor_VariableRef,
  CXCursor_FirstInvalid = 70,
  CXCursor_InvalidFile = 70,
  CXCursor_NoDeclFound = 71,
  CXCursor_NotImplemented = 72,
  CXCursor_InvalidCode = 73,
  CXCursor_LastInvalid = CXCursor_InvalidCode,
  CXCursor_FirstExpr = 100,
  CXCursor_UnexposedExpr = 100,
  CXCursor_DeclRefExpr = 101,
  CXCursor_MemberRefExpr = 102,
  CXCursor_CallExpr = 103,
  CXCursor_ObjCMessageExpr = 104,
  CXCursor_BlockExpr = 105,
  CXCursor_IntegerLiteral = 106,
  CXCursor_FloatingLiteral = 107,
  CXCursor_ImaginaryLiteral = 108,
  CXCursor_StringLiteral = 109,
  CXCursor_CharacterLiteral = 110,
  CXCursor_ParenExpr = 111,
  CXCursor_UnaryOperator = 112,
  CXCursor_ArraySubscriptExpr = 113,
  CXCursor_BinaryOperator = 114,
  CXCursor_CompoundAssignOperator = 115,
  CXCursor_ConditionalOperator = 116,
  CXCursor_CStyleCastExpr = 117,
  CXCursor_CompoundLiteralExpr = 118,
  CXCursor_InitListExpr = 119,
  CXCursor_AddrLabelExpr = 120,
  CXCursor_StmtExpr = 121,
  CXCursor_GenericSelectionExpr = 122,
  CXCursor_GNUNullExpr = 123,
  CXCursor_CXXStaticCastExpr = 124,
  CXCursor_CXXDynamicCastExpr = 125,
  CXCursor_CXXReinterpretCastExpr = 126,
  CXCursor_CXXConstCastExpr = 127,
  CXCursor_CXXFunctionalCastExpr = 128,
  CXCursor_CXXTypeidExpr = 129,
  CXCursor_CXXBoolLiteralExpr = 130,
  CXCursor_CXXNullPtrLiteralExpr = 131,
  CXCursor_CXXThisExpr = 132,
  CXCursor_CXXThrowExpr = 133,
  CXCursor_CXXNewExpr = 134,
  CXCursor_CXXDeleteExpr = 135,
  CXCursor_UnaryExpr = 136,
  CXCursor_ObjCStringLiteral = 137,
  CXCursor_ObjCEncodeExpr = 138,
  CXCursor_ObjCSelectorExpr = 139,
  CXCursor_ObjCProtocolExpr = 140,
  CXCursor_ObjCBridgedCastExpr = 141,
  CXCursor_PackExpansionExpr = 142,
  CXCursor_SizeOfPackExpr = 143,
  CXCursor_LambdaExpr = 144,
  CXCursor_ObjCBoolLiteralExpr = 145,
  CXCursor_ObjCSelfExpr = 146,
  CXCursor_OMPArraySectionExpr = 147,
  CXCursor_ObjCAvailabilityCheckExpr = 148,
  CXCursor_FixedPointLiteral = 149,
  CXCursor_LastExpr = CXCursor_FixedPointLiteral,
  CXCursor_FirstStmt = 200,
  CXCursor_UnexposedStmt = 200,
  CXCursor_LabelStmt = 201,
  CXCursor_CompoundStmt = 202,
  CXCursor_CaseStmt = 203,
  CXCursor_DefaultStmt = 204,
  CXCursor_IfStmt = 205,
  CXCursor_SwitchStmt = 206,
  CXCursor_WhileStmt = 207,
  CXCursor_DoStmt = 208,
  CXCursor_ForStmt = 209,
  CXCursor_GotoStmt = 210,
  CXCursor_IndirectGotoStmt = 211,
  CXCursor_ContinueStmt = 212,
  CXCursor_BreakStmt = 213,
  CXCursor_ReturnStmt = 214,
  CXCursor_GCCAsmStmt = 215,
  CXCursor_AsmStmt = CXCursor_GCCAsmStmt,
  CXCursor_ObjCAtTryStmt = 216,
  CXCursor_ObjCAtCatchStmt = 217,
  CXCursor_ObjCAtFinallyStmt = 218,
  CXCursor_ObjCAtThrowStmt = 219,
  CXCursor_ObjCAtSynchronizedStmt = 220,
  CXCursor_ObjCAutoreleasePoolStmt = 221,
  CXCursor_ObjCForCollectionStmt = 222,
  CXCursor_CXXCatchStmt = 223,
  CXCursor_CXXTryStmt = 224,
  CXCursor_CXXForRangeStmt = 225,
  CXCursor_SEHTryStmt = 226,
  CXCursor_SEHExceptStmt = 227,
  CXCursor_SEHFinallyStmt = 228,
  CXCursor_MSAsmStmt = 229,
  CXCursor_NullStmt = 230,
  CXCursor_DeclStmt = 231,
  CXCursor_OMPParallelDirective = 232,
  CXCursor_OMPSimdDirective = 233,
  CXCursor_OMPForDirective = 234,
  CXCursor_OMPSectionsDirective = 235,
  CXCursor_OMPSectionDirective = 236,
  CXCursor_OMPSingleDirective = 237,
  CXCursor_OMPParallelForDirective = 238,
  CXCursor_OMPParallelSectionsDirective = 239,
  CXCursor_OMPTaskDirective = 240,
  CXCursor_OMPMasterDirective = 241,
  CXCursor_OMPCriticalDirective = 242,
  CXCursor_OMPTaskyieldDirective = 243,
  CXCursor_OMPBarrierDirective = 244,
  CXCursor_OMPTaskwaitDirective = 245,
  CXCursor_OMPFlushDirective = 246,
  CXCursor_SEHLeaveStmt = 247,
  CXCursor_OMPOrderedDirective = 248,
  CXCursor_OMPAtomicDirective = 249,
  CXCursor_OMPForSimdDirective = 250,
  CXCursor_OMPParallelForSimdDirective = 251,
  CXCursor_OMPTargetDirective = 252,
  CXCursor_OMPTeamsDirective = 253,
  CXCursor_OMPTaskgroupDirective = 254,
  CXCursor_OMPCancellationPointDirective = 255,
  CXCursor_OMPCancelDirective = 256,
  CXCursor_OMPTargetDataDirective = 257,
  CXCursor_OMPTaskLoopDirective = 258,
  CXCursor_OMPTaskLoopSimdDirective = 259,
  CXCursor_OMPDistributeDirective = 260,
  CXCursor_OMPTargetEnterDataDirective = 261,
  CXCursor_OMPTargetExitDataDirective = 262,
  CXCursor_OMPTargetParallelDirective = 263,
  CXCursor_OMPTargetParallelForDirective = 264,
  CXCursor_OMPTargetUpdateDirective = 265,
  CXCursor_OMPDistributeParallelForDirective = 266,
  CXCursor_OMPDistributeParallelForSimdDirective = 267,
  CXCursor_OMPDistributeSimdDirective = 268,
  CXCursor_OMPTargetParallelForSimdDirective = 269,
  CXCursor_OMPTargetSimdDirective = 270,
  CXCursor_OMPTeamsDistributeDirective = 271,
  CXCursor_OMPTeamsDistributeSimdDirective = 272,
  CXCursor_OMPTeamsDistributeParallelForSimdDirective = 273,
  CXCursor_OMPTeamsDistributeParallelForDirective = 274,
  CXCursor_OMPTargetTeamsDirective = 275,
  CXCursor_OMPTargetTeamsDistributeDirective = 276,
  CXCursor_OMPTargetTeamsDistributeParallelForDirective = 277,
  CXCursor_OMPTargetTeamsDistributeParallelForSimdDirective = 278,
  CXCursor_OMPTargetTeamsDistributeSimdDirective = 279,
  CXCursor_LastStmt = CXCursor_OMPTargetTeamsDistributeSimdDirective,
  CXCursor_TranslationUnit = 300,
  CXCursor_FirstAttr = 400,
  CXCursor_UnexposedAttr = 400,
  CXCursor_IBActionAttr = 401,
  CXCursor_IBOutletAttr = 402,
  CXCursor_IBOutletCollectionAttr = 403,
  CXCursor_CXXFinalAttr = 404,
  CXCursor_CXXOverrideAttr = 405,
  CXCursor_AnnotateAttr = 406,
  CXCursor_AsmLabelAttr = 407,
  CXCursor_PackedAttr = 408,
  CXCursor_PureAttr = 409,
  CXCursor_ConstAttr = 410,
  CXCursor_NoDuplicateAttr = 411,
  CXCursor_CUDAConstantAttr = 412,
  CXCursor_CUDADeviceAttr = 413,
  CXCursor_CUDAGlobalAttr = 414,
  CXCursor_CUDAHostAttr = 415,
  CXCursor_CUDASharedAttr = 416,
  CXCursor_VisibilityAttr = 417,
  CXCursor_DLLExport = 418,
  CXCursor_DLLImport = 419,
  CXCursor_LastAttr = CXCursor_DLLImport,
  CXCursor_PreprocessingDirective = 500,
  CXCursor_MacroDefinition = 501,
  CXCursor_MacroExpansion = 502,
  CXCursor_MacroInstantiation = CXCursor_MacroExpansion,
  CXCursor_InclusionDirective = 503,
  CXCursor_FirstPreprocessing = CXCursor_PreprocessingDirective,
  CXCursor_LastPreprocessing = CXCursor_InclusionDirective,
  CXCursor_ModuleImportDecl = 600,
  CXCursor_TypeAliasTemplateDecl = 601,
  CXCursor_StaticAssert = 602,
  CXCursor_FriendDecl = 603,
  CXCursor_FirstExtraDecl = CXCursor_ModuleImportDecl,
  CXCursor_LastExtraDecl = CXCursor_FriendDecl,
  CXCursor_OverloadCandidate = 700
};

typedef struct {
  enum CXCursorKind kind;
  int xdata;
  const void* data[3];
} CXCursor;

using ClangGetNullCursor = CXCursor (*)(void);
using ClangGetTranslationUnitCursor = CXCursor(*)(CXTranslationUnit);
using ClangEqualCursors = unsigned (*)(CXCursor, CXCursor);
using ClangCursorIsNull = int (*)(CXCursor);
using ClangHashCursor = unsigned (*)(CXCursor);
using ClangGetCursorKind = CXCursorKind(*)(CXCursor);
using ClangIsDeclaration = unsigned (*)(enum CXCursorKind);
using ClangIsInvalidDeclaration = unsigned (*)(CXCursor);
using ClangIsReference = unsigned (*)(enum CXCursorKind);
using ClangIsExpression = unsigned (*)(enum CXCursorKind);
using ClangIsStatement = unsigned (*)(enum CXCursorKind);
using ClangIsAttribute = unsigned (*)(enum CXCursorKind);
using ClangCursorHasAttrs = unsigned (*)(CXCursor C);
using ClangIsInvalid = unsigned (*)(enum CXCursorKind);
using ClangIsTranslationUnit = unsigned (*)(enum CXCursorKind);
using ClangIsPreprocessing = unsigned (*)(enum CXCursorKind);
using ClangIsUnexposed = unsigned (*)(enum CXCursorKind);

enum CXLinkageKind {
  CXLinkage_Invalid,
  CXLinkage_NoLinkage,
  CXLinkage_Internal,
  CXLinkage_UniqueExternal,
  CXLinkage_External
};

using ClangGetCursorLinkage = enum CXLinkageKind (*)(CXCursor);

enum CXVisibilityKind {
  CXVisibility_Invalid,
  CXVisibility_Hidden,
  CXVisibility_Protected,
  CXVisibility_Default
};

using ClangGetCursorVisibility = enum CXVisibilityKind(*)(CXCursor);
using ClangGetCursorAvailability = enum CXAvailabilityKind(*)(CXCursor);

typedef struct CXPlatformAvailability {
  CXString Platform;
  CXVersion Introduced;
  CXVersion Deprecated;
  CXVersion Obsoleted;
  int Unavailable;
  CXString Message;
} CXPlatformAvailability;

using ClangGetCursorPlatformAvailability = int(*)(CXCursor, int*, CXString*, int*, CXString*, CXPlatformAvailability*, int);
using ClangDisposeCXPlatformAvailability = void(*)(CXPlatformAvailability*);

enum CXLanguageKind {
  CXLanguage_Invalid = 0,
  CXLanguage_C,
  CXLanguage_ObjC,
  CXLanguage_CPlusPlus
};

using ClangGetCursorLanguage = enum CXLanguageKind(*)(CXCursor);

enum CXTLSKind {
  CXTLS_None = 0,
  CXTLS_Dynamic,
  CXTLS_Static
};

using ClangGetCursorTLSKind = enum CXTLSKind(*)(CXCursor);
using ClangCursorGetTranslationUnit = CXTranslationUnit(*)(CXCursor);

typedef struct CXCursorSetImpl* CXCursorSet;

using ClangCreateCXCursorSet = CXCursorSet(*)(void);
using ClangDisposeCXCursorSet = void(*)(CXCursorSet);
using ClangCXCursorSetContains = unsigned(*)(CXCursorSet, CXCursor);
using ClangCXCursorSetInsert = unsigned(*)(CXCursorSet, CXCursor);
using ClangGetCursorSemanticParent = CXCursor(*)(CXCursor);
using ClangGetCursorLexicalParent = CXCursor(*)(CXCursor);
using ClangGetOverriddenCursors = void(*)(CXCursor, CXCursor**, unsigned*);
using ClangDisposeOverriddenCursors = void(*)(CXCursor*);
using ClangGetIncludedFile = CXFile(*)(CXCursor);
using ClangGetCursor = CXCursor(*)(CXTranslationUnit, CXSourceLocation);
using ClangGetCursorLocation = CXSourceLocation(*)(CXCursor);
using ClangGetCursorExtent = CXSourceRange(*)(CXCursor);

enum CXTypeKind {
  CXType_Invalid = 0,
  CXType_Unexposed = 1,
  CXType_Void = 2,
  CXType_Bool = 3,
  CXType_Char_U = 4,
  CXType_UChar = 5,
  CXType_Char16 = 6,
  CXType_Char32 = 7,
  CXType_UShort = 8,
  CXType_UInt = 9,
  CXType_ULong = 10,
  CXType_ULongLong = 11,
  CXType_UInt128 = 12,
  CXType_Char_S = 13,
  CXType_SChar = 14,
  CXType_WChar = 15,
  CXType_Short = 16,
  CXType_Int = 17,
  CXType_Long = 18,
  CXType_LongLong = 19,
  CXType_Int128 = 20,
  CXType_Float = 21,
  CXType_Double = 22,
  CXType_LongDouble = 23,
  CXType_NullPtr = 24,
  CXType_Overload = 25,
  CXType_Dependent = 26,
  CXType_ObjCId = 27,
  CXType_ObjCClass = 28,
  CXType_ObjCSel = 29,
  CXType_Float128 = 30,
  CXType_Half = 31,
  CXType_Float16 = 32,
  CXType_ShortAccum = 33,
  CXType_Accum = 34,
  CXType_LongAccum = 35,
  CXType_UShortAccum = 36,
  CXType_UAccum = 37,
  CXType_ULongAccum = 38,
  CXType_FirstBuiltin = CXType_Void,
  CXType_LastBuiltin = CXType_ULongAccum,
  CXType_Complex = 100,
  CXType_Pointer = 101,
  CXType_BlockPointer = 102,
  CXType_LValueReference = 103,
  CXType_RValueReference = 104,
  CXType_Record = 105,
  CXType_Enum = 106,
  CXType_Typedef = 107,
  CXType_ObjCInterface = 108,
  CXType_ObjCObjectPointer = 109,
  CXType_FunctionNoProto = 110,
  CXType_FunctionProto = 111,
  CXType_ConstantArray = 112,
  CXType_Vector = 113,
  CXType_IncompleteArray = 114,
  CXType_VariableArray = 115,
  CXType_DependentSizedArray = 116,
  CXType_MemberPointer = 117,
  CXType_Auto = 118,
  CXType_Elaborated = 119,
  CXType_Pipe = 120,
  CXType_OCLImage1dRO = 121,
  CXType_OCLImage1dArrayRO = 122,
  CXType_OCLImage1dBufferRO = 123,
  CXType_OCLImage2dRO = 124,
  CXType_OCLImage2dArrayRO = 125,
  CXType_OCLImage2dDepthRO = 126,
  CXType_OCLImage2dArrayDepthRO = 127,
  CXType_OCLImage2dMSAARO = 128,
  CXType_OCLImage2dArrayMSAARO = 129,
  CXType_OCLImage2dMSAADepthRO = 130,
  CXType_OCLImage2dArrayMSAADepthRO = 131,
  CXType_OCLImage3dRO = 132,
  CXType_OCLImage1dWO = 133,
  CXType_OCLImage1dArrayWO = 134,
  CXType_OCLImage1dBufferWO = 135,
  CXType_OCLImage2dWO = 136,
  CXType_OCLImage2dArrayWO = 137,
  CXType_OCLImage2dDepthWO = 138,
  CXType_OCLImage2dArrayDepthWO = 139,
  CXType_OCLImage2dMSAAWO = 140,
  CXType_OCLImage2dArrayMSAAWO = 141,
  CXType_OCLImage2dMSAADepthWO = 142,
  CXType_OCLImage2dArrayMSAADepthWO = 143,
  CXType_OCLImage3dWO = 144,
  CXType_OCLImage1dRW = 145,
  CXType_OCLImage1dArrayRW = 146,
  CXType_OCLImage1dBufferRW = 147,
  CXType_OCLImage2dRW = 148,
  CXType_OCLImage2dArrayRW = 149,
  CXType_OCLImage2dDepthRW = 150,
  CXType_OCLImage2dArrayDepthRW = 151,
  CXType_OCLImage2dMSAARW = 152,
  CXType_OCLImage2dArrayMSAARW = 153,
  CXType_OCLImage2dMSAADepthRW = 154,
  CXType_OCLImage2dArrayMSAADepthRW = 155,
  CXType_OCLImage3dRW = 156,
  CXType_OCLSampler = 157,
  CXType_OCLEvent = 158,
  CXType_OCLQueue = 159,
  CXType_OCLReserveID = 160
};

enum CXCallingConv {
  CXCallingConv_Default = 0,
  CXCallingConv_C = 1,
  CXCallingConv_X86StdCall = 2,
  CXCallingConv_X86FastCall = 3,
  CXCallingConv_X86ThisCall = 4,
  CXCallingConv_X86Pascal = 5,
  CXCallingConv_AAPCS = 6,
  CXCallingConv_AAPCS_VFP = 7,
  CXCallingConv_X86RegCall = 8,
  CXCallingConv_IntelOclBicc = 9,
  CXCallingConv_Win64 = 10,
  CXCallingConv_X86_64Win64 = CXCallingConv_Win64,
  CXCallingConv_X86_64SysV = 11,
  CXCallingConv_X86VectorCall = 12,
  CXCallingConv_Swift = 13,
  CXCallingConv_PreserveMost = 14,
  CXCallingConv_PreserveAll = 15,
  CXCallingConv_Invalid = 100,
  CXCallingConv_Unexposed = 200
};

typedef struct {
  enum CXTypeKind kind;
  void* data[2];
} CXType;

using ClangGetCursorType = CXType(*)(CXCursor);
using ClangGetTypeSpelling = CXString(*)(CXType);
using ClangGetTypedefDeclUnderlyingType = CXType(*)(CXCursor);
using ClangGetEnumDeclIntegerType = CXType(*)(CXCursor);
using ClangGetEnumConstantDeclValue = long long(*)(CXCursor);
using ClangGetEnumConstantDeclUnsignedValue = unsigned long long(*)(CXCursor);
using ClangGetFieldDeclBitWidth = int(*)(CXCursor);
using ClangCursorGetNumArguments = int(*)(CXCursor);
using ClangCursorGetArgument = CXCursor(*)(CXCursor, unsigned);

enum CXTemplateArgumentKind {
  CXTemplateArgumentKind_Null,
  CXTemplateArgumentKind_Type,
  CXTemplateArgumentKind_Declaration,
  CXTemplateArgumentKind_NullPtr,
  CXTemplateArgumentKind_Integral,
  CXTemplateArgumentKind_Template,
  CXTemplateArgumentKind_TemplateExpansion,
  CXTemplateArgumentKind_Expression,
  CXTemplateArgumentKind_Pack,
  CXTemplateArgumentKind_Invalid
};

using ClangCursorGetNumTemplateArguments = int(*)(CXCursor);
using ClangCursorGetTemplateArgumentKind = enum CXTemplateArgumentKind(*)(CXCursor, unsigned);
using ClangCursorGetTemplateArgumentType = CXType(*)(CXCursor, unsigned);
using ClangCursorGetTemplateArgumentValue = long long(*)(CXCursor, unsigned);
using ClangCursorGetTemplateArgumentUnsignedValue = unsigned long long(*)(CXCursor, unsigned);
using ClangEqualTypes = unsigned(*)(CXType, CXType);
using ClangGetCanonicalType = CXType(*)(CXType);
using ClangIsConstQualifiedType = unsigned(*)(CXType);
using ClangCursorIsMacroFunctionLike = unsigned(*)(CXCursor);
using ClangCursorIsMacroBuiltin = unsigned(*)(CXCursor);
using ClangCursorIsFunctionInlined = unsigned(*)(CXCursor);
using ClangIsVolatileQualifiedType = unsigned(*)(CXType);
using ClangIsRestrictQualifiedType = unsigned(*)(CXType);
using ClangGetAddressSpace = unsigned(*)(CXType);
using ClangGetTypedefName = CXString(*)(CXType);
using ClangGetPointeeType = CXType(*)(CXType);
using ClangGetTypeDeclaration = CXCursor(*)(CXType);
using ClangGetDeclObjCTypeEncoding = CXString(*)(CXCursor);
using ClangTypeGetObjCEncoding = CXString(*)(CXType);
using ClangGetTypeKindSpelling = CXString(*)(enum CXTypeKind);
using ClangGetFunctionTypeCallingConv = enum CXCallingConv(*)(CXType);
using ClangGetResultType = CXType(*)(CXType);
using ClangGetExceptionSpecificationType = int(*)(CXType);
using ClangGetNumArgTypes = int(*)(CXType);
using ClangGetArgType = CXType(*)(CXType, unsigned);
using ClangIsFunctionTypeVariadic = unsigned(*)(CXType);
using ClangGetCursorResultType = CXType(*)(CXCursor);
using ClangGetCursorExceptionSpecificationType = int(*)(CXCursor);
using ClangIsPODType = unsigned(*)(CXType);
using ClangGetElementType = CXType(*)(CXType);
using ClangGetNumElements = long long(*)(CXType);
using ClangGetArrayElementType = CXType(*)(CXType);
using ClangGetArraySize = long long(*)(CXType);
using ClangTypeGetNamedType = CXType(*)(CXType);
using ClangTypeIsTransparentTagTypedef = unsigned(*)(CXType);

enum CXTypeLayoutError {
  CXTypeLayoutError_Invalid = -1,
  CXTypeLayoutError_Incomplete = -2,
  CXTypeLayoutError_Dependent = -3,
  CXTypeLayoutError_NotConstantSize = -4,
  CXTypeLayoutError_InvalidFieldName = -5
};

using ClangTypeGetAlignOf = long long(*)(CXType);
using ClangTypeGetClassType = CXType(*)(CXType);
using ClangTypeGetSizeOf = long long(*)(CXType);
using ClangTypeGetOffsetOf = long long(*)(CXType, const char*);
using ClangCursorGetOffsetOfField = long long(*)(CXCursor);
using ClangCursorIsAnonymous = unsigned(*)(CXCursor);

enum CXRefQualifierKind {
  CXRefQualifier_None = 0,
  CXRefQualifier_LValue,
  CXRefQualifier_RValue
};

using ClangTypeGetNumTemplateArguments = int(*)(CXType);
using ClangTypeGetTemplateArgumentAsType = CXType(*)(CXType, unsigned);
using ClangTypeGetCXXRefQualifier = enum CXRefQualifierKind(*)(CXType);
using ClangCursorIsBitField = unsigned(*)(CXCursor);
using ClangIsVirtualBase = unsigned(*)(CXCursor);

enum CX_CXXAccessSpecifier {
  CX_CXXInvalidAccessSpecifier,
  CX_CXXPublic,
  CX_CXXProtected,
  CX_CXXPrivate
};

using ClangGetCXXAccessSpecifier = CX_CXXAccessSpecifier(*)(CXCursor);

enum CX_StorageClass {
  CX_SC_Invalid,
  CX_SC_None,
  CX_SC_Extern,
  CX_SC_Static,
  CX_SC_PrivateExtern,
  CX_SC_OpenCLWorkGroupLocal,
  CX_SC_Auto,
  CX_SC_Register
};

using ClangCursorGetStorageClass = enum CX_StorageClass(*)(CXCursor);
using ClangGetNumOverloadedDecls = unsigned(*)(CXCursor);
using ClangGetOverloadedDecl = CXCursor(*)(CXCursor, unsigned);
using ClangGetIBOutletCollectionType = CXType(*)(CXCursor);

enum CXChildVisitResult {
  CXChildVisit_Break,
  CXChildVisit_Continue,
  CXChildVisit_Recurse
};

typedef enum CXChildVisitResult(*CXCursorVisitor)(CXCursor, CXCursor, CXClientData);

using ClangVisitChildren = unsigned(*)(CXCursor, CXCursorVisitor, CXClientData);
using ClangGetCursorUSR = CXString(*)(CXCursor);
using ClangConstructUSRObjCClass = CXString(*)(const char*);
using ClangConstructUSRObjCCategory = CXString(*)(const char*, const char*);
using ClangConstructUSRObjCProtocol = CXString(*)(const char*);
using ClangConstructUSRObjCIvar = CXString(*)(const char*, CXString);
using ClangConstructUSRObjCMethod = CXString(*)(const char*, unsigned, CXString);
using ClangConstructUSRObjCProperty = CXString(*)(const char*, CXString);
using ClangGetCursorSpelling = CXString(*)(CXCursor);
using ClangCursorGetSpellingNameRange = CXSourceRange(*)(CXCursor, unsigned, unsigned);

typedef void* CXPrintingPolicy;

enum CXPrintingPolicyProperty {
  CXPrintingPolicy_Indentation,
  CXPrintingPolicy_SuppressSpecifiers,
  CXPrintingPolicy_SuppressTagKeyword,
  CXPrintingPolicy_IncludeTagDefinition,
  CXPrintingPolicy_SuppressScope,
  CXPrintingPolicy_SuppressUnwrittenScope,
  CXPrintingPolicy_SuppressInitializers,
  CXPrintingPolicy_ConstantArraySizeAsWritten,
  CXPrintingPolicy_AnonymousTagLocations,
  CXPrintingPolicy_SuppressStrongLifetime,
  CXPrintingPolicy_SuppressLifetimeQualifiers,
  CXPrintingPolicy_SuppressTemplateArgsInCXXConstructors,
  CXPrintingPolicy_Bool,
  CXPrintingPolicy_Restrict,
  CXPrintingPolicy_Alignof,
  CXPrintingPolicy_UnderscoreAlignof,
  CXPrintingPolicy_UseVoidForZeroParams,
  CXPrintingPolicy_TerseOutput,
  CXPrintingPolicy_PolishForDeclaration,
  CXPrintingPolicy_Half,
  CXPrintingPolicy_MSWChar,
  CXPrintingPolicy_IncludeNewlines,
  CXPrintingPolicy_MSVCFormatting,
  CXPrintingPolicy_ConstantsAsWritten,
  CXPrintingPolicy_SuppressImplicitBase,
  CXPrintingPolicy_FullyQualifiedName,
  CXPrintingPolicy_LastProperty = CXPrintingPolicy_FullyQualifiedName
};

using ClangPrintingPolicyGetProperty = unsigned(*)(CXPrintingPolicy, enum CXPrintingPolicyProperty);
using ClangPrintingPolicySetProperty = void(*)(CXPrintingPolicy, enum CXPrintingPolicyProperty, unsigned);
using ClangGetCursorPrintingPolicy = CXPrintingPolicy(*)(CXCursor);
using ClangPrintingPolicyDispose = void(*)(CXPrintingPolicy);
using ClangGetCursorPrettyPrinted = CXString(*)(CXCursor, CXPrintingPolicy);
using ClangGetCursorDisplayName = CXString(*)(CXCursor);
using ClangGetCursorReferenced = CXCursor(*)(CXCursor);
using ClangGetCursorDefinition = CXCursor(*)(CXCursor);
using ClangIsCursorDefinition = unsigned(*)(CXCursor);
using ClangGetCanonicalCursor = CXCursor(*)(CXCursor);
using ClangCursorGetObjCSelectorIndex = int(*)(CXCursor);
using ClangCursorIsDynamicCall = int(*)(CXCursor);
using ClangCursorGetReceiverType = CXType(*)(CXCursor);

typedef enum {
  CXObjCPropertyAttr_noattr = 0x00,
  CXObjCPropertyAttr_readonly = 0x01,
  CXObjCPropertyAttr_getter = 0x02,
  CXObjCPropertyAttr_assign = 0x04,
  CXObjCPropertyAttr_readwrite = 0x08,
  CXObjCPropertyAttr_retain = 0x10,
  CXObjCPropertyAttr_copy = 0x20,
  CXObjCPropertyAttr_nonatomic = 0x40,
  CXObjCPropertyAttr_setter = 0x80,
  CXObjCPropertyAttr_atomic = 0x100,
  CXObjCPropertyAttr_weak = 0x200,
  CXObjCPropertyAttr_strong = 0x400,
  CXObjCPropertyAttr_unsafe_unretained = 0x800,
  CXObjCPropertyAttr_class = 0x1000
} CXObjCPropertyAttrKind;

using ClangCursorGetObjCPropertyAttributes = unsigned (*)(CXCursor, unsigned);

typedef enum {
  CXObjCDeclQualifier_None = 0x0,
  CXObjCDeclQualifier_In = 0x1,
  CXObjCDeclQualifier_Inout = 0x2,
  CXObjCDeclQualifier_Out = 0x4,
  CXObjCDeclQualifier_Bycopy = 0x8,
  CXObjCDeclQualifier_Byref = 0x10,
  CXObjCDeclQualifier_Oneway = 0x20
} CXObjCDeclQualifierKind;

using ClangCursorGetObjCDeclQualifiers = unsigned(*)(CXCursor);
using ClangCursorIsObjCOptional = unsigned(*)(CXCursor);
using ClangCursorIsVariadic = unsigned(*)(CXCursor);
using ClangCursorIsExternalSymbol = unsigned(*)(CXCursor, CXString*, CXString*, unsigned*);
using ClangCursorGetCommentRange = CXSourceRange(*)(CXCursor);
using ClangCursorGetRawCommentText = CXString(*)(CXCursor);
using ClangCursorGetBriefCommentText = CXString(*)(CXCursor);
using ClangCursorGetMangling = CXString(*)(CXCursor);
using ClangCursorGetCXXManglings = CXStringSet * (*)(CXCursor);
using ClangCursorGetObjCManglings = CXStringSet * (*)(CXCursor);

typedef void* CXModule;

using ClangCursorGetModule = CXModule(*)(CXCursor);
using ClangGetModuleForFile = CXModule(*)(CXTranslationUnit, CXFile);
using ClangModuleGetASTFile = CXFile(*)(CXModule);
using ClangModuleGetParent = CXModule(*)(CXModule);
using ClangModuleGetName = CXString(*)(CXModule);
using ClangModuleGetFullName = CXString(*)(CXModule);
using ClangModuleIsSystem = int(*)(CXModule);
using ClangModuleGetNumTopLevelHeaders = unsigned(*)(CXTranslationUnit, CXModule);
using ClangModuleGetTopLevelHeader = CXFile(*)(CXTranslationUnit, CXModule, unsigned);
using ClangCXXConstructorIsConvertingConstructor = unsigned(*)(CXCursor);
using ClangCXXConstructorIsCopyConstructor = unsigned(*)(CXCursor);
using ClangCXXConstructorIsDefaultConstructor = unsigned(*)(CXCursor);
using ClangCXXConstructorIsMoveConstructor = unsigned(*)(CXCursor);
using ClangCXXFieldIsMutable = unsigned(*)(CXCursor);
using ClangCXXMethodIsDefaulted = unsigned(*)(CXCursor);
using ClangCXXMethodIsPureVirtual = unsigned(*)(CXCursor);
using ClangCXXMethodIsStatic = unsigned(*)(CXCursor);
using ClangCXXMethodIsVirtual = unsigned(*)(CXCursor);
using ClangCXXRecordIsAbstract = unsigned(*)(CXCursor);
using ClangEnumDeclIsScoped = unsigned(*)(CXCursor);
using ClangCXXMethodIsConst = unsigned(*)(CXCursor);
using ClangGetTemplateCursorKind = enum CXCursorKind(*)(CXCursor);
using ClangGetSpecializedCursorTemplate = CXCursor(*)(CXCursor);
using ClangGetCursorReferenceNameRange = CXSourceRange(*)(CXCursor, unsigned, unsigned);

enum CXNameRefFlags {
  CXNameRange_WantQualifier = 0x1,
  CXNameRange_WantTemplateArgs = 0x2,
  CXNameRange_WantSinglePiece = 0x4
};

typedef enum CXTokenKind {
  CXToken_Punctuation,
  CXToken_Keyword,
  CXToken_Identifier,
  CXToken_Literal,
  CXToken_Comment
} CXTokenKind;

typedef struct {
  unsigned int_data[4];
  void* ptr_data;
} CXToken;

using ClangGetToken = CXToken * (*)(CXTranslationUnit, CXSourceLocation);
using ClangGetTokenKind = CXTokenKind(*)(CXToken);
using ClangGetTokenSpelling = CXString(*)(CXTranslationUnit, CXToken);
using ClangGetTokenLocation = CXSourceLocation(*)(CXTranslationUnit, CXToken);
using ClangGetTokenExtent = CXSourceRange(*)(CXTranslationUnit, CXToken);
using ClangTokenize = void(*)(CXTranslationUnit, CXSourceRange, CXToken**, unsigned*);
using ClangAnnotateTokens = void(*)(CXTranslationUnit, CXToken*, unsigned, CXCursor*);
using ClangDisposeTokens = void(*)(CXTranslationUnit, CXToken*, unsigned);
using ClangGetCursorKindSpelling = CXString(*)(enum CXCursorKind);
using ClangGetDefinitionSpellingAndExtent = void(*)(CXCursor, const char**, const char**, unsigned*, unsigned*, unsigned*, unsigned*);
using ClangEnableStackTraces = void(*)(void);
using ClangExecuteOnThread = void(*)(void(*)(void*), void*, unsigned);
using ClangGetClangVersion = CXString(*)(void);
using ClangToggleCrashRecovery = void(*)(unsigned);

typedef void (*CXInclusionVisitor)(CXFile, CXSourceLocation*, unsigned, CXClientData);

using ClangGetInclusions = void(*)(CXTranslationUnit, CXInclusionVisitor, CXClientData);

typedef enum {
  CXEval_Int = 1,
  CXEval_Float = 2,
  CXEval_ObjCStrLiteral = 3,
  CXEval_StrLiteral = 4,
  CXEval_CFStr = 5,
  CXEval_Other = 6,

  CXEval_UnExposed = 0

} CXEvalResultKind;

typedef void* CXEvalResult;

using ClangCursorEvaluate = CXEvalResult(*)(CXCursor);
using ClangEvalResultGetKind = CXEvalResultKind(*)(CXEvalResult);
using ClangEvalResultGetAsInt = int(*)(CXEvalResult);
using ClangEvalResultGetAsLongLong = long long(*)(CXEvalResult);
using ClangEvalResultIsUnsignedInt = unsigned(*)(CXEvalResult);
using ClangEvalResultGetAsUnsigned = unsigned long long(*)(CXEvalResult);
using ClangEvalResultGetAsDouble = double(*)(CXEvalResult);
using ClangEvalResultGetAsStr = const char*(*)(CXEvalResult);
using ClangEvalResultDispose = void(*)(CXEvalResult);

typedef void* CXRemapping;

using ClangGetRemappings = CXRemapping(*)(const char*);
using ClangGetRemappingsFromFileList = CXRemapping(*)(const char**, unsigned);
using ClangRemapGetNumFiles = unsigned(*)(CXRemapping);
using ClangRemapGetFilenames = void(*)(CXRemapping, unsigned, CXString*, CXString*);
using ClangRemapDispose = void(*)(CXRemapping);

enum CXVisitorResult {
  CXVisit_Break,
  CXVisit_Continue
};

typedef struct CXCursorAndRangeVisitor {
  void* context;
  enum CXVisitorResult(*visit)(void* context, CXCursor, CXSourceRange);
} CXCursorAndRangeVisitor;

typedef enum {
  CXResult_Success = 0,
  CXResult_Invalid = 1,
  CXResult_VisitBreak = 2

} CXResult;

using ClangFindReferencesInFile = CXResult(*)(CXCursor, CXFile, CXCursorAndRangeVisitor);
using ClangFindIncludesInFile = CXResult(*)(CXTranslationUnit, CXFile, CXCursorAndRangeVisitor);

typedef void* CXIdxClientFile;

typedef void* CXIdxClientEntity;

typedef void* CXIdxClientContainer;

typedef void* CXIdxClientASTFile;

typedef struct {
  void* ptr_data[2];
  unsigned int_data;
} CXIdxLoc;

typedef struct {
  CXIdxLoc hashLoc;
  const char* filename;
  CXFile file;
  int isImport;
  int isAngled;
  int isModuleImport;
} CXIdxIncludedFileInfo;

typedef struct {
  CXFile file;
  CXModule module;
  CXIdxLoc loc;
  int isImplicit;
} CXIdxImportedASTFileInfo;

typedef enum {
  CXIdxEntity_Unexposed = 0,
  CXIdxEntity_Typedef = 1,
  CXIdxEntity_Function = 2,
  CXIdxEntity_Variable = 3,
  CXIdxEntity_Field = 4,
  CXIdxEntity_EnumConstant = 5,

  CXIdxEntity_ObjCClass = 6,
  CXIdxEntity_ObjCProtocol = 7,
  CXIdxEntity_ObjCCategory = 8,

  CXIdxEntity_ObjCInstanceMethod = 9,
  CXIdxEntity_ObjCClassMethod = 10,
  CXIdxEntity_ObjCProperty = 11,
  CXIdxEntity_ObjCIvar = 12,

  CXIdxEntity_Enum = 13,
  CXIdxEntity_Struct = 14,
  CXIdxEntity_Union = 15,

  CXIdxEntity_CXXClass = 16,
  CXIdxEntity_CXXNamespace = 17,
  CXIdxEntity_CXXNamespaceAlias = 18,
  CXIdxEntity_CXXStaticVariable = 19,
  CXIdxEntity_CXXStaticMethod = 20,
  CXIdxEntity_CXXInstanceMethod = 21,
  CXIdxEntity_CXXConstructor = 22,
  CXIdxEntity_CXXDestructor = 23,
  CXIdxEntity_CXXConversionFunction = 24,
  CXIdxEntity_CXXTypeAlias = 25,
  CXIdxEntity_CXXInterface = 26

} CXIdxEntityKind;

typedef enum {
  CXIdxEntityLang_None = 0,
  CXIdxEntityLang_C = 1,
  CXIdxEntityLang_ObjC = 2,
  CXIdxEntityLang_CXX = 3,
  CXIdxEntityLang_Swift = 4
} CXIdxEntityLanguage;

typedef enum {
  CXIdxEntity_NonTemplate = 0,
  CXIdxEntity_Template = 1,
  CXIdxEntity_TemplatePartialSpecialization = 2,
  CXIdxEntity_TemplateSpecialization = 3
} CXIdxEntityCXXTemplateKind;

typedef enum {
  CXIdxAttr_Unexposed = 0,
  CXIdxAttr_IBAction = 1,
  CXIdxAttr_IBOutlet = 2,
  CXIdxAttr_IBOutletCollection = 3
} CXIdxAttrKind;

typedef struct {
  CXIdxAttrKind kind;
  CXCursor cursor;
  CXIdxLoc loc;
} CXIdxAttrInfo;

typedef struct {
  CXIdxEntityKind kind;
  CXIdxEntityCXXTemplateKind templateKind;
  CXIdxEntityLanguage lang;
  const char* name;
  const char* USR;
  CXCursor cursor;
  const CXIdxAttrInfo* const* attributes;
  unsigned numAttributes;
} CXIdxEntityInfo;

typedef struct {
  CXCursor cursor;
} CXIdxContainerInfo;

typedef struct {
  const CXIdxAttrInfo* attrInfo;
  const CXIdxEntityInfo* objcClass;
  CXCursor classCursor;
  CXIdxLoc classLoc;
} CXIdxIBOutletCollectionAttrInfo;

typedef enum {
  CXIdxDeclFlag_Skipped = 0x1
} CXIdxDeclInfoFlags;

typedef struct {
  const CXIdxEntityInfo* entityInfo;
  CXCursor cursor;
  CXIdxLoc loc;
  const CXIdxContainerInfo* semanticContainer;

  const CXIdxContainerInfo* lexicalContainer;
  int isRedeclaration;
  int isDefinition;
  int isContainer;
  const CXIdxContainerInfo* declAsContainer;

  int isImplicit;
  const CXIdxAttrInfo* const* attributes;
  unsigned numAttributes;

  unsigned flags;

} CXIdxDeclInfo;

typedef enum {
  CXIdxObjCContainer_ForwardRef = 0,
  CXIdxObjCContainer_Interface = 1,
  CXIdxObjCContainer_Implementation = 2
} CXIdxObjCContainerKind;

typedef struct {
  const CXIdxDeclInfo* declInfo;
  CXIdxObjCContainerKind kind;
} CXIdxObjCContainerDeclInfo;

typedef struct {
  const CXIdxEntityInfo* base;
  CXCursor cursor;
  CXIdxLoc loc;
} CXIdxBaseClassInfo;

typedef struct {
  const CXIdxEntityInfo* protocol;
  CXCursor cursor;
  CXIdxLoc loc;
} CXIdxObjCProtocolRefInfo;

typedef struct {
  const CXIdxObjCProtocolRefInfo* const* protocols;
  unsigned numProtocols;
} CXIdxObjCProtocolRefListInfo;

typedef struct {
  const CXIdxObjCContainerDeclInfo* containerInfo;
  const CXIdxBaseClassInfo* superInfo;
  const CXIdxObjCProtocolRefListInfo* protocols;
} CXIdxObjCInterfaceDeclInfo;

typedef struct {
  const CXIdxObjCContainerDeclInfo* containerInfo;
  const CXIdxEntityInfo* objcClass;
  CXCursor classCursor;
  CXIdxLoc classLoc;
  const CXIdxObjCProtocolRefListInfo* protocols;
} CXIdxObjCCategoryDeclInfo;

typedef struct {
  const CXIdxDeclInfo* declInfo;
  const CXIdxEntityInfo* getter;
  const CXIdxEntityInfo* setter;
} CXIdxObjCPropertyDeclInfo;

typedef struct {
  const CXIdxDeclInfo* declInfo;
  const CXIdxBaseClassInfo* const* bases;
  unsigned numBases;
} CXIdxCXXClassDeclInfo;

typedef enum {
  CXIdxEntityRef_Direct = 1,
  CXIdxEntityRef_Implicit = 2
} CXIdxEntityRefKind;

typedef enum {
  CXSymbolRole_None = 0,
  CXSymbolRole_Declaration = 1 << 0,
  CXSymbolRole_Definition = 1 << 1,
  CXSymbolRole_Reference = 1 << 2,
  CXSymbolRole_Read = 1 << 3,
  CXSymbolRole_Write = 1 << 4,
  CXSymbolRole_Call = 1 << 5,
  CXSymbolRole_Dynamic = 1 << 6,
  CXSymbolRole_AddressOf = 1 << 7,
  CXSymbolRole_Implicit = 1 << 8
} CXSymbolRole;

typedef struct {
  CXIdxEntityRefKind kind;

  CXCursor cursor;
  CXIdxLoc loc;

  const CXIdxEntityInfo* referencedEntity;

  const CXIdxEntityInfo* parentEntity;

  const CXIdxContainerInfo* container;

  CXSymbolRole role;
} CXIdxEntityRefInfo;

typedef struct {

  int (*abortQuery)(CXClientData client_data, void* reserved);

  void (*diagnostic)(CXClientData client_data,
    CXDiagnosticSet, void* reserved);

  CXIdxClientFile(*enteredMainFile)(CXClientData client_data,
    CXFile mainFile, void* reserved);

  CXIdxClientFile(*ppIncludedFile)(CXClientData client_data,
    const CXIdxIncludedFileInfo*);

  CXIdxClientASTFile(*importedASTFile)(CXClientData client_data,
    const CXIdxImportedASTFileInfo*);

  CXIdxClientContainer(*startedTranslationUnit)(CXClientData client_data,
    void* reserved);

  void (*indexDeclaration)(CXClientData client_data,
    const CXIdxDeclInfo*);

  void (*indexEntityReference)(CXClientData client_data,
    const CXIdxEntityRefInfo*);

} IndexerCallbacks;

using ClangIndexIsEntityObjCContainerKind = int(*)(CXIdxEntityKind);
using ClangIndexGetObjCContainerDeclInfo = const CXIdxObjCContainerDeclInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetObjCInterfaceDeclInfo = const CXIdxObjCInterfaceDeclInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetObjCCategoryDeclInfo = const CXIdxObjCCategoryDeclInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetObjCProtocolRefListInfo = const CXIdxObjCProtocolRefListInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetObjCPropertyDeclInfo = const CXIdxObjCPropertyDeclInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetIBOutletCollectionAttrInfo = const CXIdxIBOutletCollectionAttrInfo*(*)(const CXIdxAttrInfo*);
using ClangIndexGetCXXClassDeclInfo = const CXIdxCXXClassDeclInfo*(*)(const CXIdxDeclInfo*);
using ClangIndexGetClientContainer = CXIdxClientContainer(*)(const CXIdxContainerInfo*);
using ClangIndexSetClientContainer = void(*)(const CXIdxContainerInfo*, CXIdxClientContainer);
using ClangIndexGetClientEntity = CXIdxClientEntity(*)(const CXIdxEntityInfo*);
using ClangIndexSetClientEntity = void(*)(const CXIdxEntityInfo*, CXIdxClientEntity);

typedef void* CXIndexAction;

using ClangIndexActionCreate = CXIndexAction(*)(CXIndex);
using ClangIndexActionDispose = void(*)(CXIndexAction);

typedef enum {
  CXIndexOpt_None = 0x0,
  CXIndexOpt_SuppressRedundantRefs = 0x1,
  CXIndexOpt_IndexFunctionLocalSymbols = 0x2,
  CXIndexOpt_IndexImplicitTemplateInstantiations = 0x4,
  CXIndexOpt_SuppressWarnings = 0x8,
  CXIndexOpt_SkipParsedBodiesInSession = 0x10
} CXIndexOptFlags;

using ClangIndexSourceFile = int(*)(CXIndexAction, CXClientData, IndexerCallbacks*, unsigned, unsigned, const char*, const char* const*, int, struct CXUnsavedFile*, unsigned, CXTranslationUnit*, unsigned);
using ClangIndexSourceFileFullArgv = int(*)(CXIndexAction, CXClientData, IndexerCallbacks*, unsigned, unsigned, const char*, const char* const*, int, struct CXUnsavedFile*, unsigned, CXTranslationUnit*, unsigned);
using ClangIndexTranslationUnit = int(*)(CXIndexAction, CXClientData, IndexerCallbacks*, unsigned, unsigned, CXTranslationUnit);
using ClangIndexLocGetFileLocation = void(*)(CXIdxLoc, CXIdxClientFile*, CXFile*, unsigned*, unsigned*, unsigned*);
using ClangIndexLocGetCXSourceLocation = CXSourceLocation(*)(CXIdxLoc);

typedef enum CXVisitorResult(*CXFieldVisitor)(CXCursor, CXClientData);

using ClangTypeVisitFields = unsigned(*)(CXType, CXFieldVisitor, CXClientData);

#endif // LIBCLANGUTILS_CLANG_CINDEX_H
