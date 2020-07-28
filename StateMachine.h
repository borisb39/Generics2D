/**
 * The StateMachine class defines a basic state machine
 * that can be used as a component to create all specialized state machines derived objects (action manager, animation manager..)
 *
 * @file StateMachine.h
 * @author Boris Burdin
 * @date 20201805 - Initial Release
 * @date 20200605 - Define as a Classic class instead of an Interface
					Remove pure virtual updateFixed() method
 * @date 20200613 - Add GetTransitionBounds() method
 * @date 20200613 - Return a boolean on Add/RemoveState methods call to check if they are are successful
 * @date 20200726 - Add ReadJSON() method
 * @date 20200728 - Return a boolean on Add/RemoveTransition methods call to check if they are are successful
 */

#pragma once

#include <fstream>
#include <iostream>
#include <typeinfo>

#include "Generics_types.h"
#include "json.hpp"

namespace Generics
{
	/// The StateMachine base class 
	class StateMachine
	{
	public:
		/**
		* StateMachine default constructor
		*/
		StateMachine();
		/**
	    * IStateMachine destructor
	    */
		virtual ~StateMachine();

	    /**
	    * GetActiveState will return the ID of the current active state
	    * @return the current active state ID
	    */
		typeStateID GetActiveState() const;

		/**
	    * GetAvailableTransitions will get the ID of all available transitions associated with (from) the active state
	    * @return a vector with the transitionIDs associated with (from) the active state
	    */
		std::vector<typeTransitionID> GetAvailableTransitions() const;

		/**
		* HasTransition will test if the transitionID provided is associated with and start from the active state
		* @return a boolean true if the transition start from the active state, false otherwise
		*/
		bool HasTransition(typeTransitionID transitionID) const;

		/**
		* GetTransitionBounds return the two StateIDs associated to the transitionID provided
		* @return a pair of typeStateID from/to associated to the transitionID
		*/
		std::pair<typeStateID, typeStateID> GetTransitionBounds(typeTransitionID transitionID) const;

		/**
		* DoTransition is responsible from using the transitionID provided to change the active state, if the transitionID
		* provided starts from the active state
		*/
		void DoTransition(typeTransitionID transitionID);

		/**
		* AddState will add the new stateID provided to the state machine if it does not already exist 
		* @return a boolean true if the state is inserted, false otherwise
		*/
		bool AddState(typeStateID stateID);

		/**
		* RemoveState will remove the stateID provided if this state is not the active state
		* @return a boolean true if the state is removed, false otherwise
		*/
		bool RemoveState(typeStateID stateID);

		/**
		* GetAllStates will get all the states registered in the state machine
		* @return a vector with all the stateIDs registered
		*/
		std::vector<typeStateID> GetAllStates() const;

		/**
		* AddTransition will add the new transitionID fromStateID to toStateID if the transitionID doesn't already exist,
		* if fromStateID and toStateID are registered and if fromStateID and toStateID are different. 
		* Otherwise the state machine is not modified.
		* @return a boolean true if the transition is added, false otherwise
		*/
		bool AddTransition(typeTransitionID transitionID, typeStateID fromStateID, typeStateID toStateID);

		/**
		* RemoveTransition will remove the transitionID provided if the transition was previsouly registered
		* @return a boolean true if the transition is removed, false otherwise
		*/
		bool RemoveTransition(typeTransitionID transitionID);

		/**
		* GetAllTransitions will Get all the transitions registered
		* @return a vector with all the transitionIDs registered
		*/
		std::vector<typeTransitionID> GetAllTransitions() const;

		/**
		* ReadJSON reads a json file formatted using the Evan Wallace Finite State Machine Designer (http://madebyevan.com/fsm/)
		* and creates the valid states and transitions associated
		*/
		void ReadJSON(std::string pathToJSONFile);

	private:
		// the current active state
		typeStateID mActiveState;
		// the defintion of all the transitions registered : <transitionID : <fromStateID: toStateID>, ..>
		std::map< typeTransitionID, std::pair<typeStateID, typeStateID> > mTransitions;
		// the definition of all the states registered (list of transitions from each state)
		std::map< typeStateID, std::vector<typeTransitionID> > mStates;
	};
}

/**
 * @class Generics::StateMachine
 *
 * Copyright (c) 2020- Boris Burdin
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */