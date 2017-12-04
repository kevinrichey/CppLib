#include "kwrlib.h"
#include <cmath>
#include <cstdio>

namespace kwr {

//{{{1 step, pulse, clamp, smoothstep, spline

const double pi = 3.1415927; 

double step(double a, double x) //{{{2
{
   return (double)( x >= a );
}

double pulse(double a, double b, double x) //{{{2
{
   return step(a, x) - step(b, x);
}

double clamp(double a, double b, double x) //{{{2
{
   return 
      (x < a) ? a :
      (x > b) ? b :
      x;
}

double smoothstep(double a, double b, double x) //{{{2
{
   if(x < a)  return 0.0;
   if(x >= b) return 1.0;

   x = (x - a) / (b - a);
   return (x*x * (3 - 2*x));
}

//{{{2 spline

/* Coefficients of basis matrix. */
#define CR00     -0.5
#define CR01      1.5
#define CR02     -1.5
#define CR03      0.5
#define CR10      1.0
#define CR11     -2.5
#define CR12      2.0
#define CR13     -0.5
#define CR20     -0.5
#define CR21      0.0
#define CR22      0.5
#define CR23      0.0
#define CR30      0.0
#define CR31      1.0
#define CR32      0.0
#define CR33      0.0

double spline(double x, double nknots, double *knot)
{
    int span;
    int nspans = nknots - 3;
    double c0, c1, c2, c3; // coefficients of the cubic.

    if (nspans < 1) {  // illegal
        printf("Spline has too few knots.\n");
        return 0;
    }

    /* Find the appropriate 4-point span of the spline. */
    x = clamp(0, 1, x) * nspans;
    span = (int) x;
    if (span >= nknots - 3)
        span = nknots - 3;
    x -= span;
    knot += span;

    /* Evaluate the span cubic at x using Horner's rule. */
    c3 = CR00*knot[0] + CR01*knot[1]
       + CR02*knot[2] + CR03*knot[3];
    c2 = CR10*knot[0] + CR11*knot[1]
       + CR12*knot[2] + CR13*knot[3];
    c1 = CR20*knot[0] + CR21*knot[1]
       + CR22*knot[2] + CR23*knot[3];
    c0 = CR30*knot[0] + CR31*knot[1]
       + CR32*knot[2] + CR33*knot[3];

    return ((c3*x + c2)*x + c1)*x + c0;
}

//{{{1 Random
const double RandomDouble::divisor = (double)RandomDouble::mask;

//}}}1
} // kwr

// vim: foldmethod=marker
