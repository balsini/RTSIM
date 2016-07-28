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
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>

#include <baseexc.hpp>
#include <basestat.hpp>
#include <basetype.hpp>
#include <simul.hpp>
#include <functional>

using namespace std;

namespace MetaSim {

    double BaseStat::_TDistr[MAX_DISTR];
    size_t BaseStat::_totalNumOfExp = 0;
    size_t BaseStat::_expNum = 0;
    bool BaseStat::_endOfSim = 0;
    bool BaseStat::_initFlag = false;
    Tick BaseStat::_transitory = 0;  
    bool TableOutput::_created = false;
    string TableOutput::_fname;
    BaseStat::List BaseStat::_statList;

    // A bunch of customary exception messages
    const char* const EFFECTIVE_ATTACH = 
        "Unhandled attach()!";

    const char* const GET = 
        "Can't get statistics before end of simulation\n";

    const char* const NO_INIT = 
        "Statistics not initialized";

    const char* const NEED_3 =
        "Need at least 3 run to evaluate statistic";

    static const char* const TOO_MUCH_RUNS =
        "Too many runs! \n";

    static double t1_table[30][2] = {
        {6.314, 12.706},
        {2.920, 4.303},
        {2.353, 3.182},
        {2.132, 2.776},
        {2.015, 2.571},
        {1.943, 2.447},
        {1.895, 2.365},
        {1.860, 2.306},
        {1.833, 2.262},
        {1.812, 2.228},
        {1.796, 2.201},
        {1.782, 2.179},
        {1.771, 2.160},
        {1.768, 2.145},
        {1.753, 2.131},
        {1.746, 2.120},
        {1.740, 2.110},
        {1.734, 2.101},
        {1.729, 2.093},
        {1.725, 2.086},
        {1.721, 2.080},
        {1.717, 2.074},
        {1.714, 2.069},
        {1.711, 2.064},
        {1.708, 2.060},
        {1.706, 2.056},
        {1.703, 2.052},
        {1.701, 2.048},
        {1.699, 2.045},
        {1.697, 2.042}
    };

    BaseStat::BaseStat(std::string n) :
        _name(n)
    {
        _statList.push_back(this);
    }

    BaseStat::~BaseStat()
    {
        _statList.remove(this);
    }

    void BaseStat::init(size_t n)  
    {
        _totalNumOfExp = n;
        _endOfSim = false;
        _initFlag = true;
        for_each(_statList.begin(), _statList.end(),
                 mem_fun(&BaseStat::init));
    }
  
    void BaseStat::init()
    {
        _exper.clear();
        _expNum = 0;
    }  

    void BaseStat::setTransitory(Tick t)
    {
        _transitory = t;
    }

    bool BaseStat::chkTransitory()
    {
        if (SIMUL.getTime() >= _transitory) return false;
        else return true;
    }

    double BaseStat::t_student(int alfa, int dol)
    {
        if (dol<1 || dol>30)
            return -1;
        switch (alfa) {
        case 90:
            return t1_table[dol-1][0];
            break;
        case 95:
            return t1_table[dol-1][1];
            break;
        default:
            return -1;
        }
    }

    //
    // Collect all the results
    //
    void BaseStat::endRun()
    {
        for_each(_statList.begin(), _statList.end(),
                 mem_fun(&BaseStat::collect));
        if (++_expNum >= MAX_RUN)
            throw Exc(TOO_MUCH_RUNS);
    }

    void BaseStat::endSim()
    {
        _endOfSim = true;
    }

    //
    // Initialize all the stat objs and increment expnum
    //
    void BaseStat::newRun()
    {
        for_each(_statList.begin(), _statList.end(),
                 mem_fun(&BaseStat::initValue));
    }

    //
    // Returns the mean value calculated over all the experiments
    //
    double BaseStat::getMean()
    {
        if (!_endOfSim) throw Exc(GET);
        if (!_initFlag) throw Exc(NO_INIT);

        double sum = accumulate(_exper.begin(), _exper.end(), 0.0);
        sum /= _expNum;
  
        return sum;
    }

    //
    // Returns the semi-confidence interval
    // (the mean vary in [mu - sci; mu + sci]) relative to prob (1 - a)
    //
    namespace {
        // Functor to use with accumulate() in order
        // to evaluate the variance:
        // sigma^2 = Sum( (exper[i] - _mu)^2 ) / N*(N-1)
        // being N the number of rounds
        class V {
            double _mu;
        public:
            V(double m) : _mu(m) {}
            double operator()(double result, double value) const 
                { 
                    return result + (value - _mu)*(value - _mu);
                }
        };
    }

