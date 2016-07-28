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
#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <deque>
#include <iostream>
#include <limits>
#include <typeinfo>

#include <simul.hpp>
#include <basestat.hpp>
#include <particle.hpp>
#include <plist.hpp>
#include <trace.hpp>

namespace MetaSim {

#define _EVENT_DBG_LEV "Event"

    // Forward declaration
    class Entity;

    /** 
        The basic event class. It models an event in the
        simulator: contains all the basic methods for handling an
        event.

        To define a new "type" of events in your system model, you
        need to derive a class from this, overriding the virtual
        doit() method.

        This class also inclides a static qevent queue, where all
        "active" events are enqueued. To insert an event in the
        queue, you can call the post() method specyfing a
        triggering time. Events are ordered in the queue by
        triggering time. In case of two events with the same
        triggering time, events are ordered by priority. In fact,
        it is also possible to specify a priority for every event
        object. The priority is for the object, and not for the
        class!

        When an event is "triggered" in the simulation, its doit()
        method is invoked. In most of the cases, the doit() method
        simply calls a method of an entity, which will be
        informally called "handler" of the event.

        In case the doit() method \b only calls the appropriate
        event handler, you can use the template class GEvent<X>
        instead of deriving a new class.

        An event can have a list of statistical objects. These are
        objects that are used to collect statistics on the
        system. See BaseStat for more details.

        Finally, and event has a list of tracing objects. These
        are used to trace the evolution of the system for
        analysing it later. See Trace class for more details.

        \sa GEvent<X>
        
        \ingroup metasim_ee

    */
    class Event {

    public:
        /** 
            \ingroup metasim_exc
	
            Exceptions for the Event Class. 
        */
        class Exc : public BaseExc {
        public:
            Exc(const std::string message, 
                const std::string cl = "Event", 
                const std::string md = "event.cpp") 
                : BaseExc(message,cl,md) {} ;
        };
  
    private:
        /**
           Template specialization for less function
           object. It is used to order the event in the event
           queue. Events are ordered by triggering time, and
           in case of tie, by priority. In case of another
           tie, event objects are ordered by pointer
           value. This is because the implementation of the
           priority queue does not allows ties. If you try to
           insert an event object twice, you get an exception!
        */
        class Cmp {
        public:
            bool operator() (Event* e1, Event* e2) const;
        };
                
        typedef priority_list<Event*, Cmp> EventQueue;
  
        /**
           Event queue. This is the global event queue, used
           by the simulation engine.
        */ 
        static EventQueue _eventQueue;

        /**
           counter for fifo insertion
        */
        static long counter;

        /**
           number of fifo insertion
        */
        unsigned long _order; 
  
        /// Tells if the element is in the event queue;
        bool _isInQueue;
  
        /// A queue of all the statistical object. All these
        /// objects will be "invoked" after the event handler
        /// (doit()) has been processed.  @todo will be
        /// removed, eventually
        std::deque<BaseStat *> _stats;

        /// NEW
        std::deque<ParticleInterface *> _particles;

        /// A queue of object which manage the tracing
        /// process. All these objects will be "invoked" after
        /// the event handler (doit()) has been processed.
        std::deque<Trace *> _traces;

        /// Triggering time of the event.
        Tick _time;
  
        /// This is the last time the event was
        /// triggered. Used for the purpose of collecting
        /// traces and statistics.
        Tick _lastTime;

        /** 
            Event priority. This is used to give an order to
            events with the same time, in the event queue. We
            define _DEFAULT_PRIORITY as the default priority
            for an Event. Each subclass can define its own
            default priority. The priority is set by the
            constructor.
        */
        int _priority;

        int _std_priority;
	
        /// We hide operator= to avoid improper use.
        Event& operator=(Event &);

    protected:
        /// Indicates if the event has to be destroyed after
        /// bein processed. Normally, this flag is set to
        /// false.
        bool _disposable;

        /// Checks that the event is not queued, and set the
        /// _time field;
        void setTime(Tick actTime) throw(Exc);

        /** 
            Copy constructor. This is defined to allow dynamic
            event creation using another event as a
            prototype. Statistics and traces are copied.
        */
        Event(const Event &e);

    public:
        /**
           The default priority for an event is 8. The lower
           this number the higher the priority.  */
        static const int _DEFAULT_PRIORITY = 8;
        static const int _IMMEDIATE_PRIORITY = 0;

        /** 
            Contructor.
     
            @param p Event priority
        */
        Event(int p = _DEFAULT_PRIORITY);
  
        /// Destructor.
        virtual ~Event();

