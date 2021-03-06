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
#ifndef STL2_DETAIL_RANGE_CONCEPTS_HPP
#define STL2_DETAIL_RANGE_CONCEPTS_HPP

#include <initializer_list>

#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/meta.hpp>
#include <stl2/detail/concepts/compare.hpp>
#include <stl2/detail/concepts/core.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/iterator/concepts.hpp>
#include <stl2/detail/range/access.hpp>

STL2_OPEN_NAMESPACE {
  ///////////////////////////////////////////////////////////////////////////
  // Range [ranges.range]
  //
  template <class T>
  using iterator_t = decltype(__stl2::begin(declval<T&>()));

  template <class T>
  using sentinel_t = decltype(__stl2::end(declval<T&>()));

  template <class T>
  concept bool Range() {
    return requires { typename sentinel_t<T>; };
  }

  namespace models {
    template <class>
    constexpr bool Range = false;
    __stl2::Range{R}
    constexpr bool Range<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // SizedRange [ranges.sized]
  //
  template <class R>
  constexpr bool disable_sized_range = false;

  template <class R>
  constexpr bool __sized_range = false;
  template <class R>
    requires requires (const R& r) {
      STL2_DEDUCTION_CONSTRAINT(__stl2::size(r), Integral);
      STL2_CONVERSION_CONSTRAINT(__stl2::size(r), difference_type_t<iterator_t<R>>);
    }
  constexpr bool __sized_range<R> = true;

  template <class R>
  concept bool SizedRange() {
    return Range<R>() &&
      !disable_sized_range<__uncvref<R>> &&
      __sized_range<remove_reference_t<R>>;
  }

  namespace models {
    template <class>
    constexpr bool SizedRange = false;
    __stl2::SizedRange{R}
    constexpr bool SizedRange<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // View [ranges.view]
  //
  struct view_base {};

  template <class T>
  concept bool _ContainerLike =
    Range<T>() && Range<const T>() &&
    !Same<reference_t<iterator_t<T>>,
          reference_t<iterator_t<const T>>>();

  namespace models {
    template <class>
    constexpr bool _ContainerLike = false;
    __stl2::_ContainerLike{R}
    constexpr bool _ContainerLike<R> = true;
  }

  template <class T>
  struct enable_view {};

  template <class T>
  constexpr bool __view_predicate = true;

  template <class T>
    requires _Valid<meta::_t, enable_view<T>>
  constexpr bool __view_predicate<T> = meta::_v<enable_view<T>>;

  // TODO: Be very certain that "!" here works as intended.
  template <_ContainerLike T>
    requires !(DerivedFrom<T, view_base>() ||
               _Valid<meta::_t, enable_view<T>>)
  constexpr bool __view_predicate<T> = false;

  template <class T>
  concept bool View() {
    return Range<T>() &&
      __view_predicate<T> &&
      Semiregular<T>();
  }

  namespace models {
    template <class>
    constexpr bool View = false;
    __stl2::View{V}
    constexpr bool View<V> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // BoundedRange [ranges.bounded]
  //
  template <class T>
  concept bool BoundedRange() {
    return Range<T>() && Same<iterator_t<T>, sentinel_t<T>>();
  }

  namespace models {
    template <class>
    constexpr bool BoundedRange = false;
    __stl2::BoundedRange{R}
    constexpr bool BoundedRange<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // OutputRange [ranges.output]
  // Not to spec: value category sensitive.
  //
  template <class R, class T>
  concept bool OutputRange() {
    return Range<R>() && OutputIterator<iterator_t<R>, T>();
  }

  namespace models {
    template <class, class>
    constexpr bool OutputRange = false;
    __stl2::OutputRange{R, T}
    constexpr bool OutputRange<R, T> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // InputRange [ranges.input]
  //
  template <class T>
  concept bool InputRange() {
    return Range<T>() && InputIterator<iterator_t<T>>();
  }

  namespace models {
    template <class>
    constexpr bool InputRange = false;
    __stl2::InputRange{R}
    constexpr bool InputRange<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // ForwardRange [ranges.forward]
  //
  template <class T>
  concept bool ForwardRange() {
    return Range<T>() && ForwardIterator<iterator_t<T>>();
  }

  namespace models {
    template <class>
    constexpr bool ForwardRange = false;
    __stl2::ForwardRange{R}
    constexpr bool ForwardRange<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // BidirectionalRange [ranges.bidirectional]
  //
  template <class T>
  concept bool BidirectionalRange() {
    return Range<T>() && BidirectionalIterator<iterator_t<T>>();
  }

  namespace models {
    template <class>
    constexpr bool BidirectionalRange = false;
    __stl2::BidirectionalRange{R}
    constexpr bool BidirectionalRange<R> = true;
  }

  ///////////////////////////////////////////////////////////////////////////
  // RandomAccessRange [ranges.random.access]
  //
  template <class T>
  concept bool RandomAccessRange() {
    return Range<T>() && RandomAccessIterator<iterator_t<T>>();
  }

  namespace models {
    template <class>
    constexpr bool RandomAccessRange = false;
    __stl2::RandomAccessRange{R}
    constexpr bool RandomAccessRange<R> = true;
  }

  namespace ext {
    template <class R>
    constexpr bool __contiguous_range = false;
    template <class R>
      requires requires (R& r) {
        STL2_EXACT_TYPE_CONSTRAINT(__stl2::data(r), add_pointer_t<reference_t<iterator_t<R>>>);
      }
    constexpr bool __contiguous_range<R> = true;

    template <class R>
    concept bool ContiguousRange() {
      return SizedRange<R>() && ContiguousIterator<iterator_t<R>>() &&
        __contiguous_range<remove_reference_t<R>>;
    }
  }

  namespace models {
    template <class>
    constexpr bool ContiguousRange = false;
    __stl2::ext::ContiguousRange{R}
    constexpr bool ContiguousRange<R> = true;
  }
} STL2_CLOSE_NAMESPACE

#endif
