/***************************************************************************
    begin                : Fri Oct 22 15:54:58 CEST 2004
    copyright            : (C) 2004 by Giuseppe Lipari
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
#ifndef __TICK_HPP__
#define __TICK_HPP__

#include <string>
#include <stdint.h>
#include <limits.h>
#include <cmath>
#include <iostream>

#include <baseexc.hpp>
#include <strtoken.hpp>


#ifdef _MSC_VER
#pragma warning(disable: 4290) // A function is declared using exception specification,
							   // which Visual C++ accepts but does not implement.
							   // http://msdn.microsoft.com/en-us/library/sa28fef8.aspx
/*#if _MSC_VER >= 1400
#define sprintf sprintf_s
#endif*/
#endif

#define MAXTICK INT64_MAX

#define FRIEND_DECL_SYMM_OPS(RET, OP)  \
        friend RET operator OP (const Tick &t1, const Tick &t2); \
        friend RET operator OP (const Tick &t1, long int t2); \
        friend RET operator OP (long int t1, const Tick &t2); \
        friend RET operator OP (const Tick &t1, int t2);      \
        friend RET operator OP (int t1, const Tick &t2)

#define FRIEND_DECL_ASYMM_OPS(RET, OP)  \
        friend RET operator OP (const Tick &t1, long int t2); \
        friend RET operator OP (const Tick &t1, int t2)


#define IMPL_SYMM_OPS(RET, OP) \
    inline RET operator OP(const Tick &t1, const Tick &t2) { \
        return t1.v OP t2.v;                                 \
    }                                                        \
    inline RET operator OP(const Tick &t1, long int t2) {    \
        return t1.v OP t2;                                   \
    }                                                        \
    inline RET operator OP(const Tick &t1, int t2) {         \
        return t1.v OP t2;                                   \
    }                                                        \
    inline RET operator OP(long int t1, const Tick &t2) {    \
        return t1 OP t2.v;                                   \
    }                                                        \
    inline RET operator OP(int t1, const Tick &t2) {         \
        return t1 OP t2.v;                                   \
    }

#define IMPL_ASYMM_OPS(RET, OP) \
    inline RET operator OP(const Tick &t1, long int t2) {    \
        return t1.v OP t2;                                   \
    }                                                        \
    inline RET operator OP(const Tick &t1, int t2) {         \
        return t1.v OP t2;                                   \
    }

namespace MetaSim {

    using namespace std;

    DECL_EXC(NegativeTickException, "Tick");

    /**
       \ingroup metasim_ee

       This class implements the basic type for discrete time in the
       simulation.  The current implementation is int64_t. It
       provides:

       - automatic cast and conversion from basic types;
       - resolution specification (how many ticks in a second);
       - conversion from string (with or specification of the unit measure);
       - arithmetic operations and comparison;
       - conversions to string types (with specification of the unit of measure).

       Possible units are:

       - seconds (s);
       - milli (ms);
       - micro (us);
       - nano (ns);

       The simulator handles all time variables in Ticks. 
    */

    class Tick {
    public:
        typedef int64_t impl_t;
        typedef enum {nanosec = 1, microsec = 1000, millisec = 1000000, sec = 1000000000} unit_t;
    private:
        impl_t v; // value

        static impl_t resolution;
        static unit_t default_unit;

    public:
        Tick() { v = 0; }
// #if __WORDSIZE == 64
//         Tick(long long int t) { v = t; }
// #endif
        Tick(impl_t t) { v = t; }
        Tick(int t) { v = t; }

        /// implementation in tick.pp
        Tick(const std::string &s);

        /// explicit conversion with trunking
        explicit Tick(double t) { v = (impl_t) t; }

        /// rounding
		/// FIXME: detect first if ::round() is available (e.g., VC2013)
		#define xx_round(dbl) dbl >= 0.0 ? (int)(dbl + 0.5) : ((dbl - (double)(int)dbl) <= -0.5 ? (int)dbl : (int)(dbl - 0.5))
        static Tick round(double t) { Tick q; q.v = (impl_t) xx_round(t); return q; }

        /// ceiling
        static Tick ceil(double t) { Tick q; q.v = (impl_t) ::ceil(t); return q; }

        /// floor
        static Tick floor(double t) { Tick q; q.v = (impl_t) ::floor(t); return q; }
        
        // default assignment operator and copy constructor

        Tick& operator+=(const Tick &t) { v += t.v; return *this; }
        Tick& operator-=(const Tick &t) { v -= t.v; return *this; }

        Tick& operator*=(long int t) { v *= t; return *this; }
        Tick& operator/=(long int t) { v /= t; return *this; }
        
        /// pre-increment
        Tick& operator++() { v++; return *this; } 
        /// post-increment
        Tick operator++(int) { v++; return Tick(v-1); } 

        /// pre-decrement
        Tick& operator--() { v++; return *this; } 
        /// post-decrement
        Tick operator--(int) { v--; return Tick(v+1); }

        // automatic conversion to double
        operator double() const { return (double) v; }
        /// automatic conversion to long int
        operator long int () const { return v; }
        /// automatic conversion to int (warning, this may be imprecise!)
        operator int () const { return (int)v; }

        static void set_default_unit(Tick::unit_t d) { default_unit = d; }

        /// implementation in tick.pp
        static void set_resolution(const std::string &t) throw (parse_util::ParseExc);        

        FRIEND_DECL_SYMM_OPS(bool, <);
        FRIEND_DECL_SYMM_OPS(bool, <=);
        FRIEND_DECL_SYMM_OPS(bool, ==);
        FRIEND_DECL_SYMM_OPS(bool, !=);
        FRIEND_DECL_SYMM_OPS(bool, >=);
        FRIEND_DECL_SYMM_OPS(bool, >);

        FRIEND_DECL_SYMM_OPS(Tick, *);
        FRIEND_DECL_SYMM_OPS(Tick, +);
        FRIEND_DECL_SYMM_OPS(Tick, -);

        FRIEND_DECL_ASYMM_OPS(Tick, /);

        friend Tick operator-(const Tick &t);

        friend std::ostream& operator<<(std::ostream& os, const Tick &t1);
        friend std::istream& operator>>(std::istream& is, Tick &t1);
    };

    IMPL_SYMM_OPS(bool, <);
    IMPL_SYMM_OPS(bool, <=);
    IMPL_SYMM_OPS(bool, ==);
    IMPL_SYMM_OPS(bool, !=);
    IMPL_SYMM_OPS(bool, >=);
    IMPL_SYMM_OPS(bool, >);
    
    IMPL_SYMM_OPS(Tick, *);
    IMPL_SYMM_OPS(Tick, +);
    IMPL_SYMM_OPS(Tick, -);

    IMPL_ASYMM_OPS(Tick, /);
    
    inline Tick operator-(const Tick &t) { return Tick(-t.v); }

    std::ostream& operator<<(std::ostream &os, const Tick &t1);
    std::istream& operator>>(std::istream &is, Tick &t1);

} // namespace MetaSim

#endif
