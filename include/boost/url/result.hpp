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

#ifndef BOOST_URL_STANDALONE
# include <boost/system/result.hpp>
#else
# include <variant>
# include <system_error>
# include <boost/url/mp11.hpp>
# include <boost/url/detail/except.hpp>
#endif

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

BOOST_NORETURN BOOST_NOINLINE inline void throw_exception_from_error( error_code const & e, boost::source_location const& loc )
{
    throw ( std::system_error(e) );
}

BOOST_NORETURN BOOST_NOINLINE inline void throw_exception_from_error( errc::errc_t const & e, boost::source_location const& loc )
{
    throw ( std::system_error( std::make_error_code( e ) ) );
}

BOOST_NORETURN BOOST_NOINLINE inline void throw_exception_from_error( std::error_code const & e, boost::source_location const& loc )
{
    throw ( std::system_error( e ) );
}

BOOST_NORETURN BOOST_NOINLINE inline void throw_exception_from_error( std::errc const & e, boost::source_location const& loc )
{
    throw ( std::system_error( std::make_error_code( e ) ) );
}

BOOST_NORETURN BOOST_NOINLINE inline void throw_exception_from_error( std::exception_ptr const & p, boost::source_location const& loc )
{
    if( p )
    {
        std::rethrow_exception( p );
    }
    else
    {
        throw ( std::bad_exception() );
    }
}

template<class T>
using remove_cvref = typename std::remove_cv_t<typename std::remove_reference_t<T>>;

template<class... T>
using is_errc_t = std::is_same<mp_list<remove_cvref<T>...>, mp_list<std::errc>>;


