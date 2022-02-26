//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//===----------------------------------------------------------------------===//
// Copyright (c) 2022 Cory Fields

// Warn about any function that calls into another function that returns
// an early_exit_t. The intention is to force _any_ returned early_exit_t to
// bubble all the way back up to main. Like manual exception catch/rethrowing.

template <typename T = int>
struct early_exit_t{};

early_exit_t<int> maybe_early_exit()
{
    return {};
}

early_exit_t<> caller()
{
    auto foo = maybe_early_exit();
    auto bar = maybe_early_exit();
    return foo;
}

void caller2() // should warn for not returning early_exit_t.
{
    auto foo = maybe_early_exit();
    auto bar = maybe_early_exit();
    return;
}
