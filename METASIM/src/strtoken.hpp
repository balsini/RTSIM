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
#ifndef __STRTOKEN_HPP__
#define __STRTOKEN_HPP__

#include <exception>
#include <string>
#include <vector>

#include <baseexc.hpp>

/**
   \ingroup metasim_util

   This namespace contains useful functions for parsing strings. Such
   functions are used mainly by the abstract factory that creates
   random variables.
 */
namespace parse_util {

    using namespace std;

    /**
       Removes trailing spaces from the beginning and from the end of
       the string \c tk.
    */ 
    string remove_spaces(const string &tk);

    /**
       Given a string \c code, consisting of many substrings separated
       by the sequence \c sep, returns a vector containing the substrings.
       For example, given the string "fixed(1);wait(R);" returns the 
       vector containing strings "fixed(1)" and "wait(R)".
    */
    vector<string> split(const string &code, const string &sep);

    /**
       Given a sequence of "instructions" spearated by ';', returns a
       vector containing the istructions. Notice that the last
       instructions must finish with a ';', otherwise it is ignored. 
     
       @todo add sintax error checking!
    */
    vector<string> split_instr(const string &code);

    /**
       Given an instruction \c instr of the form "token(p1)" returns the
       token.
    */
    string get_token(const string &instr, const string &open_par = "(");

    /**
       Given an instruction \c instr of the form "token(p1,p2)" returns the
       parameters "p1,p2".
    */
    string get_param(const string &instr, const string &open_par = "(",
                     const string &close_par = ")");

    /**
       Given an instruction \c instr of the form "token = param1,
       param2" returns the parameters "param1, param2" (i.e. it returns
       everything after the separation symbol)
    */
    string get_param2(const string &instr, const string &open_par = "=");

    /**
       Given a string containing the parameters "(p1,p2)", returns a
       vector of strings containing the parameters p1 and p2.
    */
    vector<string> split_param(const string &p, const string &sep = ",",
                               char open_par = '(', char close_par = ')');
  
    /**
       Given a string of the form "123.75ms" returns the double number
       at the beginning of the string in res and the unit of measure in
       unit.
    */
    void parse_double(const string &nums, double &res, string &unit);

    /**
       Exception raised by the above functions
    */
    class ParseExc : public exception {
        string _where;
        string _par;

    public:
        ParseExc(const string &where, const string &par);
        string what();
        virtual ~ParseExc() throw () {}
    };
}

#endif