template<class T, class E = error_code> class result
{
private:

    std::variant<T, E> v_;

public:

    // constructors

    // default
    template<class En2 = void, class En = typename std::enable_if<
            std::is_void<En2>::value &&
            std::is_default_constructible<T>::value
    >::type>
    constexpr result()
    noexcept( std::is_nothrow_default_constructible<T>::value )
            : v_( in_place_value )
    {
    }

    // implicit, value
    template<class A = T, typename std::enable_if<
            std::is_convertible<A, T>::value &&
            !(is_errc_t<A>::value && std::is_arithmetic<T>::value) &&
            !std::is_constructible<E, A>::value, int>::type = 0>
    constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<T, A>::value )
            : v_( in_place_value, std::forward<A>(a) )
    {
    }

    // implicit, error
    template<class A = E, class = void, typename std::enable_if<
            std::is_convertible<A, E>::value &&
            !std::is_constructible<T, A>::value, int>::type = 0>
    constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<E, A>::value )
            : v_( in_place_error, std::forward<A>(a) )
    {
    }

    // explicit, value
    template<class... A, class En = typename std::enable_if<
            std::is_constructible<T, A...>::value &&
            !(is_errc_t<A...>::value && std::is_arithmetic<T>::value) &&
            !std::is_constructible<E, A...>::value &&
            sizeof...(A) >= 1
    >::type>
    explicit constexpr result( A&&... a )
    noexcept( std::is_nothrow_constructible<T, A...>::value )
            : v_( in_place_value, std::forward<A>(a)... )
    {
    }

    // explicit, error
    template<class... A, class En2 = void, class En = typename std::enable_if<
            !std::is_constructible<T, A...>::value &&
            std::is_constructible<E, A...>::value &&
            sizeof...(A) >= 1
    >::type>
    explicit constexpr result( A&&... a )
    noexcept( std::is_nothrow_constructible<E, A...>::value )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // tagged, value
    template<class... A, class En = typename std::enable_if<
            std::is_constructible<T, A...>::value
    >::type>
    constexpr result( in_place_value_t, A&&... a )
    noexcept( std::is_nothrow_constructible<T, A...>::value )
            : v_( in_place_value, std::forward<A>(a)... )
    {
    }

    // tagged, error
    template<class... A, class En = typename std::enable_if<
            std::is_constructible<E, A...>::value
    >::type>
    constexpr result( in_place_error_t, A&&... a )
    noexcept( std::is_nothrow_constructible<E, A...>::value )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // converting
    template<class T2, class E2, class En = typename std::enable_if<
            std::is_convertible<T2, T>::value &&
            std::is_convertible<E2, E>::value
    >::type>
    constexpr result( result<T2, E2> const& r2 )
    noexcept(
    std::is_nothrow_constructible<T, T2 const&>::value &&
    std::is_nothrow_constructible<E, E2>::value &&
    std::is_nothrow_default_constructible<E2>::value &&
    std::is_nothrow_copy_constructible<E2>::value )
            : v_( in_place_error, r2.error() )
    {
        if( r2 )
        {
            v_.template emplace<0>( *r2 );
        }
    }

    template<class T2, class E2, class En = typename std::enable_if<
            std::is_convertible<T2, T>::value &&
            std::is_convertible<E2, E>::value
    >::type>
    constexpr result( result<T2, E2>&& r2 )
    noexcept(
    std::is_nothrow_constructible<T, T2&&>::value &&
    std::is_nothrow_constructible<E, E2>::value &&
    std::is_nothrow_default_constructible<E2>::value &&
    std::is_nothrow_copy_constructible<E2>::value )
            : v_( in_place_error, r2.error() )
    {
        if( r2 )
        {
            v_.template emplace<0>( std::move( *r2 ) );
        }
    }

    // queries

    constexpr bool has_value() const noexcept
    {
        return v_.index() == 0;
    }

    constexpr bool has_error() const noexcept
    {
        return v_.index() == 1;
    }

    constexpr explicit operator bool() const noexcept
    {
        return v_.index() == 0;
    }

    // checked value access
    constexpr T& value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) &
    {
        if( has_value() )
        {
            return std::get<0>( v_ );
        }
        else
        {
            throw_exception_from_error( std::get<1>( v_ ), loc );
        }
    }

    constexpr T const& value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) const&
    {
        if( has_value() )
        {
            return std::get<0>( v_ );
        }
        else
        {
            throw_exception_from_error( std::get<1>( v_ ), loc );
        }
    }

    template<class U = T>
    constexpr
    typename std::enable_if<std::is_move_constructible<U>::value, T>::type
    value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) &&
    {
        return std::move( value( loc ) );
    }

    template<class U = T>
    constexpr
    typename std::enable_if<!std::is_move_constructible<U>::value, T&&>::type
    value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) &&
    {
        return std::move( value( loc ) );
    }

    template<class U = T>
    constexpr
    typename std::enable_if<std::is_move_constructible<U>::value, T>::type
    value() const && = delete;

    template<class U = T>
    constexpr
    typename std::enable_if<!std::is_move_constructible<U>::value, T const&&>::type
    value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) const &&
    {
        return std::move( value( loc ) );
    }

    // unchecked value access

    constexpr T* operator->() noexcept
    {
        return std::get_if<0>( &v_ );
    }

    constexpr T const* operator->() const noexcept
    {
        return std::get_if<0>( &v_ );
    }

    constexpr T& operator*() & noexcept
    {
        T* p = operator->();

                BOOST_ASSERT( p != 0 );

        return *p;
    }

    constexpr T const& operator*() const & noexcept
    {
        T const* p = operator->();

                BOOST_ASSERT( p != 0 );

        return *p;
    }

    template<class U = T>
    constexpr
    typename std::enable_if<std::is_move_constructible<U>::value, T>::type
    operator*() && noexcept(std::is_nothrow_move_constructible<T>::value)
    {
        return std::move(**this);
    }

    template<class U = T>
    constexpr
    typename std::enable_if<!std::is_move_constructible<U>::value, T&&>::type
    operator*() && noexcept
    {
        return std::move(**this);
    }

    template<class U = T>
    constexpr
    typename std::enable_if<std::is_move_constructible<U>::value, T>::type
    operator*() const && noexcept = delete;

    template<class U = T>
    constexpr
    typename std::enable_if<!std::is_move_constructible<U>::value, T const&&>::type
    operator*() const && noexcept
    {
        return std::move(**this);
    }

    // error access

    constexpr E error() const
    noexcept( std::is_nothrow_default_constructible<E>::value && std::is_nothrow_copy_constructible<E>::value )
    {
        return has_error()? std::get<1>( v_ ): E();
    }

    // swap

    constexpr void swap( result& r )
    noexcept( noexcept( v_.swap( r.v_ ) ) )
    {
        v_.swap( r.v_ );
    }

    friend constexpr void swap( result & r1, result & r2 )
    noexcept( noexcept( r1.swap( r2 ) ) )
    {
        r1.swap( r2 );
    }

    // equality

    friend constexpr bool operator==( result const & r1, result const & r2 )
    noexcept( noexcept( r1.v_ == r2.v_ ) )
    {
        return r1.v_ == r2.v_;
    }

    friend constexpr bool operator!=( result const & r1, result const & r2 )
    noexcept( noexcept( !( r1 == r2 ) ) )
    {
        return !( r1 == r2 );
    }
};

