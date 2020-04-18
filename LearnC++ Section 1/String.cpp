#include <iostream>
#include <cassert>
#include "String.h"


String::String(const char* input)
{
	n_length = strlen(input) + 1;
	ptrData = new char[n_length];
	for (size_t i = 0; i < n_length; i++)
	{
		ptrData[i] = input[i];
	}

}

String::~String()
{
	delete[] ptrData;
}

String::String(const String& S)
{
	n_length = S.n_length;
	
	bool res = Init(n_length);
	assert(res);
	
	for (size_t i = 0; i < n_length; i++)
	{
		ptrData[i] = S.ptrData[i];
	}
}

String::String(size_t len)
{
	ptrData = new char[len + 1];
	n_length = len;
}

bool String::Init(size_t len)
{	
	n_length = len;
	ptrData = new char[len + 1];

	if (!ptrData)
	{
		return false;
	}
	
	return true;
}

const char* String::to_char() const
{
	return ptrData;
}

std::ostream& operator<<(std::ostream& consoleOut, const String& S)
{
	return consoleOut << S.ptrData << "\n";
}

bool String::operator==(const String& S) const
{

	if (n_length == S.n_length)
	{
		int result = strcmp(ptrData, S.ptrData);
		if (result == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

char &String::operator[](size_t index) const
{
	if (index >= n_length)
	{
		std::cout << "String index out of range" << std::endl;
		exit(0);
	}
	return ptrData[index];
}

String& String::operator=(const String& str)
{
	if (this == &str) 
	{
		return *this;
	}

	if (ptrData != nullptr)
	{
		delete[] ptrData;
	}

	bool result = Init(str.n_length);
	assert(result);

	for (size_t i = 0; i < n_length; i++)
	{
		ptrData[i] = str.ptrData[i];
	}

	return *this;
}

String String::operator+(const String& S) const
{
	int newStringLength = n_length + S.n_length - 1;
	String outString(newStringLength);

	for (size_t i = 0; i < n_length - 1; i++)
	{
		outString.ptrData[i] = ptrData[i];
	}

	for (size_t i = 0; i < S.n_length; i++)
	{
		outString.ptrData[i + n_length - 1] = S.ptrData[i];
	}

	return outString;
}

String String::operator+(const char* c) const
{
	String S2 = c;
	return *this + S2;
}

int String::find(const String& str) const
{
	int j = 0;
	int startPoint = -1;

	for (size_t i = 0; i < n_length; i++)
	{
		if (ptrData[i] == str.ptrData[j])
		{
			if (j == 0)
			{
				startPoint = i;
			}

			if (j == str.n_length - 1)
			{
				return startPoint;
			}

			j++;
		}
		else
		{
			j = 0;
		}
	}

	return startPoint;
}

String String::find(size_t index, size_t length)
{

	int j = 0;
	String outString(length);
	
	for (size_t i = index; i < length; i++)
	{
		outString.ptrData[j] = ptrData[i];
		j++;
	}

	return String();
}