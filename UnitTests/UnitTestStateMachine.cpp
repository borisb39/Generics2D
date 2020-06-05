#include "CppUnitTest.h"

#include "../StateMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class StateMachine : public Generic::StateMachine
{
public:
	virtual void UpdateFixed() {};
};

namespace UnitTests
{		
	TEST_CLASS(UnitTestIStateMachine)
	{
	public:

		TEST_METHOD(TestEmptyActiveState)
		{
			StateMachine StateMachine;
			// no active state defined
			Assert::AreEqual(std::string(""), StateMachine.GetActiveState());
			Assert::IsFalse(StateMachine.HasTransition("testTransition"));
			// try to perform operations on states/transitions not defined-> this must not affect the state machine
			StateMachine.RemoveState("testState");
			StateMachine.RemoveTransition("testTransition");
			StateMachine.AddTransition("testTransition", "testState1", "testState2");
			StateMachine.DoTransition("testTransition");
			Assert::AreEqual(std::string(""), StateMachine.GetActiveState());
			Assert::IsFalse(StateMachine.HasTransition("testTransition"));
		}

		TEST_METHOD(TestAddRemoveState)
		{
			StateMachine StateMachine;
			// add a new state -> it becomes the active state
			StateMachine.AddState("testState1");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(1), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a new state
			StateMachine.AddState("testState2");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to add a state already defined -> this must not affect the state machine
			StateMachine.AddState("testState1");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to remove the active state -> this must not affect the state machine
			StateMachine.RemoveState("testState1");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to remove a state not defined -> this must not affect the state machine
			StateMachine.RemoveState("testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// remove a state
			StateMachine.RemoveState("testState2");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(1), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
		}

		TEST_METHOD(TestAddRemoveTransitions)
		{
			StateMachine StateMachine;
			// add a new state -> it becomes the active state
			StateMachine.AddState("testState1");
			// add a new state
			StateMachine.AddState("testState2");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to add a transition to a state not defined -> this must not affect the state machine
			StateMachine.AddTransition("testTransition1", "testState1", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// try to add a transition from a state not defined -> this must not affect the state machine
			StateMachine.AddTransition("testTransition1", "testState3", "testState1");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// add a new transition (from the active state)
			StateMachine.AddTransition("testTransition1", "testState1", "testState2");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1"} == StateMachine.GetAvailableTransitions());
			// add a new state
			StateMachine.AddState("testState3");
			// try to add a transition already defined -> this must not affect the state machine
			StateMachine.AddTransition("testTransition1", "testState1", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1"} == StateMachine.GetAvailableTransitions());
			// add a new transition (from the active state)
			StateMachine.AddTransition("testTransition2", "testState1", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			// try to remove a transition not defined
			StateMachine.RemoveTransition("testTransition3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			// remove a transition (from the active state)
			StateMachine.RemoveTransition("testTransition1");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition2"} == StateMachine.GetAvailableTransitions());
		}

		TEST_METHOD(TestDoTransitions)
		{
			StateMachine StateMachine;
			// add a new state -> it becomes the active state
			StateMachine.AddState("testState1");
			// add a new state
			StateMachine.AddState("testState2");
			// add a new state
			StateMachine.AddState("testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a new transition (from the active state)
			StateMachine.AddTransition("testTransition1", "testState1", "testState2");
			// try to do a transition not defined -> this must not affect the state machine
			StateMachine.DoTransition("testTransition2");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			// do a transition (to state2)
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// try to do a transition not defined -> this must not affect the state machine
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// add a new transition (to the active state)
			StateMachine.AddTransition("testTransition2", "testState3", "testState2");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition2"));
			// try to add a transition already defined -> this must not affect the state machine
			StateMachine.AddTransition("testTransition2", "testState2", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition2"));
			// remove a transition (to the active state)
			StateMachine.RemoveTransition("testTransition2");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a transition (from the active state)
			StateMachine.AddTransition("testTransition2", "testState2", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			// do a transition (to state3 )
			StateMachine.DoTransition("testTransition2");
			Assert::AreEqual(Generic::typeStateID("testState3"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition2"));
		}

		TEST_METHOD(TestRemoveStateWithTransitions)
		{
			StateMachine StateMachine;
			// add a new state -> it becomes the active state
			StateMachine.AddState("testState1");
			// add a new state
			StateMachine.AddState("testState2");
			// add a new state
			StateMachine.AddState("testState3");
			// add a new transition (from active state)
			StateMachine.AddTransition("testTransition1", "testState1", "testState2");
			// add a new transition (from active state)
			StateMachine.AddTransition("testTransition2", "testState1", "testState2");
			// add a new transition (from active state)
			StateMachine.AddTransition("testTransition3", "testState1", "testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(3), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(3), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1","testTransition2","testTransition3"} == StateMachine.GetAvailableTransitions());
			// remove a state -> this must remove the transitions from/to this state
			StateMachine.RemoveState("testState3");
			Assert::AreEqual(Generic::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(std::vector<Generic::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			// try to remove the active state -> this must not affect the state machine
			StateMachine.RemoveState("testState1");
			// do a transition (to state2)
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// remove a state -> this must remove the transitions from/to this state
			StateMachine.RemoveState("testState1");
			Assert::AreEqual(Generic::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(1), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
		}
	};
}