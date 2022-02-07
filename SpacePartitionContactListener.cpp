#include "SpacePartitionContactListener.h"

#include "SpacePartitionCollider.h"
#include "SpacePartitionBody.h"
#include "SpacePartitionWorld.h"


namespace Generics
{
   SpacePartitionContactListener::~SpacePartitionContactListener()
    {
        if (p_world != nullptr)
           p_world->setContactListener(nullptr);
    };

    void SpacePartitionContactListener::updateContactState(SpacePartitionCollider* collider1, SpacePartitionCollider* collider2)
    {
        // both collider must belong to the same world
        if (!isBodyInListenerWorld(collider1->p_body) || !isBodyInListenerWorld(collider2->p_body))
            return;

        for (auto contact : collider1->contatsList)
        {
            // if the contact already exist -> set status to UPDATED
            if ((collider1 == contact->collider1 && collider2 == contact->collider2)
                || (collider1 == contact->collider2 && collider2 == contact->collider1))
            {
                contact->state = ContactState::UPDATED;
                return;
            }
        }

        // else we create a new contact
        SpacePartitionContact contact;
        contact.collider1 = collider1;
        contact.collider2 = collider2;
        mContactsList.push_back(contact);
        auto contact_ptr = &mContactsList.back();

        // we create a crossed ref between the contact and collider1
        collider1->contatsList.push_back(contact_ptr);
        auto iter1 = collider1->contatsList.end();
        std::advance(iter1, -1);
        contact_ptr->iterCollider1 = iter1;

        // we create a crossed ref between the contact and collider2
        collider2->contatsList.push_back(contact_ptr);
        auto iter2 = collider2->contatsList.end();
        std::advance(iter2, -1);
        contact_ptr->iterCollider2 = iter2;
    }

    bool SpacePartitionContactListener::isBodyInListenerWorld(SpacePartitionBody* body)
    {
        return ((p_world != nullptr) && (body->getWorld() == p_world));
    }

    void SpacePartitionContactListener::endSolverStep()
    {
        for (auto it = mContactsList.begin(); it != mContactsList.end();)
        {
            if (it->state == ContactState::NEW)
            {
                BeginContact(&(*it));// raise begin contact callback
                it->state = ContactState::PENDING;
                it++;
            }
            else if (it->state == ContactState::UPDATED)
            {
                it->state = ContactState::PENDING;
                it++;
            }
            else if (it->state == ContactState::PENDING)
            {
                EndContact(&(*it));// raise end contact callback
                it->collider1->contatsList.erase(it->iterCollider1);
                it->collider2->contatsList.erase(it->iterCollider2);
                it = mContactsList.erase(it);
            }
        }
    }

    void SpacePartitionContactListener::resetContactList()
    {
        for (auto it = mContactsList.begin(); it != mContactsList.end();)
        {
            it->collider1->contatsList.erase(it->iterCollider1);
            it->collider2->contatsList.erase(it->iterCollider2);
            it = mContactsList.erase(it);
        }
    }
}

