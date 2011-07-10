#ifndef kwr_MANAGED_H
#define kwr_MANAGED_H

namespace kwr
{
	struct RefCount
	{
		unsigned refCount;
		explicit RefCount() : refCount(0) {}
		virtual ~RefCount () {}
	};

} // kwr
#endif
