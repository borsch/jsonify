#ifndef _JSONIFY_H_
#define _JSONIFY_H_

#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace std;


namespace jsonify_
{
	class converter
	{
	private:
		string _source;
	public:
		converter(const string& s = "");
		operator string();
		string as_string();
		int as_int();
		double as_double();
		vector<int> as_int_vector();
	};

	class jsonify
	{
	private:
		enum state;
		class json_regular_symbols;

		map<string, converter*> _data;
		state _state;
	public:
		jsonify(const string&);
		~jsonify();
		converter operator[](const string&);
	private:
		void parse(const string&);
	};

	
}

#endif