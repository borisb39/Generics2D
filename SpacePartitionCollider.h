/**
 * The SpacePartitionCollider class defines a collider to assign to a body
 * that can be used to detect collision with other colliders
 *
 * @file SpacePartitionCollider.h
 * @author Boris Burdin
 * @date 20210424 - Initial Release
 * @date 20210425 - Add getProperties method
 */

#pragma once

#include "Generics_types.h"

namespace Generics
{
    /// The SpacePartitionCollider base class 
    class SpacePartitionCollider
    {
    public:
        /**
        * SpacePartitionCollider constructor
        */
        SpacePartitionCollider(ColliderProperties);
        /**
        * SpacePartitionCollider destructor
        */
        ~SpacePartitionCollider() {};

        /**
        * getter for internal properties
        */
        ColliderProperties getProperties() const;

        /**
        * getAABB will extrapolate the collider AABB from its internal properties
        * and return it.
        */
        AABB getAABB() const;
        
    private:
        //collider properties
        ColliderType mType;
        typeColliderTag mTag;
        Vect2d mPosition; //relative to the associated body center
        float mBoxWidth; 
        float mBoxHeight; 
        float mRadius; 
        Vect2d mVertice0; // relative to mPosition
        Vect2d mVertice1; // relative to mPosition
    };
}
