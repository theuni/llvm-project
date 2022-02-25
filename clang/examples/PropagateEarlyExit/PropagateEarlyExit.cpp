//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//===----------------------------------------------------------------------===//
// Copyright (c) 2022 Cory Fields

#include "clang/AST/ASTContext.h"
#include "clang/AST/Attr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Sema/Sema.h"
#include "clang/Sema/SemaDiagnostic.h"
#include "llvm/ADT/SmallPtrSet.h"
using namespace clang;

namespace {

static std::string g_match_type = "early_exit_t";

class CallExprVisitor : public RecursiveASTVisitor<CallExprVisitor> {
public:
  bool m_found = false;
  CallExpr* m_decl = nullptr;
  bool VisitCallExpr(CallExpr *CallExpr) {
    if (auto* CalleeDecl = CallExpr->getDirectCallee()) {
        QualType rettype = CalleeDecl->getReturnType();
        auto retstring = rettype.getAsString();
        if (retstring.find(g_match_type) == 0) {
            m_found = true;
            m_decl = CallExpr;
            return false;
        }
    }
    return true;
  }

};

class FunctionDeclVisitor : public RecursiveASTVisitor<FunctionDeclVisitor> {
public:
  FunctionDeclVisitor(DiagnosticsEngine &Diags) : Diags(Diags) {
    WarningUnpropagatedEarlyExit = Diags.getCustomDiagID(
        DiagnosticsEngine::Warning,
        "Function %q0 calls into a function that returns early_exit_t but "
        "does not itself return early_exit_t");
    NoteCallLocation = Diags.getCustomDiagID(
        DiagnosticsEngine::Note, "early_exit_t returned here");
  }
  bool VisitFunctionDecl(FunctionDecl *MethodDecl) {
    if (MethodDecl->isThisDeclarationADefinition() && MethodDecl->hasBody()) {
      auto rettype = MethodDecl->getReturnType();
      auto retstring = rettype.getAsString();
      if (retstring.find(g_match_type) != 0) {
        CallExprVisitor Visitor;
        Visitor.TraverseDecl(MethodDecl);
        if (Visitor.m_found) {
            Diags.Report(MethodDecl->getLocation(), WarningUnpropagatedEarlyExit) << MethodDecl;
            Diags.Report(Visitor.m_decl->getRParenLoc(), NoteCallLocation);
        }
      }
    }
    return true;
  }

private:
  DiagnosticsEngine &Diags;
  unsigned WarningUnpropagatedEarlyExit;
  unsigned NoteCallLocation;
};

class EarlyExitConsumer : public ASTConsumer {
public:
  void HandleTranslationUnit(ASTContext &Context) override {
    FunctionDeclVisitor Visitor(Context.getDiagnostics());
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};

class EarlyExitAction : public PluginASTAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<EarlyExitConsumer>();
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }

  PluginASTAction::ActionType getActionType() override {
    return AddBeforeMainAction;
  }
};

} // namespace

static FrontendPluginRegistry::Add<EarlyExitAction>
    X("early_exit_plugin", "clang plugin, checks every function for correct "
                           "propagation of early_exit_t.");
