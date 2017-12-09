#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mathex.h"


float math_floor(float floorNumber)
{
  float returnFlooredNumber = floorNumber;

  returnFlooredNumber = (float)((int)returnFlooredNumber);

  return returnFlooredNumber;

}

float math_ceil(float ceilNumber)
{
  float returnCieledNumber = ceilNumber;

  returnCieledNumber = (float)((int)returnCieledNumber) + 1;

  return returnCieledNumber;
}

float math_abs(float absNumber)
{
  if(absNumber < 0)
  {
    //if(absNumber == (int)absNumber)
    //{
      //int intAbsNumber = (int)absNumber;

      //intAbsNumber = intAbsNumber << 1;
      //intAbsNumber = intAbsNumber >> 1;

      ////~ k_printf("\nabs int: %d", math_abs(-10));
      //return (float)intAbsNumber;
    //}else{

    absNumber = -1 * absNumber;
      //return absNumber;

    //}
  } //else{
    //~
  return absNumber;
  //~ }

}

float math_log10 (float logNumber)
{
  //TODO make log function
  float exp = 0.0, ans = 1.0, expAdder = 1.0;

  while (ans != logNumber)
  {
    if(ans < logNumber)
    {
      ans = ans * 10;
      exp = exp + expAdder;
    }else if(ans > logNumber)
    {
      expAdder = expAdder / 10;

      ans = ans * 10;
      exp = exp - expAdder;
    }
  }

  if (ans == logNumber)
  {
    return exp;
  }else{
    /*print error*/
  }
}

int math_intLength(int intNumber)
{
  int inv = 0, count = 0, originalInt = intNumber;

  if(intNumber < 0)
  {
    intNumber = math_abs(intNumber);
  }

  while (intNumber > 0)
  {
    count = count + 1;

    //~ intNumber = intNumber / 10;

    inv = inv * 10 + (intNumber % 10);
    intNumber = intNumber / 10; //since reverseNumber is int, dividing by ten also floors integer
  }

  if(originalInt != 0)
  {
    return count;
  }else if(originalInt == 0)//if the original in is 0, above will not enter while loop, so count will equal 0
  {			    //and will return a number length of 0, so here i will brute return a length of 1
    return 1;
  }

}

int math_pow(int base, int exponent)
{
  int x, output = 1;

  for(x = 0; x < exponent; x++)
  {
    output = output * base;
  }

  return output;
}

double Summation(const double *a, size_t n)
{
    double sum = 0.0;

    while (n--)
        sum += a[n];

    return sum;
}

double SumOfSquares(const double *a, size_t n)
{
    double sum = 0.0;

    while (n--)
        sum += (a[n] * a[n]);

    return sum;
}

double SumOfProduct(const double *a1, const double *a2, size_t n)
{
    double sum = 0.0;

    while (n--)
        sum += (a1[n] * a2[n]);

    return sum;
}

double Average(const double *a, size_t n)
{
    size_t i;
    double sum = 0.0;

    if ( n < 1 ) return 0.0;

    for (i = 0; i < n; i++)
    {
        sum += a[i];
    }
    return sum / (double)n;
}

double Variance(const double *a, size_t n)
{
    size_t i;
    double sum = 0.0;
    double sqsum = 0.0;
    double avg;

    if ( n < 1 ) return 0.0;

    for (i = 0; i < n; i++)
    {
        sum += a[i];
        sqsum += a[i] * a[i];
    }

    avg = sum / (double)n;
    return sqsum / (double)n - avg * avg;
}

double UnbiasedVariance(const double *a, size_t n)
{
    size_t i;
    double sum = 0.0;
    double sqsum = 0.0;
    double avg;

    if ( n < 2 ) return 0.0;

    for (i = 0; i < n; i++)
    {
        sum += a[i];
        sqsum += a[i] * a[i];
    }

    avg = sum / (double)n;
    return sqsum / (double)(n - 1) - avg * avg;
}

