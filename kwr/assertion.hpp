#ifndef kwr_ASSERTION_HPP
#define kwr_ASSERTION_HPP

namespace kwr
{
	template <class ActualType, class ExpectedType>
	struct Assertion
	{
		const ActualType actual;
		const ExpectedType expected;
		const char* type;
		bool result;
	};

	template<class ActualType>
	struct Expectation
	{
		const ActualType& actualValue;

		template<class ExpectedType>
		Assertion<ActualType,ExpectedType> operator==(const ExpectedType& expectedValue) const
		{
			return Assertion<ActualType,ExpectedType> 
			{
				actualValue,
				expectedValue,
				"Equals",
				(actualValue == expectedValue) 
			};
		}

		template<class ExpectedType>
		Assertion<ActualType,ExpectedType> operator!=(const ExpectedType& expectedValue) const
		{
			return Assertion<ActualType,ExpectedType> 
			{
				actualValue,
				expectedValue,
				"Not equals",
				(actualValue != expectedValue) 
			};
		}

		Assertion<const char*, const char*> IsNull() const
		{
			return Assertion<const char*,const char*> 
			{
				actualValue ? "not Null" : "Null",
				 "Null",
				 "IsNull",
				 (actualValue == NULL) 
			};
		}

		Assertion<const char*, const char*> NotNull() const
		{
			return Assertion<const char*, const char*> 
			{
				actualValue ? "not Null" : "Null", 
				"not Null", 
				"NotNull", 
				(actualValue != NULL)
			};
		}

		template<class Exception>
		Assertion<const char*, const char*> Throws()
		{
			const char* actual = "exception not thrown";
			bool result = false;
			try 
			{ 
				actualValue(); 
			}
			catch(Exception&) 
			{
				actual = "thrown exception";
				result = true;
			}

			return Assertion<const char*, const char*>
			{
				actual,
				"thrown exception",
				"Throws",
				result
			};
		}
	};

	template<class Type>
	Expectation<Type> Expect(const Type& value)
	{
		return Expectation<Type> { value };
	}
}
#endif
