//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IMPL_PCT_ENCODED_RULE_HPP
#define BOOST_URL_RFC_IMPL_PCT_ENCODED_RULE_HPP

#include <boost/url/pct_encoding.hpp>
#include <boost/url/grammar/charset.hpp>

namespace boost {
namespace urls {

namespace detail {

template<class CharSet>
auto
parse_pct_encoded(
    char const*& it,
    char const* end,
    CharSet const& cs) noexcept ->
        result<pct_encoded_view>
{
    auto const start = it;
    // VFALCO TODO
    // opt.plus_to_space?
    std::size_t n = 0;
    char const* it0;
skip:
    it0 = it;
    it = grammar::find_if_not(
        it0, end, cs);
    n += it - it0;
    if(it == end)
        goto finish;
    if(*it != '%')
        goto finish;
    for(;;)
    {
        ++it;
        if(it == end)
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                grammar::error::invalid);
        }
        auto r = grammar::hexdig_value(*it);
        if(r < 0)
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                grammar::error::invalid);
        }
        ++it;
        if(it == end)
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                grammar::error::invalid);
        }
        r = grammar::hexdig_value(*it);
        if(r < 0)
        {
            // expected HEXDIG
            BOOST_URL_RETURN_EC(
                grammar::error::invalid);
        }
        ++n;
        ++it;
        if(it == end)
            break;
        if(*it != '%')
            goto skip;
    }
finish:
    return access::construct(
        string_view(start, it - start), n);
}

} // detail

//------------------------------------------------

template<class CharSet>
auto
pct_encoded_rule_t<CharSet>::
parse(
    char const*& it,
    char const* end) const noexcept ->
        result<value_type>
{
    return detail::parse_pct_encoded(
        it, end, cs_);
}

} // urls
} // boost

#endif
