/*
 * myentity.hpp
 *
 *  Created on: Mar 14, 2011
 *      Author: lipari
 */

#ifndef MYENTITY_HPP_
#define MYENTITY_HPP_

#include <entity.hpp>
#include <gevent.hpp>

using namespace MetaSim;

class MyEntity : public MetaSim::Entity {
    bool afirst;
    bool bfirst;
    
    int count;
    
    MetaSim::GEvent<MyEntity> eventA;
    MetaSim::GEvent<MyEntity> eventB;
    MetaSim::GEvent<MyEntity> eventC;
    
public:
    MyEntity();
    MyEntity(const std::string &n);
    
    void onEventA(MetaSim::Event *);
    void onEventB(MetaSim::Event *);
    void onEventC(MetaSim::Event *);
    
    void newRun();
    void endRun();
    bool isAFirst();
    int getCounter();
};


#endif /* MYENTITY_HPP_ */
