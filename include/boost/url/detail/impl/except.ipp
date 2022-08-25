//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_EXCEPT_IPP
#define BOOST_URL_DETAIL_IMPL_EXCEPT_IPP

#include <boost/url/detail/except.hpp>
#ifndef BOOST_URL_STANDALONE
# include <boost/throw_exception.hpp>
#elif defined(BOOST_URL_STANDALONE) && defined(BOOST_NO_EXCEPTIONS)
# include <exception>
#endif
#include <new>
#include <stdexcept>

#if defined(BOOST_URL_STANDALONE)
namespace boost {

#if defined(BOOST_NO_EXCEPTIONS)
// When exceptions are disabled
// in standalone, you must provide
// this function.
BOOST_NORETURN
void
throw_exception(std::exception const&);
#endif

} // boost
#endif

namespace boost {
namespace urls {
namespace detail {

#if defined(BOOST_URL_STANDALONE) && \
    ! defined(BOOST_NO_EXCEPTIONS)
// this is in the json namespace to avoid
// colliding with boost::throw_exception
template<class E>
void
BOOST_NORETURN
throw_exception(E e, source_location loc)
{
    (void) loc;
    throw e;
}
#endif

void
throw_bad_alloc(
    source_location const& loc)
{
    throw_exception(
        std::bad_alloc(), loc);
}

void
throw_length_error(
    char const* what,
    source_location const& loc)
{
    throw_exception(
        std::length_error(what), loc);
}

void
throw_invalid_argument(
    source_location const& loc)
{
    throw_exception(
        std::invalid_argument(""), loc);
}

void
throw_invalid_argument(
    char const* what,
    source_location const& loc)
{
    throw_exception(
        std::invalid_argument(what), loc);
}

void
throw_out_of_range(
    source_location const& loc)
{
    throw_exception(
        std::out_of_range(
            "out of range"), loc);
}

} // detail
} // url
} // boost

#endif
