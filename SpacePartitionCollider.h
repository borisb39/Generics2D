/**
 * The SpacePartitionCollider class defines a collider to assign to a body
 * that can be used to detect collision with other colliders
 *
 * @file SpacePartitionCollider.h
 * @author Boris Burdin
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
        bool isEnabled = false;
        bool isSensor = false;
        Vect2d position{ 0, 0 };//relative to the associated body center
        float boxWidth = 0;//center at position
        float boxHeight = 0;//center at position
        Vect2d vertice0{ 0, 0 };// relative to position
        Vect2d vertice1{ 0, 0 };// relative to position

        /**
        * getAABB will extrapolate the collider AABB from its internal properties
        * and return it.
        */
        AABB getAABB() const;
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
    * projectionPointToLineSegment will compute and return the projection 
    * coordinate H of a point P3 on a line segment P1, P2
    */
    Vect2d projectionPointToLineSegment(Vect2d& P1, Vect2d& P2, Vect2d& P3);

    /**
    * BoxEdgeDisplacementResponse will compute the displacement of the box with respect to the edge
    * normal vector, in order the move the box "outside" of the edge
    */
    Vect2d BoxEdgeDisplacementResponse(const SpacePartitionCollider& box, const SpacePartitionCollider& edge);



}
