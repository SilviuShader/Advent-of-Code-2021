#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

typedef unsigned long long ull;

void PairsIteration(unordered_map<string, ull>& polymerPairs, unordered_map<string, char>& rulesMap, unordered_map<char, ull>& characterFrequencies)
{
	unordered_map<string, ull> pairsToAdd;
	vector<string> pairsToRemove;

	for (auto& currentPair : polymerPairs)
	{
		if (rulesMap.find(currentPair.first) != rulesMap.end())
		{
			pairsToRemove.push_back(currentPair.first);
			ull currentPairsCount = currentPair.second;
			char middleCharacter = rulesMap[currentPair.first];

			if (characterFrequencies.find(middleCharacter) != characterFrequencies.end())
				characterFrequencies[middleCharacter] += currentPairsCount;
			else
				characterFrequencies[middleCharacter] = currentPairsCount;

			char addPair1[] = { currentPair.first[0], middleCharacter, '\0' };
			char addPair2[] = { middleCharacter, currentPair.first[1], '\0' };

			string strAddPair1 = string(addPair1);
			string strAddPair2 = string(addPair2);

			if (pairsToAdd.find(strAddPair1) != pairsToAdd.end())
				pairsToAdd[strAddPair1] += currentPairsCount;
			else
				pairsToAdd[strAddPair1] = currentPairsCount;

			if (pairsToAdd.find(strAddPair2) != pairsToAdd.end())
				pairsToAdd[strAddPair2] += currentPairsCount;
			else
				pairsToAdd[strAddPair2] = currentPairsCount;
		}
	}

	for (auto& pairToRemove : pairsToRemove)
		polymerPairs.erase(pairToRemove);

	for (auto& pairToAdd : pairsToAdd)
	{
		if (polymerPairs.find(pairToAdd.first) != polymerPairs.end())
			polymerPairs[pairToAdd.first] += pairToAdd.second;
		else
			polymerPairs[pairToAdd.first] = pairToAdd.second;
	}
}

ull GetResult(const unordered_map<char, ull>& characterFrequencies)
{
	ull mn = 0;
	ull mx = 0;

	bool firstStep = true;

	for (auto chrFreq : characterFrequencies)
	{
		if (firstStep)
			mn = chrFreq.second;
		else
			mn = min(mn, chrFreq.second);
		if (firstStep)
			mx = chrFreq.second;
		else
			mx = max(mx, chrFreq.second);

		firstStep = false;
	}

	return mx - mn;
}

int main()
{
	ifstream fin("input.txt");

	string initialPolymer;
	fin >> initialPolymer;

	string rule, arrow;
	char changeChar;

	unordered_map<string, char> rulesMap;

	while (fin >> rule >> arrow >> changeChar)
		rulesMap[rule] = changeChar;
	
	fin.close();

	unordered_map<string, ull> currenPolymerPairs;

	for (int i = 0; i < initialPolymer.size() - 1; i++)
	{
		char str[] = {initialPolymer[i], initialPolymer[i + 1], '\0'};
		string currentPair = string(str);
		if (currenPolymerPairs.find(currentPair) == currenPolymerPairs.end())
			currenPolymerPairs[currentPair] = 0;
		currenPolymerPairs[currentPair]++;
	}

	unordered_map<char, ull> characterFrequencies;

	for (auto& chr : initialPolymer)
	{
		if (characterFrequencies.find(chr) != characterFrequencies.end())
			characterFrequencies[chr]++;
		else
			characterFrequencies[chr] = 1;
	}

	for (int i = 0; i < 10; i++)
		PairsIteration(currenPolymerPairs, rulesMap, characterFrequencies);

	cout << GetResult(characterFrequencies) << endl;

	for (int i = 0; i < 30; i++)
		PairsIteration(currenPolymerPairs, rulesMap, characterFrequencies);

	cout << GetResult(characterFrequencies) << endl;

	return 0;
}