//===------- BitcoinTidyModule.cpp - clang-tidy ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// Copyright (c) 2022 Cory Fields

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "PropagateEarlyExit.h"
using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bitcoin {

class BitcoinModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<PropagateEarlyExit>("bitcoin-propagate-early-exit");
  }
};

// Register the BitcoinModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<BitcoinModule> X("bitcoin-module",
                                                   "Add bitcoin checks.");

} // namespace bitcoin

// This anchor is used to force the linker to link in the generated object file
// and thus register the BitcoinModule.
volatile int BitcoinModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
