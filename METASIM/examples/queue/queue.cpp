#include <stdlib.h>
#include "queue.hpp"


using namespace std;
using namespace MetaSim;

int main() 
{
        double lambda, mu;

        cout << "          ###### M/M/1 example ######\n\n";

        cout << "Insert lambda : ";
        cin >> lambda;

        cout << "Insert mu : ";
        cin >> mu;

        if (lambda >= mu) {
                cout << "lambda must be less than mu (otherwise is unstable!!)" 
                     << endl;
                exit(-1);
        }

        ExponentialVar st(lambda);
        ExponentialVar at(mu);

        Sink sink("sink");
        Queue que(&sink, &st, "M/M/1 queue");
        Source source(&que, &at, "source");

        AvgQueueSizeStat avgSizeStat(que, "avg_queue_size");
        avgSizeStat.attach(&source);

        BaseStat::setTransitory(2000);
  
        SIMUL.dbg.setStream("log.txt");
        SIMUL.dbg.enable("All");

        SIMUL.run(2000000, 8);

        cout << "The average queue length is " 
             << avgSizeStat.getMean() << endl;
        cout << "with a 95% confidence interval of " 
             << avgSizeStat.getConfInterval(BaseStat::C95) << endl;
}//end main
