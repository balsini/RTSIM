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
#ifndef __SIMUL_HPP__
#define __SIMUL_HPP__

#include <basestat.hpp>
#include <debugstream.hpp>
#include <entity.hpp>
#include <event.hpp>

namespace MetaSim {

#define _SIMUL_DBG_LEV "Simul"
  
    /** 
        \ingroup metasim_ee
  
        This singleton implements the simulation engine and some
        debugging facilities. The main function is <i>run(Tick
        lenght, size_t runs)</i> that is responsible for running
        the simulation for one or more times.
   
        The <i>getTime()</i> returns the current globalTime in the
        simulation; the dbg object is used for debugging output.
 
        @author Giuseppe Lipari, Gerardo Lamastra
    */
    //@{
    class Simulation {
        Simulation();
        Simulation(const Simulation &);

        static Simulation *instance_;
    public:
        static Simulation &getInstance();
               
        /**
           Enters the <i>lev</i> debug level.
     
           This function specifies that we are entering in the
           <i>lev</i> debug level and outputs the current
           simulation time follewed by the <i>header</i>
           string.
      
           @param lev A string that specifies the current
           debugging level.  
                   
           @param header This string that can be used for
           context sensitive information. For example, for
           printing the name of the function we are entering
           in.
     
           @see DebugStream
        */
        void dbgEnter(std::string lev, std::string header);
               

        /**
           Exits from the current debug level.
     
           This function specifies that we are exiting the
           current debug level.
     
           @see DebugStream
        */
        void dbgExit();

        /**
           This function is the main simulator engine. After defining all
           the objects in a simulation, this function should be invoked
           for running the simulation.

           @code 
           // create entities ane events, link them to each other
           // create statistics and traces, attach them to the events

           Simulation::run(10000, 10);   // run simulation 10 times, each one for 10000 ticks

           // collect and save statistics on files
           @endcode

           At the beginning and at the end of each run, initialization and
           and finalization are called, respectively, namely the
           Entity::newRun() and Entity::endRun(). The random seed is not
           initialized at every run, but it is left as it is.

           Of course, it makes sense to run multiple replicas of the same
           simulation only if the simulation itself includes random
           variables, otherwise the result will always be the same. See
           the statistical section for more details.
     
           @param length Length of each simulation run.
           @param runs Number of replicas.
        */
        void run(Tick length, int runs = 1);

        /**
           Returns the current simulation time.
        */
        const Tick getTime();
                                
        /**
           Drops and eventually deletes all events in the queue. To be
           called after an exception!
        */
        void clearEventQueue();
                
        void print();

        /**
           Function to help testing and debugging.

           This function performs one single simulation
           step. It returns the tick after the simulation step
           has been completed.

           Normally, you should not call this function. This
           is used only for debugging purposes.
        */
        const Tick sim_step();

                
        /**
           Function to help testing and debugging.

           Initializes the simulation runs. To be called just once 
           before every simulation run. 

           Normally, this function is called from the Simulation::run().
           It is exposed here as an interface for debugging.
        */
        void initRuns(int nRuns = 1);

        /**
           Function to help testing and debugging.

           Initializes a simulation run. To be called before the 
           start of every simulation run.

           Normally, this function is called from the Simulation::run().
           It is exposed here as an interface for debugging.

           An example of debug call sequence is the following:
           @code

           // (simulation objects creation and initialization)
           initRuns();
           initSingleRun();
           run_to(100);     // run up to tick 100
           // check object status
           sim_step();      // run one step
           // check object status
           // ...
           @endcode
         
        */
        void initSingleRun();


        /**
           Function to help testing and debugging.
        */
        void endSingleRun();
                
        /**
           Function to help testing and debugging.

           This function will run until a specified time, 
           without cleaning any variable. 
           It returns the final tick
           it stops before executing the first event after stop.
        */
        const Tick run_to(const Tick &stop);

        DebugStream dbg;
               
    private:
                
        /**
           Sets the current simulation time.
        */
        void setTime(Tick);

        void endSim();

        const Tick getNextEventTime();
                
        size_t numRuns;
        size_t actRuns;
        Tick globTime;
        bool end;
    };

    class DbgObj {
    public:
        DbgObj(const std::string &x, const string &y) {
            Simulation::getInstance().dbgEnter(x,y);
        }
        ~DbgObj() {
            Simulation::getInstance().dbgExit();
        }
    };

}


extern "C" {
    void libmetasim_is_present();
}

#define SIMUL         Simulation::getInstance()


#ifdef __DEBUG__

#define DBGENTER(x) DbgObj __dbg_obj__(x,__PRETTY_FUNCTION__)

#define DBGTAG(x,y)   do { SIMUL.dbgEnter(x,y);       \
                           SIMUL.dbgExit();} while(0)

#define DBGFORCE(x)   do {\
                      SIMUL.dbg.enable("__FORCE__");  \
                      SIMUL.dbg.enter("__FORCE__");   \
                      SIMUL.dbg << x << endl;         \
                      SIMUL.dbg.exit();               \
                      SIMUL.dbg.disable("__FORCE__"); } while(0)

#define DBGPRINT(x)   SIMUL.dbg << x << endl
#define DBGPRINT_2(x,y) SIMUL.dbg << x << y << endl
#define DBGPRINT_3(x,y,z) SIMUL.dbg << x << y << z << endl
#define DBGPRINT_4(x,y,z,w) SIMUL.dbg << x << y << z << w << endl
#define DBGPRINT_5(x,y,z,w,r) SIMUL.dbg << x << y << z << w << r << endl
#define DBGPRINT_6(x,y,z,w,r,s) SIMUL.dbg << x << y << z << w << r << s << endl

#define DBGVAR(x) DBGPRINT_2("  --> " #x " = ", x)

template<class X>
void __print_elem__(const X &obj)
{
    MetaSim::SIMUL.dbg << "--> " <<  obj << MetaSim::endl;
}

#include <algorithm>

template<class T>
void __print_set__(T const &x) 
{
    std::for_each(x.begin(), x.end(), __print_elem__<typename T::value_type>);
}

#define DBGVECTOR(x) do {          \
        DBGPRINT("VECTOR: " #x);   \
        __print_set__(x);          \
    } while (0)

#endif

#ifndef __DEBUG__
#define DBGENTER(x) 
#define DBGTAG(x,y)
#define DBGFORCE(x)
#define DBGPRINT(x)   
#define DBGPRINT_2(x,y)
#define DBGPRINT_3(x,y,z)
#define DBGPRINT_4(x,y,z,w)
#define DBGPRINT_5(x,y,z,w,r)
#define DBGPRINT_6(x,y,z,w,r,s)

#define DBGVAR(x)

#define DBGVECTOR(x)

#endif



#endif