double Covariance(const double *x, const double *y, int n)
{
    int i;
    double sum_x = 0.0, sum_y = 0.0;
    double avg_x, avg_y;
    double sumprod = 0.0;

    if ( n < 1 ) return 0.0;

    for (i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
        sumprod += x[i] * y[i];
    }

    avg_x = sum_x / (double)n;
    avg_y = sum_y / (double)n;
    return sumprod / (double)n - avg_x * avg_y;
}

double StdDev(const double *a, size_t n)
{
    size_t i;
    double sum = 0.0;
    double sqsum = 0.0;
    double avg, var;

    if ( n < 1 ) return 0.0;

    for (i = 0; i < n; i++)
    {
        sum += a[i];
        sqsum += a[i] * a[i];
    }

    avg = sum / (double)n;
    var = sqsum / (double)n - avg * avg;

    if ( var < 0.0 ) var = 0.0;
    return sqrt(var);

}

double Max(const double *a, size_t n)
{
    size_t i;
    double max;

    if ( n < 1 ) return 0.0;

    max = a[0];
    for (i = 1; i < n; i++)
    {
        if ( max < a[i] ) max = a[i];
    }

    return max;
}

int MaxInt(const int *a, size_t n)
{
    size_t i;
    int max;

    if ( n < 1 ) return 0;

    max = a[0];
    for (i = 1; i < n; i++)
    {
        if ( max < a[i] ) max = a[i];
    }

    return max;
}

double Min(const double *a, size_t n)
{
    size_t i;
    double min;

    if ( n < 1 ) return 0.0;

    min = a[0];
    for (i = 1; i < n; i++)
    {
        if ( min > a[i] ) min = a[i];
    }

    return min;
}

int MinInt(const int *a, size_t n)
{
    size_t i;
    int min;

    if ( n < 1 ) return 0;

    min = a[0];
    for (i = 1; i < n; i++)
    {
        if ( min > a[i] ) min = a[i];
    }

    return min;
}

int MaxMin(const double *a, size_t n, double *max, double *min)
{
    size_t i;

    if ( n < 1 ) return 0;

    *max = a[0];
    *min = a[0];
    for (i = 1; i < n; i++)
    {
        if ( *max < a[i] ) *max = a[i];
        if ( *min > a[i] ) *min = a[i];
    }

    return 1;
}

int Factorial(int n)
{
    int fact = n;

    if (n <= 0) return 1;

    while (--n)
        fact *= n;

    return fact;
}

int Permutation(int n, int r)
{
    if (r <= 0) return 1;

    return Factorial(n) / Factorial(n - r);
}

int Combination(int n, int r)
{
    if (r <= 0) return 1;

    return Factorial(n) / (Factorial(r) * Factorial(n - r));
}

double Distance(const double *p1, const double *p2, size_t n)
{
    size_t i;
    double distance = 0.0;

    for (i = 0; i < n; i++)
    {
        distance += (p2[i] - p1[i]) * (p2[i] - p1[i]);
    }
    return sqrt(distance);
}

int DivideInternally2D(const double p1[2], const double p2[2], int m, int n, double result[2])
{
    if (m < 1 && n < 1) return 0;

    result[0] = (n * p1[0] + m * p2[0]) / (m + n);
    result[1] = (n * p1[1] + m * p2[1]) / (m + n);

    return 1;
}

int DivideExternally2D(const double p1[2], const double p2[2], int m, int n, double result[2])
{
    if (m - n == 0) return 0;

    result[0] = (-n * p1[0] + m * p2[0]) / (m - n);
    result[1] = (-n * p1[1] + m * p2[1]) / (m - n);

    return 1;
}

double *Triangle_CenterOfGravity(const double p1[2], const double p2[2], const double p3[2], double result[2])
{
    result[0] = (p1[0] + p2[0] + p3[0]) / 3.0;
    result[1] = (p1[1] + p2[1] + p3[1]) / 3.0;
    return result;
}

double *ToVector(const double *p1, const double *p2, size_t n, double *vec)
{
    size_t i;

    for ( i = 0; i < n; i++ )
    {
        vec[i] = p2[i] - p1[i];
    }
    return vec;
}

