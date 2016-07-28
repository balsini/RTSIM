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
#ifndef __RANDOMVAR_HPP__
#define __RANDOMVAR_HPP__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <baseexc.hpp>

#ifdef _MSC_VER
#pragma warning(disable: 4290)
#endif

namespace MetaSim {

    using namespace std;

    typedef long int RandNum;
    const int MAX_SEEDS = 1000;

#define _RANDOMVAR_DBG_LEV  "randomvar"

    /**
       \ingroup metasim

       @{
    */

    /** 
     *  \defgroup metasim_random Random Variables  
     *
     *  This classes are used to generate pseudo-random numbers according
     *  to a certain distribution. Every derived class should implement a
     *  given distribution. This class is abstract: a derived class must
     *  overload method get() to return a double. Future Work: extend the
     *  class to support different pseudo-casual number generators.
     *
     *  @author Giuseppe Lipari, Gerardo Lamastra, Antonino Casile.  
     *  @version 1.0 
     */
    //@{
    /** 
        The basic class for Random Number Generator. It is possible to
        derive from this class to implement a new generator. */
    class RandomGen {
        RandNum _seed;
        RandNum _xn;

        // constants used by the internal pseudo-causal number generator. 
        static const RandNum A;
        static const RandNum M;
        static const RandNum Q;	// M div A
        static const RandNum R;	// M mod A

    public:
        /**
           Creates a Random Generator with s as initial seed.
           See file include/seeds.h for a list of seeds.
        */
        RandomGen(RandNum s);

        /** Initialize the generator with seed s */
        void init(RandNum s);

        /** extract the next random number from the
            sequence */
        RandNum sample();

        /** Returns the current sequence number. */
        RandNum getCurrSeed() { return _xn; }

        /** return the constant M (the module of this random
            generator */
        RandNum getModule() { return M; }
    };

    /**
       The basic abstract class for random variables.
    */
    class RandomVar {
    protected:
        static RandNum _seed;
        static RandNum _xn;

        /// Default generator.
        static RandomGen _stdgen;

        /** Pointer to the current generator (used by the next
            RandomVar object to be created. */
        static RandomGen *_pstdgen;

        /** The current random generator (used by this
            object). By default, it is equal to _pstdgen */
        RandomGen *_gen;

    public:

        typedef string BASE_KEY_TYPE;

        /**
           \ingroup metasim_exc

           Exceptions for RandomVar.
        */
        class Exc : public BaseExc {
        public:
            static const char* const _FILEOPEN;
            static const char* const _FILECLOSE;
            static const char* const _WRONGPDF;

            Exc(string wh, string cl) : 
                BaseExc(wh, cl, "randomvar.hpp") 
                {
                }
        };

        class MaxException: public Exc
        {
        public:
            MaxException(string cl)
                :Exc("Maximum value cannot be computed for this variable type", cl) {}
            MaxException(string m, string cl) :Exc(m, cl) {}
            virtual ~MaxException() throw() {}
            virtual const char* what() const throw() 
                {return _what.c_str();}

        };

        /** Constructor for RandomVar. It takes as argument
            the random number generator. By default, this is
            equal to _pstdgen. It is possible to change the
            default generator with changeGenerator().
     
            @param g The random number generator. By default,
            this is NULL, which means that the default
            generator is _pstdgen */
        RandomVar(RandomGen *g = NULL);

        /**
           Copy constructor.
        */
        RandomVar(const RandomVar &r);

        virtual ~RandomVar();

        /// Initialize the standard generator with a given
        /// seed
        static inline void init(RandNum s) { _pstdgen->init(s); }
  
        /// Change the standard generator
        static RandomGen * changeGenerator(RandomGen *g);

        /// Restore the standard generator
        static void restoreGenerator();

        /** 
            This method must be overloaded in each derived
            class to return a double according to the propoer
            distriibution. */
        virtual double get() = 0;

        virtual double getMaximum() throw(MaxException) = 0;
        virtual double getMinimum() throw(MaxException) = 0;

    };

