// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <stl2/detail/algorithm/nth_element.hpp>
#include <cassert>
#include <memory>
#include <random>
#include <algorithm>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

namespace stl2 = __stl2;

namespace { std::mt19937 gen; }

void
test_one(unsigned N, unsigned M)
{
    assert(N != 0);
    assert(M < N);
    std::unique_ptr<int[]> array{new int[N]};
    for (int i = 0; (unsigned)i < N; ++i)
        array[i] = i;
    std::shuffle(array.get(), array.get()+N, gen);
    CHECK(stl2::nth_element(array.get(), array.get()+M, array.get()+N) == array.get()+N);
    CHECK((unsigned)array[M] == M);
    std::shuffle(array.get(), array.get()+N, gen);
    CHECK(stl2::nth_element(::as_lvalue(stl2::ext::make_range(array.get(), array.get()+N)), array.get()+M) == array.get()+N);
    CHECK((unsigned)array[M] == M);
    std::shuffle(array.get(), array.get()+N, gen);
    CHECK(stl2::nth_element(stl2::ext::make_range(array.get(), array.get()+N), array.get()+M).get_unsafe() == array.get()+N);
    CHECK((unsigned)array[M] == M);
    stl2::nth_element(array.get(), array.get()+N, array.get()+N); // begin, end, end
}

void
test(unsigned N)
{
    test_one(N, 0);
    test_one(N, 1);
    test_one(N, 2);
    test_one(N, 3);
    test_one(N, N/2-1);
    test_one(N, N/2);
    test_one(N, N/2+1);
    test_one(N, N-3);
    test_one(N, N-2);
    test_one(N, N-1);
}

struct S
{
    int i,j;
};

int main()
{
    int d = 0;
    stl2::nth_element(&d, &d, &d);
    CHECK(d == 0);
    test(256);
    test(257);
    test(499);
    test(500);
    test(997);
    test(1000);
    test(1009);

    // Works with projections?
    const int N = 257;
    const int M = 56;
    S ia[N];
    for(int i = 0; i < N; ++i)
        ia[i].i = ia[i].j = i;
    std::shuffle(ia, ia+N, gen);
    stl2::nth_element(ia, ia+M, std::less<int>(), &S::i);
    CHECK(ia[M].i == M);
    CHECK(ia[M].j == M);

    return test_result();
}
