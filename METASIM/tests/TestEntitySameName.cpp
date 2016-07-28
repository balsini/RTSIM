#include <iostream>
#include <memory>
#include <entity.hpp>
#include "myentity.hpp"
#include <catch.hpp>

using namespace MetaSim;

TEST_CASE("TestEntitySameName", "testName")
{
    std::unique_ptr<MyEntity> me(new MyEntity("name"));

    Entity *p = Entity::_find("name");
    REQUIRE( p == me.get() );
    REQUIRE( p == Entity::getPointer( me->getID() ) );
    
    try {
        MyEntity e2("name");
        REQUIRE( false );
    } catch (...) {
        REQUIRE( true );
    }
    p = Entity::getPointer( me->getID() + 1 );
    REQUIRE( p == 0 );
}
