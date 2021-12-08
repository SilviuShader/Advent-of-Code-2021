#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

const int PATTERNS_COUNT    = 10;
const int DIGITS_SHOW_COUNT = 4;

using namespace std;

struct DigitData
{
	string Patterns[PATTERNS_COUNT];
	string DigitsShow[DIGITS_SHOW_COUNT];
};

int Solve1(const vector<DigitData>& digitsData)
{
	int result = 0;
	for (auto& digit : digitsData)
		for (int i = 0; i < DIGITS_SHOW_COUNT; i++)	
			if (digit.DigitsShow[i].length() == 2 ||
				digit.DigitsShow[i].length() == 4 ||
				digit.DigitsShow[i].length() == 3 ||
				digit.DigitsShow[i].length() == 7)
				result++;
	
	return result;
}

bool usedNumbers[PATTERNS_COUNT];
unordered_set<char> rightMappings[PATTERNS_COUNT];

void FillRightMappings()
{
	rightMappings[0].insert('a');
	rightMappings[0].insert('b');
	rightMappings[0].insert('c');
	rightMappings[0].insert('e');
	rightMappings[0].insert('f');
	rightMappings[0].insert('g');

	rightMappings[1].insert('c');
	rightMappings[1].insert('f');

	rightMappings[2].insert('a');
	rightMappings[2].insert('c');
	rightMappings[2].insert('d');
	rightMappings[2].insert('e');
	rightMappings[2].insert('g');

	rightMappings[3].insert('a');
	rightMappings[3].insert('c');
	rightMappings[3].insert('d');
	rightMappings[3].insert('f');
	rightMappings[3].insert('g');

	rightMappings[4].insert('b');
	rightMappings[4].insert('c');
	rightMappings[4].insert('d');
	rightMappings[4].insert('f');

	rightMappings[5].insert('a');
	rightMappings[5].insert('b');
	rightMappings[5].insert('d');
	rightMappings[5].insert('f');
	rightMappings[5].insert('g');

	rightMappings[6].insert('a');
	rightMappings[6].insert('b');
	rightMappings[6].insert('d');
	rightMappings[6].insert('e');
	rightMappings[6].insert('f');
	rightMappings[6].insert('g');

	rightMappings[7].insert('a');
	rightMappings[7].insert('c');
	rightMappings[7].insert('f');

	rightMappings[8].insert('a');
	rightMappings[8].insert('b');
	rightMappings[8].insert('c');
	rightMappings[8].insert('d');
	rightMappings[8].insert('e');
	rightMappings[8].insert('f');
	rightMappings[8].insert('g');

	rightMappings[9].insert('a');
	rightMappings[9].insert('b');
	rightMappings[9].insert('c');
	rightMappings[9].insert('d');
	rightMappings[9].insert('f');
	rightMappings[9].insert('g');
}

vector< pair<int, pair<vector<char>, vector<char>>> > CheckMappings(DigitData& currentDigit, int currentPattern, unordered_map<char, char>& mappings)
{
	vector<int> possibleDigits;
	switch (currentDigit.Patterns[currentPattern].length())
	{
	case 2:
		possibleDigits.push_back(1);
		break;
	case 3:
		possibleDigits.push_back(7);
		break;
	case 4:
		possibleDigits.push_back(4);
		break;
	case 7:
		possibleDigits.push_back(8);
		break;
	case 6:
		possibleDigits.push_back(9);
		possibleDigits.push_back(6);
		possibleDigits.push_back(0);
		break;
	case 5:
		possibleDigits.push_back(2);
		possibleDigits.push_back(3);
		possibleDigits.push_back(5);
		break;
	}

	vector< pair<int, pair<vector<char>, vector<char>>> > result;

	for (auto& possibleDigit : possibleDigits)
	{
		if (!usedNumbers[possibleDigit])
		{
			bool validNumber = true;
			
			vector<char> remainingWordChars;
			unordered_set<char> targetsToAdd = rightMappings[possibleDigit];

			for (auto& chr : currentDigit.Patterns[currentPattern])
			{
				if (mappings.find(chr) != mappings.end())
				{
					char mapping = mappings[chr];
					if (rightMappings[possibleDigit].find(mapping) == rightMappings[possibleDigit].end())
						validNumber = false;
					else
						targetsToAdd.erase(mapping);
				}
				else
					remainingWordChars.push_back(chr);
			}

			if (validNumber)
				result.push_back(make_pair(possibleDigit, make_pair(remainingWordChars, vector<char>(targetsToAdd.begin(), targetsToAdd.end()))));
		}
	}

	return result;
}

