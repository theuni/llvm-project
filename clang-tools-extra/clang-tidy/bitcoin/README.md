Detect and fix functions which don't propagate `early_exit_t`.

Example Usage:
`$ ./bin/clang-tidy -checks='-*,bitcoin-*' -fix example.cc --`

```
1 warning generated.
/home/cory/dev/llvm-project/build-tools-extra/example.cc:30:1: warning: 'caller2' should return early_exit_t. [bitcoin-propagate-early-exit]
void caller2() // should warn for not returning early_exit_t.
^~~~
early_exit_t<>
/home/cory/dev/llvm-project/build-tools-extra/example.cc:30:1: note: FIX-IT applied suggested code changes
/home/cory/dev/llvm-project/build-tools-extra/example.h:4:1: note: FIX-IT applied suggested code changes
void caller2();
^
clang-tidy applied 2 of 2 suggested fixes.
```
