#include "markov.hpp"

using namespace std;
using namespace MetaSim;

void JumpEvent::doit()
{
        _sender->clear_running();  
        _receiver->run();
}
 
void State::run()
{
        DBGENTER(_MARKOV_DBG_LEV);
        _running = true;
        _lastArrival = SIMUL.getTime();
        Tick max = MAXTICK;
        State *dest = this;
        vector<Link>::iterator i;

        DBGPRINT_2("Exploring ", getName());

        for (i = _links.begin(); i != _links.end(); ++i) {
                i->new_number();
    
                DBGPRINT_4("jump to ", (i->get_dest())->getName(), 
                           " at ", i->get_number());
                
                if (i->get_number() < max) {
                        max = i->get_number();
                        dest = i->get_dest();
                }
        }

        DBGPRINT_4("next jump to ", dest->getName(), " at ",max);
  
        _event.setReceiver(dest);
        DBGPRINT_2("posting event at time : ", (SIMUL.getTime() + max));
        _event.post(SIMUL.getTime() + max);
        
} 

void State::newRun()
{
        DBGENTER(_MARKOV_DBG_LEV);
        _lastArrival = 0;
        if (_initialState) {
                _running = true;
                Tick max = MAXTICK;
                State *dest = this;
                vector<Link>::iterator i;

                DBGPRINT_2("Exploring ", getName());
                for (i = _links.begin(); i != _links.end(); ++i) {
                        i->new_number();
      
                        DBGPRINT_4("jump to ", (i->get_dest())->getName(), 
                                   " at ", i->get_number());
                        
                        if (i->get_number() < max) {
                                max = i->get_number();
                                dest = i->get_dest();
                        }
                }
                DBGPRINT_4("next jump to ", dest->getName(), " at ",max);
    
                _event.setReceiver(dest);
                DBGPRINT_2("posting event at time : ", (SIMUL.getTime() + max));
                _event.post(SIMUL.getTime() + max);
        } 
        else
                _running = false;
        
}


int main()
{
        try {
                cout << "         ###### Markov example ######\n\n";

                State S2("primary", true);
                State S1("backup");
                State S0("fault");

                S2.put_link(10.0, &S1);
                S1.put_link(10.0, &S2);
                S1.put_link(10.0, &S0);
                S0.put_link(10.0, &S2);

                AvgTimeStateStat stat_state2("stato2"); 
                AvgTimeStateStat stat_state1("stato1"); 
                AvgTimeStateStat stat_state0("stato0"); 
 
                stat_state2.attach(&S2);
                stat_state1.attach(&S1);
                stat_state0.attach(&S0);

                BaseStat::setTransitory(2000);
  
                SIMUL.dbg.setStream("log.txt");
                SIMUL.dbg.enable(_MARKOV_DBG_LEV);
                SIMUL.dbg.enable(_SIMUL_DBG_LEV);

                SIMUL.run(10000, 5);

                cout << "The average interval of time in state 2 is " 
                     << stat_state2.getMean() 
                     << endl;
                cout << "with a 95% confidence interval of " 
                     << stat_state2.getConfInterval() << endl;
                cout << "The average interval of time in state 1 is " 
                     << stat_state1.getMean() << endl;
                cout << "with a 95% confidence interval of " 
                     << stat_state1.getConfInterval() << endl;
                cout << "The average interval of time in state 0 is " 
                     << stat_state0.getMean() << endl;
                cout << "with a 95% confidence interval of " 
                     << stat_state0.getConfInterval() << endl;

        } catch (exception& e) {
                cout << "Exception: " << e.what() << endl;
        }
}
