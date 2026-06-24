//****************************************************************************
// Program: Cubic Polynomial Curve Fitting Library
// Version: 1.0
// Date:    1998-11-18
// Author:  Rohin Gosling
//
// Description
//
//   Cubit polynomial curve fitting library, that can be used to fit piece-wise
//   cubic polynomial curve to a set of points.
//
//****************************************************************************

#include "curve.h"

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

Curve::Curve ()
{
}

//----------------------------------------------------------------------------
// Method: Curve::CubicSpline
//
// Description:
//
//   Evaluates a cubic Hermite interpolating polynomial over the interval
//   [x0, x1]. Given two sample points and their associated first
//   derivatives, the method computes the unique cubic polynomial
//   satisfying the four interpolation constraints:
//
//      f(x0) = y0,    f(x1) = y1
//     f'(x0) = D0,   f'(x1) = D1
//
//   The polynomial is expressed in the monomial basis as:
//
//                 3         2
//     f(x) = a * x  +  b * x  +  c * x  +  d
//
//   where the coefficients a, b, c, d are derived by solving the
//   4x4 linear system imposed by the above constraints. The
//   denominator common to all coefficients is:
//
//                     3
//     dx = ( x1 - x0 )
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D0 (CALC_TYPE): First derivative (slope) at x0.
//
//   D1 (CALC_TYPE): First derivative (slope) at x1.
//
//   x  (CALC_TYPE): The point at which to evaluate the polynomial.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the cubic Hermite polynomial evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::CubicSpline ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE D1, CALC_TYPE x )
{
	CALC_TYPE f = 0;								// Function value
	CALC_TYPE a = 0, b = 0, c = 0, d = 0;			// Function coefficients
	CALC_TYPE t = 0, dx = 0;						// Temporary variables
	CALC_TYPE A = 0, B = 0, C = 0, D = 0, E = 0;	// Temporary variables
	CALC_TYPE F = 0, G = 0, H = 0, I = 0, J = 0;	// Temporary variables

	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t*t;

	A  =   -D0*x0;
	B  =   -D1*x0;
	C  =    D0*x1;
	D  =    D1*x1;
	E  =  2*y0;
	F  = -2*y1;

	a = ( A + B + C + D + E + F ) / dx;

	// Compute coefficient b

	A =    D0*x0*x0;
	B =  2*D1*x0*x0;
	C =    D0*x0*x1;
	D =   -D1*x0*x1;
	E = -2*D0*x1*x1;
	F =   -D1*x1*x1;
	G = -3*x0*y0;
	H = -3*x1*y0;
	I =  3*x0*y1;
	J =  3*x1*y1;

	b = ( A + B + C + D + E + F + G + H + I + J ) / dx;

	// Compute coefficient c

	A =   -D1*x0*x0*x0;
	B = -2*D0*x0*x0*x1;
	C =   -D1*x0*x0*x1;
	D =    D0*x0*x1*x1;
	E =  2*D1*x0*x1*x1;
	F =    D0*x1*x1*x1;
	G =  6*x0*x1*y0;
	H = -6*x0*x1*y1;

	c = ( A + B + C + D + E + F + G + H ) / dx;

	// Compute coefficient d

	A =    D1*x0*x0*x0*x1;
	B =    D0*x0*x0*x1*x1;
	C =   -D1*x0*x0*x1*x1;
	D =   -D0*x0*x1*x1*x1;
	E = -3*x0*x1*x1*y0;
	F =    x1*x1*x1*y0;
	G =   -x0*x0*x0*y1;
	H =  3*x0*x0*x1*y1;

	d = ( A + B + C + D + E + F + G + H ) / dx;

	// Evaluate and return f

	f = a*x*x*x + b*x*x + c*x + d;

	return f;
}

