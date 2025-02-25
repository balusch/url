//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URL_VIEW_BASE_IPP
#define BOOST_URL_IMPL_URL_VIEW_BASE_IPP

#include <boost/url/url_view_base.hpp>
#include <boost/url/detail/except.hpp>
#include <boost/url/detail/over_allocator.hpp>

namespace boost {
namespace urls {

// construct empty view
url_view_base::
url_view_base() noexcept
    : u_(false)
{
}

// construct reference
url_view_base::
url_view_base(
    detail::url_impl const& impl) noexcept
    : u_(impl)
{
}

//------------------------------------------------

std::size_t
url_view_base::
digest(std::size_t salt) const noexcept
{
    detail::fnv_1a h(salt);
    detail::ci_digest(u_.get(id_scheme), h);
    detail::digest_encoded(u_.get(id_user), h);
    detail::digest_encoded(u_.get(id_pass), h);
    detail::ci_digest_encoded(u_.get(id_host), h);
    h.put(u_.get(id_port));
    detail::normalized_path_digest(
        u_.get(id_path), is_path_absolute(), h);
    detail::digest_encoded(u_.get(id_query), h);
    detail::digest_encoded(u_.get(id_frag), h);
    return h.digest();
}

//------------------------------------------------
//
// Observers
//
//------------------------------------------------

struct url_view_base::shared_impl
    : url_view
{
    virtual
    ~shared_impl()
    {
    }

    shared_impl(
        url_view const& u) noexcept
        : url_view(u)
    {
        u_.cs_ = reinterpret_cast<
            char const*>(this + 1);
    }
};

std::shared_ptr<url_view const>
url_view_base::
persist() const
{
    using T = shared_impl;
    using Alloc = std::allocator<char>;
    Alloc a;
    auto p = std::allocate_shared<T>(
        detail::over_allocator<T, Alloc>(
            size(), a), url_view(u_));
    std::memcpy(
        reinterpret_cast<char*>(
            p.get() + 1), data(), size());
    return p;
}

//------------------------------------------------
//
// Scheme
//
//------------------------------------------------

bool
url_view_base::
has_scheme() const noexcept
{
    auto const n = u_.len(
        id_scheme);
    if(n == 0)
        return false;
    BOOST_ASSERT(n > 1);
    BOOST_ASSERT(
        u_.get(id_scheme
            ).ends_with(':'));
    return true;
}

string_view
url_view_base::
scheme() const noexcept
{
    auto s = u_.get(id_scheme);
    if(! s.empty())
    {
        BOOST_ASSERT(s.size() > 1);
        BOOST_ASSERT(s.ends_with(':'));
        s.remove_suffix(1);
    }
    return s;
}

urls::scheme
url_view_base::
scheme_id() const noexcept
{
    return u_.scheme_;
}

//----------------------------------------------------------
//
// Authority
//
//----------------------------------------------------------

string_view
url_view_base::
encoded_authority() const noexcept
{
    auto s = u_.get(id_user, id_path);
    if(! s.empty())
    {
        BOOST_ASSERT(has_authority());
        s.remove_prefix(2);
    }
    return s;
}

authority_view
url_view_base::
authority() const noexcept
{
    detail::url_impl u(true);
    u.cs_ = encoded_authority().data();
    if(has_authority())
    {
        u.set_size(id_user, u_.len(id_user) - 2);
        u.set_size(id_pass, u_.len(id_pass));
        u.set_size(id_host, u_.len(id_host));
        u.set_size(id_port, u_.len(id_port));
    }
    else
    {
        u.set_size(id_user, u_.len(id_user));
        BOOST_ASSERT(u_.len(id_pass) == 0);
        BOOST_ASSERT(u_.len(id_host) == 0);
        BOOST_ASSERT(u_.len(id_port) == 0);
    }
    u.decoded_[id_user] = u_.decoded_[id_user];
    u.decoded_[id_pass] = u_.decoded_[id_pass];
    u.decoded_[id_host] = u_.decoded_[id_host];
    for (int i = 0; i < 16; ++i)
        u.ip_addr_[i] = u_.ip_addr_[i];
    u.port_number_ = u_.port_number_;
    u.host_type_ = u_.host_type_;
    return u.construct_authority();
}

// userinfo

bool
url_view_base::
has_userinfo() const noexcept
{
    auto n = u_.len(id_pass);
    if(n == 0)
        return false;
    BOOST_ASSERT(has_authority());
    BOOST_ASSERT(u_.get(
        id_pass).ends_with('@'));
    return true;
}

string_view
url_view_base::
encoded_userinfo() const noexcept
{
    auto s = u_.get(
        id_user, id_host);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        has_authority());
    s.remove_prefix(2);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.ends_with('@'));
    s.remove_suffix(1);
    return s;
}

