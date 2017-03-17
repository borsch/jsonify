#include "jsonify.h"
using namespace jsonify_;

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

string read(const string& path);

int main()
{
	string source = read("test.json");
	
	jsonify json(source);
	int a = json["int"].as_int();
	cout << a << endl;

	system("pause");
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