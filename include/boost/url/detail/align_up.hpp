//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/url
//

#ifndef BOOST_URL_DETAIL_ALIGN_HPP
#define BOOST_URL_DETAIL_ALIGN_HPP

#ifndef BOOST_URL_STANDALONE
#include <bost/align/align_up.hpp>
#else
#include <cstddef>
#include <memory>
#endif

namespace boost {
namespace urls {
namespace detail {

#ifndef BOOST_URL_STANDALONE

using align_up = boost::alignment::align_up;

#else

namespace internal {
template<class T, class U>
struct not_pointer {
    typedef U type;
};

template<class T, class U>
struct not_pointer<T*, U> { };
} // internal

template<class T>
constexpr inline typename internal::not_pointer<T, T>::type
align_up(T value, std::size_t alignment) noexcept
{
    return T((value + (T(alignment) - 1)) & ~T(alignment - 1));
}

#endif

} // detail
} // urls
} // boost

#endif
