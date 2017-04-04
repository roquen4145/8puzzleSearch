#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <list>
#include <queue>
#include <functional>
#include <utility>
using namespace std;

struct NODE {
	int state[9];
	int depth;
	NODE* parent;
	NODE* children[4];

	NODE(int* a, int d, NODE* p);
};

struct hNODE 
{
	int state[9];
	int depth;
	hNODE* parent;
	hNODE* children[4];
	int cost;

	hNODE(int* a, int d, hNODE* p,int h);
};

NODE::NODE(int* a, int d, NODE* p)
	:depth{ d },
	parent{ p }
{
	for (int i = 0; i < 9; i++)
	{
		state[i] = a[i];
	}
	for (int i = 0; i < 4; i++)
	{
		children[i] = nullptr;
	}
}

hNODE::hNODE(int* a, int d, hNODE* p,int h)
	:depth{ d },
	parent{ p }
{
	for (int i = 0; i < 9; i++)
	{
		state[i] = a[i];
	}
	for (int i = 0; i < 4; i++)
	{
		children[i] = nullptr;
	}

	cost = d+1;
	if (h == 1)
	{
		for (int i = 0; i < 8; i++)
		{
			if (state[i] != i + 1)
				cost += 1;
		}
	}
	else if (h == 2)
	{
		for (int i = 1; i < 9; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (state[j] == i)		// i가 j 인덱스에 있음
				{
					if (i == j + 1)     // 타일이 제자리에 있음
						break;
					else				// 다른자리
					{
						int temp = (j + 1 > i) ? j + 1 - i : i - j - 1;

						while (temp >= 3)		// 세로로 이동 수
						{
							temp /= 3;
							cost++;
						}
						cost += temp;           // 가로로 이동
					}
					
				}
			}
		}
	}

}

void stateProcessor(int stateInt);
int stateInteger(int stateArr[]);
bool isGoal(int state[]);
void printNode(NODE node);
int findZero(int state[]);
NODE* expandNode(NODE* currentNode, int state[]);
hNODE* expandNode(hNODE* currentNode, int state[],int heuristicNum);
void swapState(const int* state, int* newState, int position, int base);


void stateProcessor(int stateInt, int* stateArr)
{
	stateArr[8] = stateInt % 10;
	stateInt /= 10;
	for (int i = 7; i >= 0; i--)
	{
		stateArr[i] = stateInt % 10;
		stateInt /= 10;
	}
}
int stateInteger(int stateArr[])
{
	int stateNum = 0;

	stateNum = stateArr[0];
	for (int i = 1; i <9; i++)
	{
		stateNum *= 10;
		stateNum += stateArr[i];
	}

	return stateNum;
}


