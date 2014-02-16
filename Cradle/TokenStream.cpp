#include "TokenStream.h"

TokenStream::TokenStream()
{
	ResetStream();
}

void TokenStream::ResetStream()
{
	m_startIndex = m_endIndex = 0;
}

void TokenStream::SetTokenStream(char* data)
{
	ResetStream();
	m_data = data;
}


bool TokenStream::IsValidIdentifier(char c)
{
	if ((int)c > 32 && (int)c < 127)
		return true;

	return false;
}

bool TokenStream::IsValidIdentifier(char c, char* delimiters, int totalDelimiters)
{
	if (delimiters == 0 || totalDelimiters == 0)
		return IsValidIdentifier(c);

	for (int i = 0; i < totalDelimiters; i++)
	{
		if (c == delimiters[i])
			return false;
	}

	return true;
}

bool TokenStream::GetNextToken(std::string* buffer, char* delimiters, int totalDelimiters)
{
	m_startIndex = m_endIndex;

	bool inString = false;
	int length = (int)m_data.length();

	if (m_startIndex >= length - 1)
		return false;

	while (m_startIndex < length && IsValidIdentifier(m_data[m_startIndex], delimiters, totalDelimiters) == false)
	{
		m_startIndex++;
	}

	m_endIndex = m_startIndex + 1;

	if (m_data[m_startIndex] == '*')
		inString = !inString;

	if (m_startIndex < length)
	{
		while (m_endIndex < length && (IsValidIdentifier(m_data[m_endIndex], delimiters, totalDelimiters) || inString == true))
		{
			if (m_data[m_endIndex] == '*')
				inString = !inString;

			m_endIndex++;
		}

		if (buffer != NULL)
		{
			int size = (m_endIndex - m_startIndex);
			int index = m_startIndex;

			buffer->reserve(size + 1);
			buffer->clear();

			for (int i = 0; i < size; i++)
			{
				buffer->push_back(m_data[index++]);
			}
		}

		return true;
	}

	return false;
}

bool TokenStream::MoveToNextLine(std::string* buffer)
{
	int length = (int)m_data.length();

	if (m_startIndex < length && m_endIndex < length)
	{
		m_endIndex = m_startIndex;

		while (m_endIndex < length && (IsValidIdentifier(m_data[m_endIndex]) || m_data[m_endIndex] == ' '))
		{
			m_endIndex++;
		}

		if ((m_endIndex - m_startIndex) == 0)
			return false;

		if (m_endIndex - m_startIndex >= length)
			return false;

		if (buffer != NULL)
		{
			int size = (m_endIndex - m_startIndex);
			int index = m_startIndex;

			buffer->reserve(size + 1);
			buffer->clear();

			for (int i = 0; i < size; i++)
			{
				buffer->push_back(m_data[index++]);
			}
		}
	}
	else
	{
		return false;
	}

	m_endIndex++;
	m_startIndex = m_endIndex + 1;

	return true;
}