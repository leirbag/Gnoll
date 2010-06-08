#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../messages/include/listenercontainer.h"
#include "../../messages/include/messagetype.h"
#include "../../messages/include/listener.h"
#include "../../messages/include/exceptions.h"

BOOST_AUTO_TEST_SUITE(TestListenerContainer)

using namespace Gnoll::Core::Messages;

namespace
{
	static std::string MESSAGE_TYPENAME_1("type1");
	static std::string MESSAGE_TYPENAME_2("type2");

	static std::string MESSAGE("message");
}

struct MockListener : public Listener
{
	MockListener() :
		countCalls(0),
		lastestMessage(NULL)
	{
	}

	~MockListener()
	{
	}

	void handle(MessagePtr receivedMessage)
	{
		lastestMessage = receivedMessage.get();
		++countCalls;
	}

	int countCalls;
	Gnoll::Core::Message * lastestMessage;
};

BOOST_AUTO_TEST_CASE(can_add_listeners_with_different_types)
{
	ListenerContainer container;

	ListenerContainer::ListenerPtr listener1(new MockListener);
	ListenerContainer::ListenerPtr listener2(new MockListener);
	ListenerContainer::ListenerPtr listener3(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));
	container.add(listener2, MessageType(MESSAGE_TYPENAME_2));
	container.add(listener3, MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK_THROW(container.add(listener1, MessageType(MESSAGE_TYPENAME_1)), Exceptions::HandlerAlreadyRegistered);
}

BOOST_AUTO_TEST_CASE(can_check_if_listener_is_listening_to_type)
{
	ListenerContainer container;

	ListenerContainer::ListenerPtr listener1(new MockListener);
	ListenerContainer::ListenerPtr listener2(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK( container.isListenerAssociatedToType(listener1, MessageType(MESSAGE_TYPENAME_1)));
	BOOST_CHECK(!container.isListenerAssociatedToType(listener1, MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK(!container.isListenerAssociatedToType(listener2, MessageType(MESSAGE_TYPENAME_1)));
	BOOST_CHECK(!container.isListenerAssociatedToType(listener2, MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK( container.hasListenerForType(MessageType(MESSAGE_TYPENAME_1)));
	BOOST_CHECK(!container.hasListenerForType(MessageType(MESSAGE_TYPENAME_2)));

}

BOOST_AUTO_TEST_CASE(can_delete_listener)
{
	ListenerContainer container;

	ListenerContainer::ListenerPtr listener1(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));
	container.del(listener1, MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK(!container.isListenerAssociatedToType(listener1, MessageType(MESSAGE_TYPENAME_1)));
	BOOST_CHECK(!container.hasListenerForType(MessageType(MESSAGE_TYPENAME_1)));
}

BOOST_AUTO_TEST_CASE(cannot_delete_non_added_listener)
{
	ListenerContainer container;
	ListenerContainer::ListenerPtr listener1(new MockListener);

	BOOST_CHECK_THROW(container.del(listener1, MessageType(MESSAGE_TYPENAME_1)), Exceptions::CannotDeleteListener);
}

struct CountForEach
{
	CountForEach(int * counter) :
		m_counter(counter)
	{
	}

	void operator()(ListenerContainer::ListenerPtr & listener)
	{
		++(*m_counter);
	}

	int * m_counter;
};

BOOST_AUTO_TEST_CASE(for_each_on_type_one_listener)
{
	int counter = 0;
	ListenerContainer::ForEachFunction countObject = CountForEach(&counter);

	ListenerContainer container;

	container.forEach(MessageType(MESSAGE_TYPENAME_1), countObject);
	BOOST_CHECK_EQUAL(0, counter);

	ListenerContainer::ListenerPtr listener1(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));
	container.forEach(MessageType(MESSAGE_TYPENAME_1), countObject);
	BOOST_CHECK_EQUAL(1, counter);

	counter = 0;
	container.add(listener1, MessageType(MESSAGE_TYPENAME_2));
	container.forEach(MessageType(MESSAGE_TYPENAME_1), countObject);
	BOOST_CHECK_EQUAL(1, counter);
}

BOOST_AUTO_TEST_CASE(for_each_on_type_several_listeners)
{
	int counter = 0;
	ListenerContainer::ForEachFunction countObject = CountForEach(&counter);

	ListenerContainer container;

	ListenerContainer::ListenerPtr listener1(new MockListener);
	ListenerContainer::ListenerPtr listener2(new MockListener);
	ListenerContainer::ListenerPtr listener3(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));
	container.add(listener2, MessageType(MESSAGE_TYPENAME_1));
	container.add(listener3, MessageType(MESSAGE_TYPENAME_1));
	container.forEach(MessageType(MESSAGE_TYPENAME_1), countObject);
	BOOST_CHECK_EQUAL(3, counter);
}

BOOST_AUTO_TEST_CASE(for_each_with_delete_listener)
{
	int counter = 0;
	ListenerContainer::ForEachFunction countObject = CountForEach(&counter);

	ListenerContainer container;

	ListenerContainer::ListenerPtr listener1(new MockListener);

	container.add(listener1, MessageType(MESSAGE_TYPENAME_1));
	container.del(listener1, MessageType(MESSAGE_TYPENAME_1));

	container.forEach(MessageType(MESSAGE_TYPENAME_1), countObject);
	BOOST_CHECK_EQUAL(0, counter);
}

BOOST_AUTO_TEST_SUITE_END()

