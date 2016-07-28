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
#ifndef __DEBUGSTREAM_HPP__
#define __DEBUGSTREAM_HPP__

#include <fstream>
#include <string>
#include <vector>

#include <basetype.hpp>

namespace MetaSim {
    /**
       \ingroup metasim_util
     
       Helper class used to manipulate the debug output.
    */
    class DebugStream {
    private:
        std::ostream* _os;
        bool _autodelete;
        bool _isDebug;
        bool _isDebugAll;
        bool _isIndenting;
        int _indentLevel;
        std::vector<std::string> _dbgLevels;
        std::vector<bool> _dbgStack;

        Tick _t1;
        Tick _t2;

    public:
        DebugStream();
        ~DebugStream(); 

        /**
         * Set the debug stream.
         */
        void setStream(std::ostream& o);

        /**
         * Open the file specified by the string filename and set it as a
         * debug stream.  */
        void setStream(const char *filename);

        /**
         *  Enable the output for a certain debug level.
         *  @param s String that identifies the debug level.
         */
        void enable(std::string s);

        /**
         *  Disable the output for a certain debug level.
         *  @param s String that identifies the debug level.
         */
        void disable(std::string s);

        /**
         *  Enters in the specified debug level. From this point until
         *  the next call to enter(), every output on the debug stream 
         *  is considered belonging to the current debug level.
         */
        void enter(std::string s);

        /**
         *  Enters in the specified debug level and output the header string.
         *  From this point until the next call to enter(), every output on the 
         *  debug stream is considered in the the current debug level. 
         */
        void enter(std::string s, std::string header);

        /**
         *  Exits from the current debug level.
         */  
        void exit();

        /**
         * Enables output from tick t 
         */
        void setTransitory(Tick t);

        /** 
         * Enables output from tick t1 to t2 
         */
        void setTransitory(Tick t1, Tick t2);

        // These helper function are only called by operator << and endl
        bool filter() const;
        void resetIndent();
        void indent();
        std::ostream &getStream();
    };

    /** Template definition of operator << on DebugStream */
    template <class T>
    inline DebugStream& operator<<(DebugStream &s, T obj) 
    {
        if (s.filter()) {
            s.indent();
            s.getStream() << obj;
        }
        return s;
    }

    /** The following specialization is useful for intercepting the endl 
        modifier and properly handle the indenting. */
    inline DebugStream& operator<<(DebugStream &s, 
                                   DebugStream& (*f)(DebugStream&)) 
    {
        if (s.filter()) return f(s);
        else return s;
    }
} // namespace MetaSim

namespace std {
    /**
       \ingroup metasim_util

       Specialization of endl: must be in namespace std
       to override the standard endl() function 
    */
    inline MetaSim::DebugStream& endl(MetaSim::DebugStream &s) 
    {
        if (s.filter()) {
            s.resetIndent();
            s.getStream() << std::endl;
        }
        return s;
    }
}

#endif // __DEBUGSTREAM_HPP__
