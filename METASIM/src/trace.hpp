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
 * $Id: trace.hpp,v 1.10 2005/11/18 17:08:57 cesare Exp $
 *
 * $Log: trace.hpp,v $
 * Revision 1.10  2005/11/18 17:08:57  cesare
 * Improved the Tracer.
 *
 * Revision 1.9  2005/10/05 16:46:22  cesare
 * Added DLL support.
 *
 * Revision 1.8  2005/08/03 16:41:09  cesare
 * gcc-4 compatibility (maybe).
 *
 * Revision 1.7  2005/03/31 15:26:10  cesare
 * Corrected some documentations.
 *
 * Revision 1.6  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * Revision 1.5.2.3  2004/11/04 16:20:33  lipari
 * *** empty log message ***
 *
 * Revision 1.5.2.2  2004/10/20 12:38:34  lamastra
 * Fixed f(void) -> f() (C++ standard compliancy issues)
 *
 * Revision 1.5  2003/10/17 08:46:18  scordino
 * Bug fixing.
 *
 * Revision 1.4  2003/10/16 09:00:05  scordino
 * The class Trace is capable of opening Ascii streams.
 * Anyway, the default behaviour remains to open binary streams.
 *
 * Revision 1.3 2003/09/12 09:47:55 scordino Initialization of
 * classes' static variables moved from file .hpp to file .cpp.
 *
 * Revision 1.2  2003/04/24 14:55:54  lipari
 * *** empty log message ***
 * 
 */
#ifndef __TRACE_HPP__
#define __TRACE_HPP__

#include <fstream>
#include <iostream>

#include <baseexc.hpp>
#include <basetype.hpp>

namespace MetaSim {
  class Event;

#define _BIN_TRACE 0
#define _ASCII_TRACE 1

  /**
     \ingroup metasim_stat

     This is just the basic interface for the tracing classes. By
     default, it opens a binary stream. 
  */
  class Trace { 
  protected:
    std::string _filename;
    std::ofstream _os;
    bool toFile;
  public:

    enum Type {BINARY = 0,
	       ASCII = 1};

    /**
       \ingroup metasim_exc
       
       Exceptions for the Trace classes.
     */
    class Exc : public BaseExc {
    public:
      static const char * const _NO_OPEN;
      Exc(const std::string msg = _NO_OPEN,
	  const std::string c = "Trace", 
	  const std::string mod = "trace.hpp") : BaseExc(msg, c, mod) {} ;
    };
  
    /**
       Constructor for the trace class. It opens a stream of name
       "filename" of type. Type can be binary or ascii (only for DOS
       based systems!).
     */
    Trace(const char *filename, Type type = BINARY, bool tof = true);

    Trace(std::string filename, Type type = BINARY, bool tof = true);

    /**
      This is a virtual function that must be overridden in the
      derived class.

      DEPRECATED!

      @todo remove this function and substitute the current tracing 
      with the particle mechanism
     */
    virtual void record(Event*) {};

    /// Open the file! 
    virtual void open(bool type = BINARY);

    /// Closes the file!
    virtual void close();

    /// Destructor.
    virtual ~Trace();
  };


  /**
     \ingroup metasim_stat

     This is an ASCII stream used to monitor some internal variables.
  */
  class TraceAscii : public Trace {
  public:
    TraceAscii(char* file) : Trace(file, ASCII){}

    /// Records the value on the file, one value per line.
    //@{
    void record(double value) { _os << value << std::endl; }
    void record(long double value){_os << value << std::endl;}
    void record(int value){_os << value << std::endl;}
    void record(char* str){_os << str;}

    /// DEPRECATED substitute with the particle mechanism
    virtual void record(Event*) {}; //Inherited from Trace 
    //@}
  };    

} // namespace MetaSim

#endif
