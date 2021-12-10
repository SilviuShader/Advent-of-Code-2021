#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main()
{
	ifstream fin("input.txt");

	vector<string> lines;
	string currentLine;
	while (fin >> currentLine)
		lines.push_back(currentLine);

	fin.close();

	unordered_set<char> openingBrackets;
	openingBrackets.insert('(');
	openingBrackets.insert('[');
	openingBrackets.insert('{');
	openingBrackets.insert('<');

	unordered_map<char, int> closingBrackets;
	closingBrackets[')'] = 3;
	closingBrackets[']'] = 57;
	closingBrackets['}'] = 1197;
	closingBrackets['>'] = 25137;

	unordered_map<char, int> bracketScores;
	bracketScores['('] = 1;
	bracketScores['['] = 2;
	bracketScores['{'] = 3;
	bracketScores['<'] = 4;

	int result1 = 0;

	vector<unsigned long long> scoresArray;

	for (int i = 0; i < lines.size(); i++)
	{
		bool corrupt = false;
		vector<char> bracketsStack;

		for (auto& chr : lines[i])
		{
			if (openingBrackets.find(chr) != openingBrackets.end())
				bracketsStack.push_back(chr);
			else
			{
				if (closingBrackets.find(chr) != closingBrackets.end())
				{
					if (bracketsStack.size() <= 0)
					{
						corrupt = true;
					}
					else
					{
						if (chr == ')' && bracketsStack.back() == '(' ||
							chr == ']' && bracketsStack.back() == '[' ||
							chr == '}' && bracketsStack.back() == '{' ||
							chr == '>' && bracketsStack.back() == '<')
							bracketsStack.pop_back();
						else
							corrupt = true;
					}
				}

				if (corrupt)
				{
					result1 += closingBrackets[chr];
					break;
				}
			}
		}

		if (!corrupt)
		{
			unsigned long long currentScore = 0;
			while (!bracketsStack.empty())
			{
				char last = bracketsStack.back();
				bracketsStack.pop_back();

				currentScore = currentScore * 5 + bracketScores[last];
			}
			scoresArray.push_back(currentScore);
		}
	}

	cout << result1 << endl;

	sort(scoresArray.begin(), scoresArray.end());

	cout << scoresArray[scoresArray.size() / 2];

	return 0;
}