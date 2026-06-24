//****************************************************************************
// Program: Stack (double)
// Version: 1.0
// Date:    1993-04-03
// Author:  Rohin Gosling
//
// Description
//
//   Stack container class.
//
//****************************************************************************

#include <STDDEF.H>
#include "list.h"
#include "stack.h"

//----------------------------------------------------------------------------
// Constructor: StackDouble::StackDouble
//
// Description:
//
//   Default constructor for the StackDouble class. Inherits all
//   initialisation from the ListDouble base class.
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

StackDouble::StackDouble ( void )
{
}

//----------------------------------------------------------------------------
// Method: StackDouble::Push
//
// Description:
//
//   Pushes an element onto the top of the stack by inserting it at the
//   head of the underlying list.
//
// Parameters:
//
//   Element (double): The value to push.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void StackDouble::Push ( double Element )
{
	// Insert the element at position zero, making it the new head of the list.

	InsertElement ( 0, Element );
}

//----------------------------------------------------------------------------
// Method: StackDouble::Pop
//
// Description:
//
//   Removes and returns the element at the top of the stack.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   element (double):
//   - The value at the top of the stack.
//
//----------------------------------------------------------------------------

double StackDouble::Pop ( void )
{
	// Initialise local variables.

	double Temp = 0;

	// Retrieve the top element and remove it from the stack if not empty.

	if ( NumElements > 0 )
	{
		Temp = RetrieveHead ();
		DeleteHead ();
	}

	// Return the popped value, or zero if the stack was empty.

	return ( Temp );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
