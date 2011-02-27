#include "kwrstring.h"
#include <cstring>

namespace kwr
{
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

	std::ostream& operator<< (std::ostream& out, const String& str)
	{
		out << str.Cstr();
		return out;
	}
}
