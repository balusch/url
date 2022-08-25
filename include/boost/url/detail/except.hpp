//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_EXCEPT_HPP
#define BOOST_URL_DETAIL_EXCEPT_HPP

#include <boost/url/error.hpp>
#ifndef BOOST_URL_STANDALONE
#include <boost/assert/source_location.hpp>
#endif

namespace boost {
namespace urls {
namespace detail {

#ifndef BOOST_URL_STANDALONE
using source_location = boost::source_location;
#else
# define BOOST_CURRENT_LOCATION {}
struct source_location{};
#endif

BOOST_URL_DECL void BOOST_NORETURN throw_bad_alloc(
    source_location const& loc = BOOST_CURRENT_LOCATION);
BOOST_URL_DECL void BOOST_NORETURN throw_invalid_argument(
    source_location const& loc = BOOST_CURRENT_LOCATION);
BOOST_URL_DECL void BOOST_NORETURN throw_invalid_argument(char const* what,
    source_location const& loc = BOOST_CURRENT_LOCATION);
BOOST_URL_DECL void BOOST_NORETURN throw_length_error(char const* what,
    source_location const& loc = BOOST_CURRENT_LOCATION);
BOOST_URL_DECL void BOOST_NORETURN throw_out_of_range(
    source_location const& loc = BOOST_CURRENT_LOCATION);

} // detail
} // urls
} // boost

#endif