string_view
url_view_base::
encoded_user() const noexcept
{
    auto s = u_.get(id_user);
    if(! s.empty())
    {
        BOOST_ASSERT(
            has_authority());
        s.remove_prefix(2);
    }
    return s;
}

bool
url_view_base::
has_password() const noexcept
{
    auto const n = u_.len(id_pass);
    if(n > 1)
    {
        BOOST_ASSERT(u_.get(id_pass
            ).starts_with(':'));
        BOOST_ASSERT(u_.get(id_pass
            ).ends_with('@'));
        return true;
    }
    BOOST_ASSERT(n == 0 || u_.get(
        id_pass).ends_with('@'));
    return false;
}

string_view
url_view_base::
encoded_password() const noexcept
{
    auto s = u_.get(id_pass);
    switch(s.size())
    {
    case 1:
        BOOST_ASSERT(
            s.starts_with('@'));
        BOOST_FALLTHROUGH;
    case 0:
        return s.substr(0, 0);
    default:
        break;
    }
    BOOST_ASSERT(
        s.ends_with('@'));
    BOOST_ASSERT(
        s.starts_with(':'));
    return s.substr(1,
        s.size() - 2);
}

// host

string_view
url_view_base::
encoded_host() const noexcept
{
    return u_.get(id_host);
}

string_view
url_view_base::
encoded_hostname() const noexcept
{
    string_view s = u_.get(id_host);
    switch(u_.host_type_)
    {
    case urls::host_type::none:
        BOOST_ASSERT(s.empty());
        break;

    case urls::host_type::name:
    case urls::host_type::ipv4:
        break;

    case urls::host_type::ipv6:
    case urls::host_type::ipvfuture:
    {
        BOOST_ASSERT(s.size() >= 2);
        BOOST_ASSERT(s.front() == '[');
        BOOST_ASSERT(s.back() == ']');
        s.remove_prefix(1);
        s.remove_suffix(1);
        break;
    }
    }
    return s;
}

pct_encoded_view
url_view_base::
hostname() const noexcept
{
    string_view s = u_.get(id_host);
    std::size_t n;
    switch(u_.host_type_)
    {
    default:
    case urls::host_type::none:
        BOOST_ASSERT(s.empty());
        n = 0;
        break;

    case urls::host_type::name:
    case urls::host_type::ipv4:
        n = u_.decoded_[id_host];
        break;

    case urls::host_type::ipv6:
    case urls::host_type::ipvfuture:
    {
        BOOST_ASSERT(s.size() >= 2);
        BOOST_ASSERT(s.front() == '[');
        BOOST_ASSERT(s.back() == ']');
        s.remove_prefix(1);
        s.remove_suffix(1);
        n = u_.decoded_[id_host] - 2;
        BOOST_ASSERT(n == s.size());
        break;
    }
    }
    pct_decode_opts opt;
    opt.plus_to_space = false;
    return detail::access::construct(
        s, n, opt);
}

urls::ipv4_address
url_view_base::
ipv4_address() const noexcept
{
    if(u_.host_type_ !=
        urls::host_type::ipv4)
        return {};
    std::array<
        unsigned char, 4> bytes;
    std::memcpy(
        &bytes[0],
        &u_.ip_addr_[0], 4);
    return urls::ipv4_address(
        bytes);
}

