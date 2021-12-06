#include <fstream>
#include <iostream>

typedef unsigned long long ULL;

const int MAX_TIMER = 8;

using namespace std;

ULL SpawnCount(ULL* frequencies, int days)
{
	ULL result = 0;

	for (int day = 0; day < days; day++)
	{
		ULL zeros = frequencies[0];
		for (int bucket = 0; bucket <= MAX_TIMER; bucket++)
			frequencies[bucket] = (bucket < MAX_TIMER) ? frequencies[bucket + 1] : zeros;

		frequencies[MAX_TIMER - 2] += zeros;
	}

	for (int i = 0; i <= MAX_TIMER; i++)
		result += frequencies[i];

	return result;
}

int main()
{
	ifstream fin("input.txt");
	
	int nr;
	char comma;
	ULL frequencies[MAX_TIMER + 1];
	ULL secondFrequencies[MAX_TIMER + 1];
	memset(frequencies, 0, sizeof(ULL) * (MAX_TIMER + 1));
	while (fin >> nr)
	{
		fin >> comma;
		frequencies[nr]++;
	}
	memcpy(secondFrequencies, frequencies, sizeof(ULL) * (MAX_TIMER + 1));

	cout << SpawnCount(frequencies, 80) << endl;
	cout << SpawnCount(secondFrequencies, 256);

	fin.close();

	return 0;
}