        /** 
            Inserts the event into the event queue. If the
            event is already in the event queue, an exception
            is raised. Then, the event will be processed at
            time myTime. It is not possible to post an event
            in the past, but is possible to post an event in
            the present: that is, myTime must be greater than
            or equal to the current simulation time, otherwise
            an exception will be raised. If the event is
            marked disposable, the main simulation loop will
            delete it after it has been processed. In
            practice, by setting disp to true, we are giving
            ownership of the object to the Simulation engine,
            which will destroy the object after using it.

            Warning!!  Never set disp = true for a statically
            declared event (i.e., an event that was not
            created with new) unless you want a good old core
            dump.
     
            @param myTime triggering time for the event.
            @param disp set it to true if the event object
            must be disposed.
        */
        void post(Tick myTime, bool disp = false) throw(Exc, BaseExc);

        /**
           Processes the event immediately. 
        */
        void process(bool disp=false);

        /** 
            Drop the event from the event queue. The event is
            simply extracted from the queue, and hence will
            not be processed, but it is not destroyed.
        */
        void drop();

        /** 
            Returns the first event in the event queue.  This
            function is used by the main simulation engine and
            should never be called directly by any other
            object. The event is not extracted from the queue
        */
        static inline Event *getFirst() {
            if (_eventQueue.empty()) 
                return NULL;
            else
                return _eventQueue.front();
        }

        /** 
            Returns the event priority.  It is a identifier
            for the event priority. In the old version, events
            were distinguished by their class. For simplicity
            we decided to introduce this identifier.
        */
        inline int getPriority() const {return _priority;};

        /** 
            Set the event priority.  It is a identifier for
            the event priority. The lower the number, the
            higher the priority.
        */
        inline void setPriority(int p) { _priority = p;};

        /** 
            Restore the standard priority (the one defined in
            the constructor).
        */
        inline void restorePriority() { _priority = _std_priority;};

        /** 
            Returns the event time. Warning: if you try to get
            this field after the event has been triggered, the
            field could be inconsistent. Use getLastTime
            instead.
     
            @see getLastTime.  
        */
        inline Tick getTime() const {return _time;};

        /** 
            Return the last time in which event was triggered.
            Basically it always is the same as
            getTime(). However, consider that, if the event is
            re-posted as a consequence of the eventHandler()
            processing, if the statistical probe use
            getTime(), it could read the new posting time!
            This happens because statistical probes are
            processed AFTER the eventHandler!!
            <i>lastTime</i> is set in the action() routine to
            the current time of the event; the eventHandler
            may repost the event altering the time field, but
            the user may access the actual time in which event
            has been triggered with getLastTime().
     
            @see getTime */
        inline Tick getLastTime() const { return _lastTime; };

        /** 
            Returns the value of the disposable flag Indicates
            if the event has to be destroyed after bein
            processed.  In fact, if an event has been created
            dinamically, must be destroyed only after it has
            been processed. This is done by the Simulation
            class, only if the following flas is true.
     
            @see post */
        inline bool isDisposable() {return _disposable;};  


        inline bool isInQueue() { return _isInQueue; }

        /** 
            Add a new stat probe to this event. All the
            statistical objects that are related to this event
            will be invoked when the event is triggered, but
            remember, AFTER the event is processed!
	
            @todo deprecated, will be removed, and substituted
            by a different kind of mechanisms.
        */
        inline void addStat(BaseStat *actStat) { 
            _stats.push_back(actStat);
        }

        /** 
            Add a new particle to this event.  This is the new
            way to add statistics and traces to this object.
        */
        void addParticle(ParticleInterface *s);

        /** 
            Add a new trace probe to this event. It is useful
            for defining different kinds of tracing all at the
            same time.  */
        inline void addTrace(Trace *t) { _traces.push_back(t); }  

        /** 
            This method is called when the event is triggered.
            It contains part of the basic code of the
            simulation engine.  For this reason, it should be
            overloaded only in very, very, special cases!! Do
            not touch if you don't know what you are doing.
            Also, this method should never be invoked by any
            entity.
     
            It can throw any type of exceptions, in principle,
            so I can't specify any particular type of
            exception in the interface
        */
        void action();

        /**
           Must be defined for every derived class. This
           method is called from the action() method.  */
        virtual void doit() = 0; 

        /**
           for debugging.
        */
        virtual void print();

        /** 
            for debugging
        */
        static void printQueue() {
            EventQueue::iterator it;
    
            for (it = _eventQueue.begin(); 
                 it != _eventQueue.end(); 
                 it++) 
                (*it)->print();
        }

    };

}

#endif
