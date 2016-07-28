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
/*
 * $Id: entity.hpp,v 1.6 2005/10/05 16:46:22 cesare Exp $
 *
 * $Log: entity.hpp,v $
 * Revision 1.6  2005/10/05 16:46:22  cesare
 * Added DLL support.
 *
 * Revision 1.5  2005/04/28 01:34:47  cesare
 * Moved to sstream. Headers install. Code cleaning.
 *
 * Revision 1.4  2004/11/24 14:12:13  lipari
 * Merge between the branch and the main trunk.
 *
 * 
 */
#ifndef __ENTITY_HPP___
#define __ENTITY_HPP___

#include <map>
#include <string>

#include <baseexc.hpp>
#include <basetype.hpp>

namespace MetaSim {

#define _ENTITY_DBG_LEV "Entity"

        /** 
            The base class for every simulation object.  An entity has
            an internal status, an interface for modyfing the status,
            and can contain one or more events.  An entity can be
            referred also by its name (a string of characters) using
            the static method <i>find</i>. A specific entity class
            should redefine the <i>find</i> function for doing type
            checking.

            @author Giuseppe Lipari  
            @version 1.0 
            @sa Event 

            \ingroup metasim_ee   
        */
    class Entity {
	// Hide copy constructor and assignment operator. Entities are
	// not copyable.
	Entity(const Entity &);
	Entity &operator=(const Entity &);

	/** 
	    It contains pairs <ID, pointer to entity>. It's
	    used mainly to keep track of all the entities
	    present in the system. */
	static std::map<int , Entity *> _globMap;
  
	/**
	   It contains pairs <string, pointer to entity>. It
	   is used to refer entities by name. The string must
	   be unique!! */
	static std::map<std::string, Entity *> _index;

	/// counter for assigning unique IDs to entities
	static int _IDcount;

	/// unique ID for the entity
	int _ID;

	/// unique name for the entity
	std::string _name;

	/**
	   \ingroup metasim_exc
       
	   Exceptions for the Entity class.
	*/
	class Exc : public BaseExc {
	public:
	    Exc(std::string msg) : 
		BaseExc(msg, "Entity", "entity.cpp") {}
	};

	/**
	   Initialize the entity. It is called by the
	   different constructors, Entity(char *) and
	   Entity(string) */
	void _init();

    public:
	/** 
	    Base constructor.
      
	    @param n A (possibly unique) name for the entity:
	    by default the entity name is set equal to the
	    typeid() + the entity ID. */
	Entity(const std::string &n);
  
	/// Destructor
	virtual ~Entity();

	/** 
	    Obtains the pointer to the object from the ID.
	    Quite useful for debugging.
	    
	    @param id the object ID.  

	    @returns the pointer to the entity corresponding
	    to that ID, or NULL if it doesn't exist an object
	    with that ID. */
	static inline Entity* getPointer(int id)
	    {
		std::map<int, Entity*>::const_iterator p = _globMap.find(id);
		if (p == _globMap.end()) return NULL;
		else return p->second;
	    };

	/** 
	    Returns the pointer to the entity with the
	    spoecified name or NULL if such entity does not
	    exists.  */
	static Entity * _find(std::string n);  

	/** 
	    Calls newRun() on every entity in the system.  It is
	    automatically called at the beginning of every run by the
	    Simulation class. Not to be called by the user!!  In a
	    future release will be hidden.
      
	    @see callNewRun */
	static void callNewRun();

	/** 
	    The same as callNewRun(), but it is called at the
	    end of every run.
       
	    @see callEndRun */
	static void callEndRun();

	/// Get the entity ID
	inline int getID() const { return _ID; }

	///Get the Entity name
	inline std::string getName() const { return _name; }

	/** 
	    Resets the entity status at the beginning of every
	    run.  This function is called automatically at the
	    beginning of every run, and its job consists in
	    initializing the entity status.  Warning: in
	    newRun() is not permitted to create/destroy new
	    entity objects. */
	virtual void newRun() = 0;
  
	/** 
	    Reset the entity status at the end of every run.
	    This function is called automatically at the end
	    of every run, and its job consists in resetting
	    the entity status (for example closing files,
	    etc.)  Warning: in endRun() is not permitted to
	    create/destroy new entity objects. */
	virtual void endRun() = 0;
    };
}

#endif
