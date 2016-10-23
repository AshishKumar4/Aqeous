#ifndef MATHEX_H
#define MATHEX_H

/* functions */

double Summation(const double *a, size_t n);
double SumOfSquares(const double *a, size_t n);
double SumOfProduct(const double *a1, const double *a2, size_t n);
double Average(const double *a, size_t n);
double Variance(const double *a, size_t n);
double UnbiasedVariance(const double *a, size_t n);
double Covariance(const double *x, const double *y, int n);
double StdDev(const double *a, size_t n);
double Max(const double *a, size_t n);
int MaxInt(const int *a, size_t n);
double Min(const double *a, size_t n);
int MinInt(const int *a, size_t n);
int MaxMin(const double *a, size_t n, double *max, double *min);
int Factorial(int n);
int Permutation(int n, int r);
int Combination(int n, int r);


double Distance(const double *p1, const double *p2, size_t n);
int DivideInternally2D(const double p1[2], const double p2[2], int m, int n, double result[2]);
int DivideExternally2D(const double p1[2], const double p2[2], int m, int n, double result[2]);
double *Triangle_CenterOfGravity(const double p1[2], const double p2[2], const double p3[2], double result[2]);
double DistanceLineToDepth(double a, double b, double c, double point[2]);
double DistancePlaneToDepth(double a, double b, double c, double d, double point[3]);
double Arg(double y, double x);
double ZenithAngle(double x, double y, double z);


double *ToVector(const double *p1, const double *p2, size_t n, double *vec);
double *VectorSum(const double *vec1, const double *vec2, double *sumVec, size_t n);
double *VectorDifference(const double *vec1, const double *vec2, double *sumVec, size_t n);
double Norm(const double *vec, size_t n);
double InnerProduct(const double *vec1, const double *vec2, size_t n);
double IncludedAngle(const double *vec1, const double *vec2, size_t n);
int UnitVector(const double *vec, double *unitVec, size_t n);
double *NormalVector3D(const double *p1, const double *p2, const double *p3, double *result);
double *CrossProduct3D(const double *vec1, const double *vec2, double *result);
double VectorSimilarity(const double *vec1, const double *vec2, size_t n);
double NormalizedVectorDistance(const double *vec1, const double *vec2, size_t n);
int IsDepthOnLine3D(const double *p1, const double *p2, const double *p);
void CalculatePlane3D(const double *AB, const double *AC, const double *A, double *a, double *b, double *c, double *d);
void CalculatePlane3D_Using3Depths(double P1[3], double P2[3], double P3[3], double *a, double *b, double *c, double *d);
int ToLineCanonicalForm(double a, double b, double *ga, double *gb, double *gc);
int ToPlaneCanonicalForm(double a, double b, double c, double *ga, double *gb, double *gc, double *gd);
int ToPlaneGenericForm(double a, double b, double c, double d, double *ca, double *cb, double *cc);
int IsCorrectPlane(const double *vec1, const double *vec2, size_t n);


double Sigmoid(double x, double gain);


double pi(void);
double to_deg(double);
double to_rad(double a);
double logn(double base, double antilog);
double cbrt_newton(double a, double x);
double frac(double x, double y);



#endif
