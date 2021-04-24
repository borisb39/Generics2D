/**
 * The SpacePartionBody class defines a Physical body 
 * that can be used to assign time relative properties and collider to an entity
 *
 * @file SpacePartionBody.h
 * @author Boris Burdin
 * @date 20210423 - Initial Release
  */

#pragma once

#include "Generics_types.h"

namespace Generics
{
	/// The SpacePartionBody base class 
	class SpacePartitionBody
	{
	public:
		/**
		* SpacePartionBody constructor
		*/
		SpacePartitionBody(Vect2d position = { 0, 0 }) :
			mPosition(position) {};
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
		void setMaxVelocityMagnitude(float magnitude) {
			mMaxVelocityMagnitude = fmax(0, magnitude);
		};
		void setMaxAccelerationMagnitude(float magnitude) {
			mMaxAccelerationMagnitude = fmax(0, magnitude);
		};

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

	private:
		// time relative properties associated to the body
		Vect2d mPosition{ 0, 0 };
		Vect2d mVelocity{ 0, 0 };
		Vect2d mAcceleration{ 0, 0 };
		// max magnitude allowed for time relative properties
		float mMaxVelocityMagnitude = 0;
		float mMaxAccelerationMagnitude = 0;

		/**
		* checkMagnitude will check and correct the provided <vector> 
		* so that its magnitude does not exceed the <maxMagnitude> associated
		 @ return vect2d the corrected vector value
		*/
		Vect2d checkMagnitude(Vect2d vector, float maxMagnitude);
	};
}
