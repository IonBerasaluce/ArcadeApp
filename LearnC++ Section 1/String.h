#pragma once

#include <iostream>

class String
{
public:

	String():ptrData(nullptr), n_length(0) {}
	String(const char* input);
	String(const String& S);
	String(const size_t length);
	~String();

	bool Init(size_t len);
	inline size_t len() const { return n_length - 1; }

	friend std::ostream& operator<<(std::ostream& consoleOut, const String& S);

	String operator+(const String& S) const;
	String operator+(const char* c) const;
	bool operator==(const String& S) const;
	char &operator[](size_t index) const;
	String& operator=(const String& str);
	
	int find(const String& str) const;
	String find(size_t index, size_t length);
	const char* to_char() const;

private:
	size_t n_length;
	char* ptrData;
};