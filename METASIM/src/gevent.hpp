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
 * $Id: gevent.hpp,v 1.4 2004/11/24 14:12:13 lipari Exp $
 *
 * $Log: gevent.hpp,v $
 * Revision 1.4  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * 
 */
#ifndef __GEVENT_HPP__
#define __GEVENT_HPP__

#include <event.hpp>

namespace MetaSim {
    /**
       \ingroup metasim_ee

       This template class is used for simplifying the writing of a
       system model. Most of the times, the doit() method of an event
       must only call a handler method of some class. Normally, you must
       write a new class derived from event and override the virtual
       doit() method. However, it is a lot of repetitive work! And when
       there is a lot of repetitive work to do, template is the
       solution!

       The GEvent class takes a class derived from Entity as a template
       argument. Then, template function register_handler() must be
       called passing the pointer to the handler before using this
       event.

       An example of usage is found in example/eth/netinterface.hpp:

       \dontinclude netinterface.hpp
       \skipline class EthernetInterface
       \until EthernetInterface(

       The _trans_evt event object is of type GEvent<X>: there is not
       need of defining a special class derived from event. The
       constructor for EthernetInterface must invoke the
       register_handler() function:

       \dontinclude netinterface.cpp
       \skipline EthernetInterface::EthernetInterface
       \until register_handler

       Handler onTransmit() is called whenever the event is triggered.
    */
    template<class X>
    class GEvent : public Event {
	typedef void (X::* Pmemfun)(Event *);

	X *_obj;
	Pmemfun _fun;

    public:
	/**
	   Copy constructor. Useful when creating many events of the same
	   type for the same \b object. (Remember: all these events point
	   to the same object!)
	*/
	GEvent(GEvent<X> &e) : Event(e) { 
	    _obj = e._obj; _fun = e._fun; 
	}

	GEvent(X *obj, typename GEvent<X>::Pmemfun fun, int p = Event::_DEFAULT_PRIORITY) :
	    Event(p), _obj(obj), _fun(fun) {}

	/**
	   Simply calls the specified handler on the specified object.

	   \sa register_handler
	*/
	virtual void doit() { 
	    if ((_obj != NULL) && (_fun != NULL))
		(_obj->*_fun)(this);
	}
    };

    /**
       \ingroup metasim_ee

       This function is used to specify that a certain event of type
       GEvent<X> must refer to object obj and call its method fun.
     
       \param evt the event object
       \param obj the entity obj 
       \param fun the method of obj to be called
       \sa GEvent#doit
    */
    // template<class X>
    // void register_handler(GEvent<X> &evt, X *obj,
    // 			  typename GEvent<X>::Pmemfun fun) 
    // { 
    // 	evt._obj = obj; evt._fun = fun; 
    // }
}  

#endif
