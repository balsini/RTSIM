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
 * $Id: regvar.hpp,v 1.6 2005/10/05 16:46:22 cesare Exp $
 *
 * $Log: regvar.hpp,v $
 * Revision 1.6  2005/10/05 16:46:22  cesare
 * Added DLL support.
 *
 * Revision 1.5  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.4  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 */
#ifndef __REGVAR_HPP__
#define __REGVAR_HPP__

#include <randomvar.hpp>

namespace MetaSim {

    const RandomVar::BASE_KEY_TYPE DeltaName("delta");
    const RandomVar::BASE_KEY_TYPE UnifName1 ("unif");
    const RandomVar::BASE_KEY_TYPE UnifName2 ("uniform");
    const RandomVar::BASE_KEY_TYPE NormalName1 ("normal");
    const RandomVar::BASE_KEY_TYPE NormalName2 ("gauss");
    const RandomVar::BASE_KEY_TYPE ExponentialName1 ("exp");
    const RandomVar::BASE_KEY_TYPE ExponentialName2 ("exponential");
    const RandomVar::BASE_KEY_TYPE ParetoName ("pareto");
    const RandomVar::BASE_KEY_TYPE PoissonName ("poisson");
    const RandomVar::BASE_KEY_TYPE DetName ("trace");
    const RandomVar::BASE_KEY_TYPE GenericName ("PDF");
  
    RandomVar* parsevar(const std::string &str);

} // MetaSim

#endif
