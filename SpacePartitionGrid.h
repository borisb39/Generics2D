#pragma once

#include "Generics_types.h"

#include "SpacePartitionBody.h"

/**
* The SpacePartitionGrid class defines a 2D table
* that is used to 
*
* @file SpacePartitionGrid.h
* @author Boris Burdin
* @date 20210425 - Initial Release
*/

namespace Generics
{

	class SpacePartitionGrid
	{
	public:

		SpacePartitionGrid(int nx, int ny);

		~SpacePartitionGrid();

	private:

		std::vector< SpacePartitionBody*>* mGrid;
	};

}
