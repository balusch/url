//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/grammar/range_rule.hpp>

#include <boost/url/grammar/alpha_chars.hpp>
#include <boost/url/grammar/delim_rule.hpp>
#include <boost/url/grammar/parse.hpp>
#include <boost/url/grammar/tuple_rule.hpp>
#include <boost/url/grammar/token_rule.hpp>

#include "test_rule.hpp"

#include <algorithm>
#include <initializer_list>

namespace boost {
namespace urls {
namespace grammar {

struct range_rule_test
{
    struct big_rule
    {
        char unused[4096];
        using value_type = string_view;

        result<value_type>
        parse(
            char const*& it,
            char const* end) const noexcept
        {
            if(it == end)
                return error::mismatch;
            if(*it != ';')
                return error::mismatch;
            ++it;
            if(it == end)
                return error::mismatch;
            if(*it == ';')
                return error::mismatch;
            return string_view(it++, 1);
        }
    };

    template<class R>
    static
    void
    check(
        R const& r,
        string_view s,
        std::initializer_list<
            string_view> init)
    {
        auto rv = parse(s, r);
        if(! BOOST_TEST(rv.has_value()))
            return;
        if(! BOOST_TEST_EQ(
                rv->size(), init.size()))
            return;
        BOOST_TEST(
            std::equal(
                rv->begin(),
                rv->end(),
                init.begin()));
    }

    void
    run()
    {
        // constexpr
        {
            constexpr auto r = range_rule(
                token_rule(alpha_chars),
                tuple_rule(
                    squelch(
                        delim_rule('+')),
                    token_rule(alpha_chars)));

            check(r, "", {});
            check(r, "x", {"x"});
        }

        // javadoc
        {
            result< range<string_view> > rv = parse( ";alpha;xray;charlie",
                range_rule(
                    tuple_rule(
                        squelch( delim_rule( ';' ) ),
                        token_rule( alpha_chars ) ),
                    1 ) );
            (void)rv;
        }

        // javadoc
        {
            result< range< string_view > > rv = parse( "whiskey,tango,foxtrot",
                range_rule(
                    token_rule( alpha_chars ),          // first
                    tuple_rule(                      // next
                        squelch( delim_rule(',') ),
                        token_rule( alpha_chars ) ) ) );

            (void)rv;
        }

        // default construction
        {
            range<string_view> v;
        }

        // lower limit
        // upper limit
        {
            {
                constexpr auto r = range_rule(
                    tuple_rule(
                        squelch(
                            delim_rule(';')),
                        token_rule(alpha_chars)),
                    2, 3);

                bad(r, "", error::mismatch);
                bad(r, ";x", error::mismatch);
                check(r, ";x;y", {"x","y"});
                check(r, ";x;y;z", {"x","y","z"});
                bad(r, ";a;b;c;d", error::mismatch);
                bad(r, ";a;b;c;d;e", error::mismatch);
            }
            {
                constexpr auto r = range_rule(
                    token_rule(alpha_chars),
                    tuple_rule(
                        squelch(
                            delim_rule('+')),
                        token_rule(alpha_chars)),
                    2, 3);

                bad(r, "", error::mismatch);
                bad(r, "x", error::mismatch);
                check(r, "x+y", {"x","y"});
                check(r, "x+y+z", {"x","y","z"});
                bad(r, "a+b+c+d", error::mismatch);
                bad(r, "a+b+c+d+e", error::mismatch);
            }
        }

        // big rules
        {
            {
                constexpr auto r = range_rule(
                    big_rule{},
                    2, 3);

                bad(r, "", error::mismatch);
                bad(r, ";x", error::mismatch);
                check(r, ";x;y", {"x","y"});
                check(r, ";x;y;z", {"x","y","z"});
                bad(r, ";a;b;c;d", error::mismatch);
                bad(r, ";a;b;c;d;e", error::mismatch);
            }
            {
                constexpr auto r = range_rule(
                    big_rule{}, big_rule{},
                    2, 3);

                bad(r, "", error::mismatch);
                bad(r, "x", error::mismatch);
                check(r, ";x;y", {"x","y"});
                check(r, ";x;y;z", {"x","y","z"});
                bad(r, ";a;b;c;d", error::mismatch);
                bad(r, ";a;b;c;d;e", error::mismatch);
            }
        }
    }
};

TEST_SUITE(
    range_rule_test,
    "boost.url.grammar.range_rule");

} // grammar
} // urls
} // boost
