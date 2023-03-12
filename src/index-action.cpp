// Copyright (C) 2023 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/index-action.h"

#include "libclang-utils/libclang.h"

#include <iostream>

namespace libclang
{

IndexAction::IndexAction(IndexAction&& other) noexcept :
  api(other.api),
  index_action(other.index_action)
{
  other.index_action = nullptr;
}

IndexAction::~IndexAction()
{
  api.clang_IndexAction_dispose(*this);
}

IndexAction::IndexAction(Index& index) :
  IndexAction(index.api, index.api.clang_IndexAction_create(index.index))
{

}

IndexAction::IndexAction(LibClang& backend, CXIndexAction act) :
  api(backend),
  index_action(act)
{

}

IndexAction::operator CXIndexAction() const
{
  return index_action;
}

BasicIndexer::BasicIndexer(LibClang& api) : IndexerBase(api)
{

}

int BasicIndexer::abortQuery()
{
  return 0;
}

void BasicIndexer::diagnostic(CXDiagnosticSet)
{

}

CXIdxClientFile BasicIndexer::enteredMainFile(const File& mainFile)
{
  std::cout << "Entered main file: " << mainFile.getFileName() << std::endl;
  return nullptr;
}

CXIdxClientFile BasicIndexer::ppIncludedFile(const CXIdxIncludedFileInfo* info)
{
  if (info->isAngled)
    std::cout << "#include <" << info->filename << ">";
  else
    std::cout << "#include \"" << info->filename << "\"";

  std::cout << " --> " << libclangAPI().file(info->file).getFileName() << std::endl;

  return nullptr;
}

CXIdxClientASTFile BasicIndexer::importedASTFile(const CXIdxImportedASTFileInfo*)
{
  return nullptr;
}

CXIdxClientContainer BasicIndexer::startedTranslationUnit()
{
  std::cout << "Started translation unit." << std::endl;
  return nullptr;
}

const char* getName(const CXIdxEntityInfo* entityInfo)
{
  return entityInfo->name ? entityInfo->name : "";
}

void BasicIndexer::indexDeclaration(const CXIdxDeclInfo* info)
{
  std::cout << "  declaration " << getName(info->entityInfo) << " (" << info->entityInfo->USR << ") ";

  CXFile cxfile;
  unsigned line, col;

  libclangAPI().clang_indexLoc_getFileLocation(info->loc, nullptr, &cxfile, &line, &col, nullptr);

  std::cout << "@" << libclangAPI().file(cxfile).getFileName() << ":" << line << ":" << col;

  std::cout << std::endl;
}

void BasicIndexer::indexEntityReference(const CXIdxEntityRefInfo* info)
{
  std::cout << "    reference " << getName(info->referencedEntity) << " (" << info->referencedEntity->USR << ") ";

  CXFile cxfile;
  unsigned line, col;

  libclangAPI().clang_indexLoc_getFileLocation(info->loc, nullptr, &cxfile, &line, &col, nullptr);

  std::cout << "@" << libclangAPI().file(cxfile).getFileName() << ":" << line << ":" << col;

  std::cout << std::endl;
}

} // namespace libclang