//----------------------------------------------------------------------------
// Method: Curve::QuadraticSplineFirstSegment
//
// Description:
//
//   Evaluates a quadratic interpolating polynomial over the interval
//   [x0, x1], constrained by two sample points and a prescribed first
//   derivative at the right endpoint x1. This yields three constraints
//   for three unknowns:
//
//     f(x0) = y0,    f(x1) = y1,    f'(x1) = D1
//
//   The polynomial is expressed in the monomial basis as:
//
//                 2
//     f(x) = a * x  +  b * x  +  c
//
//   This form is employed for the first segment of a piece-wise spline,
//   where the left endpoint lacks a neighboring point from which to
//   estimate a derivative. The common denominator is:
//
//                     2
//     dx = ( x1 - x0 )
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D1 (CALC_TYPE): First derivative (slope) at x1.
//
//   x  (CALC_TYPE): The point at which to evaluate the polynomial.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the quadratic polynomial evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::QuadraticSplineFirstSegment ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D1,  CALC_TYPE x )
{
	CALC_TYPE f = 0;								// Function value
	CALC_TYPE a = 0, b = 0, c = 0;					// Function coefficients
	CALC_TYPE t = 0, dx = 0;						// Temporary variables
	CALC_TYPE A = 0, B = 0, C = 0, D = 0, E = 0;	// Temporary variables	

	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t;

	A = -D1*x0;
	B =  D1*x1;
	C =  y0 - y1;

	a = ( A + B + C ) / dx;

	// Compute coefficient b

	A =    D1*x0*x0;
	B =   -D1*x1*x1;
	C = -2*x1*y0;
	D =  2*x1*y1;

	b = ( A + B + C + D ) / dx;

	// Compute coefficient c

	A =   -D1*x0*x0*x1;
	B =    D1*x0*x1*x1;
	C =    x1*x1*y0;
	D =    x0*x0*y1;
	E = -2*x0*x1*y1;

	c = ( A + B + C + D + E ) / dx;

	// Evaluate and return f

	f = a*x*x + b*x + c;

	return f;
}

//----------------------------------------------------------------------------
// Method: Curve::QuadraticSplineLastSegment
//
// Description:
//
//   Evaluates a quadratic interpolating polynomial over the interval
//   [x0, x1], constrained by two sample points and a prescribed first
//   derivative at the left endpoint x0. This yields three constraints
//   for three unknowns:
//
//     f(x0) = y0,    f(x1) = y1,    f'(x0) = D0
//
//   The polynomial is expressed in the monomial basis as:
//
//                 2
//     f(x) = a * x  +  b * x  +  c
//
//   This form is employed for the last segment of a piece-wise spline,
//   where the right endpoint lacks a neighboring point from which to
//   estimate a derivative. The common denominator is:
//
//                     2
//     dx = ( x1 - x0 )
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D0 (CALC_TYPE): First derivative (slope) at x0.
//
//   x  (CALC_TYPE): The point at which to evaluate the polynomial.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the quadratic polynomial evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::QuadraticSplineLastSegment ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE x )
{
	CALC_TYPE f = 0;								// Function value
	CALC_TYPE a = 0, b = 0, c = 0;					// Function coefficients
	CALC_TYPE t = 0, dx = 0;						// Temporary variables
	CALC_TYPE A = 0, B = 0, C = 0, D = 0, E = 0;	// Temporary variables

	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t;

	A =  D0*x0;
	B = -D0*x1;
	C =  y1 - y0;

	a = ( A + B + C ) / dx;

	// Compute coefficient b

	A =   -D0*x0*x0;
	B =    D0*x1*x1;
	C =  2*x0*y0;
	D = -2*x0*y1;

	b = ( A + B + C + D ) / dx;

	// Compute coefficient c

	A =    D0*x0*x0*x1;
	B =   -D0*x0*x1*x1;
	C =    x1*x1*y0;
	D =    x0*x0*y1;
	E = -2*x0*x1*y0;

	c = ( A + B + C + D + E ) / dx;

	// Evaluate and return f

	f = a*x*x + b*x + c;

	return f;
}

