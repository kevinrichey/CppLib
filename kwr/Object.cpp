/// \file   Object.cpp
/// \author Kevin Richey
/// \date   1/14/2005
///
/// Copyright 2005 Kevin Richey

#include "Object.h"

namespace kwr
{

   Object::Object ()
   { }

   //-----------------------------------------------------------------------------

   Object::~Object ()
   { }

   //-----------------------------------------------------------------------------

   bool Object::Equals (const Ref<Object>& other) const
   {
      return (this == other.get());
   }

   //-----------------------------------------------------------------------------

   void Object::CheckInvariant () const
   { }

} // kwr
