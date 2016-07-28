/*
 * myentity.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: lipari
 */
#include "myentity.hpp"
#include <iostream>

using namespace std;

MyEntity::MyEntity() : Entity(""),
		       afirst(false),
		       bfirst(false),
		       count(0),
		       eventA(this, &MyEntity::onEventA),
		       eventB(this, &MyEntity::onEventB),
		       eventC(this, &MyEntity::onEventC)
{
    // register_handler(eventA, this, &MyEntity::onEventA);
    // register_handler(eventB, this, &MyEntity::onEventB);
    // register_handler(eventC, );
}

MyEntity::MyEntity(const std::string &n) : Entity(n),
					   afirst(false),
					   bfirst(false),
					   count(0),
					   eventA(this, &MyEntity::onEventA),
					   eventB(this, &MyEntity::onEventB),
					   eventC(this, &MyEntity::onEventC)
{
    // register_handler(eventA, this, &MyEntity::onEventA);
    // register_handler(eventB, this, &MyEntity::onEventB);
    // register_handler(eventC, this, &MyEntity::onEventC);
}

void MyEntity::onEventA(Event *) {
    if (!bfirst) afirst = true;
    eventA.post(SIMUL.getTime() + 5);
}

void MyEntity::onEventB(Event *) {
    if (!afirst) bfirst = true;
    eventB.post(SIMUL.getTime()+10);
}

void MyEntity::onEventC(Event *) {
    if (afirst)
	count ++;
    afirst = bfirst = false;
    eventC.post(SIMUL.getTime()+5);
}

void MyEntity::newRun()
{
    afirst = bfirst = false;
    eventA.post(0);
    eventB.post(0);
    eventC.post(1);
    count = 0;
}
void MyEntity::endRun() {}
bool MyEntity::isAFirst() { return afirst; }
int MyEntity::getCounter() { return count; }

