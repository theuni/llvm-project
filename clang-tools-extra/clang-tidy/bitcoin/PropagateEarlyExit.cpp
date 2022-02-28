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
    Finder->addMatcher(
     functionDecl(
      hasDescendant(
       callExpr(
        callee(
         functionDecl(
          returns(
           qualType(
            hasDeclaration(
             classTemplateSpecializationDecl(
              hasName("early_exit_t")))).bind("return_type")))))),
     unless(
      returns(
       qualType(
        hasDeclaration(
         classTemplateSpecializationDecl(
          hasName("early_exit_t"))))))
     ).bind("has_early_exit"), this);
}

void PropagateEarlyExit::check(const MatchFinder::MatchResult &Result) {
    const auto *decl = Result.Nodes.getNodeAs<FunctionDecl>("has_early_exit");
    SourceRange return_range = decl->getReturnTypeSourceRange();
    const auto* canon_decl = decl->getCanonicalDecl();

    if (return_range.isInvalid()) {
        // Happens (at least) with trailing return types
        // TODO: detect/work around that
        diag(decl->getLocation(), "unknown return type");
        return;
    }

    //TODO: There's probably some real void type to check against rather than string matching
    auto retstring = decl->getReturnType().getAsString();
    if (retstring == "void") {
        retstring = "";
    }

    auto loc = decl->getBeginLoc();
    if (loc.isMacroID()) {
        return;
    }

    auto user_diag = diag(loc, "%0 should return early_exit_t.") << decl;
    user_diag << FixItHint::CreateReplacement(return_range, (llvm::Twine("early_exit_t<") + retstring + ">").str());

    //TODO: This is very naive. Need to replace all occurances, not just canonical.
    if (canon_decl != decl) {
        SourceRange canon_return_range = canon_decl->getReturnTypeSourceRange();
        user_diag << FixItHint::CreateReplacement(canon_return_range, (llvm::Twine("early_exit_t<") + retstring + ">").str());
    }
}

} // namespace bitcoin
} // namespace tidy
} // namespace clang
