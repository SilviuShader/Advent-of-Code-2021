#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;

unordered_map<string, vector<string>> caveGraph;
unordered_map<string, bool> isBigCave;
unordered_map<string, int> usedCave;

int pathsCount = 0;

bool IsBigCave(string caveName)
{
	return isupper(caveName[0]);
}

void CountPaths(const string& currentNode, int maxSmallCave)
{
	if (currentNode == "end")
	{
		pathsCount++;
		return;
	}

	if (!isBigCave[currentNode])
	{
		usedCave[currentNode]++;
		if (usedCave[currentNode] == 2)
			maxSmallCave--;
	}

	for (auto& neigbour : caveGraph[currentNode])
		if (usedCave[neigbour] < maxSmallCave)
			if (neigbour != "start")
				CountPaths(neigbour, maxSmallCave);

	if (!isBigCave[currentNode])
		usedCave[currentNode]--;
}

int main()
{
	ifstream fin("input.txt");

	string line;
	while (fin >> line)
	{
		stringstream lineStream(line);
		string firstNode;
		string secondNode;
		getline(lineStream, firstNode, '-');
		getline(lineStream, secondNode, '-');

		if (caveGraph.find(firstNode) == caveGraph.end())
			caveGraph[firstNode] = vector<string>();
		if (caveGraph.find(secondNode) == caveGraph.end())
			caveGraph[secondNode] = vector<string>();

		caveGraph[firstNode].push_back(secondNode);
		caveGraph[secondNode].push_back(firstNode);

		isBigCave[firstNode] = IsBigCave(firstNode);
		isBigCave[secondNode] = IsBigCave(secondNode);

		usedCave[firstNode] = 0;
		usedCave[secondNode] = 0;
	}

	fin.close();

	CountPaths("start", 1);

	cout << pathsCount << endl;
	pathsCount = 0;

	CountPaths("start", 2);
	cout << pathsCount;

	return 0;
}