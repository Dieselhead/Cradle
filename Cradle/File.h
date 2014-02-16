#pragma once
#include <iostream>
#include <fstream>

class File
{
public:
	void Open(const char* fileName);
	void Write(const char* string);
	void Close();
private:
	std::ofstream file;

};