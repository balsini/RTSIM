#include <deque>

#include <metasim.hpp>
#include <particle.hpp>

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

using namespace MetaSim;

/**
 *  The Node class represents a generic node in the system. A node can
 *  be a sink, a source, or a queue. It has a pure virtual method,
 *  <i>put()</i>, that inserts a packet in the node. */
class Node : public Entity {
public:
  Node(const char *n) : Entity(n) {}

  virtual void put() = 0;

  virtual void newRun() {}
  virtual void endRun() {}
  virtual void print() {}
};


/**
 * This class models a sink. The put method does nothing else than
 * recording how many packet have been consumed since the beginning.  */
class Sink : public Node {
  int _consumed;
public:
  Sink(const char *n) : Node(n), _consumed(0) {}
  virtual void put() { _consumed ++;}
  virtual void newRun() { _consumed = 0;}
  virtual void endRun() {}
  virtual void print() {}
};


class Source : public Node {
  RandomVar* _at;
  Node* _dest;
public:
  class ProduceEvent : public Event {
    Source& _n;
  public:
    ProduceEvent(Source &n) : 
      Event(),
      _n(n) 
    {
    }
    
    virtual void doit()
    {
      _n.produce();
    }

  };
  
  ProduceEvent _prodEvent;
  
  Source(Node* d, RandomVar* a, const char* n) : 
    Node(n),
    _at(a),
    _dest(d),
    _prodEvent(*this)
  {
  }

  virtual void put() {};

  void produce()
  {
    _dest->put();
    _prodEvent.post(SIMUL.getTime() + Tick(_at->get()));
  }

  virtual void newRun()
  {
    _prodEvent.post(Tick(_at->get()));
  }

  virtual void endRun() {}
  virtual void print() {}
};

/**
 * This class implements a generic queue with one server, with
 * randomly distributed service times. The service time is independent
 * form the packet!! After servicing a packet, the queue node will
 * send the packet to the destination node, that can be another queue
 * or a sink. In this way we can specify simple networks of queues. */
class Queue: public Node {
        /// The destination node 
        Node* _dest;

        /** 
         *  The internal queue representation. See a description of the stl
         *  for more details on the deque (double ended queue) data
         *  structure.  This is a queue of integers: in this simple example,
         *  every packet has lenght 1. However, it is quite easy to define
         *  a sligtly different model in which the service time depends on
         *  the packet length...*/
        std::deque<int> _q;

        /**
         * The service time random variable. It is possible to define a
         * general distribution! */
        RandomVar *_st;

public:
        /**
         *  This class models an event of packed served. When a packet is
         *  ready to be served, an event of this type is <i>posted</i> in the
         *  future. When that time comes, the <i>doit()</i> method is
         *  invoked. */
        class ServiceEvent : public Event {
                Queue& _queue;
        public:
                ServiceEvent(Queue &q) : 
                        Event(_DEFAULT_PRIORITY - 1),
                        _queue(q) 
                {
                }
    
                virtual void doit()
                {
                        _queue.serve();
                }
        };

        /// The event of served packet.
        ServiceEvent _servEvent;

        /**
         * Constructor for a queue. 
         * @param d   pointer to the destination node
         * @param st  service time random variable
         * @param n   a simbolyc name for the queue.
         */
        Queue(Node* d, RandomVar* st, const char* n) : 
                Node(n),
                _dest(d),
                _q(),
                _st(st),
                _servEvent(*this) 
        {
        }
  
        virtual void put()
        {
                _q.push_back(1);
                if (_q.size() == 1)
                        _servEvent.post(SIMUL.getTime() + Tick(_st->get()));
        }

        void serve()
        {
                _q.pop_front();
                if (_q.size() != 0) 
                        _servEvent.post(SIMUL.getTime() + Tick(_st->get()));
                _dest->put();
        }

        inline int getSize() { return (int) _q.size(); }

        virtual void newRun() 
        {
                _q.clear();
        } 

        virtual void endRun() {}
        virtual void print() {}
};//xxx


/* ----------------------------------------------------------------------*/

class AvgQueueSizeStat : public StatMean {
        Queue &_queue;
        Particle<Source::ProduceEvent,AvgQueueSizeStat> *sp; 
public:
        AvgQueueSizeStat(Queue &q, const char *n) :
                StatMean(n),
                _queue(q), 
                sp(0)
        {
        }
  
        void probe(Source::ProduceEvent &e)
        {
                record(_queue.getSize()-1);
        }

        //   void probe(Event *e)
        //   {
        //     record(_queue.getSize()-1);
        //   }

        //   virtual void attach(Entity *e)
        //   {
        //     Source *n = dynamic_cast<Source*>(e);
        //     if (n == NULL) 
        //       throw BaseExc("Cannot dynamic_cast<Source*>(Entity*e)");
        //     (n->_prodEvent).addStat(this);
        //   }

        virtual void attach(Source *n) 
        {
                // create and add a new particle to collect stats to this stat object
            sp = new Particle<Source::ProduceEvent,AvgQueueSizeStat>(&n->_prodEvent, 
                                                                         this);
        }
};
