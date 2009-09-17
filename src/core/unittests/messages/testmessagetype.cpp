#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../messages/include/messagetype.h"

BOOST_AUTO_TEST_SUITE(TestMessageType)

using namespace Gnoll::Core::Messages;

static const std::string TYPENAME_ONE("randomType");
static const std::string TYPENAME_TWO("anotherType");

BOOST_AUTO_TEST_CASE(is_copy_constructible)
{
	MessageType type(TYPENAME_ONE);
	MessageType copiedType(type);

	BOOST_CHECK(type == copiedType);
}

BOOST_AUTO_TEST_CASE(can_be_assigned)
{
	MessageType type(TYPENAME_ONE);
	MessageType copiedType(TYPENAME_TWO);

	copiedType = type;

	BOOST_CHECK(type == copiedType);
}

BOOST_AUTO_TEST_CASE(inequality_is_not_equality)
{
	MessageType type(TYPENAME_ONE);
	MessageType otherType(TYPENAME_TWO);

	BOOST_CHECK(type != otherType);
	BOOST_CHECK(!(type == otherType));
}

BOOST_AUTO_TEST_CASE(type_can_deliver_type_as_string)
{
	MessageType type(TYPENAME_ONE);
	MessageType otherType(TYPENAME_TWO);

	BOOST_CHECK_EQUAL(TYPENAME_ONE, type.getTypeStr());
	BOOST_CHECK_EQUAL(TYPENAME_TWO, otherType.getTypeStr());
}

BOOST_AUTO_TEST_CASE(order_relation_is_string_relation)
{
	MessageType type(TYPENAME_ONE);
	MessageType otherType(TYPENAME_TWO);

	BOOST_CHECK(TYPENAME_TWO < TYPENAME_ONE);
	BOOST_CHECK(otherType < type);
}

BOOST_AUTO_TEST_SUITE_END()

