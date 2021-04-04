// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'libclang-utils' project
// For conditions of distribution and use, see copyright notice in LICENSE

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "libclang-utils/libclang.h"
#include "libclang-utils/clang-index.h"
#include "libclang-utils/clang-translation-unit.h"

#include <iostream>
#include <fstream>

static void write_file(const char* filename, const char* content)
{
  std::ofstream file{ filename };
  file << content << std::endl;
}

static bool findLibclangParser()
{
  try
  {
    libclang::LibClang clang_parser;
    std::cout << "libclang parser available, version = " << clang_parser.printableVersion() << std::endl;
    return true;
  }
  catch (const libclang::LibClangError& err)
  {
    std::cout << "libclang error: " << err.what() << std::endl;
    std::cout << "libclang parser tests will be skipped..." << std::endl;
  }

  return false;
}

static bool skipTest()
{
  static bool value = findLibclangParser();
  return !value;
}

TEST_CASE("The parser is able to parse a function", "[libclang]")
{
  if (skipTest())
    return;

  write_file("test.cpp",
    "const int foo(int& i, int j = 0) { return i+j; }");

  libclang::LibClang libclang;
  libclang::Index index = libclang.createIndex();

  REQUIRE_NOTHROW(index.parseTranslationUnit("test.cpp", {}));
}

TEST_CASE("The parser is able to parse a simple function body", "[libclang]")
{
  if (skipTest())
    return;

  write_file("test.cpp",
    "void foo(int n) noexcept {  if(n > 0){} }");

  libclang::LibClang libclang;
  libclang::Index index = libclang.createIndex();

  libclang::TranslationUnit tu = index.parseTranslationUnit("test.cpp", {});
}
