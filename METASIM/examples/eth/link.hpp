#ifndef __LINK_HPP__
#define __LINK_HPP__

#include <vector>

#include <entity.hpp>
#include <event.hpp>

#include "netinterface.hpp"

const char* const _ETHLINK_DBG = "EthernetLink";

class Message;

class Link : public MetaSim::Entity {
public:
        Link(const char *name);
        virtual ~Link();

        virtual void send(Message *m) = 0;
};

class EthernetLink : public Link {
        std::vector<EthernetInterface *> _interfaces;
        std::vector<EthernetInterface *> _contending;

        bool _isBusy;
        bool _isContending;
        bool _isCollision;
        int _contention_period;

        Message *_message;

public:

        MetaSim::GEvent<EthernetLink> _end_contention_evt;
        MetaSim::GEvent<EthernetLink> _collision_evt;
        MetaSim::GEvent<EthernetLink> _end_transmission_evt;

        EthernetLink(const char *name);
        virtual ~EthernetLink();

        bool isBusy();

        virtual void send(Message *m);

        void contend(EthernetInterface *eth, Message *m);
        void onEndContention(MetaSim::Event *e);
        void onCollision(MetaSim::Event *e);
        void onEndTransmission(MetaSim::Event *e);

        void setContentionPeriod(int p) { _contention_period = p; }
        int getContentionPeriod() { return _contention_period; }

        void newRun(); 
        void endRun(); 
};

#endif
