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
		bool contains(const string&);
	private:
		void parse(const string&);
	};

	enum jsonify::state
	{
		NAME,
		VALUE,

		COLON,

		NONE
	};

	class jsonify::json_regular_symbols
	{
	public:
		static const char
			OPEN_CURLY_BRAKET = '{',
			CLOSE_CURLY_BRACKET = '}',
			OPEN_SQUARE_BRACKET = '[',
			CLOSE_SQUARE_BRACKET = ']',
			DOUBLE_QUOTE = '\"',
			COLON = ':',
			COMA = ',',
			NEW_LINE = '\n',
			BACKSLASH = '\\';
	};

	converter::converter(const string& s) :
		_source(s)
	{ }

	converter::operator string() { return _source; }
	string converter::as_string() { return _source; }
	int converter::as_int() { return atoi(_source.c_str()); }
	double converter::as_double() { return atof(_source.c_str()); }
	vector<int> converter::as_int_vector() { return vector<int>(); }

	jsonify::jsonify(const string& source)
	{
		parse(source);
	}

	jsonify::~jsonify()
	{

	}

	void jsonify::parse(const string& source)
	{
		bool backslash = false;
		bool square_bracket = false;
		string name, value, current_value;
		_state = state::NONE;
		for (unsigned int i = 0; i < source.length(); ++i)
		{
			char c = source[i];
			switch (c)
			{
			case json_regular_symbols::OPEN_CURLY_BRAKET:
				if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else
				{
					_state = state::NONE;
				}
				break;
			case json_regular_symbols::CLOSE_CURLY_BRACKET:
				if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else if (_state == state::VALUE || _state == state::COLON)
				{
					value = current_value;
					_data[name] = new converter(value);
					current_value = string();
					_state = state::NONE;
				}
				else
				{
					_state = state::NONE;
				}
				break;

			case json_regular_symbols::COLON:
				if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else
				{
					_state = state::COLON;
				}
				break;

			case json_regular_symbols::DOUBLE_QUOTE:
				if (backslash)
				{
					if (_state == state::NAME || _state == state::VALUE)
					{
						current_value += c;
					}
					backslash = false;
				}
				else
				{
					if (_state == state::NAME)
					{
						//end reading field name
						_state = state::COLON;
						name = current_value;
						current_value = string();
					}
					else if (_state == state::VALUE)
					{
						//end reading field value
						current_value += c;
						_state = state::NONE;
					}
					else if (_state == state::NONE)
					{
						_state = state::NAME;
					}
					else if (_state == state::COLON)
					{
						current_value += c;
						_state = state::VALUE;
					}
				}
				break;

			case json_regular_symbols::OPEN_SQUARE_BRACKET:
				if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else if (_state == state::COLON)
				{
					_state = state::VALUE;
					current_value += c;
					square_bracket = true;
				}
				break;

			case json_regular_symbols::CLOSE_SQUARE_BRACKET:
				if (_state == state::VALUE && square_bracket)
				{
					current_value += c;
					value = current_value;
					_data[name] = new converter(value);
					_state = state::NONE;
					square_bracket = false;
				}
				else if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else
				{
					// invalid json
				}
				break;

			case json_regular_symbols::COMA:
				if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				else
				{
					value = current_value;
					_data[name] = new converter(value);
					current_value = string();
					_state = state::NONE;
				}
				break;

			case json_regular_symbols::BACKSLASH:
				backslash = !backslash;
				break;

			default:
				if (_state == state::COLON)
				{
					if (c != ' ' && c != '\t' && c != '\n')
					{
						current_value += c;
					}
				}
				else if (_state == state::NAME || _state == state::VALUE)
				{
					current_value += c;
				}
				break;
			}
		}
	}

	converter jsonify::operator[](const string& key)
	{
		converter* c = _data[key];
		if (!c)
		{
			return NULL;
		}
		return *c;
	}

	bool jsonify::contains(const string& key)
	{
		return _data[key] != NULL;
	}
}

#endif