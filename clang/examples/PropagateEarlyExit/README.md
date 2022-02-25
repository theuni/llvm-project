This is intended to be a plugin that enforces the following clang AST query:
```
functionDecl(
  hasDescendant(
    callExpr(
      callee(
        functionDecl(
          returns(
            hasDeclaration(
              classTemplateSpecializationDecl(
                hasName("early_exit_t")))))))),
  unless(
    returns(
      hasDeclaration(
        classTemplateSpecializationDecl(
          hasName("early_exit_t")))))
)
```

Example usage:

`$ bin/clang++ -fsyntax-only -fplugin=lib/PropagateEarlyExit.so example.cpp`

```
example.cpp:19:6: warning: Function 'caller2' calls into a function that returns early_exit_t but does not itself return early_exit_t
void caller2() // should warn for not returning early_exit_t.
     ^
example.cpp:21:33: note: early_exit_t returned here
    auto foo = maybe_early_exit();
                                ^
1 warning generated.
```
