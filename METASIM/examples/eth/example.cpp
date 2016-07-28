#include <simul.hpp>

#include "link.hpp"
#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4101)
#endif

using namespace std;
using namespace MetaSim;

const double UMIN = 0.5;
const double UMAX = 0.9;
const double USTEP = 0.1;

const unsigned AVG_LEN = 800;
const Tick SIM_LEN = (int) (AVG_LEN * 1000);

class CollisionStat : public StatCount {
public:
        CollisionStat(const char *name) : StatCount(name) {}

        void probe(Event *e) 
        {
                record(1);
        }
  
        void attach(Entity *e) 
        {
                EthernetLink *l = dynamic_cast<EthernetLink *>(e);
                if (l == NULL) 
                        throw BaseExc("Please, specify a Ethernet Link!");
    
                l->_collision_evt.addStat(this);
        }
};

int main()
{
        double u = 0.0;

        Node n1("Node_1");
        Node n2("Node_2");
        Node n3("Node_3");

        n1.addDestNode(n2);
        n1.addDestNode(n3);
        n2.addDestNode(n1);
        n3.addDestNode(n1);

        EthernetLink link("Eth_Link");

        EthernetInterface int1("Interface_1", n1, link);
        EthernetInterface int2("Interface_2", n2, link);
        EthernetInterface int3("Interface_3", n3, link);

        CollisionStat stat("coll.txt");
        stat.attach(&link);
  
        GnuPlotOutput output;
        output.init();

        for (u=UMIN; u<= UMAX; u+=USTEP) {
    
                double l1 = 6 * AVG_LEN / u;
                double l2 = l1;
                double l3 = l1;

                n1.setInterval(auto_ptr<RandomVar>(new UniformVar(1,l1)));
                n2.setInterval(auto_ptr<RandomVar>(new UniformVar(1,l2)));
                n3.setInterval(auto_ptr<RandomVar>(new UniformVar(1,l3)));
    
                SIMUL.dbg.setStream("log.txt");
                SIMUL.dbg.enable(_ETHLINK_DBG);
                SIMUL.dbg.enable(_ETHINTER_DBG);
                SIMUL.dbg.enable(_NODE_DBG);
 
                try {
                        cout << "U = " << u << endl;
                        SIMUL.run(SIM_LEN, 5);
                        output.write(u);
                } catch (BaseExc &e) {
                        cout << e.what() << endl;
                }
        }
}
