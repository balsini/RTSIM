#include <metasim.hpp>

#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"

using namespace std;
using namespace MetaSim;

/*-----------------------------------------------------*/

Node::Node(string const & name) 
        : Entity(name), _net_interf(0), _interval(0),
          _nodes(), 
	  _recv_evt(this, &Node::onReceive), 
	  _send_evt(this, &Node::onSend)
{
    // register_handler(_recv_evt, this, &Node::onReceive);
    // register_handler(_send_evt, this, &Node::onSend);
}

void Node::newRun()
{
    _send_evt.post((int)_interval->get());
}

void Node::endRun()
{
}


NetInterface *Node::getNetInterface()
{
    return _net_interf;
}

void Node::setNetInterface(NetInterface &n)
{
    _net_interf = &n;
}

void Node::onReceive(Event *e)
{
    // simply, record the fact that the message has succesfully been 
    // received.
    
    DBGTAG(_NODE_DBG, getName() + "::onReceive()");
}

void Node::onMessageReceived(Message *m)
{
    // simply, record the fact that the message has succesfully been 
    // received.
    
    DBGTAG(_NODE_DBG, getName() + "::onMessageReceived()");
}

void Node::setInterval(auto_ptr<RandomVar> i)
{
    _interval = i;
}

void Node::addDestNode(Node &n)
{
    _nodes.push_back(&n);
}

void Node::onSend(Event *e)
{
    UniformVar len(100,1500);
    UniformVar n(0, _nodes.size());
    int i = (int)n.get();
    
    DBGENTER(_NODE_DBG);
    
    DBGPRINT("dest node = " << _nodes[i]->getName());
    // creates a new message and send it!! 
    Message *m = new Message((int)len.get(), this, _nodes[i]);
    _net_interf->send(m);
    _send_evt.post(SIMUL.getTime() + (Tick)_interval->get());    
}
