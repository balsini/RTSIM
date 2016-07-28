/***************************************************************************
    begin                : Thu Apr 24 15:54:58 CEST 2003
    copyright            : (C) 2003 by Giuseppe Lipari
    email                : lipari@sssup.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <typeinfo>
#include <sstream>

#include <entity.hpp>
#include <simul.hpp>


namespace MetaSim {

    using namespace std;

    map<int, Entity*> Entity::_globMap;
    map<string, Entity *> Entity::_index;
    int Entity::_IDcount = 0;

    void Entity::_init()
    {
        if (_name == "") {
            std::stringstream ss;
            ss << _IDcount + 1;
            _name = string(typeid(*this).name()) + ss.str();
        }

	if (_index.find(_name) != _index.end())
  	    throw Exc("Creating an entity with the same name " + _name);

        _IDcount++;
        _ID = _IDcount;
        _globMap[_ID] = this;

        DBGENTER(_ENTITY_DBG_LEV);

        DBGPRINT_2("Entity ID: ", _ID);
        DBGPRINT_2("Entity type: ",  typeid(*this).name());
        DBGPRINT_2("Entity name:", _name);

	_index[_name] = this;
    }

    // Entity::Entity(const char *n) : _name(n) 
    //         {
    //                 _init();
    //         }

    Entity::Entity(const string &n) : _name(n) 
    {
        _init();
    }

    Entity::~Entity()
    {
        _globMap.erase(_ID);
        _index.erase(_name);
    }

    void Entity::callNewRun()
    {
  
        typedef map<int, Entity*>::iterator EI;

        EI p = _globMap.begin();

        while (p != _globMap.end()) {
            DBGENTER(_ENTITY_DBG_LEV);
            DBGPRINT_2("Calling the newRun() of ",
                       p->second->getID());
            

            p->second->newRun();
            p++;
        }
                
                
    }

    void Entity::callEndRun()
    {
        typedef map<int, Entity*>::iterator EI;

        EI p = _globMap.begin();
        while (p != _globMap.end()) {
            p->second->endRun();
            p++;
        }
                
                
    }

    Entity *Entity::_find(string n)
    {
        Entity *res = 0;
 
        typedef map<string, Entity *>::iterator NI;

        NI i = _index.find(n);
        if (i != _index.end()) res = (*i).second;
        return res;
    }

} // end namespace MetaSim
