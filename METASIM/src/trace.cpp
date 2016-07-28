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
 * $Id: trace.cpp,v 1.7 2005/11/18 17:08:57 cesare Exp $
 *
 * $Log: trace.cpp,v $
 * Revision 1.7  2005/11/18 17:08:57  cesare
 * Improved the Tracer.
 *
 * Revision 1.6  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.5  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * Revision 1.4.2.3  2004/11/24 11:46:43  lipari
 * Completed the re-working on the namespaces. Now everything
 * compiles and produces the expected results.
 *
 * Revision 1.4.2.2  2004/10/20 12:38:34  lamastra
 * Fixed f(void) -> f() (C++ standard compliancy issues)
 *
 * Revision 1.4  2003/10/16 09:00:05  scordino
 * The class Trace is capable of opening Ascii streams.
 * Anyway, the default behaviour remains to open binary streams.
 *
 * Revision 1.3  2003/09/12 09:47:55  scordino
 * Initialization of classes' static variables moved from file .hpp 
 * to file .cpp.
 *
 * Revision 1.2  2003/04/24 14:56:14  lipari
 * *** empty log message ***
 * 
 */
#include <event.hpp>
#include <trace.hpp>

namespace MetaSim {

  using namespace std;

  const char * const Trace::Exc::_NO_OPEN = "File is not open";

  Trace::Trace(const char *filename, Type type, bool tof) 
    :_filename(filename), toFile(tof)
  {
    if (tof == false) return;
    if (type == _ASCII_TRACE) 
      _os.open(_filename.c_str(), ios::out);
    else
      _os.open(_filename.c_str(), ios::binary | ios::out);
  } 

  Trace::Trace(string filename, Type type, bool tof)
    : _filename(filename)
  {
    if (tof == false) return;
    if (type == _ASCII_TRACE) 
      _os.open(_filename.c_str(), ios::out);
    else
      _os.open(_filename.c_str(), ios::binary | ios::out);
  }

  Trace::~Trace() 
  {
    if (_os.is_open()) close();
  }

  void Trace::open(bool type)
  {
    _os.close();
    if (type == _ASCII_TRACE) 
      _os.open(_filename.c_str(), ios::out);
    else
      _os.open(_filename.c_str(), ios::binary | ios::out);
    if (_os.bad()) throw Exc();
  }

  void Trace::close()
  {
    _os.close();
  }

} // namespace MetaSim  