    /**  
         This class returns always the same number (a constant).
         It's a particular case of a random distribution: it's a
         Delta of Dirac.
    */  
    class DeltaVar : public RandomVar {
        double _var;
    public:
        DeltaVar(double a) : RandomVar(), _var(a) {}
        virtual double get() { return _var; } 
        virtual ~DeltaVar() {};
        static RandomVar *createInstance(vector<string> &par);  
        virtual double getMaximum() throw(MaxException) {return _var;}
        virtual double getMinimum() throw(MaxException) {return _var;}
    };

    /** 
        This class implements an uniform distribution, between min
        and max. */
    class UniformVar : public RandomVar {
        double _min, _max;
    public:
        UniformVar(double min, double max, RandomGen *g = NULL) 
            : RandomVar(g), _min(min), _max(max) {}
        virtual double get();
        virtual ~UniformVar() {}
        static RandomVar *createInstance(vector<string> &par);
        virtual double getMaximum() throw(MaxException) {return _max;}
        virtual double getMinimum() throw(MaxException) {return _min;}
    };

    /**
       This class implements an exponential distribution, with mean m. */ 
    class ExponentialVar : public UniformVar {
        double _lambda;
    public :
        ExponentialVar(double m, RandomGen *g = NULL) : 
            UniformVar(0, 1, g), _lambda(m) {}

        virtual double get();

        static RandomVar *createInstance(vector<string> &par);
        virtual double getMaximum() throw(MaxException)
            {throw MaxException("ExponentialVar");}
        virtual double getMinimum() throw(MaxException)
            {return 0;}
    };

    /**
       This class implements a pareto distribution, with parameters m and k */
    class ParetoVar : public UniformVar {
        double _mu, _order;
    public :
        ParetoVar(double m, double k, RandomGen *g = NULL) : 
            UniformVar(0,1,g), _mu(m), _order(k) {};
        virtual double get();
        static RandomVar *createInstance(vector<string> &par);
        virtual double getMaximum() throw(MaxException)
            {throw MaxException("ExponentialVar");}
        virtual double getMinimum() throw(MaxException)
            {throw MaxException("ExponentialVar");}
    };

    /**
       This class implements a normal distribution, with mean m variance
       sigma. In this class, we use the cephes library function
       ndtri(). */
    class NormalVar : public UniformVar {
        double _mu, _sigma;
        bool _yes;
        double _oldv;
  
    public:
        NormalVar(double m, double s, RandomGen *g = NULL) : 
            UniformVar(0,1,g), _mu(m), _sigma(s), _yes(false)
            {}
        virtual double get();
        static RandomVar *createInstance(vector<string> &par);
        virtual double getMaximum() throw(MaxException)
            {throw MaxException("NormalVar");}
        virtual double getMinimum() throw(MaxException)
            {throw MaxException("NormalVar");}
    };


    /**
       This class implements a Poisson distribution, with mean lambda */
    class PoissonVar : public UniformVar {
        double _lambda;
    public:
        static const unsigned long CUTOFF;
        PoissonVar(double l, RandomGen *g = NULL) : 
            UniformVar(0, 1, g), _lambda(l) {}
        virtual double get();

        static RandomVar *createInstance(vector<string> &par);
        virtual double getMaximum() throw(MaxException)
            {throw MaxException("PoissonVar");}
        virtual double getMinimum() throw(MaxException)
            {throw MaxException("PoissonVar");}
    };

    /**
       This class implements a deterministic variable. The object must
       be initialized with an array (double[]) or a vector<double> of
       values, or with a file containing the sequence of values. Each
       call of get() returns one of the numbers in the sequence.  When
       the last number in the sequence has been read, the sequence
       starts over.
    */
    class DetVar : public RandomVar {
        vector<double> _array;
        unsigned int _count;
    public:
        DetVar(const std::string &filename);
        DetVar(vector<double> &a);
        DetVar(double a[], int s);
        virtual double get();
        virtual ~DetVar(){}
        virtual double getMaximum() throw(MaxException);
        virtual double getMinimum() throw(MaxException);

        static RandomVar *createInstance(vector<string> &par);
    };
    //@}

} // namespace MetaSim

#endif // __RANDOMVAR_HPP__