//----------------------------------------------------------------------------
// Method: Curve::CubicSplineDerivative
//
// Description:
//
//   Computes the first derivative of the cubic Hermite interpolating
//   polynomial over the interval [x0, x1]. The coefficients a, b, c
//   of the cubic are determined identically to CubicSpline, and the
//   derivative is then evaluated as:
//
//                   2
//     f'(x) = 3a * x  +  2b * x  +  c
//
//   This method is used to obtain the instantaneous rate of change of
//   the interpolated curve at any point within the interval.
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D0 (CALC_TYPE): First derivative (slope) at x0.
//
//   D1 (CALC_TYPE): First derivative (slope) at x1.
//
//   x  (CALC_TYPE): The point at which to evaluate the derivative.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the first derivative of the cubic Hermite
//     polynomial evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::CubicSplineDerivative ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE D1, CALC_TYPE x )
{
	CALC_TYPE f = 0;								// Function value
	CALC_TYPE a = 0, b  = 0, c = 0, d = 0;			// Function coefficients
	CALC_TYPE t = 0, dx = 0;						// Temporary variables
	CALC_TYPE A = 0, B  = 0, C = 0, D = 0, E = 0;	// Temporary variables
	CALC_TYPE F = 0, G  = 0, H = 0, I = 0, J = 0;	// Temporary variables

	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t*t;

	A  =   -D0*x0;
	B  =   -D1*x0;
	C  =    D0*x1;
	D  =    D1*x1;
	E  =  2*y0;
	F  = -2*y1;

	a = ( A + B + C + D + E + F ) / dx;

	// Compute coefficient b

	A =    D0*x0*x0;
	B =  2*D1*x0*x0;
	C =    D0*x0*x1;
	D =   -D1*x0*x1;
	E = -2*D0*x1*x1;
	F =   -D1*x1*x1;
	G = -3*x0*y0;
	H = -3*x1*y0;
	I =  3*x0*y1;
	J =  3*x1*y1;

	b = ( A + B + C + D + E + F + G + H + I + J ) / dx;

	// Compute coefficient c

	A =   -D1*x0*x0*x0;
	B = -2*D0*x0*x0*x1;
	C =   -D1*x0*x0*x1;
	D =    D0*x0*x1*x1;
	E =  2*D1*x0*x1*x1;
	F =    D0*x1*x1*x1;
	G =  6*x0*x1*y0;
	H = -6*x0*x1*y1;

	c = ( A + B + C + D + E + F + G + H ) / dx;

	// Evaluate and return derivative of f,

	f = 3*a*x*x + 2*b*x + c;

	return f;
}

//----------------------------------------------------------------------------
// Method: Curve::QuadraticSplineFirstSegmentDerivative
//
// Description:
//
//   Computes the first derivative of the quadratic interpolating
//   polynomial defined by QuadraticSplineFirstSegment (right-endpoint derivative
//   constraint). The coefficients a, b are determined identically to
//   QuadraticSplineFirstSegment, and the derivative is then evaluated as:
//
//     f'(x) = 2a * x  +  b
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D1 (CALC_TYPE): First derivative (slope) at x1.
//
//   x  (CALC_TYPE): The point at which to evaluate the derivative.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the first derivative of the quadratic polynomial
//     evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::QuadraticSplineFirstSegmentDerivative ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D1, CALC_TYPE x )
{
	CALC_TYPE f = 0;								// Function value
	CALC_TYPE a = 0, b  = 0, c = 0;					// Function coefficients
	CALC_TYPE t = 0, dx = 0;						// Temporary variables
	CALC_TYPE A = 0, B  = 0, C = 0, D = 0;			// Temporary variables
	
	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t;

	A = -D1*x0;
	B =  D1*x1;
	C = y0 - y1;

	a = ( A + B + C ) / dx;

	// Compute coefficient b

	A =    D1*x0*x0;
	B =   -D1*x1*x1;
	C = -2*x1*y0;
	D =  2*x1*y1;

	b = ( A + B + C + D ) / dx;

	// Evaluate and return derivative of f.

	f = 2*a*x + b;

	return f;
}

//----------------------------------------------------------------------------
// Method: Curve::QuadraticSplineLastSegmentDerivative
//
// Description:
//
//   Computes the first derivative of the quadratic interpolating
//   polynomial defined by QuadraticSplineLastSegment (left-endpoint derivative
//   constraint). The coefficients a, b are determined identically to
//   QuadraticSplineLastSegment, and the derivative is then evaluated as:
//
//     f'(x) = 2a * x  +  b
//
// Parameters:
//
//   x0 (CALC_TYPE): Left endpoint of the interpolation interval.
//
//   y0 (CALC_TYPE): Function value at x0.
//
//   x1 (CALC_TYPE): Right endpoint of the interpolation interval.
//
//   y1 (CALC_TYPE): Function value at x1.
//
//   D0 (CALC_TYPE): First derivative (slope) at x0.
//
//   x  (CALC_TYPE): The point at which to evaluate the derivative.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The value of the first derivative of the quadratic polynomial
//     evaluated at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Curve::QuadraticSplineLastSegmentDerivative ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE x )
{
	CALC_TYPE f = 0;							// Function value
	CALC_TYPE a = 0, b  = 0, c = 0;				// Function coefficients
	CALC_TYPE t = 0, dx = 0;					// Temporary variables
	CALC_TYPE A = 0, B  = 0, C = 0, D = 0;		// Temporary variables
	
	// Compute coefficient a

	t  = x1 - x0;
	dx = t*t;

	A =  D0*x0;
	B = -D0*x1;
	C =  y1 - y0;

	a = ( A + B + C ) / dx;

	// Compute coefficient b

	A =   -D0*x0*x0;
	B =    D0*x1*x1;
	C =  2*x0*y0;
	D = -2*x0*y1;

	b = ( A + B + C + D ) / dx;

	// Evaluate and return derivative of f.

	f = 2*a*x + b;

	return f;
}


