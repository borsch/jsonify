#ifndef _JSONIFY_H_
#define _JSONIFY_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;


namespace jsonify_
{
	class jsonify
	{
	private:
		enum state
		{
			NAME,
			VALUE,

			COLON,

			NONE
		};

		class json_regular_symbols
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

		map<string, int> _data;
		state _state;
	public:
		jsonify(const string& source)
		{
			parse(source);
		}
		~jsonify(){};

	private:
		void parse(const string& source)
		{
			bool backslash = false;
			bool square_bracket = false;
			string name, value, current_value;
			map<string, string> asd;
			_state = NONE;
			for (unsigned int i = 0; i < source.length(); ++i)
			{
				char c = source[i];
				switch (c)
				{
				case json_regular_symbols::OPEN_CURLY_BRAKET: 
					if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else
					{
						_state = NONE;
					}
					break;
				case json_regular_symbols::CLOSE_CURLY_BRACKET:
					if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else if (_state == VALUE || _state == COLON)
					{
						value = current_value;
						asd[name] = value;
						current_value = string();
						_state = NONE;
					}
					else
					{
						_state = NONE;
					}
					break;
				
				case json_regular_symbols::COLON:
					if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else
					{
						_state = COLON;
					}
					break;
				
				case json_regular_symbols::DOUBLE_QUOTE:
					if (backslash)
					{
						if (_state == NAME || _state == VALUE)
						{
							current_value += c;
						}
						backslash = false;
					}
					else
					{
						if (_state == NAME)
						{
							//end reading field name
							_state = COLON;
							name = current_value;
							current_value = string();
						}
						else if (_state == VALUE)
						{
							//end reading field value
							current_value += c;
							_state = NONE;
						}
						else if (_state == NONE)
						{
							_state = NAME;
						}
						else if (_state == COLON)
						{
							current_value += c;
							_state = VALUE;
						}
					}
					break;
				
				case json_regular_symbols::OPEN_SQUARE_BRACKET:
					if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else if (_state == COLON)
					{
						_state = VALUE;
						current_value += c;
						square_bracket = true;
					}
					break;
				
				case json_regular_symbols::CLOSE_SQUARE_BRACKET:
					if (_state == VALUE && square_bracket)
					{
						current_value += c;
						value = current_value;
						asd[name] = value;
						_state = NONE;
						square_bracket = false;
					}
					else if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else
					{
						// invalid json
					}
					break;
				
				case json_regular_symbols::COMA:
					if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					else
					{
						value = current_value;
						asd[name] = value;
						current_value = string();
						_state = NONE;
					}
					break;
				
				case json_regular_symbols::BACKSLASH:
					backslash = !backslash;
					break;
				
				default:
					if (_state == COLON)
					{
						if (c != ' ' && c != '\t' && c != '\n')
						{
							current_value += c;
						}
					}
					else if (_state == NAME || _state == VALUE)
					{
						current_value += c;
					}
					break;
				}
			}
			for (std::map<string, string>::iterator it = asd.begin(); it != asd.end(); ++it)
				std::cout << it->first << "=>'" << it->second << "'\n";
		}
	};
}

#endif