void DFS_for_8Puzzle(int startState[]){
	stack<NODE*> nodeStack;
	list<int> closedList;
	vector<int> positionIndex;
	NODE* myChild;
	int index = 0, maxOpen = 0;

	NODE* currentNode = new NODE(startState, 0, nullptr);

	cout << "--------------------------------------------------------------------" << endl;
	cout << "STATE : ";
	for (int i = 0; i < 9; i++)
		cout << startState[i];
	cout << "            METHOD : DFS " << endl;
	
	if (isGoal(startState))
	{
		cout << "Total Visit : " << closedList.size() << endl;
		cout << "Max Open List Size " << maxOpen << endl;
		cout << "Length : " << (*currentNode).depth + 1;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else
	{
		nodeStack.push(currentNode);

		while (1)
		{
			if (nodeStack.empty())
			{
				cout << "Fail to find the goal" << endl;
				return;
			}
			else
			{
				currentNode = nodeStack.top();
				nodeStack.pop();
				closedList.push_back(stateInteger((*currentNode).state));

	//			for (int i = 0; i < 9; i++)
	//				cout << (*currentNode).state[i];
	//			cout << " Added    Depth : " << (*currentNode).depth << endl;

				if (isGoal((*currentNode).state))
				{
					cout << "Total Visit : " << closedList.size() << endl;
					cout << "Max Open List Size : " << maxOpen << endl;
					cout << "Length : " << (*currentNode).depth + 1 << endl;
					cout << "--------------------------------------------------------------------" << endl;
					return;
				}
				else
				{
					index = findZero((*currentNode).state);

					switch (index)
					{
						/*
						0 1 2
						3 4 5
						6 7 8
						*/

					case 4:
						positionIndex = { 1, 3, 5, 7 };
						break;

					case 0:
						positionIndex = { 1, 3 };
						break;
					case 2:
						positionIndex = { 1, 5 };
						break;
					case 6:
						positionIndex = { 3, 7 };
						break;
					case 8:
						positionIndex = { 5, 7 };
						break;

					case 1:
						positionIndex = { 0, 2, 4 };
						break;
					case 3:
						positionIndex = { 0, 4, 6 };
						break;
					case 5:
						positionIndex = { 2, 4, 8 };
						break;
					case 7:
						positionIndex = { 4, 6, 8 };
						break;

					case -1:
					default:
						cout << "Error" << endl;
					}

					for (vector<int>::iterator it = positionIndex.begin(); it != positionIndex.end(); it++)
					{
						int* newState = new int[9];
						swapState(currentNode->state, newState, *it, index);

						// redundancy check

						int isExist = 0;
						for (list<int>::iterator Nit = closedList.begin(); Nit != closedList.end(); ++Nit){
							if (*Nit == stateInteger(newState))		//
							{
								isExist = 1;
								break;
							}
						}

						if (!isExist)
						{
							myChild = expandNode(currentNode, newState);
							if (myChild != nullptr)
								nodeStack.push(myChild);
						}

						delete[] newState;
					}
					maxOpen = ((unsigned)maxOpen > nodeStack.size()) ? maxOpen : nodeStack.size();
				}
			}
		}
	}
}

void BFS_for_8Puzzle(int startState[]){
	queue<NODE*> nodeQueue;
	list<int> closedList;
	vector<int> positionIndex;
	NODE* myChild;
	int index = 0, maxOpen = 0;

	NODE* currentNode = new NODE(startState, 0, nullptr);

	cout << "--------------------------------------------------------------------" << endl;
	cout << "STATE : ";
	for (int i = 0; i < 9; i++)
		cout << startState[i];
	cout << "            METHOD : BFS " << endl;

	if (isGoal(startState))
	{
		cout << "Total Visit : " << closedList.size() << endl;
		cout << "Max Open List Size " << maxOpen << endl;
		cout << "Length : " << (*currentNode).depth + 1;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else
	{
		nodeQueue.push(currentNode);

		while (1)
		{
			if (nodeQueue.empty())
			{
				cout << "Fail to find the goal" << endl;
				return;
			}
			else
			{
				currentNode = nodeQueue.front();
				nodeQueue.pop();
				closedList.push_back(stateInteger((*currentNode).state));

				//			for (int i = 0; i < 9; i++)
				//				cout << (*currentNode).state[i];
				//			cout << " Added    Depth : " << (*currentNode).depth << endl;

				if (isGoal((*currentNode).state))
				{
					cout << "Total Visit : " << closedList.size() << endl;
					cout << "Max Open List Size : " << maxOpen << endl;
					cout << "Length : " << (*currentNode).depth + 1 << endl;
					cout << "--------------------------------------------------------------------" << endl;
					return;
				}
				else
				{
					index = findZero((*currentNode).state);

					switch (index)
					{
						/*
						0 1 2
						3 4 5
						6 7 8
						*/

					case 4:
						positionIndex = { 1, 3, 5, 7 };
						break;

					case 0:
						positionIndex = { 1, 3 };
						break;
					case 2:
						positionIndex = { 1, 5 };
						break;
					case 6:
						positionIndex = { 3, 7 };
						break;
					case 8:
						positionIndex = { 5, 7 };
						break;

					case 1:
						positionIndex = { 0, 2, 4 };
						break;
					case 3:
						positionIndex = { 0, 4, 6 };
						break;
					case 5:
						positionIndex = { 2, 4, 8 };
						break;
					case 7:
						positionIndex = { 4, 6, 8 };
						break;

					case -1:
					default:
						cout << "Error" << endl;
					}

					for (vector<int>::iterator it = positionIndex.begin(); it != positionIndex.end(); it++)
					{
						int* newState = new int[9];
						swapState(currentNode->state, newState, *it, index);

						// redundancy check

						int isExist = 0;
						for (list<int>::iterator Nit = closedList.begin(); Nit != closedList.end(); ++Nit){
							if ((*Nit) == stateInteger(newState))		//
							{
								isExist = 1;
								break;
							}
						}

						if (!isExist)
						{
							myChild = expandNode(currentNode, newState);
							if (myChild != nullptr)
								nodeQueue.push(myChild);
						}

						delete[] newState;
					}
					maxOpen = ((unsigned)maxOpen > nodeQueue.size()) ? maxOpen : nodeQueue.size();
				}
			}
		}
	}
}

bool comp(hNODE* a, hNODE* b)
{
	if ((*a).cost > (*b).cost)
		return true;
	else
		return false;
}

void ASTAR_for_8Puzzle(int startState[],int heuristicNum){
	priority_queue<hNODE*,vector<hNODE*>, std::function<bool(hNODE*,hNODE*)>> nodeQueue(comp);
	list<pair<int,int>> closedList;
	vector<int> positionIndex;
	hNODE* myChild;
	int index = 0, maxOpen = 0;

	hNODE* currentNode = new hNODE(startState, 0, nullptr,heuristicNum);

	cout << "--------------------------------------------------------------------" << endl;
	cout << "STATE : ";
	for (int i = 0; i < 9; i++)
		cout << startState[i];
	cout << "            METHOD : ASTAR " << "H" << heuristicNum << endl;

	if (isGoal(startState))
	{
		cout << "Total Visit : " << closedList.size() << endl;
		cout << "Max Open List Size " << maxOpen << endl;
		cout << "Length : " << (*currentNode).depth + 1 << endl;
		cout << "--------------------------------------------------------------------" << endl;
		return;
	}
	else
	{
		nodeQueue.push(currentNode);

		while (1)
		{
			if (nodeQueue.empty())
			{
				cout << "Fail to find the goal" << endl;
				return;
			}
			else
			{
				while (1)
				{
					currentNode = nodeQueue.top();
					nodeQueue.pop();
					int isExist = 0;
					for (list<pair<int, int>>::iterator it = closedList.begin(); it != closedList.end(); it++)
					{
						if ((*it).first == stateInteger((*currentNode).state))
						{
							isExist = 1;

							if ((*it).second > (*currentNode).cost)
								closedList.erase(it);
							else
								isExist = 2;

							break;
						}
					}

					if ((isExist == 0) || (isExist == 1))
						break;
				}

				if (isGoal((*currentNode).state))
				{
					cout << "Total Visit : " << closedList.size() << endl;
					cout << "Max Open List Size " << maxOpen << endl;
					cout << "Length : " << (*currentNode).depth + 1<< endl;
					cout << "--------------------------------------------------------------------" << endl;
					return;
				}
				else
				{
					closedList.push_back(make_pair(stateInteger((*currentNode).state),(*currentNode).cost));

					index = findZero((*currentNode).state);

					switch (index)
					{
						/*
						0 1 2
						3 4 5
						6 7 8
						*/

					case 4:
						positionIndex = { 1, 3, 5, 7 };
						break;

					case 0:
						positionIndex = { 1, 3 };
						break;
					case 2:
						positionIndex = { 1, 5 };
						break;
					case 6:
						positionIndex = { 3, 7 };
						break;
					case 8:
						positionIndex = { 5, 7 };
						break;

					case 1:
						positionIndex = { 0, 2, 4 };
						break;
					case 3:
						positionIndex = { 0, 4, 6 };
						break;
					case 5:
						positionIndex = { 2, 4, 8 };
						break;
					case 7:
						positionIndex = { 4, 6, 8 };
						break;

					case -1:
					default:
						cout << "Error" << endl;
						return;
					}

					for (vector<int>::iterator it = positionIndex.begin(); it != positionIndex.end(); it++)
					{
						int* newState = new int[9];
						swapState(currentNode->state, newState, *it, index);

						// redundancy check

						int isExist = 0;
						for (list<pair<int,int>>::iterator Nit = closedList.begin(); Nit != closedList.end(); ++Nit){
							if ((*Nit).first == stateInteger(newState))		//
							{
								isExist = 1;
								break;
							}
						}

						if (!isExist)
						{
							myChild = expandNode(currentNode, newState, heuristicNum);
							if (myChild != nullptr)
								nodeQueue.push(myChild);
						}

						delete[] newState;
					}
					maxOpen = ((unsigned)maxOpen > nodeQueue.size()) ? maxOpen : nodeQueue.size();

				}





			}
		}
	}
}



bool isGoal(int state[])
{
	int i;
	for (i = 0; i < 8; i++)				// state[7] == 8 && state[8] == 0
	{
		if (state[i] != i + 1)
			return false;
	}

	return true;						// 1 to 8
}



void printNode(NODE node)
{
	cout << node.state[0] << " " << node.state[1] << " " << node.state[2] << endl;
	cout << node.state[3] << " " << node.state[4] << " " << node.state[5] << endl;
	cout << node.state[6] << " " << node.state[7] << " " << node.state[8] << endl;
}
int findZero(int state[])
{
	int i;
	for (i = 0; i < 9; i++)
		if (state[i] == 0)
			return i;

	return -1;
}
NODE* expandNode(NODE* currentNode, int newState[])
{
	NODE* child = new NODE(newState, (*currentNode).depth + 1, currentNode);

	for (int i = 0; i < 4; i++)
	{
		if ((*currentNode).children[i] == nullptr)
		{
			(*currentNode).children[i] = child;
			break;
		}
	}

	
	
	return child;
}
hNODE* expandNode(hNODE* currentNode, int newState[],int heuristicNum)
{
	hNODE* child = new hNODE(newState, (*currentNode).depth + 1, currentNode,heuristicNum);

	for (int i = 0; i < 4; i++)
	{
		if ((*currentNode).children[i] == nullptr)
		{
			(*currentNode).children[i] = child;
			break;
		}
	}



	return child;
}

void swapState(const int* state, int* newState, int position, int base)
{
	int temp = state[position];
	for (int i = 0; i < 9; i++)
	{
		newState[i] = state[i];
	}
	newState[base] = temp;
	newState[position] = 0;

}

