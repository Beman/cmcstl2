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
#ifndef STL2_DETAIL_ITERATOR_ISTREAM_ITERATOR_HPP
#define STL2_DETAIL_ITERATOR_ISTREAM_ITERATOR_HPP

#include <iosfwd>
#include <string>
#include <stl2/memory.hpp>
#include <stl2/type_traits.hpp>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/raw_ptr.hpp>
#include <stl2/detail/semiregular_box.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/iostream/concepts.hpp>
#include <stl2/detail/iterator/basic_iterator.hpp>
#include <stl2/detail/iterator/concepts.hpp>
#include <stl2/detail/iterator/default_sentinel.hpp>

STL2_OPEN_NAMESPACE {
  namespace detail {
    ///////////////////////////////////////////////////////////////////////////
    // istream_cursor [Implementation detail]
    //
    template <class T, class charT = char,
              class traits = std::char_traits<charT>,
              SignedIntegral Distance = std::ptrdiff_t>
    requires
      models::DefaultConstructible<T> &&
      models::CopyConstructible<T> &&
      models::StreamExtractable<T, std::basic_istream<charT, traits>>
    class istream_cursor : semiregular_box<T> {
      using box_t = semiregular_box<T>;
    public:
      using difference_type = Distance;
      using value_type = T;
      using istream_type = std::basic_istream<charT, traits>;
      using single_pass = true_type;

      struct mixin : protected ebo_box<istream_cursor> {
        using difference_type = istream_cursor::difference_type;
        using iterator_category = input_iterator_tag;
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using char_type = charT;
        using traits_type = traits;
        using istream_type = istream_cursor::istream_type;

        mixin() = default;
        using ebo_box<istream_cursor>::ebo_box;
      };

      constexpr istream_cursor()
      noexcept(is_nothrow_default_constructible<T>::value) = default;

      STL2_CONSTEXPR_EXT istream_cursor(istream_type& s)
      : stream_{&s}
      { next(); }

      constexpr istream_cursor(default_sentinel)
      noexcept(is_nothrow_default_constructible<T>::value)
      : istream_cursor{}
      {}

      STL2_CONSTEXPR_EXT const T& read() const noexcept {
        return box_t::get();
      }

      STL2_CONSTEXPR_EXT void next() {
        *stream_ >> box_t::get();
        if (!*stream_) {
          stream_ = nullptr;
        }
      }

      STL2_CONSTEXPR_EXT bool equal(const istream_cursor& that) const noexcept {
        return stream_ == that.stream_;
      }

      STL2_CONSTEXPR_EXT bool equal(default_sentinel) const noexcept {
        return stream_ == nullptr;
      }

    private:
      raw_ptr<istream_type> stream_ = nullptr;
    };
  }

  ///////////////////////////////////////////////////////////////////////////
  // istream_iterator [iterator.istream]
  // Not to spec:
  // * DefaultConstructible, CopyConstructible, SignedIntegral and
  //   StreamExtractable requirements are implicit.
  // * operator++(int) returns a proxy type instead of istream_iterator.
  //
  template <class T, class charT = char, class traits = std::char_traits<charT>>
  requires
    models::DefaultConstructible<T> &&
    models::CopyConstructible<T> &&
    models::StreamExtractable<T, std::basic_istream<charT, traits>>
  using istream_iterator =
    basic_iterator<detail::istream_cursor<T, charT, traits>>;
} STL2_CLOSE_NAMESPACE

#endif
