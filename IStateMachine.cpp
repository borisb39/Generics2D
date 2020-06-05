/**
 * The IStateMachine interface class defines a basic state machine interface
 * that can be used to create all specialized state machine classes (action state, animation state)
 *
 * @file IStateMachine.cpp
 * @author Boris Burdin
 * @date 20201805 - Initial Release
 */

#include "IStateMachine.h"

namespace Generic
{
	IStateMachine::IStateMachine()
	{
	}

	IStateMachine::~IStateMachine()
	{
	}

	typeStateID IStateMachine::GetActiveState() const
	{
		return mActiveState;
	}

	std::vector<typeTransitionID> IStateMachine::GetAvailableTransitions() const
	{
		std::vector<typeTransitionID> anResult = {};
		if (not mActiveState.empty())
		{
			auto it = mStates.find( mActiveState);
			anResult = it->second;
		}
		return anResult;
	}

	bool IStateMachine::HasTransition(typeTransitionID transitionID) const
	{
		bool anResult = false;
		std::vector<typeTransitionID> availableTransitions = GetAvailableTransitions();
		if (not availableTransitions.empty())
		{
			auto it = std::find(availableTransitions.begin(), availableTransitions.end(), transitionID);
			anResult = (it != availableTransitions.end());
		}
		return anResult;
	}

	void IStateMachine::DoTransition(typeTransitionID transitionID)
	{
		if (HasTransition(transitionID))
		{
			auto it = mTransitions.find(transitionID);
			typeStateID toState = it->second.second;
			mActiveState = toState;
		}
	}

	void IStateMachine::AddState(typeStateID stateID)
	{
		auto it = mStates.find(stateID);
		// if the state does not already exist
		if (it == mStates.end())
		{
			mStates.insert(std::pair<typeStateID, std::vector<typeTransitionID>>(stateID, {}));

			if (mActiveState.empty())
			{
				mActiveState = stateID;
			}
		}
	}

	void IStateMachine::RemoveState(typeStateID stateID)
	{
		// we can't remove the active state
		if (stateID == mActiveState) return;

		auto it1 = mStates.find(stateID);
		// if the state existe
		if (it1 != mStates.end())
		{
			// we remove the state
			mStates.erase(it1);

			// we find the transitions that start from/ end to this state
			for (auto it2 = mTransitions.begin(); it2 != mTransitions.end();)
			{
				typeTransitionID transitionID = it2->first;
				typeStateID fromState = it2->second.first;
				typeStateID toState = it2->second.second;

				if (toState == stateID)
				{
					//we remove the transition in the states transitions list associated to fromState
					for (auto it3 = mStates[fromState].begin(); it3 != mStates[fromState].end(); it3++)
					{
						if (*it3 == transitionID)
						{
							mStates[fromState].erase(it3);
							break;
						}
					}

					//then we remove the transition and get an iterator to the next one
					it2 = mTransitions.erase(it2);
				}
				else if (fromState == stateID)
				{
					//we remove the transition and get an iterator to the next one
					it2 = mTransitions.erase(it2);
				}
				else
				{
					// we check the next transition
					it2++;
				}
			}
		}
	}

	std::vector<typeStateID> IStateMachine::GetAllStates() const
	{
		std::vector<typeStateID> allStates = {};

		if (not mStates.empty())
		{
			for (auto it = mStates.begin(); it != mStates.end(); it++)
			{
				allStates.push_back(it->first);
			}
		}

		return allStates;
	}

	void IStateMachine::AddTransition(typeTransitionID transitionID, typeStateID fromStateID, typeStateID toStateID)
	{
		// we can't have two transitions with the same ID
		auto it1 = mTransitions.find(transitionID);
		if (it1 != mTransitions.end()) return;

		// we can't add a transition from a state to itself
		if (fromStateID == toStateID) return;

		// we need a fromStateID already registered
		auto it2 = mStates.find(fromStateID);
		if (it2 == mStates.end()) return;

		// we need a toStateID already registered
		auto it3 = mStates.find(toStateID);
		if (it3 == mStates.end()) return;

		mTransitions.insert(std::pair<typeTransitionID, std::pair<typeStateID, typeStateID>>
			(transitionID, std::pair<typeStateID, typeStateID>(fromStateID, toStateID)));
		mStates[fromStateID].push_back(transitionID);
	}

	void IStateMachine::RemoveTransition(typeTransitionID transitionID)
	{
		auto it1 = mTransitions.find(transitionID);
		if (it1 != mTransitions.end())
		{
			typeStateID fromState = it1->second.first;
			typeStateID toState = it1->second.second;

			//we need to remove the transition in the states transitions list associated to fromState
			for (auto it2 = mStates[fromState].begin(); it2 != mStates[fromState].end(); it2)
			{
				if (*it2 == transitionID)
				{
					mStates[fromState].erase(it2);
					break;
				}
			}

			//then we remove the transition
			mTransitions.erase(it1);
		}
	}

	std::vector<typeTransitionID> IStateMachine::GetAllTransitions() const
	{
		std::vector<typeStateID> allTransitions = {};
		if (!mTransitions.empty())
		{
			for (auto it = mTransitions.begin(); it != mTransitions.end(); it++)
			{
				allTransitions.push_back(it->first);
			}
		}
		return allTransitions;
	}
}

/**
 * @class Generic::IStateMachine
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