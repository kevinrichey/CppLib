#include <cstdio>
#include "UnitTest.h"
#include "ref.h"

using namespace std;
using namespace kwr;

struct RefTestClass : public RefCount
{
	int x;
};

kwr_Test(Ref_single)
{
	RefTestClass* testObject = new RefTestClass();
	testObject->x = 10;
	ref<RefTestClass> object (testObject);

	kwr_Assert( Expect(object.UseCount()) == 1 );
	kwr_Assert( Expect(object.Pointer()).NotNull() );
	kwr_Assert( Expect(object->x) == 10 );
}

kwr_Test(Ref_copy_ctor)
{
	RefTestClass* testObject = new RefTestClass();
	testObject->x = 10;

	ref<RefTestClass> object1(testObject);
	ref<RefTestClass> object2(object1);

	kwr_Assert( Expect(object1.UseCount()) == 2 );
	kwr_Assert( Expect(object1.Pointer()).NotNull() );
	kwr_Assert( Expect(object1.Pointer()) == object2.Pointer() );
	kwr_Assert( Expect(object1->x) == 10 );

	object1->x = 20;
	kwr_Assert( Expect(object2->x) == 20 );
}

kwr_Test(Ref_destructor)
{
	ref<RefTestClass> object1(new RefTestClass());
	kwr_Assert( Expect(object1.UseCount()) == 1 );

	{
		ref<RefTestClass> object2(object1);
		kwr_Assert( Expect(object1.UseCount()) == 2 );
		object2->x = 100;
	}

	kwr_Assert( Expect(object1.UseCount()) == 1 );
	kwr_Assert( Expect(object1->x) == 100 );
}

