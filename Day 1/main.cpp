#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	ifstream fin("input.txt");
	
	vector<int> numbers;

	int crtNumber;
	while (fin >> crtNumber)
		numbers.push_back(crtNumber);

	int increases = 0;
	for (int i = 0; i < numbers.size() - 1; i++)
		if (numbers[i] < numbers[i + 1])
			increases++;

	cout << increases << endl;

	int increases2 = 0;
	for (int i = 1; i < numbers.size() - 2; i++)
	{
		int current = numbers[i - 1] + numbers[i] + numbers[i + 1];
		int next = numbers[i] + numbers[i + 1] + numbers[i + 2];

		if (current < next)
			increases2++;

	}

	cout << increases2 << endl;

	return 0;
}