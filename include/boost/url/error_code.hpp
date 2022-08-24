//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_ERROR_CODE_HPP
#define BOOST_URL_ERROR_CODE_HPP

#include <boost/url/detail/config.hpp>
#ifndef BOOST_URL_STANDALONE
# include <boost/system/error_code.hpp>
# include <boost/system/system_error.hpp>
#else
# include <system_error>
#endif

namespace boost {
namespace urls {

#ifndef BOOST_URL_DOCS
namespace error_types {
#endif

#ifndef BOOST_URL_STANDALONE

/// The type of error category used by the library
using error_category = boost::system::error_category;

/// The type of error code used by the library
using error_code = boost::system::error_code;

/// The type of error condition used by the library
using error_condition = boost::system::error_condition;

/// The type of system error thrown by the library
using system_error = boost::system::system_error;

/// A function to return the generic error category used by the library
#ifdef BOOST_URL_DOCS
error_category const& generic_category();
#else
using boost::system::generic_category;
#endif

/// A function to return the system error category used by the library
#if BOOST_URL_DOCS
error_category const& system_category();
#else
using boost::system::system_category;
#endif

#else

using error_code = std::error_code;
using error_category = std::error_category;
using error_condition = std::error_condition;
using system_error = std::system_error;

/// A function to return the generic error category used by the library
#ifdef BOOST_URL_DOCS
error_category const& generic_category();
#else
using std::generic_category;
#endif

/// A function to return the system error category used by the library
#if BOOST_URL_DOCS
error_category const& system_category();
#else
using std::system_category;
#endif

#endif

/// The set of constants used for cross-platform error codes
#if BOOST_URL_DOCS
enum errc
{
    __see_below__
};
#elif !defined(BOOST_URL_STANDALONE)
namespace errc = boost::system::errc;
#else
using errc = std::errc;
#endif

#ifndef BOOST_URL_DOCS
} // error_types

using namespace error_types;
#endif

} // urls
} // boost

#endif
