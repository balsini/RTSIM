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
#ifndef __BASESTAT_HPP__
#define __BASESTAT_HPP__

#include <fstream>
#include <limits>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

#include <basetype.hpp>

namespace MetaSim {

    class Event;
    class Entity;

    /**
       \ingroup metasim

       @{
    */
    /**
       \defgroup metasim_stat Statistical classes

       This file contains the basic classes for implementing statistical
       processing.

       The class tree is the following:
       <pre>
       # lev 0             BaseStat                      
       #                /     |       \                  
       #               /      |        \                 
       #              /       |         \                
       # lev 1     StatMax   StatMin   StatMean ...       
       #           /   \                                  
       #          /     \                                
       @{
       # lev 2 StatTask1 StatTask2  ...     ...           
       #
       </pre>      
       Level 0 (BaseStat) cannot be changed: it implements the functions
       for doing statistic: it is initialized with the number of
       experiments to be done, and has an array where the data are
       recorded at the end of the simulation. It has two pure virtual
       function, probe() and record(), so no object of this class can be
       instanciated. It has some function to get the final stats (like
       getMean and getConfInterval).

       Level 1 (StatMax ...) implements the function for collecting
       statistic; for example, the StatMax class record the maximum
       value during an experiment. The user can add his own classes to
       this level, and he must implement the record() function and the
       initValue() function.
   
       Level 2 implements the probe() for a single event. The user must
       write entirely this level, depending on the variable he needs to
       measure. When implementing a class of this level, the user must
       write the probe function, which has to call the record() function
       with the appropriate value.  For example, if we want to measure
       the maximum release time of task1 the we need to write:
       <pre>
       #   class StatFT_Task1 : public StatMax {
       #       ...
       #       virtual void probe() {
       #           record(task1.release);
       #       };
       #       ...
       #   };
       </pre>
       @{
    */
    ///  The basic statistical class. 
    class BaseStat {
    public:
        typedef std::list<BaseStat*> List;

    private:
        static const int MAX_DISTR = 10000;
        static double _TDistr[MAX_DISTR];
        static size_t _totalNumOfExp;
        static size_t _expNum;
        static bool _endOfSim;
        static List _statList;

    protected:

        /**
           \ingroup metasim_exc

           The exceptions raised from the statistical classes belong to this
           class.
        */
        class Exc : public BaseExc {
        public:
            Exc(const std::string s = "Unknown Exc", 
                const std::string c = "BaseStat",
                const std::string m = "basestat.hpp") : 
                BaseExc(s, c, m) {}
        };
    
        /// name of the stat object
        std::string _name;

        /// current computed value (during the run).
        double _val;

        /// array of values, one for each run.
        typedef std::vector<double> Experiments;
        Experiments _exper;

        /** called at the end of the run, puts the current 
            value in the array of experiments. */
        inline void collect() {
            if (_exper.size() <= _expNum) _exper.push_back(_val);
            else _exper[_expNum] = _val;
        }

        // System-Wide data & functions needed to be visible 
        // also to other kind of stats!
        /// lenght of the transitory.
        static Tick _transitory;

        /// used to see if the base stat class has been
        /// initialized. (???) 
        static bool _initFlag;

        /// t-student function
        static double get_t_perc(double alpha);

        /// computes the t-student with parameter n.
        static void calc_t(int n);

        /// returns the t-student with parameter alfa and dol.
        static double t_student(int alfa, int dol);

    public:
        /// Constructors: enqueues the object in the global stat list
        BaseStat(std::string n = "");
        virtual ~BaseStat();
  
        typedef List::const_iterator iterator;
        static inline iterator begin() { return _statList.begin(); }
        static inline iterator end() { return _statList.end(); }

        /** 
            Level 1 function: it is called by the probe() (level 2) 
        */
        virtual void record(double) = 0;
        virtual void initValue() = 0;
  
        /** 
            level 2 function: called by the event action() method. 

            DEPRECATED.

            @todo remove this function. Not very OO, because it takes a generic
            event, and most of the times we must do a dynamic_cast<>. 
            By now it is deprecated.
            Must be substituted by non-virtual probe methods that take the 
            typed event object as parameters (see particle.hpp).
        */
        virtual void probe(Event *e) {};

