// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBCLANGUTILS_INDEXACTION_H
#define LIBCLANGUTILS_INDEXACTION_H

#include "libclang-utils/clang-file.h"
#include "libclang-utils/clang-index.h"
#include "libclang-utils/clang-translation-unit.h"

namespace libclang
{

class TranslationUnit;

/*!
 * \class IndexAction
 */
class LIBCLANGU_API IndexAction
{
public:
  LibClang& api;
  CXIndexAction index_action;

public:
  IndexAction() = delete;
  IndexAction(const IndexAction&) = delete;
  IndexAction(IndexAction&& other) noexcept;
  ~IndexAction();

  explicit IndexAction(Index& index);
  IndexAction(LibClang& backend, CXIndexAction act);

  template<typename IndexerType>
  void indexTranslationUnit(const TranslationUnit& tu, IndexerType& indexer, int options = CXIndexOpt_None);

  operator CXIndexAction() const;
};

class LIBCLANGU_API IndexerBase
{
protected:
  LibClang& api;

public:
  explicit IndexerBase(LibClang& api_) :
    api(api_)
  {

  }

  LibClang& libclangAPI() const
  {
    return api;
  }

  CXIdxClientEntity getClientData(const CXIdxEntityInfo* info) const
  {
    return libclangAPI().clang_index_getClientEntity(info);
  }

  void setClientData(const CXIdxEntityInfo* entity, CXIdxClientEntity data)
  {
    libclangAPI().clang_index_setClientEntity(entity, data);
  }

  void* getClientData(const CXIdxContainerInfo* info) const
  {
    return libclangAPI().clang_index_getClientContainer(info);
  }

  void setClientData(const CXIdxContainerInfo* info, void* data)
  {
    libclangAPI().clang_index_setClientContainer(info, data);
  }

  struct FileLocation
  {
    File file;
    CXIdxClientFile client_data = nullptr;
    int line = 0;
    int column = 0;
    int offset = 0;;

    explicit FileLocation(const File& f) : file(f) { }
  };

  FileLocation getFileLocation(CXIdxLoc	loc) const
  {
    CXIdxClientFile client_data;
    CXFile file;
    unsigned line, col, offset;
    libclangAPI().clang_indexLoc_getFileLocation(loc, &client_data, &file, &line, &col, &offset);

    FileLocation r{ libclangAPI().file(file) };
    r.client_data = client_data;
    r.line = line;
    r.column = col;
    r.offset = offset;
    return r;
  }

  /*
   * The API expected by libclang
   */
  int abortQuery();
  void diagnostic(CXDiagnosticSet);
  CXIdxClientFile enteredMainFile(const File& mainFile);
  CXIdxClientFile ppIncludedFile(const CXIdxIncludedFileInfo*);
  CXIdxClientASTFile importedASTFile(const CXIdxImportedASTFileInfo*);
  CXIdxClientContainer startedTranslationUnit();
  void indexDeclaration(const CXIdxDeclInfo*);
  void indexEntityReference(const CXIdxEntityRefInfo*);
};

class LIBCLANGU_API BasicIndexer : public IndexerBase
{
public:
  BasicIndexer() = delete;
  BasicIndexer(const BasicIndexer&) = delete;
  ~BasicIndexer() = default;

  explicit BasicIndexer(LibClang& api);

  int abortQuery();
  void diagnostic(CXDiagnosticSet);
  CXIdxClientFile enteredMainFile(const File& mainFile);
  CXIdxClientFile ppIncludedFile(const CXIdxIncludedFileInfo*);
  CXIdxClientASTFile importedASTFile(const CXIdxImportedASTFileInfo*);
  CXIdxClientContainer startedTranslationUnit();
  void indexDeclaration(const CXIdxDeclInfo*);
  void indexEntityReference(const CXIdxEntityRefInfo*);

  BasicIndexer& operator=(const BasicIndexer&) = delete;
};

namespace details
{


template<typename IndexerType>
struct IndexerCallbacks
{
  static int abortQuery(CXClientData client_data, void* reserved)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->abortQuery();
  }

  static void diagnostic(CXClientData client_data, CXDiagnosticSet diagnostics, void* reserved)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->diagnostic(diagnostics);
  }

  static CXIdxClientFile enteredMainFile(CXClientData client_data, CXFile mainFile, void* reserved)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->enteredMainFile(indexer->libclangAPI().file(mainFile));
  }

  static CXIdxClientFile ppIncludedFile(CXClientData client_data, const CXIdxIncludedFileInfo* info)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->ppIncludedFile(info);
  }

  static CXIdxClientASTFile importedASTFile(CXClientData client_data, const CXIdxImportedASTFileInfo* info)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->importedASTFile(info);
  }

  static CXIdxClientContainer startedTranslationUnit(CXClientData client_data, void* reserved)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->startedTranslationUnit();

  }

  static void indexDeclaration(CXClientData client_data, const CXIdxDeclInfo* info)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->indexDeclaration(info);
  }

  static void indexEntityReference(CXClientData client_data, const CXIdxEntityRefInfo* info)
  {
    auto* indexer = reinterpret_cast<IndexerType*>(client_data);
    return indexer->indexEntityReference(info);
  }
};

} // namespace details

template<typename IndexerType>
inline void IndexAction::indexTranslationUnit(const TranslationUnit& tu, IndexerType& indexer, int options)
{
  IndexerCallbacks callbacks;
  callbacks.abortQuery = &details::IndexerCallbacks<IndexerType>::abortQuery;
  callbacks.diagnostic = &details::IndexerCallbacks<IndexerType>::diagnostic;
  callbacks.enteredMainFile = &details::IndexerCallbacks<IndexerType>::enteredMainFile;
  callbacks.ppIncludedFile = &details::IndexerCallbacks<IndexerType>::ppIncludedFile;
  callbacks.importedASTFile = &details::IndexerCallbacks<IndexerType>::importedASTFile;
  callbacks.startedTranslationUnit = &details::IndexerCallbacks<IndexerType>::startedTranslationUnit;
  callbacks.indexDeclaration = &details::IndexerCallbacks<IndexerType>::indexDeclaration;
  callbacks.indexEntityReference = &details::IndexerCallbacks<IndexerType>::indexEntityReference;

  api.clang_indexTranslationUnit(*this, &indexer, &callbacks, sizeof(IndexerCallbacks), options, tu);
}

} // namespace libclang

#endif // LIBCLANGUTILS_INDEXACTION_H