bool usedChar['g' - 'a' + 1];
vector<char> currentPermSolution;

void GetPermutations(vector<char> initialString, int index, vector<vector<char>>& permutations)
{
	if (index >= initialString.size())
	{
		permutations.push_back(currentPermSolution);
		return;
	}

	for (int i = 0; i < initialString.size(); i++)
	{
		if (!usedChar[i])
		{
			usedChar[i] = true;
			currentPermSolution.push_back(initialString[i]);
			GetPermutations(initialString, index + 1, permutations);
			currentPermSolution.pop_back();
			usedChar[i] = false;
		}
	}
}

bool foundMatch;
int result2;

void FindMatch(DigitData& currentDigit, int currentPattern, unordered_map<char, char>& mappings)
{
	if (currentPattern >= PATTERNS_COUNT)
	{
		if (foundMatch)
			return;

		foundMatch = true;
		int number = 0;

		for (int i = 0; i < DIGITS_SHOW_COUNT; i++)
		{
			auto& currentDigitString = currentDigit.DigitsShow[i];
			int digit = -1;
			for (int i = 0; i < PATTERNS_COUNT && digit == -1; i++)
			{
				bool good = true;
				if (currentDigitString.length() == rightMappings[i].size())
				{
					for (auto& chr : currentDigitString)
						if (rightMappings[i].find(mappings[chr]) == rightMappings[i].end())
							good = false;
				}
				else
				{
					good = false;
				}
				if (good)
					digit = i;
			}
			if (digit == -1)
				foundMatch = false;
			number = number * 10 + digit;
		}

		if (foundMatch)
			result2 += number;

		return;
	}

	auto checkResult = CheckMappings(currentDigit, currentPattern, mappings);

	for (auto& checkPosibility : checkResult)
	{
		usedNumbers[checkPosibility.first] = true;

		if (checkPosibility.second.first.size() == 0)
		{
			FindMatch(currentDigit, currentPattern + 1, mappings);
		}
		else
		{
			vector<vector<char>> permutations;
			memset(usedChar, false, sizeof(bool) * ('g' - 'a' + 1));
			currentPermSolution.clear();
			GetPermutations(checkPosibility.second.first, 0, permutations);

			for (auto& permutation : permutations)
			{
				for (int i = 0; i < permutation.size(); i++)
					mappings[permutation[i]] = checkPosibility.second.second[i];

				FindMatch(currentDigit, currentPattern + 1, mappings);

				for (int i = 0; i < permutation.size(); i++)
					mappings.erase(permutation[i]);
			}
		}
		usedNumbers[checkPosibility.first] = false;
	}
}

int main()
{
	ifstream fin("input.txt");

	string line;
	vector<DigitData> digitsData;
	while (getline(fin, line))
	{
		istringstream iss(line);
		DigitData crtDigit;

		for (int i = 0; i < PATTERNS_COUNT; i++)
			iss >> crtDigit.Patterns[i];
		
		string separator;
		iss >> separator;

		for (int i = 0; i < DIGITS_SHOW_COUNT; i++)
			iss >> crtDigit.DigitsShow[i];
	
		digitsData.push_back(crtDigit);
	}

	fin.close();

	cout << Solve1(digitsData) << endl;

	unordered_map<int, int> lengthOrders;
	lengthOrders[2] = 0;
	lengthOrders[3] = 1;
	lengthOrders[4] = 2;
	lengthOrders[7] = 3;
	lengthOrders[6] = 4;
	lengthOrders[5] = 5;
	for (auto& digit : digitsData)
		sort(digit.Patterns, digit.Patterns + PATTERNS_COUNT, [&](const string& left, const string& right)
		{
				return lengthOrders[left.length()] < lengthOrders[right.length()];
		});

	FillRightMappings();
	
	result2 = 0;
	for (auto& digit : digitsData)
	{
		memset(usedNumbers, false, sizeof(bool) * PATTERNS_COUNT);
		unordered_map<char, char> mappings;
		foundMatch = false;
		FindMatch(digit, 0, mappings);
	}

	cout << result2;

	return 0;
}