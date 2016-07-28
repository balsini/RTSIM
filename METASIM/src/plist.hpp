/***************************************************************************
    begin                : Thu Apr 24 15:54:58 CEST 2003
    copyright            : (C) 2003 by Giuseppe Lipari
    email                : lipari@sssup.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 * $Id: plist.hpp,v 1.6 2005/04/28 01:34:47 cesare Exp $
 *
 * $Log: plist.hpp,v $
 * Revision 1.6  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.5  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 */
#ifndef __PLIST_HPP__
#define __PLIST_HPP__

#include <set>

#include <baseexc.hpp>

/**
   \ingroup metasim_util

   Priority List built according to STL framework. 
*/
#ifndef __STL_LIMITED_DEFAULT_TEMPLATES
template <class T, 
	  class Compare = std::less<T>,
	  class Alloc = std::allocator<T> >
#else
template <class T, class Compare, class Alloc = alloc>
#endif
class priority_list : private std::set<T, Compare, Alloc> {
  typedef std::set<T, Compare, Alloc> Impl;
public:
  typedef typename Impl::iterator iterator;
  typedef typename Impl::const_iterator const_iterator;
  typedef typename Impl::pointer pointer;
  typedef typename Impl::const_pointer const_pointer;
  typedef typename Impl::reference reference;
  typedef typename Impl::const_reference const_reference;
  typedef typename Impl::reverse_iterator reverse_iterator;
  typedef typename Impl::const_reverse_iterator const_reverse_iterator;
  typedef typename Impl::size_type size_type;
  typedef typename Impl::difference_type difference_type;

  iterator begin() { return Impl::begin(); }
  iterator end() { return Impl::end(); }
  const_iterator begin() const { return Impl::begin(); }
  const_iterator end() const { return Impl::end(); }
  reverse_iterator rbegin() const { return Impl::rbegin(); }
  reverse_iterator rend() const { return Impl::rend(); }

  std::pair<iterator,bool> insert(const_reference x) 
  {
    // The underlying set container will signal setting the bool flag
    // that the element has already been inserted; we throw an exception
    // in this case!
    // Gerardo: I don't think that throwing an exception is
    // a good idea here... I prefer to rely on the STL
    // standard behavior
    return Impl::insert(x);
  };
  
  void erase(const_reference x) { Impl::erase(x); }
  // Peppe: does not compile on gcc 3.3.3 on cygwin 
  // reference front() { return *begin(); } 
  const_reference front() const { return *begin(); }
  reference back() { return *(--end()); }
  const_reference back() const { return *(--end()); }
  bool empty() {return Impl::empty(); }
  void clear() { Impl::clear(); }
  size_type size() const { return Impl::size(); }
};

#endif // __PLIST_HPP__
