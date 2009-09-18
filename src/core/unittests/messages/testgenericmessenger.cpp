#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../messages/include/genericmessenger.h"
#include "../../messages/include/messagetype.h"
#include "../../messages/include/listener.h"
#include "../../messages/include/exceptions.h"
#include "../../include/cmessage.h"

BOOST_AUTO_TEST_SUITE(TestGenericMessenger)

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
	Gnoll::Core::CMessage * lastestMessage;
};

BOOST_AUTO_TEST_CASE(empty_message_type_throws)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	BOOST_CHECK_THROW(messenger.addListener(listener1, MessageType("")), Exceptions::InvalidMessage);
}

BOOST_AUTO_TEST_CASE(can_add_listeners_with_different_types)
{
	GenericMessenger messenger;

	Messenger::ListenerPtr listener1(new MockListener);
	Messenger::ListenerPtr listener2(new MockListener);
	Messenger::ListenerPtr listener3(new MockListener);

	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
	messenger.addListener(listener2, MessageType(MESSAGE_TYPENAME_2));
	messenger.addListener(listener3, MessageType(MESSAGE_TYPENAME_1));

	BOOST_CHECK_THROW(messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1)), Exceptions::HandlerAlreadyRegistered);
}

BOOST_AUTO_TEST_CASE(can_delete_listener)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
	messenger.delListener(listener1, MessageType(MESSAGE_TYPENAME_1));
}

BOOST_AUTO_TEST_CASE(cannot_delete_listener_if_wrong_type)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
	BOOST_CHECK_THROW(messenger.delListener(listener1, MessageType(MESSAGE_TYPENAME_2)), Exceptions::CannotDeleteListener);
}

BOOST_AUTO_TEST_CASE(cannot_delete_non_added_listener)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	BOOST_CHECK_THROW(messenger.delListener(listener1, MessageType(MESSAGE_TYPENAME_1)), Exceptions::CannotDeleteListener);
}

BOOST_AUTO_TEST_CASE(cannot_trigger_when_no_listener)
{
	GenericMessenger messenger;
	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK_THROW(messenger.triggerMessage(message), Exceptions::NoOneIsListening);
}

BOOST_AUTO_TEST_CASE(cannot_trigger_when_listener_with_different_type)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK_THROW(messenger.triggerMessage(message), Exceptions::NoOneIsListening);
}

struct ThreeListenersFixture
{
	ThreeListenersFixture() :
		listener1(new MockListener),
		listener2(new MockListener),
		listener3(new MockListener)
	{
		messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
		messenger.addListener(listener2, MessageType(MESSAGE_TYPENAME_2));
		messenger.addListener(listener3, MessageType(MESSAGE_TYPENAME_1));
	}

	GenericMessenger messenger;

	boost::shared_ptr<MockListener> listener1;
	boost::shared_ptr<MockListener> listener2;
	boost::shared_ptr<MockListener> listener3;
};

BOOST_FIXTURE_TEST_CASE(trigger_calls_listeners_for_correct_type, ThreeListenersFixture)
{
	Messenger::MessagePtr message1(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)));
	messenger.triggerMessage(message1);

	BOOST_CHECK_EQUAL(1, listener1->countCalls);
	BOOST_CHECK_EQUAL(0, listener2->countCalls);
	BOOST_CHECK_EQUAL(1, listener3->countCalls);

	Messenger::MessagePtr message2(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));
	messenger.triggerMessage(message2);

	BOOST_CHECK_EQUAL(1, listener1->countCalls);
	BOOST_CHECK_EQUAL(1, listener2->countCalls);
	BOOST_CHECK_EQUAL(1, listener3->countCalls);
}

BOOST_AUTO_TEST_CASE(cannot_queue_when_no_listener)
{
	GenericMessenger messenger;
	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK_THROW(messenger.queueMessage(message), Exceptions::NoOneIsListening);
}

BOOST_AUTO_TEST_CASE(cannot_queue_when_listener_with_different_type)
{
	GenericMessenger messenger;
	Messenger::ListenerPtr listener1(new MockListener);

	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));
	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));

	BOOST_CHECK_THROW(messenger.queueMessage(message), Exceptions::NoOneIsListening);
}