    //
    // Returns the variance for the experimental samples
    //
    double BaseStat::getVariance()
    {
        double sum = 0;
        double mu;		// the mean
  
        if (!_endOfSim) throw Exc(GET);
        if (!_initFlag) throw Exc(NO_INIT);
        if (_expNum < 3) throw Exc(NEED_3);

        mu = getMean();

        sum = accumulate(_exper.begin(), _exper.end(), 0.0, V(mu));
        return sqrt(sum/((_expNum - 1) * _expNum));
    }

    double BaseStat::getConfInterval(CONFIDENCE_INTERVAL c)
    {
        double mu;		// the mean
        double s;		// the variance
  
        if (!_endOfSim) throw Exc(GET);
        if (!_initFlag) throw Exc(NO_INIT);
        if (_expNum < 3) throw Exc(NEED_3);

        mu = getMean();
        s = getVariance();
        return t_student(c, (unsigned int) _expNum - 1) * s;
    }

    void BaseStat::printAll()
    {
        for_each(BaseStat::begin(), BaseStat::end(), mem_fun(&BaseStat::print));
    }	 
  
    void BaseStat::print()
    {
        cout << "[" << getName()
             << "]:" << getMean()
             << "  (Conf[95%]=" << getConfInterval(BaseStat::C95)
             << ")" 
             << endl;
    }
  
    /* Output class 
       This class produce formatted output to be read by gnuplot.
       Just call GnuPlotOutput::init() before simulation, and 
       GnuPlotOutput::write("%f", par) after every simulation, where 
       par is a float parameter of the simulation. 
       There can be more than one parameter, and of different types!

       See ex7.ccp for more details.
    */
    void GnuPlotOutput::init()
    {
        BaseStat::List::const_iterator i  = BaseStat::begin();
        while (i != BaseStat::end()) {
            BaseStat* p = *i;
            cout << "Name: " << p->getName() << endl;

            if (p->getName() != "") {
                ofstream f(p->getName().c_str());

                if (!f.is_open())
                    throw Exc("Cannot open file " + p->getName());

                f << "# " << p->getName() << endl;
            }
            ++i;
        }
    }
  
    /* Output class 
       This class produce formatted output in a table-like format.
       Every statistic has its own line with:
       - name of the statistic;
       - average value
       - confidence interval

       The init method takes the file name.

       The write() method takes a string that will be 
       printed at the beginning of the file. 
    */

    void TableOutput::init(string filename)
    {
        if (!_created)
            _fname = filename;
        _created = true;
    }    

    void TableOutput::write(string message)
    {

        if (!_created) 
            throw BaseExc("Init function not called",
                          "TableOutput",
                          "basestat.cpp");

        ofstream f(_fname.c_str());
        f << message << endl;

        BaseStat::List::const_iterator i  = BaseStat::begin();
        BaseStat* p = *i;
        while (p != NULL) {
            f << p->getName() << "\t\t | "
              << p->getMean() << "\t\t | "
              << p->getConfInterval() << endl;
            ++i;
            p = *i;
        }
    }
}

/* ------------------------------------------ */

#ifdef __BASESTAT_TEST__

#include "event.hpp"
#include "randomvar.hpp"

using namespace MetaSim;

// only for debug!!

class Dummy : public StatMean {
    NormalVar nv;
public:
    Dummy(int v) : StatMean("StatMean",0), nv(v, 1) {};
    virtual void probe(Event *e) { record(nv.get()); }
    virtual void attach(Entity* e) {}
};

Dummy *d1;
Dummy *d2;

int main()
{
    int n;
    int i,j;

    try {  
        cout << "Insert n (n > 3): ";
        cin >> n;
        BaseStat::init(n);
    
        d1 = new Dummy(7);
        d2 = new Dummy(10);
    
        for (j = 0; j < n; j++) {
            for (i = 0; i < 1000; i++) {
                d1->probe(0);
                d2->probe(0);
            }
      
            BaseStat::endRun();
      
            if (j < n-1) BaseStat::newRun();
        }
    
        BaseStat::printall();
    } catch (exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

} // namespace MetaSim

#endif // __TEST__
