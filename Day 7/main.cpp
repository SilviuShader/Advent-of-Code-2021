#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	ifstream fin("input.txt");

	int nr;
	char comma;
	vector<int> numbers;
	while (fin >> nr)
	{
		fin >> comma;
		numbers.push_back(nr);
	}
	fin.close();

	sort(numbers.begin(), numbers.end());

	int median = numbers[numbers.size() / 2];
	int result1 = 0;
	for (auto& number : numbers)
		result1 += abs(number - median);

	cout << result1 << endl;

	int result2 = -1;
	int maxN = numbers[numbers.size() - 1];
	for (int number = 0; number <= maxN; number++)
	{
		int crtResult = 0;
		for (auto& testNumber : numbers)
		{
			int diff = abs(testNumber - number);
			crtResult += (diff * (diff + 1)) / 2;
		}
		if (result2 == -1 || crtResult < result2)
			result2 = crtResult;
	}

	cout << result2 << endl;

	return 0;
}