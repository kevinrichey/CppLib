#ifndef kwr_REF_H

#define kwr_REF_H



namespace kwr

{

	struct RefCount

	{

		unsigned refCount;

		explicit RefCount() : refCount(0) {}

		virtual ~RefCount () {}

	};



   template <class RefType>

   class ref

   {

      RefType *pointer;



		public:



      template <class ParamType>

      explicit ref(ParamType* obj)

			: pointer(obj)

      {

         if (pointer)

         {

            ++pointer->refCount;

         }

		}



      ref(const ref& other)

			: pointer(other.Pointer())

      { 

         if (pointer)

         {

            ++pointer->refCount;

         }

		}



      template <class ParamType>

      ref(const ref<ParamType>& other)

			: pointer(other.get())

      {

         if (pointer)

         {

            ++pointer->refCount;

         }

		}



      ~ref()

      {

         Release();

      }



      void Release()

      {

         if ( !(pointer && --pointer->refCount) ) 

			{

				delete pointer;

			}

      }



      ref& operator=(const ref& other)

      {

         Swap( ref<RefType>(other) );

			return *this;

      }



      void Swap(ref& other)

      {

			// refactor this

			RefType* temp = pointer;

			pointer = other.pointer;

			other.pointer = temp;

      }



      RefType& operator*() const

      {

         return *pointer;

      }



      RefType* operator->() const

      {

         return pointer;

      }



      RefType* Pointer() const { return pointer; }



      bool IsValid() const

      {

         return pointer;

      }



      bool IsEmpty() const

      {

         return !IsValid();

      }



      unsigned UseCount() const

      {

         return pointer ? pointer->refCount : 0;

      }



   };



   template <class ParamType>

   bool operator== (const ref<ParamType>& left, const ref<ParamType>& right)

   {

      return left.get() == right.get();

   }



} // kwr

#endif

