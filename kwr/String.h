#ifndef kwr_STRING_H
#define kwr_STRING_H

namespace kwr
{
	class String
	{
		char* data;

		public:
		explicit String(const char* s) 
			: data(new char[strlen(s)+1])
		{
			strcpy(data, s);
		}

		String(const String& other)
			: data(new char[other.Length()+1])
		{
			strcpy(data, (const char*)other);
		}

	// Modifiers
	
		String& operator= (const String& other)
		{
			Copy(other);
			return *this;
		}

		void Copy(const String& other)
		{
			String(other).Swap(*this);
		}

		void Swap(String& other)
		{
			char* temp = data;
			data = other.data;
			other.data = temp;
		}

	// Queries

		unsigned Length() const
		{
			return strlen(data);
		}

		bool Equals(const String& other) const
		{
			return !strcmp(data, (const char*)other);
		}

		const char * const Cstr() const
		{
			return data;
		}

	// Comparisons

		friend bool operator== (const String& left, const String& right);
		friend bool operator!= (const String& left, const String& right);

	// Desctructor

		~String()
		{
			delete[] data;
		}
	};

	bool operator== (const String& left, const String& right)
	{
		return left.Equals(right);
	}

	bool operator!= (const String& left, const String& right)
	{
		return !(left == right);
	}

}
#endif
