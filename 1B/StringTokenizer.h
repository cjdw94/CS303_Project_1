#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H
#include <string>

int getNumber(int low, int high, std::string message);


/** The string_tokenizer class splits a string into a sequence of subtrings,
called tokens, separated by delimeters.
*/
class String_Tokenizer
{
public:
	/** Construct a String_Tokenizer
	@param source The string to be split into tokens
	@param delim The string containing the delimeters. If
	this parameter is omitted, a space character is assumed.
	*/
	String_Tokenizer(std::string source, std::string delim = " ") :
		the_source(source), the_delim(delim), start(0), end(0) {
		find_next();
	}

	/** Determine if there are more tokens
	@return true if there are more tokens
	*/
	bool String_Tokenizer::has_more_tokens() {
		return start != std::string::npos;
	}

	/** Retrieve the next token
	@return the next token. If there are no more
	tokens, an empty string is returned
	*/
	std::string String_Tokenizer::next_token() {
		// Make sure there is a next token
		if (!has_more_tokens())
			return "";
		// Save the next token in return_value
		/*<snippet id="3" omit="false">*/
		std::string token = the_source.substr(start, end - start);
		/*</snippet>*/
		// Find the following token
		find_next();
		// Return the next token
		return token;
	}

private:
	/** Position start and end so that start is the index of the start
	of the next token and end is the end.
	*/
	void String_Tokenizer::find_next() {
		// Find the first character that is not a delimeter
		/*<snippet id="1" omit="false">*/
		start = the_source.find_first_not_of(the_delim, end);
		/*</snippet>*/
		// Find the next delimeter
		/*<snippet id="2" omit="false">*/
		end = the_source.find_first_of(the_delim, start);
		/*</snippet>*/
	}

	/** The string to be split into tokens */
	std::string the_source;
	/** The string of delimeters */
	std::string the_delim;
	/** The index of the start of the next token */
	size_t start;
	/** The index of the end of the next token*/
	size_t end;
};


#endif