//----------------------------------------------------------------------------
// Method: Cubic::Cubic ( Constructor )
//
// Description:
//
//   Default constructor for the Cubic spline class. Initialises the
//   internal data pointer to NULL and the sample point count to zero.
//   The object remains in an uninitialized state until Init is called.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

Cubic::Cubic ()
{
	p = NULL;
	N = 0;
};

//----------------------------------------------------------------------------
// Method: Cubic::~Cubic ( Destructor )
//
// Description:
//
//   Destructor for the Cubic spline class. Releases all far-heap
//   memory allocated during Init.
//
//   - Deallocates each per-point data array p[i].
//
//   - Deallocates the top-level pointer array p.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

#if 1
Cubic::~Cubic ()
{
	// free ( p );

	// Deallocate each per-point data array.

	for ( long i = 0; i < N; i++ )
	{
		farfree ( p [ i ] );
	}

	// Deallocate the top-level pointer array.

	farfree ( p );
};
#endif

//----------------------------------------------------------------------------
// Method: Cubic::Init
//
// Description:
//
//   Initialises the cubic spline with a set of sample points. Allocates
//   far-heap memory for N points, where each point stores three values:
//
//     p[i][0] = x   (horizontal coordinate)
//     p[i][1] = y   (vertical coordinate)
//     p[i][2] = D   (slope, initialised to zero)
//
//   After initialisation, all slopes default to zero. Call AutoSlopes
//   or ModifySlope to assign meaningful derivative values prior to
//   evaluation.
//
// Parameters:
//
//   Number_Points (long): The number of sample points in the data set.
//
//   Points (const CALC_TYPE[][2]): A two-dimensional array of sample
//     points, where Points[i][0] is the x-coordinate and
//     Points[i][1] is the y-coordinate of the i-th point.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Cubic::Init ( long Number_Points, const CALC_TYPE Points [][ 2 ] )
{
	// Initialize data

	N = Number_Points;
	p = (CALC_TYPE **) farmalloc ( N*sizeof ( CALC_TYPE * ) );

	// Allocate far-heap storage for each point's data (x, y, slope).

	for ( long i = 0; i < N; i++ )
	{
		p [ i ]  = (CALC_TYPE *) farmalloc ( 3*sizeof ( CALC_TYPE ) );
	}

	// Load data

	for ( i = 0; i < N; i++ )
	{
		p [ i ][ 0 ] = Points [ i ][ 0 ];	// x
		p [ i ][ 1 ] = Points [ i ][ 1 ];	// y
		p [ i ][ 2 ] = 0;					// Slope

	}
};

//----------------------------------------------------------------------------
// Method: Cubic::ResetSlopes
//
// Description:
//
//   Resets the slope (first derivative) at every sample point to zero.
//   This effectively removes any previously assigned or automatically
//   computed derivative information from the spline data.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Cubic::ResetSlopes ( void )
{
	// Set the slope at every sample point to zero.

	for ( long i = 0; i < N; i++ )
	{
		p [ i ][ 2 ] = 0;
	}
};

