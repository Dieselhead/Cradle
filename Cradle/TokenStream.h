#ifndef _TOKENSTREAM_
#define _TOKENSTREAM_

#include <string>


class TokenStream
{
public:
	TokenStream();
	void ResetStream();
	void SetTokenStream(char* data);
	bool GetNextToken(std::string* buffer, char* delimiters, int totalDelimiters);
	bool MoveToNextLine(std::string* buffer);

	bool IsValidIdentifier(char c);
	bool IsValidIdentifier(char c, char* delimiters, int totalDelimiters);


private:
	int m_startIndex;
	int m_endIndex;
	std::string m_data;
};

#endif