#include <algorithm>

#include <metasim.hpp>

#include "link.hpp"
#include "message.hpp"
#include "netinterface.hpp"
#include "node.hpp"

using namespace std;
using namespace MetaSim;

NetInterface::NetInterface(const char *name, Node &n) :
    Entity(name), _node(&n)
{
    _node->setNetInterface(*this);
}

NetInterface::~NetInterface()
{
}

EthernetInterface::EthernetInterface(const char *name, Node &n, EthernetLink &l) :
    NetInterface(name,n), _link(&l), _queue(), _received(), _blocked(),
    _trans_evt(this, &EthernetInterface::onTransmit)
{
    _cont_per = _link->getContentionPeriod();
    _backoff = _cont_per;
    _coll = 0;
}

EthernetInterface::~EthernetInterface()
{}

void EthernetInterface::newRun()
{
        vector<Message *>::iterator i;

        _queue.clear();
        for (i = _received.begin(); i != _received.end(); ++i) delete (*i);

        _received.clear();
        _blocked.clear();

}
 
void EthernetInterface::endRun()
{
}


void EthernetInterface::send(Message *m)
{
        DBGENTER(_ETHINTER_DBG);

        _queue.push_back(m);
  
        if (_queue.size() == 1) _trans_evt.process();
        else 
                DBGPRINT("Message enqueued");

        
}

void EthernetInterface::onTransmit(Event *e)
{
        DBGENTER(_ETHINTER_DBG);

        if (_link->isBusy()) onCollision();
        else _link->contend(this, _queue.front());

        
}

void EthernetInterface::onCollision()
{
        DBGENTER(_ETHINTER_DBG);

        _trans_evt.post(SIMUL.getTime() + nextTransTime());

        
}

void EthernetInterface::onMessageSent(Message *m)
{
        DBGENTER(_ETHINTER_DBG);

        _queue.pop_front();

        _coll = 0;
        _backoff = _cont_per;

        if (!_queue.empty()) _trans_evt.process();

        
}

Tick EthernetInterface::nextTransTime()
{
        DBGTAG(_ETHINTER_DBG, getName() + "::nextTransTime()");

        _coll++;

        if (_coll <= 10) _backoff *= 2;

        UniformVar a(1, _backoff);

        return (Tick) a.get();
}

void EthernetInterface::onMessageReceived(Message *m)
{
        DBGENTER(_ETHINTER_DBG);

        vector<Node *>::iterator i = find(_blocked.begin(), _blocked.end(), m->getDestNode());

        if (i != _blocked.end()) {
                (*i)->onMessageReceived(m);
                _blocked.erase(i);
        }
        else 
                _received.push_back(m);

        
}

Message * EthernetInterface::receive(Node *n)
{
        DBGTAG(_ETHINTER_DBG, getName() + "::receive()");

        vector<Message *>::iterator i = _received.begin();
        Message *m = NULL;

        while (i != _received.end()) {
                if ((*i)->getDestNode() == n) {
                        m = *i;
                        _received.erase(i);
                        return m;
                }
                else ++i;
        }
        _blocked.push_back(n);
        return m;
}