//----------------------------------------------------------------------------
// Method: Cubic::AutoSlopes
//
// Description:
//
//   Automatically estimates the slope (first derivative) at each sample
//   point using a central finite-difference approximation. For each
//   interior point i (where 1 <= i <= N-2), the slope is computed as
//   the secant slope through the two neighboring points:
//
//             y[i-1] - y[i+1]
//     D[i] = -----------------
//             x[i-1] - x[i+1]
//
//   For the two boundary points (first and last), the slope is
//   estimated using the forward or backward difference with the single
//   adjacent point.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Cubic::AutoSlopes ( void )
{
	// Initialise local variables. 

	CALC_TYPE d  = 0;
	CALC_TYPE x0 = 0, y0 = 0, x1 = 0, y1 = 0;

	// General points

	for ( long i = 1; i < N - 1; i++ )
	{
		x0 = p [ i - 1 ][ 0 ];
		y0 = p [ i - 1 ][ 1 ];

		x1 = p [ i + 1 ][ 0 ];
		y1 = p [ i + 1 ][ 1 ];

		d = ( y0 - y1 ) / ( x0 - x1 );

		p [ i ][ 2 ] = d;
	}

	// End points

	// First point

	x0 = p [ 0 ][ 0 ];
	y0 = p [ 0 ][ 1 ];

	x1 = p [ 1 ][ 0 ];
	y1 = p [ 1 ][ 1 ];

	d = ( y0 - y1 ) / ( x0 - x1 );

	p [ 0 ][ 2 ] = d;

	// Last point

	x0 = p [ N - 2 ][ 0 ];
	y0 = p [ N - 2 ][ 1 ];

	x1 = p [ N - 1 ][ 0 ];
	y1 = p [ N - 1 ][ 1 ];

	d = ( y0 - y1 ) / ( x0 - x1 );

	p [ N - 1 ][ 2 ] = d;

};

//----------------------------------------------------------------------------
// Method: Cubic::ModifySlope
//
// Description:
//
//   Manually overrides the slope (first derivative) stored at a given
//   sample point. This allows the caller to impose specific tangent
//   constraints on the spline, for example forcing a zero slope at
//   local turning points.
//
// Parameters:
//
//   Index (long): Zero-based index of the sample point whose slope is
//     to be modified.
//
//   Slope (CALC_TYPE): The new slope value to assign at the given point.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns -1 if Index exceeds the number of sample points.
//
//----------------------------------------------------------------------------

int Cubic::ModifySlope ( long Index, CALC_TYPE Slope)
{
	// Validate index range.

	if ( Index > N )
		return -1;
	else
		p [ Index ][ 2 ] = Slope;

	// Return success.

	return 0;
};

//----------------------------------------------------------------------------
// Method: Cubic::GetX
//
// Description:
//
//   Returns the x-coordinate of the sample point at the specified
//   index.
//
// Parameters:
//
//   Index (long): Zero-based index of the sample point.
//
// Return Values:
//
//   x (CALC_TYPE):
//   - The x-coordinate stored at p[Index][0].
//
//----------------------------------------------------------------------------

CALC_TYPE Cubic::GetX ( long Index )
{
	return p [ Index ][ 0 ];
}

//----------------------------------------------------------------------------
// Method: Cubic::GetY
//
// Description:
//
//   Returns the y-coordinate of the sample point at the specified
//   index.
//
// Parameters:
//
//   Index (long): Zero-based index of the sample point.
//
// Return Values:
//
//   y (CALC_TYPE):
//   - The y-coordinate stored at p[Index][1].
//
//----------------------------------------------------------------------------

CALC_TYPE Cubic::GetY ( long Index )
{
	return p [ Index ][ 1 ];
}

//----------------------------------------------------------------------------
// Method: Cubic::GetSlope
//
// Description:
//
//   Returns the slope (first derivative) stored at the specified
//   sample point index.
//
// Parameters:
//
//   Index (long): Zero-based index of the sample point.
//
// Return Values:
//
//   slope (CALC_TYPE):
//   - The slope value stored at p[Index][2].
//
//----------------------------------------------------------------------------

CALC_TYPE Cubic::GetSlope ( long Index )
{
	return p [ Index ][ 2 ];
}

