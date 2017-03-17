#include "jsonify.h"
using namespace jsonify_;

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	string s;
	ifstream stream("test.json");
	string line;
	while (getline(stream, line))
	{
		s += line;
	}

	cout << s << endl;

	jsonify json(s);

	system("pause");
	return 0;
}