double *VectorSum(const double *vec1, const double *vec2, double *sumVec, size_t n)
{
    while (n--)
        sumVec[n] = vec1[n] + vec2[n];
    return sumVec;
}

double *VectorDifference(const double *vec1, const double *vec2, double *diffVec, size_t n)
{
    while (n--)
        diffVec[n] = vec1[n] - vec2[n];
    return diffVec;
}

double Norm(const double *vec, size_t n)
{
    size_t i;
    double s = 0.0;

    for ( i = 0; i < n; i++ )
    {
        s += vec[i] * vec[i];
    }

    return sqrt(s);
}

double InnerProduct(const double *vec1, const double *vec2, size_t n)
{
    size_t i;
    double s = 0.0;

    for ( i = 0; i < n; i++ )
    {
        s += vec1[i] * vec2[i];
    }

    return s;
}

double IncludedAngle(const double *vec1, const double *vec2, size_t n)
{
    return acos(InnerProduct(vec1, vec2, n) /
        (Norm(vec1, n) * Norm(vec2, n) + DBL_MIN));
}

int UnitVector(const double *vec, double *unitVec, size_t n)
{
    size_t i;
    double norm = Norm(vec, n);


    if (norm > 0.0)
    {

        for (i = 0; i < n; i++)
        {
            unitVec[i] = vec[i] / Norm(vec, n);
        }
        return 1;

    }
     else
     {
        for (i = 0; i < n; i++)
        {
            unitVec[i] = 0;
        }
        return 0;
    }
}

double *NormalVector3D(const double *p1, const double *p2, const double *p3, double *result)
{
    double vec1[3];
    double vec2[3];

    ToVector(p2, p1, 3, vec1);
    ToVector(p2, p3, 3, vec2);

    CrossProduct3D(vec1, vec2, result);

    return result;
}

double *CrossProduct3D(const double *vec1, const double *vec2, double *result)
{
    result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];

    return result;
}

double VectorSimilarity(const double *vec1, const double *vec2, size_t n)
{
    return InnerProduct(vec1, vec2, n) /
        ((Norm(vec1, n) * Norm(vec2, n)) + DBL_MIN);
}

double NormalizedVectorDistance(const double *vec1, const double *vec2, size_t n)
{
    size_t i;
    double unitDiff;
    double sum = 0.0;

    for (i = 0; i < n; i++)
    {
        unitDiff = (vec1[i] / (Norm(vec1, n) + DBL_MIN)) - (vec2[i] / (Norm(vec2, n) + DBL_MIN));
        sum += unitDiff * unitDiff;
    }

    return sqrt(sum);
}

