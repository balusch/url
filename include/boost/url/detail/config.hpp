//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_CONFIG_HPP
#define BOOST_URL_DETAIL_CONFIG_HPP

#ifndef BOOST_URL_STANDALONE
# include <boost/config.hpp>
# include <boost/assert.hpp>
# include <boost/static_assert.hpp>;
# include <boost/config/workaround.hpp>
# include <boost/throw_exception.hpp>
#else
# include <cassert>
#endif
#include <cstdint>
#include <type_traits>
#include <utility>
#include <limits.h>
#include <stdint.h>

#if CHAR_BIT != 8
# error unsupported platform
#endif

#if defined(BOOST_URL_DOCS)
# define BOOST_URL_DECL
#else
# if (defined(BOOST_URL_DYN_LINK) || defined(BOOST_ALL_DYN_LINK)) && !defined(BOOST_URL_STATIC_LINK)
#  if defined(BOOST_URL_SOURCE)
#   define BOOST_URL_DECL  BOOST_SYMBOL_EXPORT
#   define BOOST_URL_BUILD_DLL
#  else
#   define BOOST_URL_DECL  BOOST_SYMBOL_IMPORT
#  endif
# endif // shared lib
# ifndef  BOOST_URL_DECL
#  define BOOST_URL_DECL
# endif
# if !defined(BOOST_URL_SOURCE) && !defined(BOOST_ALL_NO_LIB) && !defined(BOOST_URL_NO_LIB)
#  define BOOST_LIB_NAME boost_url
#  if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_URL_DYN_LINK)
#   define BOOST_DYN_LINK
#  endif
#  include <boost/config/auto_link.hpp>
# endif
#endif

// BOOST_NORETURN ---------------------------------------------//
// Macro to use before a function declaration/definition to designate
// the function as not returning normally (i.e. with a return statement
// or by leaving the function scope, if the function return type is void).
#if !defined(BOOST_NORETURN)
#  if defined(_MSC_VER)
#    define BOOST_NORETURN __declspec(noreturn)
#  elif defined(__GNUC__)
#    define BOOST_NORETURN __attribute__ ((__noreturn__))
#  elif defined(__has_attribute) && defined(__SUNPRO_CC) && (__SUNPRO_CC > 0x5130)
#    if __has_attribute(noreturn)
#      define BOOST_NORETURN [[noreturn]]
#    endif
#  elif defined(__has_cpp_attribute)
#    if __has_cpp_attribute(noreturn)
#      define BOOST_NORETURN [[noreturn]]
#    endif
#  endif
#endif

#ifndef BOOST_ASSERT
#define BOOST_ASSERT assert
#endif

#ifndef BOOST_STATIC_ASSERT
#define BOOST_STATIC_ASSERT( ... ) static_assert(__VA_ARGS__, #__VA_ARGS__)
#endif

#ifndef BOOST_FALLTHROUGH
#define BOOST_FALLTHROUGH [[fallthrough]]
#endif

#ifndef BOOST_FORCEINLINE
# ifdef _MSC_VER
#  define BOOST_FORCEINLINE __forceinline
# elif defined(__GNUC__) || defined(__clang__)
#  define BOOST_FORCEINLINE inline __attribute__((always_inline))
# else
#  define BOOST_FORCEINLINE inline
# endif
#endif

#ifndef BOOST_NOINLINE
# ifdef _MSC_VER
#  define BOOST_NOINLINE __declspec(noinline)
# elif defined(__GNUC__) || defined(__clang__)
#  define BOOST_NOINLINE __attribute__((noinline))
# else
#  define BOOST_NOINLINE
# endif
#endif

#ifndef BOOST_THROW_EXCEPTION
# ifndef BOOST_NO_EXCEPTIONS
#  define BOOST_THROW_EXCEPTION(x) throw(x)
# else
#  define BOOST_THROW_EXCEPTION(x) do{}while(0)
# endif
#endif

#if ! defined(BOOST_URL_NO_SSE2) && \
    ! defined(BOOST_URL_USE_SSE2)
# if (defined(_M_IX86) && _M_IX86_FP == 2) || \
      defined(_M_X64) || defined(__SSE2__)
#  define BOOST_URL_USE_SSE2
# endif
#endif

#ifndef BOOST_SYMBOL_VISIBLE
#define BOOST_SYMBOL_VISIBLE
#endif

#ifndef BOOST_URL_STANDALONE
#if BOOST_WORKAROUND( BOOST_GCC_VERSION, <= 72000 ) || \
    BOOST_WORKAROUND( BOOST_CLANG_VERSION, <= 35000 )
# define BOOST_URL_CONSTEXPR
#else
# define BOOST_URL_CONSTEXPR constexpr
#endif
#else
# define BOOST_URL_CONSTEXPR constexpr
#endif

#ifndef BOOST_URL_STANDALONE
// Add source location to error codes
#ifdef BOOST_URL_NO_SOURCE_LOCATION
# define BOOST_URL_ERR(ev) (ev)
# define BOOST_URL_RETURN_EC(ev) return (ev)
#else
# define BOOST_URL_ERR(ev) (::boost::system::error_code( (ev), [] { \
         static constexpr auto loc(BOOST_CURRENT_LOCATION); \
         return &loc; }()))
# define BOOST_URL_RETURN_EC(ev) \
    static constexpr auto loc ## __LINE__(BOOST_CURRENT_LOCATION); \
    return ::boost::system::error_code((ev), &loc ## __LINE__)
#endif
#else
# define BOOST_URL_ERR(ev) (ev)
# define BOOST_URL_RETURN_EC(ev) return (ev)
#endif

// detect 32/64 bit
#if UINTPTR_MAX == UINT64_MAX
# define BOOST_URL_ARCH 64
#elif UINTPTR_MAX == UINT32_MAX
# define BOOST_URL_ARCH 32
#else
# error Unknown or unsupported architecture, please open an issue
#endif

#ifndef BOOST_URL_STACK_BYTES
#define BOOST_URL_STACK_BYTES 4096
#endif

#ifndef BOOST_URL_STANDALONE
#if BOOST_WORKAROUND( BOOST_GCC_VERSION, < 80000 ) || \
    BOOST_WORKAROUND( BOOST_CLANG_VERSION, < 30900 )
#define BOOST_URL_RETURN(x) return std::move((x))
#else
#define BOOST_URL_RETURN(x) return (x)
#endif
#else
# define BOOST_URL_RETURN(x) return (x)
#endif

using pos_t = size_t;

#ifndef BOOST_URL_MAX_SIZE
// we leave room for a null,
// and still fit in pos_t
#define BOOST_URL_MAX_SIZE ((pos_t(-1))-1)
#endif

#ifdef BOOST_GCC
#define BOOST_URL_NO_INLINE [[gnu::noinline]]
#else
#define BOOST_URL_NO_INLINE
#endif

#endif