template<class Ch, class Tr, class T, class E> std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& os, result<T, E> const & r )
{
    if( r.has_value() )
    {
        os << "value:" << *r;
    }
    else
    {
        os << "error:" << r.error();
    }

    return os;
}

// result<void>

template<class E> class result<void, E>
{
private:

    std::variant<std::monostate, E> v_;

public:

    // constructors

    // default
    constexpr result() noexcept
            : v_( in_place_value )
    {
    }

    // explicit, error
    template<class A, class En = typename std::enable_if<
            std::is_constructible<E, A>::value &&
            !std::is_convertible<A, E>::value
    >::type>
    explicit constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<E, A>::value )
            : v_( in_place_error, std::forward<A>(a) )
    {
    }

    // implicit, error
    template<class A, class En2 = void, class En = typename std::enable_if<
            std::is_convertible<A, E>::value
    >::type>
    constexpr result( A&& a )
    noexcept( std::is_nothrow_constructible<E, A>::value )
            : v_( in_place_error, std::forward<A>(a) )
    {
    }

    // more than one arg, error
    template<class... A, class En2 = void, class En3 = void, class En = typename std::enable_if<
            std::is_constructible<E, A...>::value &&
            sizeof...(A) >= 2
    >::type>
    constexpr result( A&&... a )
    noexcept( std::is_nothrow_constructible<E, A...>::value )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // tagged, value
    constexpr result( in_place_value_t ) noexcept
            : v_( in_place_value )
    {
    }

    // tagged, error
    template<class... A, class En = typename std::enable_if<
            std::is_constructible<E, A...>::value
    >::type>
    constexpr result( in_place_error_t, A&&... a )
    noexcept( std::is_nothrow_constructible<E, A...>::value )
            : v_( in_place_error, std::forward<A>(a)... )
    {
    }

    // queries

    constexpr bool has_value() const noexcept
    {
        return v_.index() == 0;
    }

    constexpr bool has_error() const noexcept
    {
        return v_.index() == 1;
    }

    constexpr explicit operator bool() const noexcept
    {
        return v_.index() == 0;
    }

    // checked value access

    constexpr void value( boost::source_location const& loc = BOOST_CURRENT_LOCATION ) const
    {
        if( has_value() )
        {
        }
        else
        {
            throw_exception_from_error( std::get<1>( v_ ), loc );
        }
    }

    // unchecked value access

    constexpr void* operator->() noexcept
    {
        return std::get_if<0>( &v_ );
    }

    constexpr void const* operator->() const noexcept
    {
        return std::get_if<0>( &v_ );
    }

    constexpr void operator*() const noexcept
    {
                BOOST_ASSERT( has_value() );
    }

    // error access

    constexpr E error() const
    noexcept( std::is_nothrow_default_constructible<E>::value && std::is_nothrow_copy_constructible<E>::value )
    {
        return has_error()? std::get<1>( v_ ): E();
    }

    // swap

    constexpr void swap( result& r )
    noexcept( noexcept( v_.swap( r.v_ ) ) )
    {
        v_.swap( r.v_ );
    }

    friend constexpr void swap( result & r1, result & r2 )
    noexcept( noexcept( r1.swap( r2 ) ) )
    {
        r1.swap( r2 );
    }

    // equality

    friend constexpr bool operator==( result const & r1, result const & r2 )
    noexcept( noexcept( r1.v_ == r2.v_ ) )
    {
        return r1.v_ == r2.v_;
    }

    friend constexpr bool operator!=( result const & r1, result const & r2 )
    noexcept( noexcept( !( r1 == r2 ) ) )
    {
        return !( r1 == r2 );
    }
};

template<class Ch, class Tr, class E> std::basic_ostream<Ch, Tr>& operator<<( std::basic_ostream<Ch, Tr>& os, result<void, E> const & r )
{
    if( r.has_value() )
    {
        os << "value:void";
    }
    else
    {
        os << "error:" << r.error();
    }

    return os;
}

} // internal

template<class T>
using result = internal::result<T, error_code>;

#endif

} // urls
} // boost

#endif
