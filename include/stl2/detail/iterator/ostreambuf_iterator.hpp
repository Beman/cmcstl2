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
#ifndef STL2_DETAIL_ITERATOR_OSTREAMBUF_ITERATOR_HPP
#define STL2_DETAIL_ITERATOR_OSTREAMBUF_ITERATOR_HPP

#include <iosfwd>
#include <string>
#include <stl2/detail/fwd.hpp>
#include <stl2/detail/raw_ptr.hpp>
#include <stl2/detail/concepts/fundamental.hpp>
#include <stl2/detail/concepts/object.hpp>
#include <stl2/detail/iterator/concepts.hpp>
#include <stl2/detail/iterator/default_sentinel.hpp>

STL2_OPEN_NAMESPACE {
  // Not to spec: MoveConstructible requirement is implicit
  // Extension: models EqualityComparable and Sentinel<default_sentinel>
  template <MoveConstructible charT, class traits = std::char_traits<charT>>
  class ostreambuf_iterator {
  public:
    using iterator_category = output_iterator_tag;
    using difference_type = std::ptrdiff_t;

    using char_type = charT;
    using traits_type = traits;
    using streambuf_type = std::basic_streambuf<charT, traits>;
    using ostream_type = std::basic_ostream<charT, traits>;

    constexpr ostreambuf_iterator() noexcept = default;
    STL2_CONSTEXPR_EXT ostreambuf_iterator(streambuf_type* s) noexcept
    : sbuf_{s}
    {}
    ostreambuf_iterator(ostream_type& s) noexcept
    : ostreambuf_iterator{s.rdbuf()}
    {}

    // Extension
    constexpr ostreambuf_iterator(default_sentinel) noexcept :
      ostreambuf_iterator{} {}

    STL2_CONSTEXPR_EXT ostreambuf_iterator& operator*() {
      return *this;
    }
    STL2_CONSTEXPR_EXT ostreambuf_iterator& operator++() & {
      return *this;
    }
    STL2_CONSTEXPR_EXT ostreambuf_iterator operator++(int) & {
      return *this;
    }

    ostreambuf_iterator& operator=(charT c) & {
      if (sbuf_) {
        if (traits::eq_int_type(sbuf_->sputc(__stl2::move(c)), traits::eof())) {
          sbuf_ = nullptr;
        }
      }
      return *this;
    }

    STL2_CONSTEXPR_EXT bool failed() const noexcept {
      return sbuf_ == nullptr;
    }

    // Extensions:
    friend constexpr bool operator==(
      const ostreambuf_iterator& lhs, const ostreambuf_iterator& rhs) noexcept
    {
      return lhs.sbuf_ == rhs.sbuf_;
    }
    friend constexpr bool operator!=(
      const ostreambuf_iterator& lhs, const ostreambuf_iterator& rhs) noexcept
    {
      return !(lhs == rhs);
    }

    friend constexpr bool operator==(
      const ostreambuf_iterator& lhs, default_sentinel) noexcept
    {
      return lhs.failed();
    }
    friend constexpr bool operator==(
      default_sentinel lhs, const ostreambuf_iterator& rhs) noexcept
    {
      return rhs == lhs;
    }

    friend constexpr bool operator!=(
      const ostreambuf_iterator& lhs, default_sentinel rhs) noexcept
    {
      return !(lhs == rhs);
    }
    friend constexpr bool operator!=(
      default_sentinel lhs, const ostreambuf_iterator& rhs) noexcept
    {
      return !(rhs == lhs);
    }

  private:
    detail::raw_ptr<streambuf_type> sbuf_ = nullptr;
  };
} STL2_CLOSE_NAMESPACE

#endif
