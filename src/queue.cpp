//****************************************************************************
// Program: Queue (double)
// Version: 1.0
// Date:    1993-04-03
// Author:  Rohin Gosling
//
// Description
//
//   Queue container class.
//
//****************************************************************************

#include <STDLIB.H>
#include "list.h"
#include "queue.h"

//----------------------------------------------------------------------------
// Constructor: QueueDouble::QueueDouble
//
// Description:
//
//   Default constructor for the QueueDouble class. Inherits all
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

QueueDouble::QueueDouble ( void )
{
}

//----------------------------------------------------------------------------
// Method: QueueDouble::Enqueue
//
// Description:
//
//   Adds an element to the back of the queue by inserting it at the
//   tail of the underlying list.
//
// Parameters:
//
//   Element (double): The value to enqueue.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void QueueDouble::Enqueue ( double Element )
{
	InsertAtTail ( Element );
}

//----------------------------------------------------------------------------
// Method: QueueDouble::Dequeue
//
// Description:
//
//   Removes and returns the element at the front of the queue.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   element (double):
//   - The value at the front of the queue.
//
//----------------------------------------------------------------------------

double QueueDouble::Dequeue ( void )
{
	// Initialise local variables.

	double Temp = 0;

	// Retrieve the front element and remove it from the queue if not empty.

	if ( NumElements > 0 )
	{
		Temp = RetrieveHead ();
		DeleteHead ();
	}

	// Return the dequeued value, or zero if the queue was empty.

	return ( Temp );
}

//----------------------------------------------------------------------------
// Method: QueueDouble::QueueLength
//
// Description:
//
//   Returns the number of elements currently in the queue.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   length (long):
//   - The number of elements in the queue.
//
//----------------------------------------------------------------------------

long QueueDouble::QueueLength ( void )
{
	// Return the current number of elements in the queue.

	return ( NumElements );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
