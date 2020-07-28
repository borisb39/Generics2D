/**
 * The StateMachine class defines a basic state machine
 * that can be used as a component to create all specialized state machines derived objects (action manager, animation manager..)
 *
 * @file StateMachine.cpp
 * @author Boris Burdin
 * @date 20201805 - Initial Release
 * @date 20200605 - Define as a Classic class instead of an Interface
 * @date 20200613 - Add GetTransitionBounds() method
 * @date 20200613 - Return a boolean on Add/RemoveState methods call to check if they are are successful
 * @date 20200726 - Add ReadJSON() method
 * @date 20200728 - Return a boolean on Add/RemoveTransition methods call to check if they are are successful
 */

#include <cassert>
#include "StateMachine.h"

namespace Generics
{
	StateMachine::StateMachine()
	{
	}

	StateMachine::~StateMachine()
	{
	}

	typeStateID StateMachine::GetActiveState() const
	{
		return mActiveState;
	}

	std::vector<typeTransitionID> StateMachine::GetAvailableTransitions() const
	{
		std::vector<typeTransitionID> anResult = {};
		if (not mActiveState.empty())
		{
			auto it = mStates.find( mActiveState);
			anResult = it->second;
		}
		return anResult;
	}

	bool StateMachine::HasTransition(typeTransitionID transitionID) const
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

	std::pair<typeStateID, typeStateID> StateMachine::GetTransitionBounds(typeTransitionID transitionID) const
	{
		std::pair<typeStateID, typeStateID> bounds{ "", "" };
		auto it = mTransitions.find(transitionID);
		if (it != mTransitions.end())
		{
			bounds = it->second;
		}
		return bounds;
	}

	void StateMachine::DoTransition(typeTransitionID transitionID)
	{
		if (HasTransition(transitionID))
		{
			auto it = mTransitions.find(transitionID);
			typeStateID toState = it->second.second;
			mActiveState = toState;
		}
	}

	bool StateMachine::AddState(typeStateID stateID)
	{
		assert(!stateID.empty() && "Empty stateID not allowed");

		bool status = false;
		auto it = mStates.find(stateID);
		// if the state does not already exist
		if (it == mStates.end())
		{
			status = true;
			mStates.insert(std::pair<typeStateID, std::vector<typeTransitionID>>(stateID, {}));
			if (mActiveState.empty())
			{
				mActiveState = stateID;
			}
		}
		return status;
	}

	bool StateMachine::RemoveState(typeStateID stateID)
	{
		bool status = false;
		// we can't remove the active state
		if (stateID == mActiveState) return status;

		auto it1 = mStates.find(stateID);
		// if the state existe
		if (it1 != mStates.end())
		{
			status = true;
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
		return status;
	}

	std::vector<typeStateID> StateMachine::GetAllStates() const
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

	bool StateMachine::AddTransition(typeTransitionID transitionID, typeStateID fromStateID, typeStateID toStateID)
	{
		assert(!transitionID.empty() && "Empty transitionID not allowed");

		bool status = false;
		// we can't have two transitions with the same ID
		auto it1 = mTransitions.find(transitionID);
		if (it1 != mTransitions.end()) return status;

		// we can't add a transition from a state to itself
		if (fromStateID == toStateID) return status;

		// we need a fromStateID already registered
		auto it2 = mStates.find(fromStateID);
		if (it2 == mStates.end()) return status;

		// we need a toStateID already registered
		auto it3 = mStates.find(toStateID);
		if (it3 == mStates.end()) return status;

		mTransitions.insert(std::pair<typeTransitionID, std::pair<typeStateID, typeStateID>>
			(transitionID, std::pair<typeStateID, typeStateID>(fromStateID, toStateID)));
		mStates[fromStateID].push_back(transitionID);
		status = true;

		return status;
	}

	bool StateMachine::RemoveTransition(typeTransitionID transitionID)
	{
		bool status = false;
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
			status = true;
		}
		return status;
	}

	std::vector<typeTransitionID> StateMachine::GetAllTransitions() const
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

	void StateMachine::ReadJSON(std::string pathToJSONFile)
	{
		// read a JSON file
		std::ifstream input(pathToJSONFile);
		nlohmann::json json;
		try
		{
			input >> json;
		}
		//if bad file format
		catch (const std::exception& e) 
		{  
			return;
		}

		std::vector< typeStateID> jsonStates;

		//we read and create the states
		auto it1 = json.find("nodes");
		if (it1 != json.end())
		{
			for (auto& node : it1.value())
			{
				//we check if the required keys are present
				if (node.find("text") != node.end())
				{
					//we check if the associated values have the correct type
					if (node["text"].is_string())
					{
						AddState(node["text"]);
						jsonStates.push_back(node["text"]);
					}
				}
			}
		}

		//we read and create the transitions
		auto it2 = json.find("links");
		if (it2 != json.end())
		{
			for (auto& link : it2.value())
			{
				//we check if all the required keys are present
				if    (link.find("text")  == link.end() 
					or link.find("nodeA") == link.end() 
					or link.find("nodeB") == link.end())
					continue;

				//we check if the associated values have the correct type
				if    (!link["text"].is_string()
					or !link["nodeA"].is_number_unsigned()
					or !link["nodeB"].is_number_unsigned())
					continue;

				unsigned int stateA_idx = link["nodeA"];
				unsigned int stateB_idx = link["nodeB"];
				//we check if the found indexes correspond to the states previously registered
				if    (stateA_idx < 0 or jsonStates.size() <= stateA_idx
					or stateB_idx < 0 or jsonStates.size() <= stateB_idx)
					continue;

				typeStateID fromState = jsonStates[stateA_idx];
				typeStateID toState = jsonStates[stateB_idx];
				typeTransitionID transition = link["text"];
				AddTransition(transition, fromState, toState);
			}
		}
	}
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