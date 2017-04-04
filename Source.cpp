#include "Functions.h"

int main(){

//	int startStateArr[9];
	int startStateArr1[9] = {4,1,0,6,3,2,7,5,8};
	int startStateArr2[9] = { 4, 3, 1, 6, 5, 2, 0, 7, 8 };
	int startStateArr3[9] = { 3, 1, 0, 4, 5, 2, 6, 7, 8 };
	int startStateArr4[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
	


//	stateProcessor(startStateInt,startStateArr);

	/*
	DFS_for_8Puzzle(startStateArr1);
	DFS_for_8Puzzle(startStateArr2);
	DFS_for_8Puzzle(startStateArr3);
	DFS_for_8Puzzle(startStateArr4);
	*/
	BFS_for_8Puzzle(startStateArr1);
	BFS_for_8Puzzle(startStateArr2);
	BFS_for_8Puzzle(startStateArr3);
	BFS_for_8Puzzle(startStateArr4);
	ASTAR_for_8Puzzle(startStateArr1, 1);
	ASTAR_for_8Puzzle(startStateArr2, 1);
	ASTAR_for_8Puzzle(startStateArr3, 1);
	ASTAR_for_8Puzzle(startStateArr4, 1);
	ASTAR_for_8Puzzle(startStateArr1, 2);
	ASTAR_for_8Puzzle(startStateArr2, 2);
	ASTAR_for_8Puzzle(startStateArr3, 2);
	ASTAR_for_8Puzzle(startStateArr4, 2);

	return 0;
}
