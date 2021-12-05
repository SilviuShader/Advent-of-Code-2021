#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

struct Line
{
	pair<int, int> StartPoint;
	pair<int, int> EndPoint;
};

void FillHorizontal(int** map, const Line& line)
{
	int start = min(line.StartPoint.first, line.EndPoint.first);
	int end = max(line.StartPoint.first, line.EndPoint.first);

	for (int i = start; i <= end; i++)
		map[line.StartPoint.second][i]++;
}

void FillVertical(int** map, const Line& line)
{
	int start = min(line.StartPoint.second, line.EndPoint.second);
	int end = max(line.StartPoint.second, line.EndPoint.second);

	for (int i = start; i <= end; i++)
		map[i][line.StartPoint.first]++;
}

void FillDiagonal(int** map, const Line& line)
{
	int startX = line.StartPoint.first;
	int dirX = line.StartPoint.first < line.EndPoint.first ? 1 : -1;

	int startY = line.StartPoint.second;
	int dirY = line.StartPoint.second < line.EndPoint.second ? 1 : -1;

	for (int x = startX, y = startY; x != line.EndPoint.first + dirX && y != line.EndPoint.second + dirY; x += dirX, y += dirY)
		map[y][x]++;
}

int main()
{
	ifstream fin("input.txt");
	int x1, y1, x2, y2;
	char comma;
	string delimiter;

	vector<Line> lines;

	while (fin >> x1 >> comma >> y1 >> delimiter >> x2 >> comma >> y2)
		lines.push_back({ make_pair(x1, y1), make_pair(x2, y2) });

	fin.close();

	int maxX = 0;
	int maxY = 0;
	
	for (int i = 0; i < lines.size(); i++)
	{
		maxX = max(maxX, lines[i].StartPoint.first);
		maxX = max(maxX, lines[i].EndPoint.first);

		maxY = max(maxY, lines[i].StartPoint.second);
		maxY = max(maxY, lines[i].EndPoint.second);
	}

	int** lineCounters = new int*[maxY + 1];
	for (int i = 0; i <= maxY; i++)
	{
		lineCounters[i] = new int[maxX + 1];
		memset(lineCounters[i], 0, sizeof(int) * (maxX + 1));
	}

	for (int i = 0; i < lines.size(); i++)
	{
		auto& line = lines[i];
		if (line.StartPoint.second == line.EndPoint.second)
			FillHorizontal(lineCounters, line);

		if (line.StartPoint.first == line.EndPoint.first)
			FillVertical(lineCounters, line);
	}

	int result1 = 0;
	for (int i = 0; i <= maxY; i++)
		for (int j = 0; j <= maxX; j++)
			result1 += lineCounters[i][j] >= 2 ? 1 : 0;
		
	cout << result1 << endl;

	for (int i = 0; i < lines.size(); i++)
	{
		auto& line = lines[i];
		if (abs(line.StartPoint.first - line.EndPoint.first) == abs(line.StartPoint.second - line.EndPoint.second) &&
			line.StartPoint.first != line.EndPoint.first)
			FillDiagonal(lineCounters, line);
	}

	int result2 = 0;
	for (int i = 0; i <= maxY; i++)
		for (int j = 0; j <= maxX; j++)
			result2 += lineCounters[i][j] >= 2 ? 1 : 0;

	cout << result2;

	for (int i = 0; i <= maxY; i++)
	{
		if (lineCounters[i])
		{
			delete[] lineCounters[i];
			lineCounters[i] = nullptr;
		}
	}

	if (lineCounters)
	{
		delete[] lineCounters;
		lineCounters = nullptr;
	}
	
	return 0;
}