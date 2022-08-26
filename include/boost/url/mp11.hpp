//
// Copyright (c) 2022 Jianyong Chen (baluschch@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_MP11_HPP
#define BOOST_URL_MP11_HPP

#ifndef BOOST_URL_STANDALONE
# include<boost/mp11/mp_list.hpp>
#endif

namespace boost {
namespace urls {

///////////////////////////////////////////////////////////////////////////////
// index_sequence
///////////////////////////////////////////////////////////////////////////////

#if ! defined(BOOST_URL_STANDALONE)

template <std::size_t... Is>
using index_sequence = boost::mp11::index_sequence<Is...>;

template <std::size_t N>
using make_index_sequence = boost::mp11::make_index_sequence<N>;

template<class... T>
using index_sequence_for = boost::mp11::index_sequence_for<T...>;

#else

template <std::size_t... Is>
using index_sequence = std::index_sequence<Is...>;

template <std::size_t N>
using make_index_sequence = std::make_index_sequence<N>;

template<class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;

#endif

///////////////////////////////////////////////////////////////////////////////
// mp class
///////////////////////////////////////////////////////////////////////////////

// mp_bool

#if ! defined(BOOST_URL_STANDALONE)

#else

template<bool B>
using mp_bool = std::integral_constant<bool, B>;

using mp_true = mp_bool<true>;
using mp_false = mp_bool<false>;

// mp_to_bool
template<class T>
using mp_to_bool = mp_bool<static_cast<bool>( T::value )>;

// mp_not<T>
template<class T>
using mp_not = mp_bool< !T::value >;

// mp_int
template<int I>
using mp_int = std::integral_constant<int, I>;

// mp_size_t
template<std::size_t N>
using mp_size_t = std::integral_constant<std::size_t, N>;

#endif

// mp_list

#if ! defined(BOOST_URL_STANDALONE)

#else

#endif

// mp algorithm

} // urls
} // boost

#endif