urls::ipv6_address
url_view_base::
ipv6_address() const noexcept
{
    if(u_.host_type_ ==
        urls::host_type::ipv6)
    {
        std::array<
            unsigned char, 16> bytes;
        std::memcpy(
            &bytes[0],
            &u_.ip_addr_[0], 16);
        return urls::ipv6_address(
            bytes);
    }
    return urls::ipv6_address();
}

string_view
url_view_base::
ipvfuture() const noexcept
{
    if(u_.host_type_ ==
        urls::host_type::ipvfuture)
    {
        string_view s = u_.get(id_host);
        BOOST_ASSERT(s.size() >= 6);
        BOOST_ASSERT(s.front() == '[');
        BOOST_ASSERT(s.back() == ']');
        s.remove_prefix(1);
        s.remove_suffix(1);
        return s;
    }
    return {};
}

// port

bool
url_view_base::
has_port() const noexcept
{
    auto const n = u_.len(id_port);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        u_.get(id_port).starts_with(':'));
    return true;
}

string_view
url_view_base::
port() const noexcept
{
    auto s = u_.get(id_port);
    if(s.empty())
        return s;
    BOOST_ASSERT(has_port());
    return s.substr(1);
}

std::uint16_t
url_view_base::
port_number() const noexcept
{
    BOOST_ASSERT(
        has_port() ||
        u_.port_number_ == 0);
    return u_.port_number_;
}

string_view
url_view_base::
encoded_host_and_port() const noexcept
{
    return u_.get(id_host, id_path);
}

string_view
url_view_base::
encoded_origin() const noexcept
{
    if(u_.len(id_user) < 2)
        return {};
    return u_.get(id_scheme, id_path);
}

//----------------------------------------------------------
//
// Query
//
//----------------------------------------------------------

bool
url_view_base::
has_query() const noexcept
{
    auto const n = u_.len(
        id_query);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        u_.get(id_query).
            starts_with('?'));
    return true;
}

string_view
url_view_base::
encoded_query() const noexcept
{
    auto s = u_.get(id_query);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.starts_with('?'));
    return s.substr(1);
}

params_encoded_view
url_view_base::
encoded_params() const noexcept
{
    auto s = u_.get(id_query);
    if(s.empty())
        return params_encoded_view(s, 0);
    BOOST_ASSERT(s[0] == '?');
    s.remove_prefix(1);
    return params_encoded_view(s, u_.nparam_);
}

params_view
url_view_base::
params() const noexcept
{
    auto s = u_.get(id_query);
    if(s.empty())
        return {s, 0};
    BOOST_ASSERT(s[0] == '?');
    s.remove_prefix(1);
    return {s, u_.nparam_};
}

//----------------------------------------------------------
//
// Fragment
//
//----------------------------------------------------------

bool
url_view_base::
has_fragment() const noexcept
{
    auto const n = u_.len(id_frag);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        u_.get(id_frag).
            starts_with('#'));
    return true;
}

string_view
url_view_base::
encoded_fragment() const noexcept
{
    auto s = u_.get(id_frag);
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.starts_with('#'));
    return s.substr(1);
}

//------------------------------------------------
//
// Comparisons
//
//------------------------------------------------

int
url_view_base::
compare(const url_view_base& other) const noexcept
{
    int comp = detail::ci_compare(
        scheme(),
        other.scheme());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_user(),
        other.encoded_user());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_password(),
        other.encoded_password());
    if ( comp != 0 )
        return comp;

    comp = detail::ci_compare_encoded(
        encoded_host(),
        other.encoded_host());
    if ( comp != 0 )
        return comp;

    comp = detail::compare(
        port(),
        other.port());
    if ( comp != 0 )
        return comp;

    comp = detail::normalized_path_compare(
        encoded_path(),
        other.encoded_path(),
        is_path_absolute(),
        other.is_path_absolute());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_query(),
        other.encoded_query());
    if ( comp != 0 )
        return comp;

    comp = detail::compare_encoded(
        encoded_fragment(),
        other.encoded_fragment());
    if ( comp != 0 )
        return comp;

    return 0;
}

} // urls
} // boost

#endif
