//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_ERROR_IPP
#define BOOST_URL_GRAMMAR_IMPL_ERROR_IPP

#include <boost/url/grammar/error.hpp>

namespace boost {
namespace urls {
namespace grammar {

error_code
make_error_code(
    error e) noexcept
{
    struct codes : error_category
    {
        const char*
        name() const noexcept override
        {
            return "boost.url.bnf";
        }

        std::string
        message(int ev) const override
        {
            switch(static_cast<error>(ev))
            {
            default:
case error::need_more: return "need more";
case error::mismatch: return "mismatch";
case error::invalid: return "invalid";
case error::range_end: return "range end";
case error::leftover: return "leftover";
case error::overflow: return "overflow";
case error::syntax: return "syntax";
            }
        }

        error_condition
        default_error_condition(
            int ev) const noexcept override
        {
            switch(static_cast<error>(ev))
            {
case error::invalid:
case error::overflow:
case error::syntax:
                return condition::fatal;
            default:
                return {ev, *this};
            }
        }
    };

    static codes const cat{};
    return error_code{static_cast<
        std::underlying_type<error>::type>(e), cat};
}

//------------------------------------------------

error_condition
make_error_condition(
    condition c) noexcept
{
    struct codes : error_category
    {
        const char*
        name() const noexcept override
        {
            return "boost.url.grammar";
        }

        std::string
        message(int cv) const override
        {
            switch(static_cast<condition>(cv))
            {
            default:
            case condition::fatal:
                return "fatal condition";
            }
        }
    };
    static codes const cat{};
    return error_condition{static_cast<
        std::underlying_type<condition>::type>(c), cat};
}

} // grammar
} // urls
} // boost

#endif
