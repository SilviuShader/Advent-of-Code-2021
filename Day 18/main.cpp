#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Pair
{
public:

	Pair(int regularNumber = -1) :
		RegularNumber(regularNumber),
		LeftChild(nullptr),
		RightChild(nullptr),
		parent(nullptr)
	{
	}

	Pair(Pair* copyPair) :
		parent(nullptr)
	{
		RegularNumber = copyPair->RegularNumber;
		LeftChild = copyPair->LeftChild ? new Pair(copyPair->LeftChild) : nullptr;
		RightChild = copyPair->RightChild ? new Pair(copyPair->RightChild) : nullptr;

		if (LeftChild)
			LeftChild->parent = this;
		if (RightChild)
			RightChild->parent = this;
	}

public:

	int   RegularNumber;
	Pair* LeftChild;
	Pair* RightChild;

	Pair* parent;
};

pair<Pair*, int> CreatePair(const string& str, int startIndex)
{
	if (str[startIndex] >= '0' && str[startIndex] <= '9')
	{
		int crtDigit = str[startIndex] - '0';
		int value = crtDigit;
		startIndex++;
		while (str[startIndex] >= '0' && str[startIndex] <= '9' && startIndex < str.size())
		{
			value = value * 10 + str[startIndex] - '0';
			startIndex++;
		}

		Pair* result = new Pair(value);
		return make_pair(result, startIndex);
	}
	else
	{
		startIndex++;
		Pair* result = new Pair();
		auto leftResult = CreatePair(str, startIndex);
		result->LeftChild = leftResult.first;
		startIndex = leftResult.second + 1;
		auto rightResult = CreatePair(str, startIndex);
		result->RightChild = rightResult.first;

		result->LeftChild->parent = result;
		result->RightChild->parent = result;

		return make_pair(result, rightResult.second + 1);
	}
}

pair<pair<Pair*, Pair*>, bool> Explode(Pair* previousLeaf, Pair* currentPair, Pair* explodedPair, int nestCount)
{
	if (nestCount >= 5 && explodedPair == nullptr)
	{
		if (currentPair->LeftChild != nullptr && currentPair->RightChild != nullptr)
		{
			if (currentPair->LeftChild->RegularNumber != -1 && currentPair->RightChild->RegularNumber != -1)
			{
				if (previousLeaf != nullptr)
					previousLeaf->RegularNumber += currentPair->LeftChild->RegularNumber;

				return make_pair(make_pair(previousLeaf, currentPair), false);
			}
		}
	}

	if (currentPair->LeftChild == nullptr || currentPair->RightChild == nullptr)
	{
		if (explodedPair == nullptr)
			return make_pair(make_pair(currentPair, explodedPair), false);
		else
		{
			currentPair->RegularNumber += explodedPair->RightChild->RegularNumber;
			return make_pair(make_pair(currentPair, explodedPair), true);
		}
	}
	auto leftOutput = Explode(previousLeaf, currentPair->LeftChild, explodedPair, nestCount + 1);
	explodedPair = leftOutput.first.second;
	previousLeaf = leftOutput.first.first;
	if (leftOutput.second)
		return leftOutput;

	auto rightOutput = Explode(previousLeaf, currentPair->RightChild, explodedPair, nestCount + 1);
	return rightOutput;
}

bool Split(Pair* pairToSplit)
{
	if (pairToSplit->RegularNumber != -1)
	{
		if (pairToSplit->RegularNumber >= 10)
		{
			int leftHalf = pairToSplit->RegularNumber / 2;
			int rightHalf = pairToSplit->RegularNumber % 2 == 0 ? leftHalf : leftHalf + 1;
			pairToSplit->RegularNumber = -1;
			pairToSplit->LeftChild = new Pair(leftHalf);
			pairToSplit->RightChild = new Pair(rightHalf);

			pairToSplit->LeftChild->parent = pairToSplit;
			pairToSplit->RightChild->parent = pairToSplit;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool resLeft = Split(pairToSplit->LeftChild);
	if (resLeft)
		return true;
	bool resRight = Split(pairToSplit->RightChild);
	return resRight;
}

void DebugPair(Pair* pairToDebug)
{
	if (pairToDebug->RegularNumber != -1)
		cout << pairToDebug->RegularNumber;
	else
	{
		cout << "[";
		DebugPair(pairToDebug->LeftChild);
		cout << ",";
		DebugPair(pairToDebug->RightChild);
		cout << "]";
	}
}

void Reduce(Pair* pairToReduce)
{
	bool isReduced = false;

	while (!isReduced)
	{
		isReduced = true;
		pair<pair<Pair*, Pair*>, bool> explodeResult = make_pair(make_pair(nullptr, nullptr), false);
		do
		{
			explodeResult = Explode(nullptr, pairToReduce, nullptr, 1);
			if (explodeResult.first.second)
			{
				explodeResult.first.second->RegularNumber = 0;

				if (explodeResult.first.second->LeftChild)
				{
					delete explodeResult.first.second->LeftChild;
					explodeResult.first.second->LeftChild = nullptr;
				}
				if (explodeResult.first.second->RightChild)
				{
					delete explodeResult.first.second->RightChild;
					explodeResult.first.second->RightChild = nullptr;
				}
				isReduced = false;
			}
		} while (explodeResult.first.second != nullptr);

		if (Split(pairToReduce))
			isReduced = false;
	}
}

Pair* Add(Pair* leftAdd, Pair* rightAdd)
{
	Pair* result = new Pair();

	result->LeftChild = leftAdd;
	result->RightChild = rightAdd;

	Reduce(result);

	return result;
}

int Magnitude(Pair* crtPair)
{
	if (crtPair->RegularNumber != -1)
		return crtPair->RegularNumber;

	return 3 * Magnitude(crtPair->LeftChild) + 2 * Magnitude(crtPair->RightChild);
}

int main()
{
	ifstream fin("input.txt");

	vector<Pair*> pairs;

	string crtPair;
	while (fin >> crtPair)
		pairs.push_back(CreatePair(crtPair, 0).first);

	fin.close();

	int maxMagnitude = 0;

	// This is for the second part.
	// The whole program leaks a ton of memory
	// But I ain't gonna bother fixing it, it's pretty late
	// when I'm coding this. And the solution already works,
	// So I won't get back to it anytime soon.
	for (int i = 0; i < pairs.size(); i++)
	{
		for (int j = 0; j < pairs.size(); j++)
		{
			if (i == j)
				continue;

			Pair* copy1 = new Pair(pairs[i]);
			Pair* copy2 = new Pair(pairs[j]);

			Pair* crtPair = Add(copy1, copy2);
			maxMagnitude = max(Magnitude(crtPair), maxMagnitude);
		}
	}

	cout << maxMagnitude << endl;

	Pair* resultPair = pairs[0];
	for (int i = 1; i < pairs.size(); i++)
		resultPair = Add(resultPair, pairs[i]);

	cout << Magnitude(resultPair) << endl;

	return 0;
}