        /** 
            This function links the statistical probe to the correct events
            associated with a given entity.

            DEPRECATED.
	
            @todo remove this function. Not very OO, because it forces to use
            dynamic casts. By now it is deprecated. Substituted by 
            addParticle() (see particle.hpp).
        */
        virtual void attach(Entity *e) {};

        /*--------------------------------------------*/
    
        /**
           Returns the name of this stat.
        */
        inline std::string getName() { return _name; }

        /**
           Returns the current value of the stat object.
        */
        inline double getValue() { return _val; }

        /**
           Returns the data collected in the last run
        */
        inline double getLastValue() {
            if (_expNum > 0) 
                return _exper[_expNum-1]; 
            else return 0;
        }

        /// Returns the mean value
        double getMean();

        /// Returns the variance
        double getVariance();

        /// possible values for the confidence intervals
        enum CONFIDENCE_INTERVAL {
            C90 = 90,
            C95 = 95
        };

        /** Returns the 90% or 95% confidence interval 
            because of the underlying implementation 
            of the t_student() function, which has
            been pre-calculated only for 90% and 95%
            values
	
            @param c  can be C90 or C95 */
        double getConfInterval(CONFIDENCE_INTERVAL c = C95);
	
        /*--------------------------------------------*/

        // debug!!
        inline size_t getExpNum() { return _expNum; }
        static void printAll();	
        void print();

        /** 
            Must be called before the first run. 
            @param n number of runs.
        */ 
        static void init(size_t n);
        void init();

        /// automatically called at the end of the run, 
        /// collects all stats.
        static void endRun();

        /// automatically called at the beginning of the run, 
        /// prepare the int values
        static void newRun();

        /// automatically called at the end of the sim, 
        /// write the files.
        static void endSim();

        /// specify how long the transitory will be
        /// data collected during transitory is discarded
        static void setTransitory(Tick t);
    
        /// check if we are currently inside the transitory
        static bool chkTransitory();
    };

    /* ---------------------------------------------------------
       t_distr[]    : t-student distr.;
       num		: total number of experiments;
       expnum	: actual experiment number;
       head		: pointer to a global list of stat objects;
       globNext	: pointer to the following object in the global list;

       *next        : pointer to next BaseStat Object in the list of the event;
       *exper	: array of values for each experiment (dinamic alloc.);

       get_t_perc() : get the percentile corresponding to alpha (0 < alpha < 1);

       val		: value
       init()	: initialize t_distr[];
       endRun()     : to be called after the end of the experiment
       newRun()	: to be called before doing a new experiment;
       endSim()     : to be called at the end of simulation;
       BaseStat()	: Base constructor;

       probe()	: called by the event to which the stat object is linked;
       record()	: called by the probe() to record the datum/a;
       initValue() : is called by newRun() to initialize the values;

       getNext()	: get the next stat object.
       getHead()    : get the first first stat object
       getMean()	: get the mean value (over all the experiments)
       getConfInterval() : get the dim of the conf. interval relative to
       a (0 < a < 1). Typically, a = .9;
		    
    */

    /** @name Some typical statistic classes (level 1) */
    //@{
    /// Computes the max value 
    class StatMax : public BaseStat {
        double _ini;
    public:
        StatMax(std::string name = "", 
                double i = std::numeric_limits<double>::min()) : 
            BaseStat(name), _ini(i)
            {
            }

        virtual void record(double a) 
            { 
                if (chkTransitory()) return;
                _val = std::max(_val, a);
            }
        virtual void initValue() { _val = _ini; }
    };

    /// Computes the min value
    class StatMin : public BaseStat {
        double _ini;
    public:
        StatMin(std::string name = "", 
                double i = std::numeric_limits<double>::max()) : 
            BaseStat(name), _ini(i)
            {
            }

        virtual void record(double a) 
            { 
                if (chkTransitory()) return;
                _val = std::min(_val, a);
            }
        virtual void initValue() { _val = _ini; }
    };

