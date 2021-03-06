// cmcstl2 - A concept-enabled C++ standard library
//
//  Copyright Casey Carter 2015
//  Copyright Eric Niebler 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/caseycarter/cmcstl2
//
#ifndef STL2_DETAIL_FUNCTIONAL_COMPARISONS_HPP
#define STL2_DETAIL_FUNCTIONAL_COMPARISONS_HPP

#include <functional>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/meta.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>

STL2_OPEN_NAMESPACE {
  ///////////////////////////////////////////////////////////////////////////
  // equal_to [comparisons]
  //
  template <class = void> struct equal_to;

  EqualityComparable{T}
  struct equal_to<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a == b;
    }
  };

  template <>
  struct equal_to<void> {
    EqualityComparable{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return t == u;
    }

    using is_transparent = true_type;
  };

  ///////////////////////////////////////////////////////////////////////////
  // not_equal_to
  //
  template <class = void> struct not_equal_to;

  EqualityComparable{T}
  struct not_equal_to<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a != b;
    }
  };

  template <>
  struct not_equal_to<void> {
    EqualityComparable{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return t != u;
    }

    using is_transparent = true_type;
  };

  ///////////////////////////////////////////////////////////////////////////
  // greater
  //
  template <class = void> struct greater;

  StrictTotallyOrdered{T}
  struct greater<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a > b;
    }
  };

  template <>
  struct greater<void> : private std::greater<void> {
    StrictTotallyOrdered{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return std::greater<void>::operator()(t, u);
    }

    using is_transparent = true_type;
  };

  template <class T>
  struct greater<T*> : private std::greater<T*> {
    using std::greater<T*>::operator();
  };

  ///////////////////////////////////////////////////////////////////////////
  // less
  //
  template <class = void> struct less;

  StrictTotallyOrdered{T}
  struct less<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a < b;
    }
  };

  template <>
  struct less<void> : private std::less<void> {
    StrictTotallyOrdered{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return std::less<void>::operator()(t, u);
    }

    using is_transparent = true_type;
  };

  template <class T>
  struct less<T*> : private std::less<T*> {
    using std::less<T*>::operator();
  };

  ///////////////////////////////////////////////////////////////////////////
  // greater_equal
  //
  template <class = void> struct greater_equal;

  StrictTotallyOrdered{T}
  struct greater_equal<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a >= b;
    }
  };

  template <>
  struct greater_equal<void> : private std::greater_equal<void> {
    StrictTotallyOrdered{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return std::greater_equal<void>::operator()(t, u);
    }

    using is_transparent = true_type;
  };

  template <class T>
  struct greater_equal<T*> : private std::greater_equal<T*> {
    using std::greater_equal<T*>::operator();
  };

  ///////////////////////////////////////////////////////////////////////////
  // less_equal
  //
  template <class = void> struct less_equal;
  
  StrictTotallyOrdered{T}
  struct less_equal<T> {
    constexpr bool operator()(const T& a, const T& b) const {
      return a <= b;
    }
  };

  template <>
  struct less_equal<void> : private std::less_equal<void> {
    StrictTotallyOrdered{T, U}
    constexpr decltype(auto) operator()(const T& t, const U& u) const {
      return std::less_equal<void>::operator()(t, u);
    }

    using is_transparent = true_type;
  };

  template <class T>
  struct less_equal<T*> : private std::less_equal<T*> {
    using std::less_equal<T*>::operator();
  };
} STL2_CLOSE_NAMESPACE

#endif
