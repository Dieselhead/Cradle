#include "File.h"

void File::Open(const char* fileName)
{
	if (file.is_open())
		file.close();

	file.open(fileName);
}

void File::Write(const char* string)
{
	if (file.is_open())
	{
		file << string;
	}
}

void File::Close()
{
	if (file.is_open())
		file.close();
}