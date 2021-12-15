#include <fstream>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef pair<int, pair<int, int>> QueueElement;

bool ValidPos(const pair<int, int>& pos, const vector<vector<int>>& riskLevels)
{
	if (pos.first >= 0 && pos.first < riskLevels.size() &&
		pos.second >= 0 && pos.second < riskLevels[pos.first].size())
		return true;

	return false;
}

int LeastRiskyPath(vector<vector<int>>& rawRisks, int repeat)
{
	vector<vector<int>> riskLevels;
	for (int i = 0; i < rawRisks.size(); i++)
	{
		vector<int> crtLine;
		for (int j = 0; j < rawRisks[i].size(); j++)
			crtLine.push_back(rawRisks[i][j]);
		riskLevels.push_back(crtLine);
	}

	for (int i = 0; i < rawRisks.size(); i++)
	{
		for (int k = 0; k < repeat - 1; k++)
		{
			for (int j = 0; j < rawRisks[i].size(); j++)
			{
				int prevValue = riskLevels[i][j + k * rawRisks[i].size()];
				int newValue = prevValue + 1;
				if (newValue >= 10)
					newValue = 1;
				riskLevels[i].push_back(newValue);
			}
		}
	}

	for (int k = 0; k < repeat - 1; k++)
	{
		for (int i = 0; i < rawRisks.size(); i++)
		{
			vector<int> newLine;
			for (int j = 0; j < riskLevels[0].size(); j++)
			{
				int prevValue = riskLevels[i + k * rawRisks.size()][j];
				int newValue = prevValue + 1;
				if (newValue >= 10)
					newValue = 1;
				newLine.push_back(newValue);
			}
			riskLevels.push_back(newLine);
		}
	}

	priority_queue<QueueElement, vector<QueueElement>, greater<QueueElement>> dijkstraQueue;
	vector<vector<int>> distances;
	for (int i = 0; i < riskLevels.size(); i++)
	{
		vector<int> crtLine;
		for (int j = 0; j < riskLevels[i].size(); j++)
			crtLine.push_back(-1);
		distances.push_back(crtLine);
	}

	bool foundSol = false;

	dijkstraQueue.push(make_pair(riskLevels[0][0], make_pair(0, 0)));
	distances[0][0] = riskLevels[0][0];

	int dx[] = { 0, -1, 0, 1 };
	int dy[] = { -1, 0, 1, 0 };

	while (!foundSol && !dijkstraQueue.empty())
	{
		auto currentElement = dijkstraQueue.top();
		dijkstraQueue.pop();

		pair<int, int> currentPos = currentElement.second;

		if (currentPos.first == riskLevels.size() - 1)
			if (currentPos.second == riskLevels[currentPos.first].size() - 1)
				foundSol = true;

		for (int i = 0; i < 4; i++)
		{
			pair<int, int> newPos = make_pair(currentPos.first + dx[i], currentPos.second + dy[i]);
			
			if (ValidPos(newPos, riskLevels))
			{
				int newDistance = currentElement.first + riskLevels[newPos.first][newPos.second];
				if (newDistance < distances[newPos.first][newPos.second] || distances[newPos.first][newPos.second] == -1)
				{
					distances[newPos.first][newPos.second] = newDistance;
					dijkstraQueue.push(make_pair(newDistance, newPos));
				}
			}
		}
	}

	return distances[riskLevels.size() - 1][riskLevels[riskLevels.size() - 1].size() - 1] - riskLevels[0][0];
}

int main()
{
	ifstream fin("input.txt");

	vector<string> riskLines;
	string currentLine;
	while (fin >> currentLine)
		riskLines.push_back(currentLine);

	fin.close();

	vector<vector<int>> riskLevels;
	for (auto& line : riskLines)
	{
		vector<int> currentRiskLine;
		for (auto& chr : line)
			currentRiskLine.push_back(chr - '0');

		riskLevels.push_back(currentRiskLine);
	}

	cout << LeastRiskyPath(riskLevels, 1) << endl;
	cout << LeastRiskyPath(riskLevels, 5) << endl;


	return 0;
}