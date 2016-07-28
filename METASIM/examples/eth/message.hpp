#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <metasim.hpp>

#define _MESSAGE_DBG "EthernetLink"

class Node;
class NetInterface;

class Message {

  int _len;

  //  partito dal nodo e arrivato all'interfaccia
  MetaSim::Tick _start_time;
  // partito dall'interfaccia e arrivato sul link
  MetaSim::Tick _trans_time;
  // fine della trasmission, arrivo sull'interfaccia destinazione
  MetaSim::Tick _arr_time;

  Node *_dst;
  Node *_src;

  NetInterface *_dst_net_interf;
  NetInterface *_src_net_interf;

public:

  Message(int l, Node *s, Node *d);

  void setStartTime(MetaSim::Tick t);
  MetaSim::Tick getStartTime();

  void setTransTime(MetaSim::Tick t);
  MetaSim::Tick getTransTime();

  void setArrTime(MetaSim::Tick t);
  MetaSim::Tick getArrTime();

  int getLength();

  void setSourceInterface(NetInterface *n);
  void setDestInterface(NetInterface *n);

  NetInterface *getSourceInterface();
  NetInterface *getDestInterface();

  Node *getSourceNode();
  Node *getDestNode();
};

#endif
