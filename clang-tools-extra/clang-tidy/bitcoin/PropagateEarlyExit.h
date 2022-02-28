//===--- PropagateEarlyExit.h - clang-tidy-----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// Copyright (c) 2022 Cory Fields

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BITCOIN_PROPAGATE_EARLY_EXIT_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BITCOIN_PROPAGATE_EARLY_EXIT_H

#include "../ClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace bitcoin {

class PropagateEarlyExit : public ClangTidyCheck {
public:
  PropagateEarlyExit(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  bool isLanguageVersionSupported(const LangOptions &LangOpts) const override {
    return LangOpts.CPlusPlus;
  }
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // namespace bitcoin
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BITCOIN_PROPAGATE_EARLY_EXIT_H
