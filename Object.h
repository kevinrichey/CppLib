/// \file   Object.h
/// \author Kevin Richey
/// \date   1/14/2005
///
/// Copyright 2005 Kevin Richey

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Managed.h"
#include "Ref.h"

namespace kwr
{

   //-------------------------------------------------------------------
   /// \brief Base class for reference types.
   ///
   /// \ingroup kwr
   ///
   class Object : public Managed
   {
   public:     /// \name Public Interface

      Object ();
      virtual ~Object ();

      //-------------------------------------------------------------------
      /// \brief  Compare with another object.
      /// \param  other    The object to compare.
      /// \return \a true if the objects are equal value.
      /// \par Example Implementation:
      ///   \code
      ///   bool Equals(const Ref<Object>& other) const
      ///   {
      ///      KWR_STACK_TRACE;
      ///
      ///      Ref<this_type> obj( other.as <this_type>() );
      ///      if ( obj.IsValid() )
      ///      {
      ///         return (Value() == obj->Value());
      ///      }
      ///      return false;
      ///   }
      ///   \endcode
      ///
      virtual bool Equals(const Ref<Object>& other) const;

      virtual void CheckInvariant() const;

   private:    /// \name Attributes

   };

} // kwr

#endif
