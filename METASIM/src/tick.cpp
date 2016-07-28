/***************************************************************************
    begin                : Fri Oct 22 15:54:58 CEST 2004
    copyright            : (C) 2004 by Cesare Bartolini
    email                : c.bartolini@isti.cnr.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

#include <strtoken.hpp>
#include <tick.hpp>

namespace MetaSim {

    using namespace std;
    using namespace parse_util;

    Tick::unit_t Tick::default_unit = Tick::nanosec;
    Tick::impl_t Tick::resolution = 1;

    std::ostream& operator<<(std::ostream &os, const Tick &t1)
    {
        os << t1.v; return os;
    }

    std::istream& operator>>(std::istream &is, Tick &t1)
    {
        is >> t1.v; return is;
    }    

    void Tick::set_resolution(const string &s) throw (ParseExc)
    {        
        string unit;
        double num;

        parse_double(s, num, unit);

        if (unit != "s" && unit != "ms" && unit != "us" && unit != "ns") 
            throw ParseExc("Tick::setDefaultUnit(const string()", unit);

        if (unit == "s") { resolution = (impl_t) (num * 1000000000);}
        else if (unit == "ms")  { resolution = (impl_t) (num * 1000000); }
        else if (unit == "us")  { resolution = (impl_t) (num * 1000); }
        else if (unit == "ns")  { resolution = (impl_t) num ; } 
    }
    
    Tick::Tick(const string &s)
    {
        string unit;
        double num;

        parse_double(s, num, unit);

        if (unit != "s" && unit != "ms" && unit != "us" && unit != "ns" && unit != "") 
            throw ParseExc("Cannot understand time unit: ", unit);

        if (unit == "") { v = (impl_t)((num * default_unit) / resolution); }
        else if (unit == "s") { v = (impl_t)((num * 1000000000) / resolution);}
        else if (unit == "ms")  { v = (impl_t)((num * 1000000) / resolution); }
        else if (unit == "us")  { v = (impl_t)((num * 1000) / resolution); }
        else if (unit == "ns")  { v = (impl_t)((num) / resolution); }
    }


} // namespace MetaSim
