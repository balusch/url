//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_QUERY_HPP
#define BOOST_URL_RFC_QUERY_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/bnf/range.hpp>
#include <boost/url/rfc/pct_encoded.hpp>
#include <boost/optional.hpp>

namespace boost {
namespace urls {
namespace rfc {

struct query_param
{
    pct_encoded_str key;
    optional<
        pct_encoded_str> value;
};

/** BNF for query

    @par BNF
    @code
    query         = *( pchar / "/" / "?" )
                  / path-absolute
                  / path-rootless
                  / path-empty
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-3.4
*/
struct query
{
    using value_type =
        bnf::range<query_param>;
    
    value_type& v;

    BOOST_URL_DECL
    static
    bool
    begin(
        char const*& it,
        char const* const end,
        error_code& ec,
        query_param& t) noexcept;

    BOOST_URL_DECL
    static
    bool
    increment(
        char const*& it,
        char const* const end,
        error_code& ec,
        query_param& t) noexcept;

    friend
    bool
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        query const& t)
    {
        return bnf::parse_range(
            it, end, ec, t.v, t);
    }
};

} // rfc
} // urls
} // boost

#endif