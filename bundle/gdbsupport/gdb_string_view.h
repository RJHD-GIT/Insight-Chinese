// Components for manipulating non-owning sequences of characters -*- C++ -*-


#ifndef COMMON_GDB_STRING_VIEW_H
#define COMMON_GDB_STRING_VIEW_H

// Note: This file has been stolen from the gcc repo
// (libstdc++-v3/include/experimental/string_view) and has local modifications.

// Copyright (C) 2013-2021 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

//
// N3762 basic_string_view library
//


#if __cplusplus >= 201703L

#include <string_view>

namespace gdb {
  using string_view = std::string_view;
} /* namespace gdb */

#else /* __cplusplus < 201703L */

#include <string>
#include <limits>
#include "gdb_assert.h"

namespace gdb {

  /**
   *  @class basic_string_view <experimental/string_view>
   *  @brief  A non-owning reference to a string.
   *
   *  @ingroup strings
   *  @ingroup sequences
   *  @ingroup experimental
   *
   *  @tparam _CharT  Type of character
   *  @tparam _Traits  Traits for character type, defaults to
   *                   char_traits<_CharT>.
   *
   *  A basic_string_view looks like this:
   *
   *  @code
   *    _CharT*    _M_str
   *    size_t     _M_len
   *  @endcode
   */
  template<typename _CharT, typename _Traits = std::char_traits<_CharT>>
    class basic_string_view
    {
    public:

      // types
      using traits_type = _Traits;
      using value_type = _CharT;
      using pointer = const _CharT*;
      using const_pointer = const _CharT*;
      using reference = const _CharT&;
      using const_reference = const _CharT&;
      using const_iterator = const _CharT*;
      using iterator = const_iterator;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;
      using reverse_iterator = const_reverse_iterator;
      using size_type = size_t;
      using difference_type = ptrdiff_t;
      static constexpr size_type npos = size_type(-1);

      // [string.view.cons], construct/copy

      constexpr
      basic_string_view() noexcept
      : _M_len{0}, _M_str{nullptr}
      { }

      constexpr basic_string_view(const basic_string_view&) noexcept = default;

      template<typename _Allocator>
	basic_string_view(const std::basic_string<_CharT, _Traits,
			  _Allocator>& __str) noexcept
	: _M_len{__str.length()}, _M_str{__str.data()}
	{ }

      /*constexpr*/ basic_string_view(const _CharT* __str)
      : _M_len{__str == nullptr ? 0 : traits_type::length(__str)},
	_M_str{__str}
      { }

      constexpr basic_string_view(const _CharT* __str, size_type __len)
      : _M_len{__len},
	_M_str{__str}
      { }

      basic_string_view&
      operator=(const basic_string_view&) noexcept = default;

      // [string.view.iterators], iterators

      constexpr const_iterator
      begin() const noexcept
      { return this->_M_str; }

      constexpr const_iterator
      end() const noexcept
      { return this->_M_str + this->_M_len; }

      constexpr const_iterator
      cbegin() const noexcept
      { return this->_M_str; }

      constexpr const_iterator
      cend() const noexcept
      { return this->_M_str + this->_M_len; }

      const_reverse_iterator
      rbegin() const noexcept
      { return const_reverse_iterator(this->end()); }

      const_reverse_iterator
      rend() const noexcept
      { return const_reverse_iterator(this->begin()); }

      const_reverse_iterator
      crbegin() const noexcept
      { return const_reverse_iterator(this->end()); }

      const_reverse_iterator
      crend() const noexcept
      { return const_reverse_iterator(this->begin()); }

      // [string.view.capacity], capacity

      constexpr size_type
      size() const noexcept
      { return this->_M_len; }

      constexpr size_type
      length() const noexcept
      { return _M_len; }

      constexpr size_type
      max_size() const noexcept
      {
	return (npos - sizeof(size_type) - sizeof(void*))
		/ sizeof(value_type) / 4;
      }

      constexpr bool
      empty() const noexcept
      { return this->_M_len == 0; }

      // [string.view.access], element access

      constexpr const _CharT&
      operator[](size_type __pos) const
      {
	// TODO: Assert to restore in a way compatible with the constexpr.
	// __glibcxx_assert(__pos < this->_M_len);
	return *(this->_M_str + __pos);
      }

      constexpr const _CharT&
      at(size_type __pos) const
      {
	return __pos < this->_M_len
	     ? *(this->_M_str + __pos)
	     : (error (_("basic_string_view::at: __pos "
			 "(which is %zu) >= this->size() "
			 "(which is %zu)"),
		       __pos, this->size()),
		*this->_M_str);
      }

      constexpr const _CharT&
      front() const
      {
	// TODO: Assert to restore in a way compatible with the constexpr.
	// __glibcxx_assert(this->_M_len > 0);
	return *this->_M_str;
      }

