#include "CppUnitTest.h"

#include "../StateMachine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class StateMachine : public Generics::StateMachine
{
public:
	virtual void UpdateFixed() {};
};

namespace UnitTests
{		
	TEST_CLASS(UnitTestStateMachine)
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
			Assert::IsTrue(StateMachine.AddState("testState1"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(1), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a new state
			Assert::IsTrue(StateMachine.AddState("testState2"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to add a state already defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.AddState("testState1"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to remove the active state -> this must not affect the state machine
			Assert::IsFalse(StateMachine.RemoveState("testState1"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to remove a state not defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.RemoveState("testState3"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// remove a state
			Assert::IsTrue(StateMachine.RemoveState("testState2"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
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
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// try to add a transition to a state not defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.AddTransition("testTransition1", "testState1", "testState3"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"",""} == StateMachine.GetTransitionBounds("testTransition1"));
			// try to add a transition from a state not defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.AddTransition("testTransition1", "testState3", "testState1"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"", ""} == StateMachine.GetTransitionBounds("testTransition1"));
			// add a new transition (from the active state)
			Assert::IsTrue(StateMachine.AddTransition("testTransition1", "testState1", "testState2"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1"} == StateMachine.GetAvailableTransitions());
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"testState1", "testState2"} == StateMachine.GetTransitionBounds("testTransition1"));
			// add a new state
			StateMachine.AddState("testState3");
			// try to add a transition already defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.AddTransition("testTransition1", "testState1", "testState3"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1"} == StateMachine.GetAvailableTransitions());
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"testState1", "testState2"} == StateMachine.GetTransitionBounds("testTransition1"));
			// add a new transition (from the active state)
			Assert::IsTrue(StateMachine.AddTransition("testTransition2", "testState1", "testState3"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"testState1", "testState3"} == StateMachine.GetTransitionBounds("testTransition2"));
			// try to remove a transition not defined -> this must not affect the state machine
			Assert::IsFalse(StateMachine.RemoveTransition("testTransition3"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"testState1", "testState3"} == StateMachine.GetTransitionBounds("testTransition2"));
			// remove a transition (from the active state)
			Assert::IsTrue(StateMachine.RemoveTransition("testTransition1"));
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition2"} == StateMachine.GetAvailableTransitions());
			Assert::IsTrue(std::pair<Generics::typeStateID, Generics::typeStateID>{"", ""} == StateMachine.GetTransitionBounds("testTransition1"));
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
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a new transition (from the active state)
			StateMachine.AddTransition("testTransition1", "testState1", "testState2");
			// try to do a transition not defined -> this must not affect the state machine
			StateMachine.DoTransition("testTransition2");
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition1"));
			// do a transition (to state2)
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// try to do a transition not defined -> this must not affect the state machine
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition1"));
			// add a new transition (to the active state)
			StateMachine.AddTransition("testTransition2", "testState3", "testState2");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition2"));
			// try to add a transition already defined -> this must not affect the state machine
			StateMachine.AddTransition("testTransition2", "testState2", "testState3");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			Assert::IsFalse(StateMachine.HasTransition("testTransition2"));
			// remove a transition (to the active state)
			StateMachine.RemoveTransition("testTransition2");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// add a transition (from the active state)
			StateMachine.AddTransition("testTransition2", "testState2", "testState3");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(1), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(StateMachine.HasTransition("testTransition2"));
			// do a transition (to state3 )
			StateMachine.DoTransition("testTransition2");
			Assert::AreEqual(Generics::typeStateID("testState3"), StateMachine.GetActiveState());
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
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(3), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(3), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(3), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1","testTransition2","testTransition3"} == StateMachine.GetAvailableTransitions());
			// remove a state -> this must remove the transitions from/to this state
			StateMachine.RemoveState("testState3");
			Assert::AreEqual(Generics::typeStateID("testState1"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAvailableTransitions().size());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"testTransition1", "testTransition2"} == StateMachine.GetAvailableTransitions());
			// try to remove the active state -> this must not affect the state machine
			StateMachine.RemoveState("testState1");
			// do a transition (to state2)
			StateMachine.DoTransition("testTransition1");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(2), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(2), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
			// remove a state -> this must remove the transitions from/to this state
			StateMachine.RemoveState("testState1");
			Assert::AreEqual(Generics::typeStateID("testState2"), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(1), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAvailableTransitions().size());
		}

		TEST_METHOD(ReadGoodJSON)
		{
			StateMachine StateMachine;
			StateMachine.ReadJSON("state_machine_exemple.json");
			Assert::AreEqual(Generics::typeStateID("state1"), StateMachine.GetActiveState());
			Assert::IsTrue(std::vector<Generics::typeStateID>{"state1", "state2", "state3"} == StateMachine.GetAllStates());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition4","transition2"} == StateMachine.GetAvailableTransitions());
			StateMachine.DoTransition("transition4");
			Assert::AreEqual(Generics::typeStateID("state2"), StateMachine.GetActiveState());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition3"} == StateMachine.GetAvailableTransitions());
			StateMachine.DoTransition("transition3");
			Assert::AreEqual(Generics::typeStateID("state3"), StateMachine.GetActiveState());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition1"} == StateMachine.GetAvailableTransitions());
			StateMachine.DoTransition("transition1");
			Assert::AreEqual(Generics::typeStateID("state1"), StateMachine.GetActiveState());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition4", "transition2"} == StateMachine.GetAvailableTransitions());
			StateMachine.DoTransition("transition2");
			Assert::AreEqual(Generics::typeStateID("state3"), StateMachine.GetActiveState());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition1"} == StateMachine.GetAvailableTransitions());
		}

		TEST_METHOD(ReadBadJSONValue)
		{
			StateMachine StateMachine;
			StateMachine.ReadJSON("state_machine_exemple_bad_value.json");
			Assert::IsTrue(std::vector<Generics::typeStateID>{"state1", "state2", "state4"} == StateMachine.GetAllStates());
			Assert::IsTrue(std::vector<Generics::typeTransitionID>{"transition1", "transition3"} == StateMachine.GetAllTransitions());
		}

		TEST_METHOD(ReadBadJSONFormat)
		{
			// in case of wrong file format the state machine in not changed
			StateMachine StateMachine;
			StateMachine.ReadJSON("state_machine_exemple_bad_format.json");
			Assert::AreEqual(std::string(""), StateMachine.GetActiveState());
			Assert::AreEqual(size_t(0), StateMachine.GetAllStates().size());
			Assert::AreEqual(size_t(0), StateMachine.GetAllTransitions().size());
		}
	};
}