
#include "SpacePartitionGrid.h"

namespace Generics
{
	SpacePartitionGrid::SpacePartitionGrid(int nx, int ny)
	{
		mGrid = new std::vector< SpacePartitionBody*>[nx * ny];
	}

	SpacePartitionGrid::~SpacePartitionGrid()
	{
		delete[] mGrid;
	}
}