#include <metasim.hpp>

#include "link.hpp"
#include "message.hpp"

using namespace std;
using namespace MetaSim;

Link::Link(const char *name) : Entity(name)
{
}

Link::~Link()
{
}

EthernetLink::EthernetLink(const char *name) 
    : Link(name), _interfaces(), _contending(), 
      _isBusy(false),
      _isContending(false),
      _isCollision(false),
      _contention_period(10),
      _message(0),
      _end_contention_evt(this, &EthernetLink::onEndContention),
      _collision_evt(this, &EthernetLink::onCollision),
      _end_transmission_evt(this, &EthernetLink::onEndTransmission)
{
    //register_handler(_end_contention_evt, this, &EthernetLink::onEndContention);
    //register_handler(_collision_evt, this, &EthernetLink::onCollision);
    //register_handler(_end_transmission_evt, this, &EthernetLink::onEndTransmission);
}

EthernetLink::~EthernetLink()
{
}

void EthernetLink::newRun()
{
    _contending.clear();
    _isBusy = false;
    _isContending = false;
    _isCollision = false;
    _message = 0;
}
 
void EthernetLink::endRun()
{
}

bool EthernetLink::isBusy()
{
    return _isBusy;
}

void EthernetLink::send(Message *m)
{
}

void EthernetLink::contend(EthernetInterface *eth, Message *m)
{
    DBGENTER(_ETHLINK_DBG);
 
    if (_isContending) {
	_end_contention_evt.drop();
	if (!_isCollision) {
	    _isCollision = true;
	    _collision_evt.post(SIMUL.getTime() + 3);
	}
    }
    else {
	_isContending = true;
	_message = m;
	_end_contention_evt.post(SIMUL.getTime() + _contention_period);
    }
    _contending.push_back(eth);

        
}

void EthernetLink::onEndContention(Event *e)
{
        DBGENTER(_ETHLINK_DBG);

        _isContending = false;
        _isBusy = true;
        _end_transmission_evt.post(SIMUL.getTime() + _message->getLength());

        _contending.clear();

        
}

void EthernetLink::onCollision(Event *e)
{
    DBGENTER(_ETHLINK_DBG);

    _isContending = false;
    _isCollision = false;
    _isBusy = false;
    while (!_contending.empty()) {
	_contending.back()->onCollision();
	_contending.pop_back();
    }
    _message = 0;        
}

void EthernetLink::onEndTransmission(Event *e)
{
    Message *m = _message;

    DBGENTER(_ETHLINK_DBG);

    NetInterface *dst = _message->getDestInterface();
    NetInterface *src = _message->getSourceInterface();

    _isBusy = false;
    _message = 0;

    dst->onMessageReceived(m);
    src->onMessageSent(m);        
}
