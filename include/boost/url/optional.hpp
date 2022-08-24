//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_OPTIONAL_HPP
#define BOOST_URL_OPTIONAL_HPP

#include <boost/url/detail/config.hpp>
#ifndef BOOST_URL_STANDALONE
# include <boost/optional.hpp>
# ifndef BOOST_NO_CXX17_HDR_OPTIONAL
#  include <optional>
# endif
#else
# if __has_include(<optional>)
#  include <optional>
#  if __cpp_lib_optional < 201606L
#   error Support for std::optional is required to use Boost.URL standalone
#  endif
# else
#  error Header <optional> is required to use Boost.URL standalone
# endif
#endif
#include <type_traits>

namespace boost {
namespace urls {

#ifdef BOOST_URL_DOCS

/** The type of optional used by the library.

    This type alias is set depending
    on how the library is configured:

    @par Use with Boost

    If the macro `BOOST_URL_STANDALONE` is
    not defined, this type will be an alias
    for `boost::optional`.
    Compiling a program using the library will
    require Boost, and a compiler conforming
    to C++11 or later.

    @par Use without Boost

    If the macro `BOOST_URL_STANDALONE` is
    defined, this type will be an alias
    for `std::optional`.
    Compiling a program using the library will
    require only a compiler conforming to C++17
    or later.

    @see https://en.cppreference.com/w/cpp/utility/optional
*/
using optional = __see_below__;

#elif !defined(BOOST_URL_STANDALONE)

template<class T>
using optional = boost::optional<T>;

#else

template<class T>
using optional = std::optional<T>;

#endif

} // urls
} // boost

#endif
