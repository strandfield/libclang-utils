// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "libclang-utils/clang-index.h"

#include "libclang-utils/clang-translation-unit.h"

#include <vector>

/*!
 * \namespace libclang
 */

namespace libclang
{

/*!
 * \class Index
 */

/*!
 * \fn TranslationUnit parseTranslationUnit(const std::string& file, const std::set<std::string>& includedirs, int options = 0)
 */
TranslationUnit Index::parseTranslationUnit(const std::string& file, const std::set<std::string>& includedirs, int options)
{
  const char* command_line_args[128] = { nullptr };
  std::vector<std::string> argv{ "-x", "c++", "-Xclang", "-ast-dump", "-fsyntax-only" };
  for (const std::string& f : includedirs)
  {
    argv.push_back("--include-directory");
    argv.push_back(f);
  }
  for (int i(0); i < argv.size(); ++i)
    command_line_args[i] = argv.at(i).data();

  CXTranslationUnit tu = nullptr;

  CXErrorCode error = api.clang_parseTranslationUnit2(this->index, file.data(), command_line_args, static_cast<int>(argv.size()), nullptr, 0, options, &tu);

  if (error)
    throw std::runtime_error{ "Could not parse translation unit" };

  return TranslationUnit{ api, tu };
}

/*!
 * \endclass
 */

 /*!
  * \endnamespace
  */

} // namespace libclang
