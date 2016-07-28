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
 * $Id: basetype.hpp,v 1.8 2005/05/20 19:34:20 cesare Exp $
 *
 * $Log: basetype.hpp,v $
 * Revision 1.8  2005/05/20 19:34:20  cesare
 * Fixed the problem with debug messages.
 *
 * Revision 1.7  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.6  2005/03/03 10:08:44  scordino
 * I need bigger values for MAX_RUN
 *
 * Revision 1.5  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * 
 */
#ifndef NULL
#define NULL 0
#endif

#ifndef __BASETYPE_HPP__
#define __BASETYPE_HPP__

#include <limits>
#include <tick.hpp>

// #ifndef MAXTICK
// #ifndef __LONG_MAX__
// #error "Cannot find definition of __LONG_MAX__"
// #else
// #define MAXTICK Tick(__LONG_MAX__)
// #endif
// #endif

namespace MetaSim {
  /* MAX_RUN is the maximum number of runs allowable in this simulator
   * it is an arbitrary value, feel free to increase it.
   */
  static const size_t MAX_RUN = 100000;
}

#endif
