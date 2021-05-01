/**
 * The SpacePartitionCollider class defines a collider to assign to a body
 * that can be used to detect collision with other colliders
 *
 * @file SpacePartitionCollider.h
 * @author Boris Burdin
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
        float radius = 0;//center at position
        Vect2d vertice0{ 0, 0 };// relative to position
        Vect2d vertice1{ 0, 0 };// relative to position

        /**
        * getAABB will extrapolate the collider AABB from its internal properties
        * and return it.
        */
        AABB getAABB() const;     
    };
}
