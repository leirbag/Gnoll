#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/finitestatemachine.h"

#define INIT_CALL_COUNT(METHODNAME) \
	METHODNAME##CallCount = 0;

#define MOCK_METHOD_VOID(METHODNAME, SIGNATURE) \
	int METHODNAME##CallCount; \
	void METHODNAME SIGNATURE \
	{ \
		++METHODNAME##CallCount; \
	}

#define MOCK_METHOD_RETURN(RETURNTYPE, METHODNAME, SIGNATURE) \
	int METHODNAME##CallCount; \
	RETURNTYPE METHODNAME##ReturnValue; \
	RETURNTYPE METHODNAME SIGNATURE \
	{ \
		++METHODNAME##CallCount; \
		return METHODNAME##ReturnValue; \
	}

struct MockState : public Gnoll::Core::State
{
	MockState()
	{
		INIT_CALL_COUNT(onInit);
		INIT_CALL_COUNT(onProcess);
		INIT_CALL_COUNT(onExit);
	}

	~MockState()
	{
	}

	MOCK_METHOD_VOID(onInit, ());
	MOCK_METHOD_VOID(onProcess, ());
	MOCK_METHOD_VOID(onExit, ());

};

struct MockTransition : public Gnoll::Core::Transition
{
	MockTransition()
	{
		INIT_CALL_COUNT(isValid);
	}

	~MockTransition()
	{
	}

	MOCK_METHOD_RETURN(bool, isValid, ());

};

// Setting states
BOOST_AUTO_TEST_CASE( SettingActiveStateCallsInitAndInitOnly )
{
	Gnoll::Core::CFSM fsm;
	boost::shared_ptr<MockState> mockState(new MockState);

	fsm.setActiveState(mockState);

	BOOST_CHECK_EQUAL(1, mockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, mockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(0, mockState->onExitCallCount);
}

BOOST_AUTO_TEST_CASE( SettingAnotherActiveStateCallsInitAndInitOnly )
{
	Gnoll::Core::CFSM fsm;
	boost::shared_ptr<MockState> firstMockState(new MockState);
	boost::shared_ptr<MockState> secondMockState(new MockState);

	fsm.setActiveState(firstMockState);
	fsm.setActiveState(secondMockState);

	BOOST_CHECK_EQUAL(1, secondMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onExitCallCount);
}

BOOST_AUTO_TEST_CASE( SettingAnotherActiveStateCallsExitOnOldState )
{
	Gnoll::Core::CFSM fsm;
	boost::shared_ptr<MockState> firstMockState(new MockState);
	boost::shared_ptr<MockState> secondMockState(new MockState);

	fsm.setActiveState(firstMockState);
	fsm.setActiveState(secondMockState);

	BOOST_CHECK_EQUAL(1, firstMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, firstMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(1, firstMockState->onExitCallCount);
}

struct TwoStatesAndATransition
{
	TwoStatesAndATransition() :
		firstMockState(new MockState),
		secondMockState(new MockState),
		mockTransition(new MockTransition)
	{
	}

	Gnoll::Core::CFSM fsm;
	boost::shared_ptr<MockState> firstMockState;
	boost::shared_ptr<MockState> secondMockState;
	boost::shared_ptr<MockTransition> mockTransition;
};

BOOST_FIXTURE_TEST_CASE(ExecutingWithoutInitialStateDoesNothing, TwoStatesAndATransition)
{
	fsm.execute();

	BOOST_CHECK_EQUAL(0, firstMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, firstMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(0, firstMockState->onExitCallCount);

	BOOST_CHECK_EQUAL(0, secondMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onExitCallCount);
}

BOOST_FIXTURE_TEST_CASE(ExecutingChecksValidityOfTransition, TwoStatesAndATransition)
{
	fsm.add(firstMockState, secondMockState, mockTransition);
	fsm.setActiveState(firstMockState);
	fsm.execute();

	BOOST_CHECK_EQUAL(1, mockTransition->isValidCallCount);
}

BOOST_AUTO_TEST_CASE( ExecutingExecutesTheCurrentState )
{
	Gnoll::Core::CFSM fsm;
	boost::shared_ptr<MockState> firstMockState(new MockState);

	fsm.setActiveState(firstMockState);
	fsm.execute();

	BOOST_CHECK_EQUAL(1, firstMockState->onProcessCallCount);
}

BOOST_FIXTURE_TEST_CASE( ExecutingWithNoValidTransitionKeepsExecutingTheSameState, TwoStatesAndATransition)
{
	fsm.add(firstMockState, secondMockState, mockTransition);
	mockTransition->isValidReturnValue = false;

	fsm.setActiveState(firstMockState);
	fsm.execute();
	fsm.execute();

	BOOST_CHECK_EQUAL(2, firstMockState->onProcessCallCount);
}

BOOST_FIXTURE_TEST_CASE( ExecutingWithAValidTransitionChangeState, TwoStatesAndATransition)
{
	fsm.add(firstMockState, secondMockState, mockTransition);
	mockTransition->isValidReturnValue = true;

	fsm.setActiveState(firstMockState);
	fsm.execute();

	BOOST_CHECK_EQUAL(1, firstMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(1, firstMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(1, firstMockState->onExitCallCount);

	BOOST_CHECK_EQUAL(1, secondMockState->onInitCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onProcessCallCount);
	BOOST_CHECK_EQUAL(0, secondMockState->onExitCallCount);
}