//----------------------------------------------------------------------------
// Method: Cubic::f
//
// Description:
//
//   Evaluates the piece-wise cubic spline at a given x-coordinate.
//   The method performs the following steps:
//
//   - Locates the interval [x[i-1], x[i]] that encloses the query
//     point x by linear search through the sorted sample points.
//
//   - Shifts the interval endpoints toward the origin to reduce the
//     magnitude of the polynomial coefficients, thereby improving
//     numerical stability.
//
//   - Delegates to the appropriate interpolation routine based on
//     segment position:
//     - CubicSpline for interior segments (both slopes available).
//     - QuadraticSplineFirstSegment for the first segment (right slope only).
//     - QuadraticSplineLastSegment for the last segment (left slope only).
//
// Parameters:
//
//   x (CALC_TYPE): The point at which to evaluate the spline.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The interpolated function value at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Cubic::f ( CALC_TYPE x )
{
	// Initialise local variables. 

	long      i  = 0;							// Count
	CALC_TYPE f  = 0;							// Function value
	CALC_TYPE x0 = 0, x1 = 0, y0 = 0, y1 = 0;	// Sample points
	CALC_TYPE D0 = 0, D1 = 0;					// Slopes

	// Search for nearest sample point (i is the index of the sample point)

	i  = 1;
	x1 = p [ i ][ 0 ];

	// Advance through sample points until x falls within the current interval.

	while ( ( x > x1 ) && ( i < N - 1 ) )
	{
		i++;
		x1 = p [ i ][ 0 ];
	}

	// Select sample points

	if ( ( i > 0 ) && ( i < N ) )
	{
		x0 = p [ i - 1 ][ 0 ];
		y0 = p [ i - 1 ][ 1 ];
		D0 = p [ i - 1 ][ 2 ];

		x1 = p [ i ][ 0 ];
		y1 = p [ i ][ 1 ];
		D1 = p [ i ][ 2 ];

		// Shift points to alleviate excessively large constants

		if ( ( x0 >= 0 ) && ( x1 >= 0 ) )
		{
			x1 -= x0;
			x  -= x0;
			x0 = 0;

		}
		else if ( ( x0 <= 0 ) && ( x1 <= 0 ) )
		{
			x0 -= x1;
			x  -= x1;
			x1 = 0;
		}
	}

	// Evaluate function between sample points

	// General curve segments

	if ( ( i > 1 ) && ( i < N - 1 ) )
	{
		f = CubicSpline ( x0, y0, x1, y1, D0, D1, x );
	}

	// End segments

	// First curve segment

	if ( i <= 1 )
	{
		f = QuadraticSplineFirstSegment ( x0, y0, x1, y1, D1, x );
	}

	// Last curve segment

	if ( i >= N - 1 )
	{
		f = QuadraticSplineLastSegment ( x0, y0, x1, y1, D0, x );
	}

	// Return the interpolated function value.

	return f;
};

//----------------------------------------------------------------------------
// Method: Cubic::d
//
// Description:
//
//   Evaluates the first derivative of the piece-wise cubic spline at a
//   given x-coordinate. The segment selection and coordinate-shifting
//   logic is identical to Cubic::f, but the evaluation delegates to
//   the derivative routines:
//
//   - CubicSplineDerivative for interior segments.
//
//   - QuadraticSplineFirstSegmentDerivative for the first segment.
//
//   - QuadraticSplineLastSegmentDerivative for the last segment.
//
// Parameters:
//
//   x (CALC_TYPE): The point at which to evaluate the derivative.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - The first derivative of the interpolated curve at x.
//
//----------------------------------------------------------------------------

CALC_TYPE Cubic::d ( CALC_TYPE x )
{
	// Initialise local variables. 

	long      i  = 0;							// Count
	CALC_TYPE f  = 0;							// Function value
	CALC_TYPE x0 = 0, x1 = 0, y0 = 0, y1 = 0;	// Sample points
	CALC_TYPE D0 = 0, D1 = 0;					// Slopes

	// Search for nearest sample point (i is the index of the sample point)

	i  = 1;
	x1 = p [ i ][ 0 ];

	while ( ( x > x1 ) && ( i < N - 1 ) )
	{
		i++;
		x1 = p [ i ][ 0 ];
	}

	// Select sample points

	if ( ( i > 0 ) && ( i < N ) )
	{
		x0 = p [ i - 1 ][ 0 ];
		y0 = p [ i - 1 ][ 1 ];
		D0 = p [ i - 1 ][ 2 ];

		x1 = p [ i ][ 0 ];
		y1 = p [ i ][ 1 ];
		D1 = p [ i ][ 2 ];

		// Shift points to alleviate excessively large constants

		if ( ( x0 >= 0 ) && ( x1 >= 0 ) )
		{
			x1 -= x0;
			x  -= x0;
			x0 = 0;

		}
		else if ( ( x0 <= 0 ) && ( x1 <=0 ) )
		{
			x0 -= x1;
			x  -= x1;
			x1 = 0;
		}
	}

	// Evaluate function between sample points

	// General curve segments

	if ( ( i > 1 ) && ( i < N - 1 ) )
	{
		f = CubicSplineDerivative ( x0, y0, x1, y1, D0, D1, x );
	}

	// End segments

	// First curve segment

	if ( i <= 1 )
	{
		f = QuadraticSplineFirstSegmentDerivative ( x0, y0, x1, y1, D1, x );
	}

	// Last curve segment

	if ( i >= N - 1 )
	{
		f = QuadraticSplineLastSegmentDerivative ( x0, y0, x1, y1, D0, x );
	}

	return f;
};

