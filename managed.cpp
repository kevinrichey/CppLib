#include "Managed.h"

namespace kwr
{
   Managed::Managed ()
		: refCount (0)
   {
      ++objectCount;
   }

   Managed::~Managed ()
   {
      --objectCount;
   }

} // kwr
