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
 * $Id: factory.hpp,v 1.3 2005/04/28 01:34:47 cesare Exp $
 *
 * $Log: factory.hpp,v $
 * Revision 1.3  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.2  2004/11/22 00:35:24  cesare
 * *** empty log message ***
 *
 * Revision 1.2  2003/04/24 14:55:53  lipari
 * *** empty log message ***
 * 
 */
#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef std::string defaultIDKeyType;

// The abstract factory itself.
// Implemented using the Singleton pattern
template <class manufacturedObj, typename classIDKey=defaultIDKeyType>
class genericFactory 
{
    // a BASE_CREATE_FN is a function that takes no parameters
    // and returns an auto_ptr to a manufactuedObj.  Note that
    // we use no parameters, but you could add them
    // easily enough to allow overloaded ctors, e.g.:
    //   typedef std::auto_ptr<manufacturedObj> (*BASE_CREATE_FN)(int);
    typedef std::auto_ptr<manufacturedObj> (*BASE_CREATE_FN)(std::vector<std::string> &par);

    // FN_REGISTRY is the registry of all the BASE_CREATE_FN
    // pointers registered.  Functions are registered using the
    // regCreateFn member function (see below).
    typedef std::map<classIDKey, BASE_CREATE_FN> FN_REGISTRY;
    FN_REGISTRY registry;

    // Singleton implementation - private ctor & copying, with
    // no implementation on the copying.
    genericFactory();
    genericFactory(const genericFactory&); // Not implemented
    genericFactory &operator=(const genericFactory&); // Not implemented
    public:

    // Singleton access.
    static genericFactory &instance();

    // Classes derived from manufacturedObj call this function once
    // per program to register the class ID key, and a pointer to
    // the function that creates the class.
    void regCreateFn(const classIDKey &, BASE_CREATE_FN);

    // Create a new class of the type specified by className.
    std::auto_ptr<manufacturedObj> create(const classIDKey &className, std::vector<std::string> &parms) const;
};

////////////////////////////////////////////////////////////////////////
// Implementation details.  If no comments appear, then I presume
// the implementation is self-explanatory.

template <class manufacturedObj, typename classIDKey>
genericFactory<manufacturedObj, classIDKey>::genericFactory()
{
}

template <class manufacturedObj, typename classIDKey>
genericFactory<manufacturedObj, classIDKey> &genericFactory<manufacturedObj, classIDKey>::instance()
{
  // Note that this is not thread-safe!
  static genericFactory theInstance;
  return theInstance;
}

// Register the creation function.  This simply associates the classIDKey
// with the function used to create the class.  The return value is a dummy
// value, which is used to allow static initialization of the registry.
// See example implementations in base.cpp and derived.cpp
template <class manufacturedObj, typename classIDKey>
void genericFactory<manufacturedObj, classIDKey>::regCreateFn(const classIDKey &clName, BASE_CREATE_FN func)
{
  registry[clName]=func;
}

// The create function simple looks up the class ID, and if it's in the list,
// the statement "(*i).second();" calls the function.
template <class manufacturedObj, typename classIDKey>
std::auto_ptr<manufacturedObj> genericFactory<manufacturedObj, classIDKey>::create(const classIDKey &className, std::vector<std::string> &parms) const
{
  std::auto_ptr<manufacturedObj> ret(0);
  
  typename FN_REGISTRY::const_iterator regEntry=registry.find(className);
  if (regEntry != registry.end()) {
    return (*regEntry).second(parms);
  }
  return ret;
}

// Helper template to make registration painless and simple.
template <class ancestorType,
          class manufacturedObj,
          typename classIDKey=defaultIDKeyType>
class registerInFactory {
  public:
  static std::auto_ptr<ancestorType> createInstance(std::vector<std::string> &par)
  {
    return std::auto_ptr<ancestorType>(manufacturedObj::createInstance(par));
  }
  registerInFactory(const classIDKey &id)
  {
    genericFactory<ancestorType>::instance().regCreateFn(id, createInstance);
  }
};

#define FACT(xxx) genericFactory<xxx>::instance()


#endif