//----------------------------------------------------------------------------
// Method: Bezier::Bezier (Constructor)
//
// Description:
//
//   Default constructor for the Bezier spline class. Placeholder
//   implementation; no initialisation is performed.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

Bezier::Bezier ()
{
};

//----------------------------------------------------------------------------
// Method: Bezier::~Bezier (Destructor)
//
// Description:
//
//   Destructor for the Bezier spline class. Placeholder
//   implementation; no deallocation is performed.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

Bezier::~Bezier ()
{
};

//----------------------------------------------------------------------------
// Method: Bezier::Init
//
// Description:
//
//   Initialises the Bezier spline with a set of sample points and
//   control data. Placeholder implementation; no operation is
//   performed.
//
// Parameters:
//
//   N (long): The number of sample points.
//
//   Points (CALC_TYPE**): Pointer to the array of point and control
//     data.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Bezier::Init ( long N, CALC_TYPE **Points )
{
};

//----------------------------------------------------------------------------
// Method: Bezier::ResetSlopes
//
// Description:
//
//   Resets the slope at every control point to zero. Placeholder
//   implementation; no operation is performed.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Bezier::ResetSlopes ( void )
{
};

//----------------------------------------------------------------------------
// Method: Bezier::AutoSlopes
//
// Description:
//
//   Automatically computes slopes at each control point using finite
//   differences. Placeholder implementation; no operation is performed.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void Bezier::AutoSlopes ( void )
{
};

//----------------------------------------------------------------------------
// Method: Bezier::ModifySlope
//
// Description:
//
//   Manually overrides the slope at a specified control point.
//   Placeholder implementation; always returns zero.
//
// Parameters:
//
//   Index (long): Zero-based index of the control point.
//
//   Slope (CALC_TYPE): The new slope value to assign.
//
// Return Values:
//
//   result (int):
//   - Always returns 0.
//
//----------------------------------------------------------------------------

int Bezier::ModifySlope ( long Index, CALC_TYPE Slope )
{
	return 0;
};

//----------------------------------------------------------------------------
// Method: Bezier::SetCurveTension
//
// Description:
//
//   Sets a uniform tension parameter for the entire Bezier curve. The
//   tension value governs how tightly the curve conforms to the control
//   polygon. Placeholder implementation; no operation is performed.
//
// Parameters:
//
//   t (CALC_TYPE): The tension parameter, typically in the range [0, 1].
//
// Return Values:
//
//   result (int):
//   - Return code (placeholder).
//
//----------------------------------------------------------------------------

int Bezier::SetCurveTension ( CALC_TYPE t )
{
};

//----------------------------------------------------------------------------
// Method: Bezier::SetPointTension
//
// Description:
//
//   Sets a per-point tension parameter at a specified control point.
//   This allows localized control over the tightness of the curve near
//   individual points. Placeholder implementation; always returns zero.
//
// Parameters:
//
//   Index (long): Zero-based index of the control point.
//
//   t (CALC_TYPE): The tension parameter for the specified point.
//
// Return Values:
//
//   result (int):
//   - Always returns 0.
//
//----------------------------------------------------------------------------

int Bezier::SetPointTension ( long Index, CALC_TYPE t )
{
	return 0;
};

//----------------------------------------------------------------------------
// Method: Bezier::f
//
// Description:
//
//   Evaluates the piece-wise Bezier spline at a given x-coordinate.
//   Placeholder implementation; always returns zero.
//
// Parameters:
//
//   x (CALC_TYPE): The point at which to evaluate the spline.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - Always returns 0.
//
//----------------------------------------------------------------------------

CALC_TYPE Bezier::f ( CALC_TYPE x )
{
	return 0;
};

//----------------------------------------------------------------------------
// Method: Bezier::d
//
// Description:
//
//   Evaluates the first derivative of the piece-wise Bezier spline at
//   a given x-coordinate. Placeholder implementation; always returns
//   zero.
//
// Parameters:
//
//   x (CALC_TYPE): The point at which to evaluate the derivative.
//
// Return Values:
//
//   f (CALC_TYPE):
//   - Always returns 0.
//
//----------------------------------------------------------------------------

CALC_TYPE Bezier::d ( CALC_TYPE x )
{
	return 0;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
