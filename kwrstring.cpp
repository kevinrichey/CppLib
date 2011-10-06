#include "kwrstring.h"
#include <cstdio>
#include <cstring>

namespace kwr
{
	using namespace std;

	String::String(const char* s) 
		: data(new char[strlen(s)+1])
	{
		strcpy(data, s);
	}

	String::String(const String& other)
		: data(new char[other.Length()+1])
	{
		strcpy(data, other.Cstr());
	}

	String::String(int number)
		: data(new char[sizeof(long)*3])
	{
		sprintf(data, "%d", number);
	}

	String::String(unsigned int number)
		: data(new char[sizeof(unsigned int)*3])
	{
		sprintf(data, "%u", number);
	}

	String::String(bool boolean)
		: data(new char[6])
	{
		strcpy(data, boolean ? "true" : "false");
	}

	String::String(double number)
		: data(new char[100])
	{
		sprintf(data, "%f", number);
	}

	String& String::operator= (const String& other)
	{
		Copy(other);
		return *this;
	}

	void String::Copy(const String& other)
	{
		String(other).Swap(*this);
	}

	void String::Swap(String& other)
	{
		char* temp = data;
		data = other.data;
		other.data = temp;
	}

	unsigned String::Length() const
	{
		return strlen(data);
	}

	bool String::Equals(const String& other) const
	{
		return !strcmp(data, other.Cstr());
	}

	const char * const String::Cstr() const
	{
		return data;
	}

	String::~String()
	{
		delete[] data;
	}

	bool operator== (const String& left, const String& right)
	{
		return left.Equals(right);
	}

	bool operator!= (const String& left, const String& right)
	{
		return !(left == right);
	}

}
