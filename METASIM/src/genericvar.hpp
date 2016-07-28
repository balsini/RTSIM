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
 * $Id: genericvar.hpp,v 1.3 2005/04/28 01:34:47 cesare Exp $
 *
 * $Log: genericvar.hpp,v $
 * Revision 1.3  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.2  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * Revision 1.1.2.5  2004/11/17 11:03:23  lipari
 * Solved discrepancy btw genericvar.hhp and genericvar.cpp,
 * function declaration and implementation should throw the same
 * exceptions!
 * Modified the basetype.hpp, removed forward declarations of Entity and
 * Event, they should be moved in the appropriate hpp files.
 *
 * Revision 1.1.2.4  2004/11/03 13:28:00  lipari
 * Added documentation to most classes. Also, added a documentation file in
 * doc/include. The documentation has to be compiled with doxygen,
 * The configuration file is doc.conf
 *
 * Revision 1.1.2.3  2004/10/29 17:03:48  lipari
 * Added tick.hpp and tick.cpp. By now the development is not finished!
 * They define a class called MyTick which is tested alone. Added also the
 * relative test file testtick.cpp.
 * Added function parse_double in parse_util (strtoken.hpp and .cpp).
 *
 * Revision 1.1.2.1  2004/10/29 15:37:04  lipari
 * Cambiato nome a GenericVar.hpp --> genericvar.hpp
 *
 * Revision 1.2  2003/04/24 14:55:54  lipari
 * *** empty log message ***
 * 
 */
#ifndef __GENERICVAR_HPP__
#define __GENERICVAR_HPP__

#include <iostream>
#include <map>

#include <randomvar.hpp>

namespace MetaSim {

    /**
       This random variable is used to model a generic distribution.
    */
    class GenericVar: public UniformVar {
        std::map<int, double> _pdf;
        void readPDF(std::ifstream &f, int mode = 0);// throw(Exc);
    public:
        GenericVar(const std::string &filename);

        virtual ~GenericVar() {}
  
        virtual double get(void);

        static RandomVar *createInstance(vector<string> &par);
  };

} // namespace metasim

#endif /* __GENERICVAR_HPP__ */
