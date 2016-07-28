#ifndef __NETINTERFACE_HPP__
#define __NETINTERFACE_HPP__

#include <deque>
#include <vector>

#include <metasim.hpp>

#define _ETHINTER_DBG "EthernetInterface"

class Node;
class Message;
class EthernetLink;

class NetInterface : public MetaSim::Entity {
protected:
  Node *_node;
public:
  NetInterface(const char *name, Node &n);
  virtual ~NetInterface();
  
  virtual void send(Message *m) = 0;
  virtual Message * receive(Node *n) = 0;
  
  virtual void onMessageSent(Message *m) = 0;
  virtual void onMessageReceived(Message *m) = 0;
};

class EthernetInterface : public NetInterface {
protected:
  EthernetLink* _link;
  std::deque<Message*> _queue;
  std::vector<Message*> _received;
  std::vector<Node*> _blocked;

  int _cont_per;
  int _backoff;
  int _coll;

public:

  MetaSim::GEvent<EthernetInterface> _trans_evt;

  EthernetInterface(const char* name, Node& n, EthernetLink& l);
  virtual ~EthernetInterface();

  MetaSim::Tick nextTransTime();

  virtual void send(Message* m);
  virtual void onCollision();
  virtual void onTransmit(MetaSim::Event* e);
  virtual Message* receive(Node* n);
  virtual void onMessageSent(Message* m); 
  virtual void onMessageReceived(Message* m);

  void newRun();
  void endRun();
};

#endif