BOOST_FIXTURE_TEST_CASE(process_after_queue_calls_listeners_for_correct_type, ThreeListenersFixture)
{
	Messenger::MessagePtr message1(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)));
	messenger.queueMessage(message1);

	BOOST_CHECK_EQUAL(0, listener1->countCalls);
	BOOST_CHECK_EQUAL(0, listener2->countCalls);
	BOOST_CHECK_EQUAL(0, listener3->countCalls);

	Messenger::MessagePtr message2(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2)));
	messenger.queueMessage(message2);
	messenger.queueMessage(message1);

	BOOST_CHECK_EQUAL(0, listener1->countCalls);
	BOOST_CHECK_EQUAL(0, listener2->countCalls);
	BOOST_CHECK_EQUAL(0, listener3->countCalls);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(2, listener1->countCalls);
	BOOST_CHECK_EQUAL(1, listener2->countCalls);
	BOOST_CHECK_EQUAL(2, listener3->countCalls);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(2, listener1->countCalls);
	BOOST_CHECK_EQUAL(1, listener2->countCalls);
	BOOST_CHECK_EQUAL(2, listener3->countCalls);
}

struct ThreeMessagesType121 : public ThreeListenersFixture
{
	ThreeMessagesType121() :
		message1(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1))),
		message2(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_2))),
		message3(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)))
	{
		messenger.queueMessage(message1);
		messenger.queueMessage(message2);
		messenger.queueMessage(message3);
	}

	Messenger::MessagePtr message1;
	Messenger::MessagePtr message2;
	Messenger::MessagePtr message3;
};

BOOST_FIXTURE_TEST_CASE(abort_first_of_type_1, ThreeMessagesType121)
{
	messenger.abortFirstMessage(MESSAGE_TYPENAME_1);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(1, listener1->countCalls);
	BOOST_CHECK_EQUAL(message3.get(), listener1->lastestMessage);
	BOOST_CHECK_EQUAL(1, listener2->countCalls);
	BOOST_CHECK_EQUAL(message2.get(), listener2->lastestMessage);
}

BOOST_FIXTURE_TEST_CASE(abort_all_of_type_1, ThreeMessagesType121)
{
	messenger.abortAllMessages(MESSAGE_TYPENAME_1);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(0, listener1->countCalls);
	BOOST_CHECK_EQUAL(1, listener2->countCalls);
	BOOST_CHECK_EQUAL(message2.get(), listener2->lastestMessage);
}

BOOST_FIXTURE_TEST_CASE(abort_first_of_type_2, ThreeMessagesType121)
{
	messenger.abortFirstMessage(MESSAGE_TYPENAME_2);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(2, listener1->countCalls);
	BOOST_CHECK_EQUAL(message3.get(), listener1->lastestMessage);
	BOOST_CHECK_EQUAL(0, listener2->countCalls);
}

BOOST_FIXTURE_TEST_CASE(abort_all_of_type_2, ThreeMessagesType121)
{
	messenger.abortAllMessages(MESSAGE_TYPENAME_2);

	messenger.processQueue();

	BOOST_CHECK_EQUAL(2, listener1->countCalls);
	BOOST_CHECK_EQUAL(message3.get(), listener1->lastestMessage);
	BOOST_CHECK_EQUAL(0, listener2->countCalls);
}

struct MockLoopListener : public MockListener
{
	MockLoopListener(Messenger & messengerToCall) :
		messenger(messengerToCall),
		loopMessage(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)))
	{
	}

	~MockLoopListener()
	{
	}

	void handle(MessagePtr receivedMessage)
	{
		if (countCalls == 0)
		{
				messenger.queueMessage(loopMessage);
		}

		MockListener::handle(receivedMessage);
	}

	Messenger & messenger;
	Messenger::MessagePtr loopMessage;
};

BOOST_AUTO_TEST_CASE(listener_adding_a_message_does_not_cause_a_loop)
{
	GenericMessenger messenger;

	boost::shared_ptr<MockListener> listener1(new MockLoopListener(messenger));
	messenger.addListener(listener1, MessageType(MESSAGE_TYPENAME_1));

	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)));
	messenger.queueMessage(message);

	messenger.processQueue();
	BOOST_CHECK_EQUAL(1, listener1->countCalls);

	messenger.processQueue();
	BOOST_CHECK_EQUAL(2, listener1->countCalls);

	messenger.processQueue();
	BOOST_CHECK_EQUAL(2, listener1->countCalls);
}

// But if the listener triggers a message ? No protection...

// Is it really what we want ?
BOOST_AUTO_TEST_CASE(cannot_listen_to_anytype_messages)
{
	GenericMessenger messenger;

	Messenger::ListenerPtr listener1(new MockListener);
	messenger.addListener(listener1, MessageType(Gnoll::Core::MSG_ANYTYPE));

	Messenger::MessagePtr message(new Gnoll::Core::CMessage(MessageType(MESSAGE_TYPENAME_1)));

	BOOST_CHECK_THROW(messenger.triggerMessage(message), Exceptions::NoOneIsListening);
}

BOOST_AUTO_TEST_SUITE_END()

