#include "jsonify.h"
using namespace jsonify_;

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

string read(const string& path);

int main()
{
	jsonify json(read("test.json"));

	return 0;
}

string read(const string& path)
{
	string s;
	ifstream stream("test.json");
	string line;
	while (getline(stream, line))
	{
		s += line;
	}

	return s;
}