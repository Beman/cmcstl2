// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_ALGORITHM_FIND_IF_NOT_HPP
#define STL2_DETAIL_ALGORITHM_FIND_IF_NOT_HPP

#include <initializer_list>
#include <stl2/functional.hpp>
#include <stl2/iterator.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/algorithm/find_if.hpp>
#include <stl2/detail/concepts/callable.hpp>

///////////////////////////////////////////////////////////////////////////
// find_if_not [alg.find]
//
STL2_OPEN_NAMESPACE {
  template <class I, class S, class Pred, class Proj = identity>
  requires
    models::InputIterator<__f<I>> &&
    models::Sentinel<__f<S>, __f<I>> &&
    models::IndirectCallablePredicate<
      __f<Pred>, projected<__f<I>, __f<Proj>>>
  __f<I> find_if_not(I&& first, S&& last, Pred&& pred, Proj&& proj = Proj{})
  {
    return __stl2::find_if(__stl2::forward<I>(first), __stl2::forward<S>(last),
                           __stl2::not_fn(__stl2::forward<Pred>(pred)),
                           __stl2::forward<Proj>(proj));
  }

  template <InputRange Rng, class Pred, class Proj = identity>
  requires
    models::IndirectCallablePredicate<
      __f<Pred>, projected<iterator_t<Rng>, __f<Proj>>>
  safe_iterator_t<Rng>
  find_if_not(Rng&& rng, Pred&& pred, Proj&& proj = Proj{})
  {
    return __stl2::find_if(__stl2::begin(rng), __stl2::end(rng),
                           __stl2::not_fn(__stl2::forward<Pred>(pred)),
                           __stl2::forward<Proj>(proj));
  }

  // Extension
  template <class E, class Pred, class Proj = identity>
  requires
    models::IndirectCallablePredicate<
      __f<Pred>, projected<const E*, __f<Proj>>>
  dangling<const E*>
  find_if_not(std::initializer_list<E>&& il, Pred&& pred, Proj&& proj = Proj{})
  {
    return __stl2::find_if(il.begin(), il.end(),
                           __stl2::not_fn(__stl2::forward<Pred>(pred)),
                           __stl2::forward<Proj>(proj));
  }
} STL2_CLOSE_NAMESPACE

#endif
