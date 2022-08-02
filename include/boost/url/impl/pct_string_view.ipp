//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PCT_STRING_VIEW_IPP
#define BOOST_URL_IMPL_PCT_STRING_VIEW_IPP

#include <boost/url/pct_string_view.hpp>
#include <boost/url/detail/decode.hpp>
#include <boost/url/grammar/hexdig_chars.hpp>
#include <boost/url/detail/except.hpp>

namespace boost {
namespace urls {

namespace detail {

pct_string_view
make_pct_string_view(
    string_view s) noexcept
{
    return make_pct_string_view(s,
        decode_bytes_unchecked(s));
}

} // detail

//------------------------------------------------

pct_string_view::
pct_string_view(
    string_view s)
    : pct_string_view(
        make_pct_string_view(s
            ).value(BOOST_URL_POS))
{
}

//------------------------------------------------

std::string
pct_string_view::
decode_to_string(
    decode_opts const& opt) const
{
    std::string s;
    if(dn_ > 0)
    {
        s.resize(dn_);
        detail::decode_unchecked(
            &s[0],
            &s[0] + s.size(),
            s_, opt);
    }
    return s;
}

//------------------------------------------------

result<pct_string_view>
make_pct_string_view(
    string_view s) noexcept
{
    auto p = s.begin();
    auto const end = s.end();
    std::size_t dn = 0;
    if(s.size() >= 3)
    {
        auto const safe_end = end - 2;
        while(p < safe_end)
        {
            if(*p != '%')
            {
                ++p;
            }
            else if(
                grammar::hexdig_value(p[1]) >= 0 &&
                grammar::hexdig_value(p[2]) >= 0)
            {
                // percent-escape
                p += 3;
            }
            else
            {
                // invalid encoding
                BOOST_URL_RETURN_EC(
                    error::bad_pct_hexdig);
            }
            ++dn;
        }
    }
    auto const n = end - p;
    if( (n >= 1 && p[0] == '%') ||
        (n >= 2 && p[1] == '%'))
    {
        // invalid encoding
        BOOST_URL_RETURN_EC(
            error::incomplete_encoding);
    }
    dn += n;
    return detail::make_pct_string_view(s, dn);
}

} // urls
} // boost

#endif

