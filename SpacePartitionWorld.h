/**
* The SpacePartitionWorld class defines a 2D physical world
* where a 2D spatial partition is used for handling collisions between entities
*
* @file SpacePartionWorld.h
* @author Boris Burdin
* @date 20211128 - Algo for Min collision detection
*                  Add method collisionResolutionDynamicVSstaticBodies_Min
*                  Update method collisionResolutionMeanDynamicVSstaticBodies > collisionResolutionMeanDynamicVSstaticBodies_Mean
* @date 20211113 add static method collisionResolutionMeanDynamicVSstaticBodies
* @date 20210425 - Initial Release
*/

#pragma once

#include <set>

#include "Generics_types.h"

#include "SpacePartitionBody.h"
#include "SpacePartitionGrid.h"

namespace Generics
{
	class SpacePartitionWorld
	{
	public:


		/**
		* Constructor. Used to create a container (world) for the management
		* of bodies. The bodies are referenced in a Grid where the collision detection
		* is done in an efficiant way. 
		* @Parameters 
		*	int nx : nb of cells along x in the Grid 
		*	int ny : nb of cells along y in the Grid 
		*	width and height : the Grid size
		*/
		SpacePartitionWorld(int nx, int ny, float width, float height);

		/**
		* addBody will add a copy of the provided body in the World container, and reference it in the Grid.
		*/
		void addBody(SpacePartitionBody& body);

		/**
		* collisionResolutionDynamicVSstaticBodies_Mean will resolve the collision between the provided dynamic <body1> and static <body2>
		* and return the Collision state.
		* The resolution considers each possible collision between the colliders of the
		* two bodies independently, and computes the mean response value.
		*/
		static Collision collisionResolutionDynamicVSstaticBodies_Mean(SpacePartitionBody& body1, SpacePartitionBody& body2);

		/**
		* collisionResolutionDynamicVSstaticBodies_Min will resolve the collision between the provided dynamic <body1> and static <body2>
		* and return the Collision state.
		* The resolution search for the minimal displacement of body1 that removes the intersection between the two bodies.
		*/
		static Collision collisionResolutionDynamicVSstaticBodies_Min(SpacePartitionBody& body1, SpacePartitionBody& body2);



	private:

		// Container of Static bodies 
		std::list<SpacePartitionBody> mStaticBodies;
		// Container of Dynamic bodies 
		std::list<SpacePartitionBody> mDynamicBodies;
		// 2D partition Grid where the mStaticBodies and mDynamicBodies are referenced.
		SpacePartitionGrid mGrid;
	};

}