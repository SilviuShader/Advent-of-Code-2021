#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

bool ValidPos(pair<int, int> position, int n, int m)
{
	if (position.first >= 0 && position.first < n &&
		position.second >= 0 && position.second < m)
		return true;

	return false;
}

int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, -1, 0, 1 };

pair<int, vector<pair<int, int>>> Solve1(int** numbers, int n, int m)
{
	int result = 0;
	vector<pair<int, int>> positions;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			int crtVal = numbers[i][j];
			bool minimum = true;

			for (int k = 0; k < 4; k++)
			{
				pair<int, int> nextPos = make_pair(i + dx[k], j + dy[k]);
				if (ValidPos(nextPos, n, m))
				{
					int nextVal = numbers[nextPos.first][nextPos.second];
					if (nextVal <= crtVal)
						minimum = false;
				}
			}

			if (minimum)
			{
				result += crtVal + 1;
				positions.push_back(make_pair(i, j));
			}
		}
	}

	return make_pair(result, positions);
}

int Area(int** numbers, bool** usedPos, int n, int m, pair<int, int> startPosition)
{
	queue<pair<int, int>> leeQueue;
	int result = 1;

	leeQueue.push(startPosition);
	usedPos[startPosition.first][startPosition.second] = true;

	while (!leeQueue.empty())
	{
		pair<int, int> currentPosition = leeQueue.front();
		leeQueue.pop();

		int crtNumber = numbers[currentPosition.first][currentPosition.second];

		for (int i = 0; i < 4; i++)
		{
			pair<int, int> nextPos = make_pair(currentPosition.first + dx[i], currentPosition.second + dy[i]);
			if (ValidPos(nextPos, n, m))
			{
				if (!usedPos[nextPos.first][nextPos.second])
				{
					int nextNumber = numbers[nextPos.first][nextPos.second];
					if (nextNumber != 9 && nextNumber > crtNumber)
					{
						leeQueue.push(nextPos);
						usedPos[nextPos.first][nextPos.second] = true;
						result++;
					}
				}
			}
		}
	}

	return result;
}

int main()
{
	ifstream fin("input.txt");

	vector<string> readLines;

	string line;
	while (fin >> line)
		readLines.push_back(line);
	
	fin.close();

	auto linesCount = readLines.size();
	int** numbers = new int*[linesCount];
	bool** usedPos = new bool* [linesCount];
	int n, m;
	n = linesCount;

	for (int i = 0; i < linesCount; i++)
	{
		const char* cStr = readLines[i].c_str();
		int lineLength = readLines[i].length();
		m = lineLength;

		numbers[i] = new int[lineLength];
		usedPos[i] = new bool[lineLength];

		for (int j = 0; j < lineLength; j++)
			numbers[i][j] = cStr[j] - '0';

		memset(usedPos[i], false, sizeof(bool) * lineLength);
	}

	auto solution1 = Solve1(numbers, n, m);
	cout << solution1.first << endl;

	vector<int> basinSizes;
	for (auto& startPos : solution1.second)
		basinSizes.push_back(Area(numbers, usedPos, n, m, startPos));

	sort(basinSizes.begin(), basinSizes.end(), [&](int a, int b) { return a > b; }); // lazy dev being lazy

	cout << basinSizes[0] * basinSizes[1] * basinSizes[2];
	
	for (int i = 0; i < linesCount; i++)
	{
		if (usedPos[i])
		{
			delete[] usedPos[i];
			usedPos[i] = nullptr;
		}

		if (numbers[i])
		{
			delete[] numbers[i];
			numbers[i] = nullptr;
		}
	}


	if (usedPos)
	{
		delete[] usedPos;
		usedPos = nullptr;
	}

	if (numbers)
	{
		delete[] numbers;
		numbers = nullptr;
	}

	return 0;
}