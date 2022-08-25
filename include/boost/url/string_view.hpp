//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_STRING_VIEW_HPP
#define BOOST_URL_STRING_VIEW_HPP

#include <boost/url/detail/config.hpp>
#ifndef BOOST_URL_STANDALONE
# include <boost/utility/string_view.hpp>
# ifndef BOOST_NO_CXX17_HDR_STRING_VIEW
#  include <string_view>
# endif
#else
# if __has_include(<string_view>)
#  include <string_view>
#  if __cpp_lib_string_view < 201603L
#   error Support for std::string_view is required to use Boost.URL standalone
#  endif
# else
#  error Header <string_view> is required to use Boost.URL standalone
# endif
#endif
#include <type_traits>

namespace boost {
namespace urls {

#ifdef BOOST_URL_DOCS

/** The type of string view used by the library.

    This type alias is set depending
    on how the library is configured:

    @par Use with Boost

    If the macro `BOOST_URL_STANDALONE` is
    not defined, this type will be an alias
    for `boost::string_view`.
    Compiling a program using the library will
    require Boost, and a compiler conforming
    to C++11 or later.

    @par Use without Boost

    If the macro `BOOST_URL_STANDALONE` is
    defined, this type will be an alias
    for `std::string_view`.
    Compiling a program using the library will
    require only a compiler conforming to C++17
    or later.

    @see https://en.cppreference.com/w/cpp/string/basic_string_view
*/
using string_view = __see_below__;

#elif !defined(BOOST_URL_STANDALONE)

using string_view = boost::string_view;

#else

using string_view = std::string_view;

// starts_with

constexpr bool starts_with(string_view s1, string_view s2) noexcept {
    return s1.size() >= s2.size() && std::char_traits<char>::compare( s1.data(), s2.data(), s2.size() ) == 0;
}

constexpr bool starts_with(string_view s, char x) noexcept {
    return !s.empty() && s.front() == x;
}

constexpr bool starts_with(string_view s1, char const* s2) noexcept {
    return starts_with(s1, string_view(s2));
}

// ends_with

constexpr bool ends_with(string_view s1, string_view s2) noexcept {
    return s1.size() >= s2.size() && std::char_traits<char>::compare(s1.data() + s1.size() - s2.size(), s2.data(), s2.size() ) == 0;
}

constexpr bool ends_with(string_view s, char x) noexcept {
    return !s.empty() && s.back() == x;
}

constexpr bool ends_with(string_view s1, char const* s2) noexcept {
    return ends_with(s1, string_view(s2));
}

#endif

namespace detail {

template <class T>
using is_string_viewish = typename std::enable_if<
    std::is_convertible<T const &, string_view>::value &&
    !std::is_convertible<T const &, char const *>::value>::type;

} // detail

} // urls
} // boost

#endif
