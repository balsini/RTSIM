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
 * $Id: genericvar.cpp,v 1.3 2005/04/28 01:34:47 cesare Exp $
 *
 * $Log: genericvar.cpp,v $
 * Revision 1.3  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.2  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * Revision 1.1.2.3  2004/11/15 13:14:24  lamastra
 * - Gerardo
 * Dropped throw() in genericvar.cpp
 *
 * Revision 1.2  2003/04/24 14:56:14  lipari
 * *** empty log message ***
 * 
 */
#include <cmath>

#include <genericvar.hpp>
#include <strtoken.hpp>

namespace MetaSim {

    using namespace std;
    using namespace parse_util;

    static const double PDF_ERR = 0.00000000001;

    void GenericVar::readPDF(ifstream &f, int mode)
    {
        int n;
        double p;
        double sum;

        sum = 0;
        while(!f.eof()) {
            f >> n;
            f >> p;
#ifdef __DEBUG__
            cout << n << "\t" << p << "\n";
#endif
            if (!f.eof()) {
                if (_pdf[n] != 0) {
                    string errMsg = Exc::_WRONGPDF + string("\n");
                    throw Exc(errMsg, "GenericVar");
                }
                sum += p;
                _pdf[n] = p;
            }
        }

        if (sum > 1) {
            string errMsg = Exc::_WRONGPDF + string("\n");
            throw Exc(errMsg, "GenericVar");
        }

        if (sum < (1.0 - PDF_ERR)) {
            cerr << "Warning: PDF values sum to " << sum << " < 1\n";
            if (mode == 0) {
                _pdf[n] += (1 - sum);
            } else {
                _pdf[1] += (1 - sum);
            }
        }
    }    


    GenericVar::GenericVar(const std::string &fileName) : 
        UniformVar(0, 1, NULL)
    {
        ifstream inFile(fileName.c_str());

        if (!inFile.is_open()) {
            string errMsg = Exc::_FILEOPEN  + string(fileName) + "\n";
            throw Exc(errMsg, "GenericVar");
        }

        readPDF(inFile);
    }

    double GenericVar::get()
    {
        double v, CDF;
        map<int, double>::iterator i;

        v = UniformVar::get();
        CDF = 0;

        for(i = _pdf.begin(); i != _pdf.end(); i++) {
            CDF = CDF + i->second;
            if (CDF > v) {
                return i->first;
            }
        }

        /* We cannot arrive here... */
        /* It means that v > 1 or that lim_{i -> \infty} CDF < 1... */
        throw Exc("Panic", "GenericVar");
    }
    
    RandomVar *GenericVar::createInstance(vector<string> &par)
    {
        if (par.size() != 1) 
            throw ParseExc("Wrong number of parameters", "GenericVar");
        
        return new GenericVar(par[0]);
    }


} // namespace MetaSim
