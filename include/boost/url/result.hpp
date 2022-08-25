//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RESULT_HPP
#define BOOST_URL_RESULT_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error_code.hpp>
#include <boost/system/result.hpp>

namespace boost {
namespace urls {

/** The type of result returned by library functions

    This is an alias template used as the return type
    for functions that can either return a container,
    or fail with an error code. This is a brief
    synopsis of the type:

    @par Declaration
    @code
    template< class T >
    class result
    {
    public:
        // Return true if the result contains an error
        constexpr bool has_error() const noexcept;

        // These return true if the result contains a value
        constexpr bool has_value() const noexcept;
        constexpr explicit operator bool() const noexcept;

        // Return the value or throw an exception if has_value()==false
        constexpr T& value();
        constexpr T& operator*();
        constexpr T const& value() const;
        constexpr T const& operator*() const;

        // Return the error, which is default constructed if has_error()==false
        constexpr error_code error() const noexcept;

        ...more
    @endcode

    @par Usage
    Given the function @ref parse_uri with this signature:
    @code
    result< url_view > parse_uri( string_view s ) noexcept;
    @endcode

    The following statement captures the value in a
    variable upon success, otherwise throws:
    @code
    url_view u = parse_uri( "http://example.com/path/to/file.txt" ).value();
    @endcode

    This statement captures the result in a local
    variable and inspects the error condition:
    @code
    result< url_view > r = parse_uri( "http://example.com/path/to/file.txt" );

    if( r.has_error() )
        std::cout << r.error();
    else
        std::cout << r.value();
    @endcode

    @note For a full synopsis of the type, please see
    the corresponding documentation in Boost.System.

    @tparam T The type of value held by the result.
*/

#ifndef BOOST_URL_STANDALONE

template<class T>
using result = boost::system::result<T, error_code>;

#else

using in_place_value_t = std::in_place_index_t<0>;
constexpr in_place_value_t in_place_value{};

using in_place_error_t = std::in_place_index_t<1>;
constexpr in_place_error_t in_place_error{};

namespace internal {
template<class T>
using remove_cvref = typename std::remove_cv_t<typename std::remove_reference_t<T>>;

template<class... T>
using is_errc_t = std::is_same<mp11::mp_list<remove_cvref<T>...>, mp11::mp_list<errc::errc_t>>;
}

template<class T, class E = std::error_code>
class result {
private:
    std::variant<T, E> v_;

public:

    // constructors

    // default
    template<class En2 = void, class En = typename std::enable_if<
            std::is_void_v<En2> &&
            std::is_default_constructible_v<T>>::type>
    constexpr result() noexcept( std::is_nothrow_default_constructible_v<T> )
    : v_( in_place_value )
    {

    }

    // implicit, value
    template<class A = T, typename std::enable_if<
            std::is_convertible_v<A, T> &&
            !(detail::is_errc_t<A>::value && std::is_arithmetic_v<T>) &&
            !std::is_constructible_v<E, A>, int>::type = 0>
    constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<T, A>::value )
            : v_( in_place_value, std::forward<A>(a) )
    {
    }

    // implicit, error
    template<class A = E, class = void, typename std::enable_if<
            std::is_convertible_v<A, E> &&
            !std::is_constructible_v<T, A>, int>::type = 0>
    constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<E, A>::value )
            : v_( in_place_error, std::forward<A>(a) )
    {
    }

    // explicit, value
    template<class... A, class En = typename std::enable_if<
            std::is_constructible<T, A...>::value &&
            !(detail::is_errc_t<A...>::value && std::is_arithmetic_v<T>) &&
            !std::is_constructible_v<E, A...> &&
            sizeof...(A) >= 1
    >::type>
    explicit constexpr result( A&&... a )
    noexcept( std::is_nothrow_constructible<T, A...>::value )
            : v_( in_place_value, std::forward<A>(a)... )
    {
    }

    // explicit, error
    template<class... A, class En2 = void, class En = typename std::enable_if<
            !std::is_constructible_v<T, A...> &&
            std::is_constructible_v<E, A...> &&
            sizeof...(A) >= 1
    >::type>
    explicit constexpr result( A&&... a )
    noexcept( std::is_nothrow_constructible_v<E, A...> )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // tagged, value
    template<class... A, class En = typename std::enable_if<
            std::is_constructible_v<T, A...>
    >::type>
    constexpr result( in_place_value_t, A&&... a )
    noexcept( std::is_nothrow_constructible_v<T, A...> )
            : v_( in_place_value, std::forward<A>(a)... )
    {
    }

    // tagged, error
    template<class... A, class En = typename std::enable_if<
            std::is_constructible_v<E, A...>
    >::type>
    constexpr result( in_place_error_t, A&&... a )
    noexcept( std::is_nothrow_constructible_v<E, A...> )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // converting
    template<class T2, class E2, class En = typename std::enable_if<
            std::is_convertible_v<T2, T> &&
            std::is_convertible_v<E2, E>
    >::type>
    constexpr result( result<T2, E2> const& r2 )
    noexcept(
    std::is_nothrow_constructible_v<T, T2 const&> &&
    std::is_nothrow_constructible_v<E, E2> &&
    std::is_nothrow_default_constructible_v<E2> &&
    std::is_nothrow_copy_constructible_v<E2> )
            : v_( in_place_error, r2.error() )
    {
        if( r2 )
        {
            v_.template emplace<0>( *r2 );
        }
    }

    template<class T2, class E2, class En = typename std::enable_if<
            std::is_convertible_v<T2, T> &&
            std::is_convertible_v<E2, E>
    >::type>
    constexpr result( result<T2, E2>&& r2 )
    noexcept(
    std::is_nothrow_constructible_v<T, T2&&> &&
    std::is_nothrow_constructible_v<E, E2> &&
    std::is_nothrow_default_constructible_v<E2> &&
    std::is_nothrow_copy_constructible_v<E2> )
            : v_( in_place_error, r2.error() )
    {
        if( r2 )
        {
            v_.template emplace<0>( std::move( *r2 ) );
        }
    }
};

#endif

} // urls
} // boost

#endif
