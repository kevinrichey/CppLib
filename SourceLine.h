#ifndef KWR_SourceLine_H
#define KWR_SourceLine_H

namespace kwr
{
	struct SourceLine
	{
		const char* const filename;
		const int linenumber;
	};

	#define KWR_STRINGIFICATE(x) #x
	#define KWR_SOURCE_LINE_STR(file, line) file ":"  KWR_STRINGIFICATE(line)
	#define KWR_SOURCE_LINE  KWR_SOURCE_LINE_STR(__FILE__, __LINE__)

	#define kwr_SourceLine ::kwr::SourceLine { __FILE__, __LINE__ }
}

#endif