int IsDepthOnLine3D(const double *p1, const double *p2, const double *p)
{
    double x = p[0];
    double y = p[1];
    double z = p[2];

    double v1 = (x - p1[0]) / (p2[0] - p1[0] + (1.0e-8));
    double v2 = (y - p1[1]) / (p2[1] - p1[1] + (1.0e-8));
    double v3 = (z - p1[2]) / (p2[2] - p1[2] + (1.0e-8));

    if ( (fabs(v1 - v2) <= 1.0e-8) && (fabs(v1 - v3) <= 1.0e-8))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void CalculatePlane3D(const double *AB, const double *AC, const double *A, double *a, double *b, double *c, double *d)
{
    double norm;


    *a = AB[1] * AC[2] - AB[2] * AC[1];
    *b = AB[2] * AC[0] - AB[0] * AC[2];
    *c = AB[0] * AC[1] - AB[1] * AC[0];

    norm = sqrt(*a * *a + *b * *b + *c * *c);
    *a = *a / (norm + DBL_MIN);
    *b = *b / (norm + DBL_MIN);
    *c = *c / (norm + DBL_MIN);


    *d = -1.0 * *a * A[0] + -1.0 * *b * A[1] + -1.0 * *c * A[2];
}

void CalculatePlane3D_Using3Depths(double P1[3], double P2[3], double P3[3], double *a, double *b, double *c, double *d)
{
    double AB[3], AC[3];
    double norm;


    ToVector(P1, P2, 3, AB);
    ToVector(P1, P3, 3, AC);


    *a = AB[1] * AC[2] - AB[2] * AC[1];
    *b = AB[2] * AC[0] - AB[0] * AC[2];
    *c = AB[0] * AC[1] - AB[1] * AC[0];

    norm = sqrt(*a * *a + *b * *b + *c * *c);
    *a = *a / (norm + DBL_MIN);
    *b = *b / (norm + DBL_MIN);
    *c = *c / (norm + DBL_MIN);


    *d = -1.0 * *a * P1[0] + -1.0 * *b * P1[1] + -1.0 * *c * P1[2];
}

int ToLineCanonicalForm(double a, double b, double *ga, double *gb, double *gc)
{
    double norm;


    norm = sqrt(a * a + 1.0);
    *ga = a / (norm + DBL_MIN);
    *gb = -1.0 / (norm + DBL_MIN);


    *gc = b;
    return 1;
}

int ToPlaneCanonicalForm(double a, double b, double c, double *ga, double *gb, double *gc, double *gd)
{

    double norm;

    norm = sqrt(a * a + b * b + 1.0);
    *ga = a / (norm + DBL_MIN);
    *gb = b / (norm + DBL_MIN);
    *gc = -1.0 / (norm + DBL_MIN);


    *gd = c;

    return 1;
}

int ToPlaneGenericForm(double a, double b, double c, double d, double *ca, double *cb, double *cc)
{
    if (c != 0)
    {
        *ca = - a / c;
        *cb = - b / c;

        *cc = - d / c;
        return 1;
    }
    else
    {
        *ca = 0;
        *cb = 0;
        *cc = 0;
        return 0;
    }
}

double DistanceLineToDepth(double a, double b, double c, double point[2])
{
    double x = point[0];
    double y = point[1];

    return fabs(a * x + b * y + c) / (sqrt(a * a + b * b) + DBL_MIN);
}

double DistancePlaneToDepth(double a, double b, double c, double d, double point[3])
{
    double x = point[0];
    double y = point[1];
    double z = point[2];

    return fabs(a * x + b * y + c * z + d) / (sqrt(a * a + b * b + c * c) + DBL_MIN);
}

int IsCorrectPlane(const double *vec1, const double *vec2, size_t n)
{
    double angle;
    double t = pi() / 6.0;


    angle = IncludedAngle(vec1, vec2, n);


    if ( angle > t && angle < pi() - t )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double Arg(double y, double x)
{
    double pi = atan(1.0) * 4.0;

    if (x >= 0 && y >= 0)
    {
        return atan2(y, x);
    }
    else if (x <= 0 && y >= 0)
    {
        return atan2(y, x);
    }
    else if (x <= 0 && y <= 0 )
    {
        return 2.0 * pi + atan2(y, x);
    }
    else
    {
        return 2.0 * pi + atan2(y, x);
    }
}

double ZenithAngle(double x, double y, double z)
{
    double pi = atan(1.0) * 4.0;
    double angle;

    angle = atan2(z, sqrt(x * x + y * y));
    return pi / 2.0 - angle;
}

double Sigmoid(double x, double gain)
{
  return 1.0 / (1.0 + exp(-gain * x));
}

double pi(void)
{
    return atan(1.0) * 4.0;
}

double to_deg(double r)
{
    return r * 180.0 / (atan(1.0) * 4.0);
}

double to_rad(double a)
{
    return a * atan(1.0) * 4.0 / 180.0;
}

double logn(double base, double antilog)
{
    return log(antilog) / log(base);
}

double cbrt_newton(double a, double x)
{
    double e;

    do
    {
        e = (x * x * x - a) / (3.0 * x * x);
        x = x - e;
    } while ( fabs(e) > 1.0e-16 );

    return x;
}

double frac(double x, double y)
{
    return x / (y + DBL_MIN);
}
