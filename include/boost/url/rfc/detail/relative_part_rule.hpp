//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_DETAIL_RELATIVE_PART_RULE_HPP
#define BOOST_URL_RFC_DETAIL_RELATIVE_PART_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/result.hpp>
#include <boost/url/pct_encoded_view.hpp>
#include <boost/url/rfc/authority_rule.hpp>
#include <boost/url/grammar/range_rule.hpp>

namespace boost {
namespace urls {
namespace detail {

/** Rule for relative-part

    @par BNF
    @code
    relative-part = "//" authority path-abempty
                  / path-absolute
                  / path-noscheme
                  / path-abempty
                  / path-empty
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-4.2
        >4.2. Relative Reference (rfc3986)</a>
    @li <a href="https://www.rfc-editor.org/errata/eid5428"
        >Errata ID: 5428 (rfc3986)</a>

    @see
        @ref authority_rule.
*/
struct relative_part_rule_t
{
    struct value_type
    {
        bool has_authority = false;
        decltype(authority_rule)::value_type authority;
        grammar::range<pct_encoded_view> path;
    };

    auto
    parse(
        char const*& it,
        char const* end
            ) const noexcept ->
        result<value_type>;
};

constexpr relative_part_rule_t relative_part_rule{};

} // detail
} // urls
} // boost

#endif
