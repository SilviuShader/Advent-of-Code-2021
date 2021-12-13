#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

struct Fold
{
	bool IsXAxis;
	int Value;
};

int DoFold(char** paperDots, int& n, int& m, const Fold& fold)
{
	int result = 0;
	if (fold.IsXAxis)
	{
		int prevX = fold.Value - 1;
		int nextX = fold.Value + 1;

		for (; nextX < m; nextX++)
		{
			for (int y = 0; y < n; y++)
			{
				if (paperDots[y][nextX] == '#')
					paperDots[y][prevX] = '#';
			}
			prevX--;
		}

		m = fold.Value;
	}
	else
	{
		int prevY = fold.Value - 1;
		int nextY = fold.Value + 1;

		for (; nextY < n; nextY++)
		{
			for (int x = 0; x < m; x++)
			{
				if (paperDots[nextY][x] == '#')
					paperDots[prevY][x] = '#';
			}
			prevY--;
		}

		n = fold.Value;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (paperDots[i][j] == '#')
				result++;
		}
	}

	return result;
}

int main()
{
	ifstream fin("input.txt");

	vector<pair<int, int>> pointPositions;

	int x, y;
	char comma;
	int n = 0, m = 0;
	while (fin >> x >> comma >> y)
	{
		pointPositions.push_back(make_pair(x, y));
		n = max(n, y);
		m = max(m, x);
	}

	n++;
	m++;

	int initialN = n;

	fin.clear();
	fin.seekg(0);

	string tmp;
	for (int i = 0; i < pointPositions.size(); i++)
		fin >> tmp;

	string fld, along, actualFold;
	vector<Fold> folds;

	while (fin >> fld >> along >> actualFold)
	{
		Fold fold;
		fold.IsXAxis = actualFold[0] == 'x';
		fold.Value = atoi(actualFold.c_str() + 2);

		folds.push_back(fold);
	}

	char** paperDots = new char* [n];
	for (int i = 0; i < n; i++)
	{
		paperDots[i] = new char[m];
		memset(paperDots[i], '.', sizeof(char) * m);
	}

	for (auto& point : pointPositions)
		paperDots[point.second][point.first] = '#';

	cout << DoFold(paperDots, n, m, folds[0]) << endl;
	for (int i = 1; i < folds.size(); i++)
		DoFold(paperDots, n, m, folds[i]);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			cout << paperDots[i][j];
		cout << endl;
	}

	// fix this
	for (int i = 0; i < initialN; i++)
	{
		if (paperDots[i])
		{
			delete[] paperDots[i];
			paperDots[i] = nullptr;
		}
	}

	if (paperDots)
	{
		delete[] paperDots;
		paperDots = nullptr;
	}

	return 0;
}