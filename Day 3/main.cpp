#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

void UpdateFrequencies(vector<int>& frequencies, const string& removedStr)
{
	for (int i = 0; i < removedStr.size(); i++)
		if (removedStr[i] == '1')
			frequencies[i]--;
}

int main()
{
	ifstream fin("input.txt");

	string crtWord;
	vector<int> frequencies;
	list<string> oxigenGeneratorNumbers;
	list<string> scrubberRatingNumbers;
	int numbersCount = 0;

	while (fin >> crtWord)
	{
		const char* cStr = crtWord.c_str();
		if (frequencies.size() < crtWord.size())
			frequencies.resize(crtWord.size(), 0);

		bool number = false;

		for (int i = 0; i < crtWord.size(); i++)
		{
			switch (cStr[i])
			{
			case '0':
				number = true;
				break;
			case '1':
				frequencies[i]++;
				number = true;
				break;
			}
		}

		if (number)
		{
			oxigenGeneratorNumbers.push_back(crtWord);
			scrubberRatingNumbers.push_back(crtWord);
			numbersCount++;
		}
	}

	unsigned int gamma = 0;
	unsigned int epsilon = 0;

	for (int i = 0; i < frequencies.size(); i++)
	{
		int ones = frequencies[i];
		int zeros = numbersCount - ones;
		int digit = ones > zeros ? 1 : 0;
		gamma = (gamma << 1) + digit;
		epsilon = (epsilon << 1) + (1 - digit);
	}

	cout << gamma * epsilon << endl;

	vector<int> frequencies2(frequencies);

	for (int i = 0; i < frequencies.size(); i++)
	{
		int ones = frequencies[i];
		int zeros = oxigenGeneratorNumbers.size() - ones;
		int digit = ones >= zeros ? 1 : 0;

		if (oxigenGeneratorNumbers.size() > 1)
			oxigenGeneratorNumbers.remove_if([&](const string& str)
				{
					bool result = str[i] - '0' != digit;
					if (result)
						UpdateFrequencies(frequencies, str);
					return result;
				});
	}

	for (int i = 0; i < frequencies2.size(); i++)
	{
		int ones = frequencies2[i];
		int zeros = scrubberRatingNumbers.size() - ones;
		int digit = ones < zeros ? 1 : 0;

		if (scrubberRatingNumbers.size() > 1)
			scrubberRatingNumbers.remove_if([&](const string& str)
				{
					bool result = str[i] - '0' != digit;
					if (result)
						UpdateFrequencies(frequencies2, str);
					return result;
				});
	}

	string oxigenGen = oxigenGeneratorNumbers.front();
	string scrrubberR = scrubberRatingNumbers.front();

	unsigned int oxigenGenerator = 0;
	unsigned int scrubberRating = 0;
	
	for (int i = 0; i < frequencies.size(); i++)
	{
		oxigenGenerator = (oxigenGenerator << 1) + (oxigenGen[i] - '0');
		scrubberRating = (scrubberRating << 1) + (scrrubberR[i] - '0');
	}

	cout << oxigenGenerator * scrubberRating;

	return 0;
}