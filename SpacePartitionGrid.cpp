
#include "SpacePartitionGrid.h"

#include "SpacePartitionBody.h"
#include <cassert>

namespace Generics
{
	SpacePartitionGrid::SpacePartitionGrid(int nx, int ny, float width, float height )
	{
		assert(nx > 0  && "number of partition along x must be >= 1");
		assert(ny > 0 && "number of partition along y must be >= 1");
		assert(width > 0 && "Grid width must be > 0");
		assert(height > 0 && "Grid height must be > 0");

		mGrid = new std::vector< SpacePartitionBody*>[nx * ny + 1]; //+1 to store out of bound bodies
		mNX = nx;
		mNY = ny;
		mHeight = height;
		mWidth = width;
		mSizeX = width / nx;
		mSizeY = height / ny;
	}

	SpacePartitionGrid::~SpacePartitionGrid()
	{
		delete[] mGrid;
	}

	int SpacePartitionGrid::gridID(float x, float y) const
	{
		int i = floor(y / mSizeY);
		int j = floor(x / mSizeX);
		return gridID(i, j);
	}

	int SpacePartitionGrid::gridID(int i, int j) const
	{
		if (j < 0 || j >= mNX || i < 0 || i >= mNY)
			return OOBgID();
		return i * mNX + j;
	}

	void SpacePartitionGrid::setBody(SpacePartitionBody* body)
	{
		//if the body is already in the Grid we remove it
		if (mBodiesGridIDs.find(body) != mBodiesGridIDs.end())
		{
			for (auto it1 = mBodiesGridIDs[body].begin(); it1 != mBodiesGridIDs[body].end(); it1++)
			{
				int gID = *it1;
				for (auto it2 = mGrid[gID].begin(); it2 != mGrid[gID].end();)
				{
					if (*it2 == body)
						it2 = mGrid[gID].erase(it2);
					else
						it2++;
				}
			}
			mBodiesGridIDs.erase(body);
		}

		//we add the body in the cells of the Grid where the body AABB is overlaping
		AABB aabb = body->getAABB();
		aabb.position += body->getPosition();
		for (int j = floor(aabb.left() / mSizeX); j <= floor(aabb.right() / mSizeX); j++)
		{
			for (int i = floor(aabb.bottom() / mSizeY); i <= floor(aabb.top() / mSizeY); i++)
			{
				int gID = gridID(i, j);
				mGrid[gID].push_back(body);
				mBodiesGridIDs[body].push_back(gID);
			}
		}
	}

	std::vector<SpacePartitionBody*> SpacePartitionGrid::getBodiesAtgID(int gID) const
	{
		std::vector<SpacePartitionBody*> bodies;
		if (gID >= 0 && gID <= OOBgID())
			bodies = mGrid[gID];
		return bodies;
	}

	std::vector<int> SpacePartitionGrid::getBodygIDs(SpacePartitionBody* body) const
	{
		std::vector<int> gIDs;
		if (mBodiesGridIDs.find(body) != mBodiesGridIDs.end())
			gIDs = mBodiesGridIDs.at(body);
		return gIDs;
	}

	int SpacePartitionGrid::OOBgID() const
	{
		return mNX * mNY;
	}

}