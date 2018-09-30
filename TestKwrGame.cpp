#include <cstring>
#include "kwrlib.h"
#include "kwrgame.h"
#include "TestCase.h"

using namespace std;
using namespace kwr;

kwr_TestCase(HitBox)
{
   HitBox box;
   box.rect = { 10, 20, 30, 40 };

   kwr_TEST(box.Left() == 10);
   kwr_TEST(box.Top() == 20);
   kwr_TEST(box.Right() == 40);
   kwr_TEST(box.Bottom() == 60);
   
   HitBox movedBox = box.MovedBy(5, 5);
   kwr_TEST(movedBox.Left() == 15);
   kwr_TEST(movedBox.Top() == 25);

   kwr_TEST(box.CollidesWith(movedBox));

   HitBox leftBox( SDL_Rect { 0,0, 8,8 } );
   kwr_TEST(!box.CollidesWith(leftBox));

   HitBox rightBox( SDL_Rect { 50, 20, 30, 40 } );
   kwr_TEST(!box.CollidesWith(rightBox));

   HitBox topBox( SDL_Rect { 10, 0, 30, 10 } );
   kwr_TEST(!box.CollidesWith(topBox));

   HitBox bottomBox( SDL_Rect { 10, 70, 30, 40 } );
   kwr_TEST(!box.CollidesWith(bottomBox));
}

// vim: foldmethod=marker