      constexpr const _CharT&
      back() const
      {
	// TODO: Assert to restore in a way compatible with the constexpr.
	// __glibcxx_assert(this->_M_len > 0);
	return *(this->_M_str + this->_M_len - 1);
      }

      constexpr const _CharT*
      data() const noexcept
      { return this->_M_str; }

      // [string.view.modifiers], modifiers:

      /*constexpr*/ void
      remove_prefix(size_type __n)
      {
	gdb_assert (this->_M_len >= __n);
	this->_M_str += __n;
	this->_M_len -= __n;
      }

      /*constexpr*/ void
      remove_suffix(size_type __n)
      { this->_M_len -= __n; }

      /*constexpr*/ void
      swap(basic_string_view& __sv) noexcept
      {
	auto __tmp = *this;
	*this = __sv;
	__sv = __tmp;
      }


      // [string.view.ops], string operations:

      template<typename _Allocator>
	explicit operator std::basic_string<_CharT, _Traits, _Allocator>() const
	{
	  return { this->_M_str, this->_M_len };
	}

      size_type
      copy(_CharT* __str, size_type __n, size_type __pos = 0) const
      {
	gdb_assert (__str != nullptr || __n == 0);
	if (__pos > this->_M_len)
	  error (_("basic_string_view::copy: __pos "
		   "(which is %zu) > this->size() "
		   "(which is %zu)"),
		 __pos, this->size());
	size_type __rlen{std::min(__n, size_type{this->_M_len  - __pos})};
	for (auto __begin = this->_M_str + __pos,
	     __end = __begin + __rlen; __begin != __end;)
	  *__str++ = *__begin++;
	return __rlen;
      }


      // [string.view.ops], string operations:

      /*constexpr*/ basic_string_view
      substr(size_type __pos, size_type __n=npos) const
      {
	return __pos <= this->_M_len
	     ? basic_string_view{this->_M_str + __pos,
				std::min(__n, size_type{this->_M_len  - __pos})}
	     : (error (_("basic_string_view::substr: __pos "
			 "(which is %zu) > this->size() "
			 "(which is %zu)"),
		       __pos, this->size()), basic_string_view{});
      }

      /*constexpr*/ int
      compare(basic_string_view __str) const noexcept
      {
	int __ret = traits_type::compare(this->_M_str, __str._M_str,
					 std::min(this->_M_len, __str._M_len));
	if (__ret == 0)
	  __ret = _S_compare(this->_M_len, __str._M_len);
	return __ret;
      }

      /*constexpr*/ int
      compare(size_type __pos1, size_type __n1, basic_string_view __str) const
      { return this->substr(__pos1, __n1).compare(__str); }

      /*constexpr*/ int
      compare(size_type __pos1, size_type __n1,
	      basic_string_view __str, size_type __pos2, size_type __n2) const
      { return this->substr(__pos1, __n1).compare(__str.substr(__pos2, __n2)); }

      /*constexpr*/ int
      compare(const _CharT* __str) const noexcept
      { return this->compare(basic_string_view{__str}); }

      /*constexpr*/ int
      compare(size_type __pos1, size_type __n1, const _CharT* __str) const
      { return this->substr(__pos1, __n1).compare(basic_string_view{__str}); }

      /*constexpr*/ int
      compare(size_type __pos1, size_type __n1,
	      const _CharT* __str, size_type __n2) const
      {
	return this->substr(__pos1, __n1)
		   .compare(basic_string_view(__str, __n2));
      }

      /*constexpr*/ size_type
      find(basic_string_view __str, size_type __pos = 0) const noexcept
      { return this->find(__str._M_str, __pos, __str._M_len); }

      /*constexpr*/ size_type
      find(_CharT __c, size_type __pos=0) const noexcept;

      /*constexpr*/ size_type
      find(const _CharT* __str, size_type __pos, size_type __n) const noexcept;

      /*constexpr*/ size_type
      find(const _CharT* __str, size_type __pos=0) const noexcept
      { return this->find(__str, __pos, traits_type::length(__str)); }

      /*constexpr*/ size_type
      rfind(basic_string_view __str, size_type __pos = npos) const noexcept
      { return this->rfind(__str._M_str, __pos, __str._M_len); }

      /*constexpr*/ size_type
      rfind(_CharT __c, size_type __pos = npos) const noexcept;

      /*constexpr*/ size_type
      rfind(const _CharT* __str, size_type __pos, size_type __n) const noexcept;

      /*constexpr*/ size_type
      rfind(const _CharT* __str, size_type __pos = npos) const noexcept
      { return this->rfind(__str, __pos, traits_type::length(__str)); }

      /*constexpr*/ size_type
      find_first_of(basic_string_view __str, size_type __pos = 0) const noexcept
      { return this->find_first_of(__str._M_str, __pos, __str._M_len); }

      /*constexpr*/ size_type
      find_first_of(_CharT __c, size_type __pos = 0) const noexcept
      { return this->find(__c, __pos); }