    /// Computes a mean value X_m = (Sigma{X_i}i=1,N)/N
    class StatMean : public BaseStat {
    protected:
        double _ini;
        double _count;
    public:
        StatMean(std::string name = "", 
                 double i = 0) : 
            BaseStat(name), _ini(i), _count(0) 
            {
            }

        virtual void record(double a)
            {
                if (chkTransitory()) return;
                _val = _val * _count + a;
                _val /= ++_count;
            };
        virtual void initValue() { _val = _ini; _count = 0; };
    };

    /// Computes the quadratic mean value 
    /// X2_m = (Sigma{X_i^2}i=1,N)/(N*(N-1))
    class StatSqrMean : public BaseStat {
    protected:
        double _ini;
        double _count;
    public:
        StatSqrMean(std::string name = "", 
                    double i = 0) : 
            BaseStat(name), _ini(i), _count(0) 
            {
            }

        virtual void initValue() { _val = _ini; _count = 0; };
    };


    /// Counts the number of occurrences of an event.
    /**
       This class is used to simply count the number of occurrences of an
       event.
    */
    class StatCount : public BaseStat {
    protected:
        double _ini;
    public:
        StatCount(std::string name = "", 
                  double i = 0) : 
            BaseStat(name), _ini(i) 
            { 
            }

        virtual void record(double increment)
            {
                if (chkTransitory()) return;
                _val += increment;
            }
  
        virtual void initValue() { _val = _ini; }
    };


    /// Computes the percentage of occurrences of an event
    /** 
     *  Computes the percentage of times the record is called with a value
     *  greater than 0. For example, if record is called as: record(1),
     *  record(1), record(0), the value is 2/3.
     */
    class StatPercent : public BaseStat {
    protected:
        double _ini;
        double _num, _den;
    public:
        StatPercent(std::string name = "", 
                    double i = 0) : 
            BaseStat(name), _ini(i), _num(i),
            _den(std::max(1.0, i))
            { 
            }
    
        virtual void record(double value)
            {
                if (chkTransitory()) return;
                _den += 1;
                if (value > 0.0) _num += 1;
                _val = _num / _den;
            }
        virtual void initValue() 
            { 
                _val = _ini;
                _num = _ini;
                _den = std::max(1.0,_ini);
            }
        int getNumSamples() 
            {
                return _den;
            }
    };


    /// Produces output in gnuplot format
    /**
       Output for gnuplot. This class open a file for each statistical object   
       and on each file write:

       <ul>
       <li> the parameters of the simulation (passed to output())
       <li> the mean 
       <li> the confidence interval at .9
       </ul>

       Just call

       <pre> GnuPlotOutput::init(); </pre> 

       before the simulation and call

       <pre> GnuPlotOutput::write("%f",par); </pre>

       after each simulation. The format string is the same as printf.
       There can be more than one parameter, and of different types!
    */
    class GnuPlotOutput {    
    public :
        class Exc : public BaseExc {
        public:
            Exc(const std::string s, 
                const std::string c = "GnuPlotOutput",
                const std::string m = "basestat.hpp") : 
                BaseExc(s, c, m) {}
        };
        static void init();
        //static void write(char *fmt ...);    

        template <typename T>
        static void write(const T& t)
            {
                BaseStat::iterator i  = BaseStat::begin();

                while (i != BaseStat::end()) {
                    BaseStat* p = *i;
                    if (p->getName() != "") {
                        std::ofstream f(p->getName().c_str(),
                                        std::ios::app);
                        if (!f.is_open())
                            throw Exc("Cannot open file " + p->getName());	
                        f << t << '\t' << p->getMean() 
                          << '\t' << p->getConfInterval()
                          << endl;
                    }
                    ++i;
                }
            }

    };

    /**
       The output is formatted as a table.
    */
    class TableOutput {
        static bool _created;
        static std::string _fname;
    public:
        void init(std::string filename);
        void write(std::string message);
    };
    /*@}*/ 
    /*@}*/ 
} 

#endif
