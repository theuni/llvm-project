set(LLVM_LINK_COMPONENTS
  support
  FrontendOpenMP
  )

add_clang_library(clangTidyBitcoinModule
  BitcoinTidyModule.cpp
  PropagateEarlyExit.cpp

  LINK_LIBS
  clangTidy
  clangTidyUtils

  DEPENDS
  omp_gen
  )

clang_target_link_libraries(clangTidyBitcoinModule
  PRIVATE
  clangAST
  clangASTMatchers
  clangBasic
  clangLex
  )
