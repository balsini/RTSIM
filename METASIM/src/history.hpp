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
 * $Id: history.hpp,v 1.4 2005/04/28 01:34:47 cesare Exp $
 *
 * $Log: history.hpp,v $
 * Revision 1.4  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.3  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * Revision 1.2.2.2  2004/10/20 12:38:34  lamastra
 * Fixed f(void) -> f() (C++ standard compliancy issues)
 *
 * Revision 1.2  2003/04/24 14:55:54  lipari
 * *** empty log message ***
 * 
 */
#ifndef __HISTORY_HPP__
#define __HISTORY_HPP__

#include <vector>

/** 
 * History List built according to STL framework. The history<> class
 * can use deque<> and list<> as underlying container; it is coded
 * after the queue adapter, and it relys on the push_back(),
 * pop_front() interface; so any container implementing this can be
 * used in this way. */
template <class T>
class history {
public:
  typedef typename std::vector<T>::iterator iterator;
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::pointer pointer;
  typedef typename std::vector<T>::const_pointer const_pointer;
  typedef typename std::vector<T>::reference reference;
  typedef typename std::vector<T>::const_reference const_reference;
  typedef typename std::vector<T>::reverse_iterator reverse_iterator;
  typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;
  typedef typename std::vector<T>::size_type size_type;
  typedef typename std::vector<T>::difference_type difference_type;

  const_iterator begin() const { return c.begin(); }
  const_iterator end() const { return c.end(); }
  iterator begin() { return c.begin(); }
  iterator end() { return c.end(); }

  const_reverse_iterator rbegin() const { return c.rbegin(); }
  const_reverse_iterator rend() const { return c.rend(); }
  reverse_iterator rbegin() { return c.rbegin(); }
  reverse_iterator rend() { return c.rend(); }

private:
  std::vector<T> c;
  size_type _size;
  int curr_pos;

public:
  history(size_type s) : 
    c(s), _size(s), curr_pos(0) {}
  history(size_type s, const T& v) : 
    c(s, v), _size(s), curr_pos(0) {}
  history(const history<T>& h) : 
    c(h.c), _size(h._size), curr_pos(h.curr_pos) {}
	
  size_type size() const { return _size; }
  void push_back(const T& v) 
  { 
    c[curr_pos] = v; curr_pos = (curr_pos + 1) % _size;
  }
  
  void push(const T& v)
  { 
    c[curr_pos] = v; curr_pos = (curr_pos + 1) % _size;
  }
	
  void push_back(T& v) 
  { 
    c[curr_pos] = v; curr_pos = (curr_pos + 1) % _size;
  }
  
  void push(T& v)
  { 
    c[curr_pos] = v; curr_pos = (curr_pos + 1) % _size;
  }

  T operator[](int i) 
  { 
    int pos = curr_pos - 1 - i; if (pos < 0) pos += _size; return c[pos];
  }

  template<class In>
  history(In first, In last) : c(first,last)
  {
    _size = c.size();
    curr_pos = 0;
  }
};

#endif // __HISTORY_HPP__

#ifdef __TEST__
#include <iostream>

using namespace std;

template <class C>
void print(C& c)
{
  typedef typename C::iterator CI;
  CI i;
  cout << '[';
  for (i = c.begin(); i != c.end(); ++i) {
    cout << *i << " ";
  }
  cout << ']' << endl;
}

int main()
{
  history<int> h1(10);

  for (register int i = 0; i < 10; i++) {
    h1.push(i);
  }
  print(h1);
  cout << "Front=" << h1.front() << endl;
  cout << "Back=" << h1.back() << endl;
  for (register int i = 10; i < 20; i++) {
    h1.push(i);
  }
  print(h1);
  cout << "Front=" << h1.front() << endl;
  cout << "Back=" << h1.back() << endl;
}

#endif // __TEST__
