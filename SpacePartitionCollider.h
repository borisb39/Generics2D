/**
 * The SpacePartitionCollider class defines a collider to assign to a body
 * that can be used to detect collision with other colliders
 *
 * @file SpacePartitionCollider.h
 * @author Boris Burdin
 * @date 20220116 - Add Contact Listener
 *                  add contatsList attribut
 * @date 20220108 - Improve Grid efficiency
 *                  Update  BoxEdgeDisplacementResponse mehod
 * @date 20220107 - Code refactoring to manage colliders as world factory objects
 *                  Add getPosition_globalFrame method, p_body attribut
 *                  Replace position -> getPosition_globalFrame()
 * @date 20211226 - Remove isEnabled attribut, consider isSensor attibut
 * @date 20211214 - Edge collider : consider restitution vector
 *                  Update methods BoxEdgeDisplacementResponse, Remove method projectionPointToLineSegment
 * @date 20210529 - Generic collision between two colliders
 *                  Add method collisionResolution
 * @date 20210506 - Box edge collision response 
 *                  Update function BoxEdgeIntersect
 *                  Add function BoxEdgeDisplacementResponse, projectionPointToLineSegment
 * @date 20210502 - Collider intersection check
 *                  Removes CIRCLE collider
 *                  Add collidersIntersect, BoxBoxIntersect, BoxEdgeIntersect functions
 * @date 20210501 - Collider class > struct
 *                  Remove non-default constructor, getProperties method
 *                  Update method getAABB
 * @date 20210105 - Add getAABB method, mIsEnabled, mIsSensor attribut
 * @date 20210425 - Add getProperties method
 * @date 20210424 - Initial Release
 */

#pragma once

#include "Generics_types.h"


namespace Generics
{
    /// The SpacePartitionCollider base class 
    struct SpacePartitionCollider
    {
        //collider properties
        ColliderType type = ColliderType::BOX;
        typeColliderTag tag = "None";
        bool isSensor = false;
        Vect2d position{ 0, 0 };//relative to the associated body center
        float boxWidth = 0;//center at position
        float boxHeight = 0;//center at position
        Vect2d vertice0{ 0, 0 };// relative to position
        Vect2d vertice1{ 0, 0 };// relative to position
        Vect2d restitutionVector{ 0, 0 }; 

        SpacePartitionBody* p_body = nullptr; // parent body

        // List of contacts where the collider is involved
        std::list<SpacePartitionContact*> contatsList;

        /**
        * getAABB will extrapolate the collider AABB from its internal properties
        * and return it.
        */
        AABB getAABB() const;

        /**
        * getPosition_globalFrame will get the position of the collider in global frame
        * (frame associated to the p_body).
        */
        Vect2d getPosition_globalFrame() const;

        /**
        * collisionResolution will compute the collision between the two provided colliders
        * and return the collision response associated as a <Collision> object
        */
        static Collision collisionResolution(const SpacePartitionCollider& a, const SpacePartitionCollider& b);

    };

    /**
    * collidersIntersect will check if the two provided colliders intersect.
    * @param a, b : the two colliders to test
    * @return bool : true if they intersect, false otherwise
    */
    bool collidersIntersect(const SpacePartitionCollider& a, const SpacePartitionCollider& b);

    /**
    * BoxBoxIntersect will check if the two provided BOX colliders intersect.
    * @param a, b : the two colliders to test
    * @return bool : true if they intersect, false otherwise
    */
    bool BoxBoxIntersect(const SpacePartitionCollider& a, const SpacePartitionCollider& b);

    /**
    * BoxEdgeIntersect will check if the provided BOX colliders intersect with the provided edge collider.
    * @param box, edge : the two colliders to test
    * @return bool : true if they intersect, false otherwise
    */
    bool BoxEdgeIntersect(const SpacePartitionCollider& box, const SpacePartitionCollider& edge);

    /**
    * BoxEdgeDisplacementResponse will compute the displacement of the box with respect to the edge
    * restitution vector, in order the move the box "outside" of the edge
    */
    Vect2d BoxEdgeDisplacementResponse(const SpacePartitionCollider& box, const SpacePartitionCollider& edge);
}
