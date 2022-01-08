/**
* The SpacePartitionGrid class defines a 2D table
* that is used to partition the space in cells and manage
* collision detection in a restricted area for each bodies
* registered in the Grid.
*
* @file SpacePartitionGrid.h
* @author Boris Burdin
* @date 20220108 - Improve Grid efficiency
*                  Update  setBody mehod
* @date 20210705 - Initial Release with bodies management
* @date 20210425 - Initial Release
*/

#pragma once

#include "Generics_types.h"

namespace Generics
{
	/// The SpacePartitionGrid base class 
	class SpacePartitionGrid
	{
	public:

		/**
	    * Constructor. Used to create (allocate) a Grid with
		* nx cells along x, ny cells along y, and a size 
		* corresponding to the provided width and height.
	    */
		SpacePartitionGrid(int nx, int ny, float width, float height);

		/**
		* Destructor. The Grid is deallocated.
		*/
		~SpacePartitionGrid();

		/**
		* setBody will append or update the provided <body> into the Grid.
		*/
		void setBody(SpacePartitionBody * body);

		/**
		* GridID will return the ID of the grid(from 0 to NumberOfCells) associated to
		* the coordinates <x, y> provided.The -OutOfBound- special gID is returned
		* when the provided coordinates <x, y> is out of Grid bounds.
		*/
		int gridID(float x, float y) const;

		/**
		* OOBgID will return the -OutOfBound- special gID (= NumberOfCells).
		*/
		int OOBgID() const;

		/**
		* getBodiesAtgID will return the list of bodies present in the cell
		* at provided <gID>.
		*/
		std::vector<SpacePartitionBody*> getBodiesAtgID(int gID) const;

		/**
		* getBodygIDs will return the list of gIDs where the provided <body> belong to.
		*/
		std::vector<int> getBodygIDs(SpacePartitionBody* body) const;

	private:

		//The 2D table that lists which bodies are present in each cell
		std::vector< SpacePartitionBody*>* mGrid;
		//For each body registered in the Grid, the cells of the Grid where the body belong to
		std::unordered_map< SpacePartitionBody*, std::vector<int>> mBodiesGridIDs;
		//height of the Grid
		float mHeight;
		//width of the Grid
		float mWidth;
		//number of cells along X in the Grid
		int mNX;
		//number of cells along Y in the Grid
		int mNY;
		//width of a cell
		float mSizeX;
		//height of a cell
		float mSizeY;

		/**
		* GridID will return the ID of the grid (from 0 to NumberOfCells) associated to
		* the Grid line, colomn <i, j> provided. The -OutOfBound- special gID is returned 
		* when the provided couple <i, j> is out of Grid bounds.
		*/
		int gridID(int i, int j) const;
	};

}
