#include <fstream>
#include <iostream>
#include <list>

using namespace std;

void Extend(list<list<char>>& image, int timesExtended)
{
	for (int i = 0; i < timesExtended; i++)
	{
		int len = image.front().size();
		list<char> lineToAdd;
		for (int j = 0; j < len; j++)
			lineToAdd.push_back('.');
		for (int j = 0; j < 3; j++)
		{
			image.push_front(lineToAdd);
			image.push_back(lineToAdd);
		}
		for (auto& line : image)
		{
			for (int j = 0; j < 3; j++)
			{
				line.push_front('.');
				line.push_back('.');
			}
		}
	}
}

int ImageAlgo(const string& enchAlgo, list<list<char>>& currentImage, int infinityDigit)
{
	int n = currentImage.size();
	int m = n;
	char** charLines = new char*[n];
	int index = 0;
	for (auto& line : currentImage)
	{
		m = line.size();
		charLines[index] = new char[m];
		int index2 = 0;
		for (auto& chr : line)
			charLines[index][index2++] = chr;

		index++;
	}

	int i = 0;
	for (auto& line : currentImage)
	{
		int j = 0;
		for (auto& chr : line)
		{
			int algoIndex = 0;
			for (int kernelI = -1; kernelI <= 1; kernelI++)
			{
				int iIndex = 0;
				for (int kernelJ = -1; kernelJ <= 1; kernelJ++)
				{
					int newI = kernelI + i;
					int newJ = kernelJ + j;
					int digit = infinityDigit;
					if (newI >= 0 && newI < n &&
						newJ >= 0 && newJ < m)
						digit = charLines[newI][newJ] == '.' ? 0 : 1;
					
					iIndex = (iIndex << 1) + digit;
				}
				algoIndex = (algoIndex << 3) + iIndex;
			}
			chr = enchAlgo[algoIndex];
			j++;
		}
		i++;
	}

	int result = 0;
	if (currentImage.front().front() == '#')
		result = 1;

	for (int i = 0; i < currentImage.size(); i++)
	{
		if (charLines[i])
		{
			delete[] charLines[i];
			charLines[i] = nullptr;
		}
	}

	if (charLines)
	{
		delete[] charLines;
		charLines = nullptr;
	}

	return result;
}

int main()
{
	ifstream fin("input.txt");
	string enchAlgo;
	list<list<char>> currentImage;
	fin >> enchAlgo;

	string crtLine;
	while (fin >> crtLine)
	{
		list<char> crtChars;

		for (auto& chr : crtLine)
			crtChars.push_back(chr);

		currentImage.push_back(crtChars);
	}

	fin.close();

	Extend(currentImage, 50);
	int infinityDigit = 0;
	for (int i = 0; i < 2; i++)
		infinityDigit = ImageAlgo(enchAlgo, currentImage, infinityDigit);

	int result1 = 0;
	for (auto& line : currentImage)
		for (auto& chr : line)
			result1 += chr == '#';
	
	cout << result1 << endl;

	for (int i = 0; i < 48; i++)
		infinityDigit = ImageAlgo(enchAlgo, currentImage, infinityDigit);

	int result2 = 0;
	for (auto& line : currentImage)
		for (auto& chr : line)
			result2 += chr == '#';

	cout << result2 << endl;

	return 0;
}