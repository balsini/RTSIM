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
#include <algorithm>
#include <iostream>

#include <debugstream.hpp>
#include <simul.hpp>

using namespace std;

namespace MetaSim {

    DebugStream::DebugStream() : 
        _os(&cerr),
        _autodelete(false),
        _isDebug(false),
        _isDebugAll(false),
        _isIndenting(false),
        _indentLevel(0),
        _dbgLevels(),
        _dbgStack(),
        _t1(0),
        _t2(MAXTICK)
    {    
    }

    DebugStream::~DebugStream()
    { 
        if (_autodelete) delete _os;
    }

    void DebugStream::setStream(std::ostream& o)
    {
        if (_autodelete) delete _os;
        _autodelete = false;
        _os = &o; 
    }

    void DebugStream::setStream(const char *filename)
    {
        if (_autodelete) delete _os;
        _autodelete = true;
        _os = new ofstream(filename); 
    }

    void DebugStream::enable(std::string s) 
    {
        if (s == "All") _isDebugAll = true;
        else {
            _dbgLevels.push_back(s);
        }
    }

    void DebugStream::disable(std::string s) 
    {
        if (s == "All") _isDebugAll = false;
        else {
            std::vector<std::string>::iterator i = 
                find(_dbgLevels.begin(),_dbgLevels.end(),s);
            if (i != _dbgLevels.end()) _dbgLevels.erase(i);
        }
    }

    void DebugStream::enter(std::string s) 
    {
        _dbgStack.push_back(_isDebug);
        std::vector<std::string>::iterator i = 
            find(_dbgLevels.begin(), _dbgLevels.end(), s);
        if (i == _dbgLevels.end()
            || (SIMUL.getTime() < _t1) 
            || (SIMUL.getTime() > _t2)) 
            _isDebug = false;
        else
            _isDebug = true;
        _isIndenting = true;
    }

    void DebugStream::enter(std::string s, std::string header) 
    {
        enter(s);
        if (filter()) {
            indent();
            (*_os) << header << endl;
            resetIndent();
            _indentLevel++;
        }
    }

    void DebugStream::exit() 
    {
        if (filter()) {
            _indentLevel--;
        }
        _isDebug = _dbgStack.back();
        _dbgStack.pop_back();
        _isIndenting = true;
    }

    void DebugStream::setTransitory(Tick t)
    {
        _t1 = t;
    }

    void DebugStream::setTransitory(Tick t1, Tick t2)
    {
        _t1 = t1;
        _t2 = t2;
    }

    bool DebugStream::filter() const
    { 
        if (_isDebug || _isDebugAll)
            return true;
        else return false;
    }

    void DebugStream::resetIndent() 
    { 
        _isIndenting = true; 
    } 

    void DebugStream::indent() 
    { 
        if (_isIndenting) {
            for (int i = 0; i < _indentLevel; ++i) 
                (*_os) << "  ";
            _isIndenting = false;
        } 
    }

    ostream& DebugStream::getStream() 
    { 
        return *_os; 
    }

} // namespace MetaSim
