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

	String StringConvert(int value)
	{
		return StringConvert((long)value);
	}

	String StringConvert(unsigned int value)
	{
		char buffer[sizeof(unsigned int)*3];
		sprintf(buffer, "%u", value);
		return String(buffer);
	}

	String StringConvert(long value)
	{
		char buffer[sizeof(long)*3];
		sprintf(buffer, "%ld", value);
		return String(buffer);
	}

	String StringConvert(const String& value)
	{
		return value;
	}

	String StringConvert(const char* value)
	{
		return String(value);
	}

	String StringConvert(bool value)
	{
		return String( value ? "true" : "false" );
	}

	String StringConvert(double value)
	{
		char buffer[100];
		sprintf(buffer, "%f", value);
		return String(buffer);
	}

}
