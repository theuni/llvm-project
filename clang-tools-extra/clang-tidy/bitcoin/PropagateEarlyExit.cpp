//===--- PropagateEarlyExit.cpp - clang-tidy-------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// Copyright (c) 2022 Cory Fields

#include "PropagateEarlyExit.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace bitcoin {

void PropagateEarlyExit::registerMatchers(MatchFinder *Finder) {
}

void PropagateEarlyExit::check(const MatchFinder::MatchResult &Result) {
}

} // namespace bitcoin
} // namespace tidy
} // namespace clang
