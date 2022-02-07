/**
 * The SpacePartitionContactListener defines a listener that tracks contacts between bodies interacting in world container.
 *
 * @file SpacePartionBody.h
 * @author Boris Burdin

 * @date 20220207 - Fix bug on listener delete
 *                  Update methods ~SpacePartitionContactListener
 * @date 20220109 - Initial Release
 */

#pragma once

#include "Generics_types.h"

namespace Generics
{

    // Contact description
    struct SpacePartitionContact
    {
        SpacePartitionCollider* collider1 = nullptr;
        SpacePartitionCollider* collider2 = nullptr;

        // for faster deletion of contact node
        std::list<SpacePartitionContact*>::iterator iterCollider1;
        std::list<SpacePartitionContact*>::iterator iterCollider2;

        ContactState state = ContactState::NEW;
    };

    /// The SpacePartitionContactListener base class 
    class SpacePartitionContactListener
    {
    public:

        /**
        * SpacePartitionContactListener constructor
        */
        ~SpacePartitionContactListener();

        /**
        * updateContactState will check if a contact between the two rpovided colliders is already tracked.
        * if yes the contact state is set to UPDATED. If new a NEW contact is created.
        */
        void updateContactState(SpacePartitionCollider*, SpacePartitionCollider*);

        /**
        * isBodyInListenerWorld will check if the provided body belongs to the same word than the Contact Listener.
        * @return bool true if it does, false otherwise
        */
        bool isBodyInListenerWorld(SpacePartitionBody* body);

        /**
        * setWorld will register a ptr to world associated to the listener
        */
        void setWorld(SpacePartitionWorld* world) {   p_world = world; };

        /**
        * endSolverStep can be called at end og global calculation loop and will track the changes in contacts states 
        * issued from the calculation. It will also check if a contact begin/end callback must be raised
        */
        void endSolverStep();

        /**
        * resetContactList will untrack all contacts and free the mContactsList.
        */
        void resetContactList();

        /**
        * BeginContact must be overridden by specialized class. Raised when a new contact is registered.
        */
        virtual void BeginContact(SpacePartitionContact*) {};

        /**
        * EndContact must be overridden by specialized class. Raised when a contact is finished.
        */
        virtual void EndContact(SpacePartitionContact*) {};

        /**
        * getNumberOfContacts will return the number of contacts currently tracked.
        */
        int getNumberOfContacts() { return mContactsList.size(); };

    private:

        // list of effective contacts 
        std::list< SpacePartitionContact> mContactsList;

        // world container associated to the listener
        SpacePartitionWorld* p_world = nullptr;

    };
}

