#ifndef kwr_STRING_H
#define kwr_STRING_H

namespace kwr
{
	class String
	{
		char* data;

		public:
		explicit String(const char* s);
		String(const String& other);
		
		explicit String(int number);
		explicit String(unsigned int number);
		explicit String(bool boolean);
		explicit String(double number);

		String& operator= (const String& other);
		void Copy(const String& other);
		void Swap(String& other);

		unsigned Length() const;
		bool Equals(const String& other) const;
		const char * const Cstr() const;

		friend bool operator== (const String& left, const String& right);
		friend bool operator!= (const String& left, const String& right);

		~String();
	};

	bool operator== (const String& left, const String& right);
	bool operator!= (const String& left, const String& right);

}
#endif
