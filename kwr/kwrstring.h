#ifndef kwr_STRING_H
#define kwr_STRING_H

#include <iostream>

namespace kwr
{
	class String
	{
		char* data;

		public:
		explicit String(const char* s);
		String(const String& other);

	// Modifiers
	
		String& operator= (const String& other);
		void Copy(const String& other);
		void Swap(String& other);

	// Queries

		unsigned Length() const;
		bool Equals(const String& other) const;
		const char * const Cstr() const;

	// Comparisons

		friend bool operator== (const String& left, const String& right);
		friend bool operator!= (const String& left, const String& right);

	// Destructor

		~String();
	};

	bool operator== (const String& left, const String& right);
	bool operator!= (const String& left, const String& right);
	std::ostream& operator<< (std::ostream& out, const String& str);

}
#endif
