//****************************************************************************
// Program: Cubic Polynomial Curve Fitting Library
// Version: 1.0
// Date:    1998-11-18
// Author:  Rohin Gosling
//
// Description
//
//   Cubic polynomial curve fitting library, that can be used to fit piece-wise 
//   cubic polynomial curve to a set of points. 
//
//****************************************************************************

#ifndef CURVE
#define CURVE

#include <ALLOC.H>

//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------

#define CALC_TYPE double

//----------------------------------------------------------------------------
// Class: Curve
//----------------------------------------------------------------------------

// Generic curve class

class Curve
{

public:

	CALC_TYPE CubicSpline                           ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE D1, CALC_TYPE x );
	CALC_TYPE QuadraticSplineFirstSegment           ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D,  CALC_TYPE x );
	CALC_TYPE QuadraticSplineLastSegment            ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D,  CALC_TYPE x );
	CALC_TYPE CubicSplineDerivative                 ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D0, CALC_TYPE D1, CALC_TYPE x );
	CALC_TYPE QuadraticSplineFirstSegmentDerivative ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D,  CALC_TYPE x );
	CALC_TYPE QuadraticSplineLastSegmentDerivative  ( CALC_TYPE x0, CALC_TYPE y0, CALC_TYPE x1, CALC_TYPE y1, CALC_TYPE D,  CALC_TYPE x );

	Curve ();
};

//----------------------------------------------------------------------------
// Class: Cubic
//----------------------------------------------------------------------------

class Cubic : public Curve
{
	// Member variables. 	

	CALC_TYPE **p;		// p [ n ][ 3 ]
	long        N;

public:

	// Acessors

	CALC_TYPE GetX     ( long Index );
	CALC_TYPE GetY     ( long Index );
	CALC_TYPE GetSlope ( long Index );

	// Mutators

	// Constructor(s)

	Cubic ();

	// Destructor(s)

	~Cubic ();

	// Methods

	void      Init        ( long Number_Points, const CALC_TYPE Points [][ 2 ] );
	int       ModifySlope ( long Index,               CALC_TYPE Slope );	
	void      ResetSlopes ( void );
	void      AutoSlopes  ( void );	
	CALC_TYPE f           ( CALC_TYPE x);
	CALC_TYPE d           ( CALC_TYPE x);	
};

//----------------------------------------------------------------------------
// Class: Bezier
//----------------------------------------------------------------------------

class Bezier : public Curve {

	CALC_TYPE **p;		// p [ n ][ 6 ]
	long        N;

public:

	// Accessors

	// Mutators

	int  SetCurveTension ( CALC_TYPE t );
	int  SetPointTension ( long Index, CALC_TYPE t );

	// Constructor(s)

	Bezier  ();

	// Destructor(s)

	~Bezier ();

	// Methods

	void      Init        ( long Number_Points, CALC_TYPE **Points );
	int       ModifySlope ( long Index,         CALC_TYPE Slope );
	void      ResetSlopes ( void );
	void      AutoSlopes  ( void );
	CALC_TYPE f           ( CALC_TYPE x );
	CALC_TYPE d           ( CALC_TYPE x );
};


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

