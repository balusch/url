//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_OPTIONAL_RULE_HPP
#define BOOST_URL_GRAMMAR_OPTIONAL_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/optional.hpp>
#include <boost/url/result.hpp>

namespace boost {
namespace urls {
namespace grammar {

/** Match a rule, or the empty string

    Optional BNF elements are denoted with
    square brackets. If the specified rule
    returns any error it is treated as if
    the rule did not match.

    @par Value Type
    @code
    using value_type = optional< typename Rule::value_type >;
    @endcode

    @par Example
    Rules are used with the function @ref grammar::parse.
    @code
    result< optional< string_view > > rv = parse( "", optional_rule( token_rule( alpha_chars ) ) );
    @endcode

    @par BNF
    @code
    optional     = [ rule ]
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc5234#section-3.8"
        >3.8.  Optional Sequence (rfc5234)</a>

    @param r The rule to match

    @see
        @ref alpha_chars,
        @ref parse,
        @ref optional,
        @ref token_rule.
*/
#ifdef BOOST_URL_DOCS
template<class Rule>
constexpr
__implementation_defined__
optional_rule( Rule r ) noexcept;
#else
template<class Rule>
struct optional_rule_t
{
    using value_type = optional<
        typename Rule::value_type>;

    result<value_type>
    parse(
        char const*& it,
        char const* end) const;

    template<class R_>
    friend
    constexpr
    auto
    optional_rule(
        R_ const& r) ->
            optional_rule_t<R_>;

private:
    constexpr
    optional_rule_t(
        Rule const& r) noexcept
        : r_(r)
    {
    }

    Rule r_;
};

template<class Rule>
auto
constexpr
optional_rule(
    Rule const& r) ->
        optional_rule_t<Rule>
{
    return { r };
}
#endif

} // grammar
} // urls
} // boost

#include <boost/url/grammar/impl/optional_rule.hpp>

#endif
