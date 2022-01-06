/**
 * The SpacePartionBody class defines a Physical body 
 * that can be used to assign time relative properties and collider to an entity
 *
 * @file SpacePartionBody.h
 * @author Boris Burdin
 * @date 20220106 - Add body multiconfig
 *                  Update SpacePartitionBody constructor, 
 *                  Add methods get/set CurrentConfig + mCurrentConfig attribut
 *                  Update  appendCollider and getColliderAt methods, mColliders attribut
 * @date 20211218 - Add userData ptr
 *                  Add attribut mUserData
 * @date 20211215 - Generic method to catch elapsed time for dynamic bodies
 *                  Add attribut mWorldID
 * @date 20211128 - Algo for Min collision detection 
 *                  Add method getColliderAt_globalFrame
 * @date 20211123 - Add BodyType argument to SpacePartitionBody constructor  
 * @date 20210501 - Collider class > struct
 *                  Replace method  createCollider > appendCollider
 *                  Update method getColliderAt
 * @date 20210501 - Add Axis Aligned bounding Box
 *                  Add methods updateAABB, getAABB, add attribut mAABB
 * @date 20210425 - Add Collider managment
 *                  Add method createCollider, getColliderPropertiesAt, getNumberOfColliders
 *                  Add mColliders attribut
 * @date 20210423 - Initial Release
 */

#pragma once

#include "Generics_types.h"

#include "SpacePartitionCollider.h"

namespace Generics
{
	/// The SpacePartionBody base class 
	class SpacePartitionBody
	{
	public:
		/**
		* SpacePartionBody constructor
		*/
		SpacePartitionBody(Vect2d position = { 0, 0 }, BodyType type = BodyType::STATIC);

		/**
		* SpacePartionBody destructor
		*/
		~SpacePartitionBody() {};

		/**
		* Setters for time relative properties with respect to the
		* max magnitude associated to each property if any
		*/
		void setPosition(Vect2d position);
		void setVelocity(Vect2d velocity);
		void setAcceleration(Vect2d acceleration);
		/**
		* Getters for time relative properties 
		*/
		Vect2d getPosition() const { return mPosition; };
		Vect2d getVelocity() const { return mVelocity; };
		Vect2d getAcceleration() const { return mAcceleration; };

		/**
		* Setters for max magnitude properties
		*/
		void setMaxVelocityMagnitude(float magnitude);
		void setMaxAccelerationMagnitude(float magnitude);

		/**
		* updateFromVelocity will update the position of 
		* the body based on its current velocity and the 
		* elapsed time since last update <dt>
		*/
		void updateFromVelocity(double dt);

		/**
		* updateFromAcceleration will update the position and velocity
		* of the body based on its current acceleration and the
		* elapsed time since last update <dt>
		*/
		void updateFromAcceleration(double dt);

		/**
		* getter for Body AABB
		*/
		AABB getAABB() const { return mAABB; }

		/**
		* set the current config to the provided <config> flag.
		* A config is associated to a list of colliders used to deal with collision detection.
		* If the <config> is not already registered internally it is created and assigned to an empty list of colliders.
		**/
		void setCurrentConfig(typeBodyconfigID config);

		/**
		* getter for bodyconfig
		**/
		typeBodyconfigID getCurrentConfig() const { return mCurrentConfig; }

		/**
		* appendCollider will append to the internal colliders container for the current bodyconfig,
		* a copy of the provided collider .
		*/
		void appendCollider(SpacePartitionCollider);

		/**
		* getColliderAt will get a copy of 
		* the collider situated at indice <idx> in the collider container for the current bodyconfig. 
		* In case of out of bound indice a default collider is returned.
		*/
		SpacePartitionCollider getColliderAt(int idx) const;

		/**
		* getColliderAt_globalFrame will get a copy of
		* the collider situated at indice <idx> in the collider container for the current bodyconfig
		* and return it in global (body) frame coordinate.
		* In case of out of bound indice a default collider is returned.
		*/
		SpacePartitionCollider getColliderAt_globalFrame(int idx) const;

		/**
		* getNumberOfColliders will get the number of colliders stored in the container.
		*/
		int getNumberOfColliders() const;

		BodyType getType() const { return mType; }

		// Getters and Setter for worldID 
		void setWorldID(int id);
		int getWorldID() const { return mWorldID; }

		// Getters and Setter for userData
		void setUserData(void* userData) { mUserData = userData; }
		void* getUserData() const { return mUserData; }

	private:
		BodyType mType = BodyType::STATIC;
		// time relative properties associated to the body in World coordinates
		Vect2d mPosition{ 0, 0 };
		Vect2d mVelocity{ 0, 0 };
		Vect2d mAcceleration{ 0, 0 };
		// max magnitude allowed for time relative properties
		float mMaxVelocityMagnitude = 0;
		float mMaxAccelerationMagnitude = 0;

		//Container for associated colliders
		//For each bodyconfig a specific list of colliders is defined
		//to deal with collision detection.
		std::map<typeBodyconfigID, std::vector<SpacePartitionCollider>> mColliders;

		// current config (= list of colliders) associated to the collision detection of the body
		typeBodyconfigID mCurrentConfig = "default";

		//Body AABB included all associated colliders
		AABB mAABB;

		// When the body is added to a world container it will be assigned to 
		// an unique positive ID that identifies it in the world objects pool.
		int mWorldID = -1;

		// void ptr to attach any user object
		void* mUserData = nullptr;

		/**
		* checkMagnitude will check and correct the provided <vector> 
		* so that its magnitude does not exceed the <maxMagnitude> 
		 @ return vect2d the corrected vector value
		*/
		Vect2d checkMagnitude(Vect2d vector, float maxMagnitude);

		/**
		* updateAABB will update (increase the size of) the Body AABB according to 
		* the provided collider if the collider AABB is out of Body AABB bounds
		*/
		void updateAABB(SpacePartitionCollider);
	};
}
