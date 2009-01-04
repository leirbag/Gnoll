#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/singleton.h"

struct DummyClass
{
    DummyClass()
    {
        initialized = true;
    }

    ~DummyClass()
    {
        initialized = false;
    }

    static bool initialized;
};

bool DummyClass::initialized = false;

BOOST_AUTO_TEST_CASE( GetInstancePointerIsNotNull )
{
    DummyClass * dummy = Gnoll::Core::Singleton<DummyClass>::getInstancePtr();

    BOOST_CHECK(NULL != dummy);

    Gnoll::Core::Singleton<DummyClass>::destroy();
}

BOOST_AUTO_TEST_CASE( GettingInstanceInitializesClass )
{
    BOOST_CHECK( !DummyClass::initialized );
    DummyClass * dummy = Gnoll::Core::Singleton<DummyClass>::getInstancePtr();
    BOOST_CHECK( DummyClass::initialized );

    Gnoll::Core::Singleton<DummyClass>::destroy();
}

BOOST_AUTO_TEST_CASE( DestroyingSingletonCallsDestructor )
{
    DummyClass * dummy = Gnoll::Core::Singleton<DummyClass>::getInstancePtr();
    Gnoll::Core::Singleton<DummyClass>::destroy();
    BOOST_CHECK( !DummyClass::initialized );
}

BOOST_AUTO_TEST_CASE( DestroyingWithoutGettingPointerIsSafe )
{
    BOOST_CHECK( !DummyClass::initialized );
    Gnoll::Core::Singleton<DummyClass>::destroy();
    BOOST_CHECK( !DummyClass::initialized );
}

BOOST_AUTO_TEST_CASE( GettingPointerTwiceGivesSameInstance )
{
    DummyClass * dummy1 = Gnoll::Core::Singleton<DummyClass>::getInstancePtr();
    DummyClass * dummy2 = Gnoll::Core::Singleton<DummyClass>::getInstancePtr();

    BOOST_CHECK_EQUAL(dummy1, dummy2);
}

