//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_VARIANT_HPP
#define BOOST_URL_VARIANT_HPP

#include <boost/url/detail/config.hpp>
#ifndef BOOST_URL_STANDALONE
# include <boost/variant2/variant.hpp>
#else
# include<variant>
#endif

namespace boost {
namespace urls {

/** The type of variant used by the library
*/

#ifndef BOOST_URL_STANDALONE
template<class... Ts>
using variant =
    boost::variant2::variant<Ts...>;
#else
template<class... Ts>
using variant = std::variant<Ts...>;
#endif

} // urls
} // boost

#endif
