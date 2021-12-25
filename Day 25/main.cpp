#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool Step(char** rawMap, int n, int m)
{
	char** copy = new char* [n];
	for (int i = 0; i < n; i++)
	{
		copy[i] = new char[m];
		memcpy(copy[i], rawMap[i], sizeof(char) * m);
	}

	bool canStep = false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			char currentChr = copy[i][j];
			if (currentChr == '>')
			{
				int nextJ = (j + 1) % m;
				if (copy[i][nextJ] == '.')
				{
					rawMap[i][j] = '.';
					rawMap[i][nextJ] = '>';
					canStep = true;
				}
			}
		}
	}

	for (int i = 0; i < n; i++)
		memcpy(copy[i], rawMap[i], sizeof(char) * m);

	for (int j = 0; j < m; j++)
	{
		for (int i = 0; i < n; i++)
		{
			char currentChr = copy[i][j];
			if (currentChr == 'v')
			{
				int nextI = (i + 1) % n;
				if (copy[nextI][j] == '.')
				{
					rawMap[i][j] = '.';
					rawMap[nextI][j] = 'v';
					canStep = true;
				}
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		if (copy[i])
		{
			delete copy[i];
			copy[i] = nullptr;
		}
	}

	if (copy)
	{
		delete[] copy;
		copy = nullptr;
	}

	return canStep;
}

int main()
{
	ifstream fin("input.txt");

	vector<string> readLines;
	string currentLine;
	while (fin >> currentLine)
		readLines.push_back(currentLine);

	fin.close();
	
	int n, m;
	n = readLines.size();
	m = n;
	char** rawMap = new char*[n];
	for (int i = 0; i < n; i++)
	{
		m = readLines[i].size();
		rawMap[i] = new  char[m];
		memcpy(rawMap[i], readLines[i].c_str(), sizeof(char) * m);
	}

	int step = 0;
	while (Step(rawMap, n, m))
		step++;

	cout << step + 1 << endl;

	for (int i = 0; i < n; i++)
	{
		if (rawMap[i])
		{
			delete[] rawMap[i];
			rawMap[i] = nullptr;
		}
	}

	if (rawMap)
	{
		delete[] rawMap;
		rawMap = nullptr;
	}

	return 0;
}