      /*constexpr*/ size_type
      find_first_of(const _CharT* __str, size_type __pos, size_type __n) const;

      /*constexpr*/ size_type
      find_first_of(const _CharT* __str, size_type __pos = 0) const noexcept
      { return this->find_first_of(__str, __pos, traits_type::length(__str)); }

      /*constexpr*/ size_type
      find_last_of(basic_string_view __str,
		   size_type __pos = npos) const noexcept
      { return this->find_last_of(__str._M_str, __pos, __str._M_len); }

      size_type
      find_last_of(_CharT __c, size_type __pos=npos) const noexcept
      { return this->rfind(__c, __pos); }

      /*constexpr*/ size_type
      find_last_of(const _CharT* __str, size_type __pos, size_type __n) const;

      /*constexpr*/ size_type
      find_last_of(const _CharT* __str, size_type __pos = npos) const noexcept
      { return this->find_last_of(__str, __pos, traits_type::length(__str)); }

      /*constexpr*/ size_type
      find_first_not_of(basic_string_view __str,
			size_type __pos = 0) const noexcept
      { return this->find_first_not_of(__str._M_str, __pos, __str._M_len); }

      /*constexpr*/ size_type
      find_first_not_of(_CharT __c, size_type __pos = 0) const noexcept;

      /*constexpr*/ size_type
      find_first_not_of(const _CharT* __str,
			size_type __pos, size_type __n) const;

      /*constexpr*/ size_type
      find_first_not_of(const _CharT* __str, size_type __pos = 0) const noexcept
      {
	return this->find_first_not_of(__str, __pos,
				       traits_type::length(__str));
      }

      /*constexpr*/ size_type
      find_last_not_of(basic_string_view __str,
		       size_type __pos = npos) const noexcept
      { return this->find_last_not_of(__str._M_str, __pos, __str._M_len); }

      /*constexpr*/ size_type
      find_last_not_of(_CharT __c, size_type __pos = npos) const noexcept;

      /*constexpr*/ size_type
      find_last_not_of(const _CharT* __str,
		       size_type __pos, size_type __n) const;

      /*constexpr*/ size_type
      find_last_not_of(const _CharT* __str,
		       size_type __pos = npos) const noexcept
      {
	return this->find_last_not_of(__str, __pos,
				      traits_type::length(__str));
      }

    private:

      static constexpr int
      _S_compare(size_type __n1, size_type __n2) noexcept
      {
	return difference_type(__n1 - __n2) > std::numeric_limits<int>::max()
	     ? std::numeric_limits<int>::max()
	     : difference_type(__n1 - __n2) < std::numeric_limits<int>::min()
	     ? std::numeric_limits<int>::min()
	     : static_cast<int>(difference_type(__n1 - __n2));
      }

      size_t	    _M_len;
      const _CharT* _M_str;
    };

  // [string.view.comparison], non-member basic_string_view comparison functions

  namespace __detail
  {
    // Identity transform to create a non-deduced context, so that only one
    // argument participates in template argument deduction and the other
    // argument gets implicitly converted to the deduced type. See n3766.html.
    template<typename _Tp>
      using __idt = typename std::common_type<_Tp>::type;
  }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator==(basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.size() == __y.size() && __x.compare(__y) == 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator==(basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return __x.size() == __y.size() && __x.compare(__y) == 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator==(__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.size() == __y.size() && __x.compare(__y) == 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator!=(basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return !(__x == __y); }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator!=(basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return !(__x == __y); }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator!=(__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return !(__x == __y); }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator< (basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) < 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator< (basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return __x.compare(__y) < 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator< (__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) < 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator> (basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) > 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator> (basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return __x.compare(__y) > 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator> (__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) > 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator<=(basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) <= 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator<=(basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return __x.compare(__y) <= 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator<=(__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) <= 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator>=(basic_string_view<_CharT, _Traits> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) >= 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator>=(basic_string_view<_CharT, _Traits> __x,
	       __detail::__idt<basic_string_view<_CharT, _Traits>> __y) noexcept
    { return __x.compare(__y) >= 0; }

  template<typename _CharT, typename _Traits>
    /*constexpr*/ bool
    operator>=(__detail::__idt<basic_string_view<_CharT, _Traits>> __x,
	       basic_string_view<_CharT, _Traits> __y) noexcept
    { return __x.compare(__y) >= 0; }

  // basic_string_view typedef names

  using string_view = basic_string_view<char>;
} /* namespace gdb */

#include "gdb_string_view.tcc"

#endif // __cplusplus < 201703L

namespace gdb {

static inline std::string
to_string(const gdb::string_view &view)
{
  return { view.data (), view.size () };
}

}

#endif /* COMMON_GDB_STRING_VIEW_H */
