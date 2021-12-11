#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

pair<int, int> Simulate(vector<vector<int>>& energies, int startStep, int endStep)
{
	int result = 0;
	int stopStep = -1;

	int dx[] = { -1, 0, 1, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 1, 0, -1, -1, 1, 1, -1 };

	for (int step = startStep; step < endStep; step++)
	{
		vector<pair<int, int>> nextFlashQueue;

		for (int i = 0; i < energies.size(); i++)
			for (int j = 0; j < energies[i].size(); j++)
			{
				energies[i][j]++;
				if (energies[i][j] > 9)
					nextFlashQueue.push_back(make_pair(i, j));
			}
		
		for (int i = 0; i < nextFlashQueue.size(); i++)
		{
			auto currentPos = nextFlashQueue[i];
			for (int k = 0; k < 8; k++)
			{
				pair<int, int> newPos = make_pair(currentPos.first + dx[k], currentPos.second + dy[k]);
				if (newPos.first >= 0 && newPos.first < energies.size() &&
					newPos.second >= 0 && newPos.second < energies[newPos.first].size())
				{
					energies[newPos.first][newPos.second]++;
					if (energies[newPos.first][newPos.second] == 10)
						nextFlashQueue.push_back(newPos);
				}
			}
		}

		for (int i = 0; i < nextFlashQueue.size(); i++)
		{
			auto currentPos = nextFlashQueue[i];
			energies[currentPos.first][currentPos.second] = 0;
		}

		if (stopStep == -1)
		{
			if (nextFlashQueue.size() == 100)
			{
				stopStep = step + 1;
			}
		}

		result += nextFlashQueue.size();
	}

	return make_pair(result, stopStep);
}

int main()
{
	ifstream fin("input.txt");

	vector<vector<int>> energies;
	for (int i = 0; i < 10; i++)
	{
		string line;
		fin >> line;

		vector<int> crtNumbers;
		for (auto& chr : line)
			crtNumbers.push_back(chr - '0');

		energies.push_back(crtNumbers);
	}
	
	fin.close();

	auto result1 = Simulate(energies, 0, 100);
	cout << result1.first << endl;
	if (result1.second == -1)
		cout << Simulate(energies, 100, 1000).second;
	else	
		cout << result1.second;

	return 0;
}