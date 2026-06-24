//****************************************************************************
// Program: Doubly-Linked List (double)
// Version: 1.0
// Date:    1993-04-03
// Author:  Rohin Gosling
//
// Description
//
//   Doubly-linked list container class. 
//   Uses far-heap allocation for the large memory model.
//
//****************************************************************************

#include <ALLOC.H>
#include <MEM.H>
#include "list.h"

//----------------------------------------------------------------------------
// Constructor: ListDouble::ListDouble
//
// Description:
//
//   Default constructor for the ListDouble class. Initialises all node
//   pointers to NULL, sets the element count to zero, and marks the
//   maximum element count as unknown (-1).
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

ListDouble::ListDouble ( void )
{
	Node        =  NULL;
	Head        =  NULL;
	Tail        =  NULL;
	NumElements =  0;
	MaxElements = -1;
}

//----------------------------------------------------------------------------
// Method: ListDouble::SearchElement
//
// Description:
//
//   Sequentially searches for the node at the specified index by
//   traversing the linked list from the head. Returns a pointer to
//   the node if found, or NULL if the index is out of range.
//
// Parameters:
//
//   Index (long): Zero-based index of the node to find.
//
// Return Values:
//
//   node (NodeType*):
//   - Pointer to the node at the given index.
//   - NULL if the index is out of range or the list is empty.
//
//----------------------------------------------------------------------------

NodeType *ListDouble::SearchElement ( long Index )
{
	// Initialise local variables.

	long Count = 0;

	// Validate that the list is not empty and the index is within bounds before searching.

	if ( ( Head != NULL ) && ( Index >= 0 ) && ( Index < NumElements ) )
	{
		// Traverse the list from the head to the specified index.

		Node = Head;

		while ( Count < Index )
		{
			Node = Node->Next;
			Count++;
		}
	}
	else
	{
		// Index out of range or list is empty.

		return ( NULL );
	}

	// Return the node at the specified index.

	return ( Node );
}

//----------------------------------------------------------------------------
// Method: ListDouble::InsertElement
//
// Description:
//
//   Inserts a new element into the list at the position specified by
//   Index. If the list is empty and Index is zero, the first node is
//   created. Otherwise the new element is inserted before the node
//   currently at the given index.
//
// Parameters:
//
//   Index (long): The position at which to insert the new element.
//
//   Element (double): The value to insert.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns 1 if the index is out of range.
//
//----------------------------------------------------------------------------

int ListDouble::InsertElement ( long Index, double Element )
{
	// Initialise local variables.

	NodeType *NewNode;

	if ( ( Head == NULL ) && ( Index == 0 ) )
	{
		// Create first node if list is empty.

		Head          = (NodeType *) farmalloc ( sizeof ( NodeType ) );
		Head->Element = Element;
		Head->Prev    = NULL;
		Head->Next    = NULL;

		Tail = Head;
	}
	else if ( ( Head != NULL ) && ( Index >= 0 ) && ( Index <= NumElements ) )
	{
		// Create the new node.

		NewNode          = (NodeType *) farmalloc ( sizeof ( NodeType ) );
		NewNode->Element = Element;

		// Insert the new node.

		if ( Index == 0 )
		{
			// Insert new head.

			NewNode->Prev = NULL;
			NewNode->Next = Head;
			Head->Prev    = NewNode;
			Head          = NewNode;
		}
		else if ( Index == NumElements - 1 )
		{
			// Insert before tail.

			NewNode->Prev    = Tail->Prev;
			NewNode->Next    = Tail;
			Tail->Prev->Next = NewNode;
			Tail->Prev       = NewNode;
		}
		else if ( Index == NumElements )
		{
			// Insert new tail.

			NewNode->Prev = Tail;
			NewNode->Next = NULL;
			Tail->Next    = NewNode;
			Tail          = NewNode;
		}
		else
		{
			// Insert at any other position.

			Node = SearchElement ( Index );

			NewNode->Prev    = Node->Prev;
			NewNode->Next    = Node;
			Node->Prev->Next = NewNode;
			Node->Prev       = NewNode;
		}
	}
	else
	{
		return ( 1 );
	}

	NumElements++;

	return ( 0 );
}

//----------------------------------------------------------------------------
// Method: ListDouble::DeleteElement
//
// Description:
//
//   Deletes the element at the specified index from the list and
//   releases its far-heap memory.
//
// Parameters:
//
//   Index (long): Zero-based index of the element to delete.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns 1 if the index is out of range or the list is empty.
//
//----------------------------------------------------------------------------

int ListDouble::DeleteElement ( long Index )
{
	// Validate that the list is not empty and the index is within bounds before deleting.

	if ( ( Head != NULL ) && ( Index >= 0 ) && ( Index < NumElements ) )
	{
		// Determine the position of the element to delete and unlink it from the list.

		if ( Index == 0 )
		{
			// Delete head.

			if ( Head->Next != NULL )
			{
				Node       = Head;
				Head       = Head->Next;
				Head->Prev = NULL;
			}
			else
			{
				Node = Head;
				Head = NULL;
			}

			farfree ( Node );
		}
		else if ( Index == NumElements - 1 )
		{
			// Delete tail.

			Node       = Tail;
			Tail       = Tail->Prev;
			Tail->Next = NULL;

			farfree ( Node );
		}
		else
		{
			// Delete any other node.

			Node = SearchElement ( Index );
			Node->Prev->Next = Node->Next;
			Node->Next->Prev = Node->Prev;

			farfree ( Node );
		}
	}
	else
	{
		// Index out of range or list is empty.

		return ( 1 );
	}

	// Decrement the element count to reflect the removal.

	NumElements--;

	// Return success.

	return ( 0 );
}

//----------------------------------------------------------------------------
// Method: ListDouble::RetrieveElement
//
// Description:
//
//   Returns the value of the element at the specified index.
//
// Parameters:
//
//   Index (long): Zero-based index of the element to retrieve.
//
// Return Values:
//
//   element (double):
//   - The value stored at the given index.
//
//----------------------------------------------------------------------------

double ListDouble::RetrieveElement ( long Index )
{
	// Initialise local variables.

	NodeType *Temp;

	// Search for the node at the specified index.

	if ( ( Index >= 0 ) && ( Index < NumElements ) )
	{
		Temp = SearchElement ( Index );
	}

	// Return the element value stored in the node.

	return ( Temp->Element );
}

//----------------------------------------------------------------------------
// Method: ListDouble::ModifyElement
//
// Description:
//
//   Replaces the element at the specified index with a new value.
//   Internally inserts the new value before the existing element, then
//   deletes the old element.
//
// Parameters:
//
//   Index (long): Zero-based index of the element to modify.
//
//   NewElement (double): The replacement value.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns 1 if the index is out of range.
//
//----------------------------------------------------------------------------

int ListDouble::ModifyElement ( long Index, double NewElement )
{
	// Validate that the index is within bounds before modifying.

	if ( ( Index >= 0 ) && ( Index < NumElements ) )
	{
		// Insert the new value before the existing element, then delete the old one.

		InsertElement ( Index, NewElement );
		DeleteElement ( Index + 1 );
	}
	else
	{
		// Index out of range.

		return ( 1 );
	}

	// Return success.

	return ( 0 );
}

//----------------------------------------------------------------------------
// Method: ListDouble::DeleteAll
//
// Description:
//
//   Deletes all elements in the list, releasing all far-heap memory.
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

void ListDouble::DeleteAll ( void )
{
	// Delete elements from the tail toward the head until the list is empty.

	while ( NumElements > 0 )
	{
		// Remove the last element in the list.

		DeleteElement ( NumElements - 1 );
	}
}

//----------------------------------------------------------------------------
// Method: ListDouble::InsertAtHead
//
// Description:
//
//   Inserts an element at the head (beginning) of the list.
//
// Parameters:
//
//   Element (double): The value to insert.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void ListDouble::InsertAtHead ( double Element )
{
	InsertElement ( 0, Element );
}

//----------------------------------------------------------------------------
// Method: ListDouble::InsertAtTail
//
// Description:
//
//   Inserts an element at the tail (end) of the list.
//
// Parameters:
//
//   Element (double): The value to insert.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void ListDouble::InsertAtTail ( double Element )
{
	InsertElement ( NumElements, Element );
}

//----------------------------------------------------------------------------
// Method: ListDouble::DeleteHead
//
// Description:
//
//   Deletes the first element in the list.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns 1 if the list is empty.
//
//----------------------------------------------------------------------------

int ListDouble::DeleteHead ( void )
{
	// Check that the list is not empty before attempting to delete.

	if ( Head != NULL )
	{
		// Delete the first element in the list.

		DeleteElement ( 0 );
	}
	else
	{
		// List is empty, nothing to delete.

		return ( 1 );
	}

	// Return success.

	return ( 0 );
}

//----------------------------------------------------------------------------
// Method: ListDouble::DeleteTail
//
// Description:
//
//   Deletes the last element in the list.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns 1 if the list is empty.
//
//----------------------------------------------------------------------------

int ListDouble::DeleteTail ( void )
{
	// Check that the list is not empty before attempting to delete.

	if ( Tail != NULL )
	{
		// Delete the last element in the list.

		DeleteElement ( NumElements - 1 );
	}
	else
	{
		// List is empty, nothing to delete.

		return ( 1 );
	}

	// Return success.

	return ( 0 );
}

//----------------------------------------------------------------------------
// Method: ListDouble::RetrieveHead
//
// Description:
//
//   Returns the value of the first element in the list.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   element (double):
//   - The value stored at the head of the list.
//
//----------------------------------------------------------------------------

double ListDouble::RetrieveHead ( void )
{
	// Initialise local variables.

	double Temp = 0;

	// Retrieve the first element if the list is not empty.

	if ( Head != NULL )
	{
		Temp = RetrieveElement ( 0 );
	}

	// Return the retrieved value, or zero if the list was empty.

	return ( Temp );
}

//----------------------------------------------------------------------------
// Method: ListDouble::RetrieveTail
//
// Description:
//
//   Returns the value of the last element in the list.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   element (double):
//   - The value stored at the tail of the list.
//
//----------------------------------------------------------------------------

double ListDouble::RetrieveTail ( void )
{
	// Initialise local variables.

	double Temp = 0;

	// Retrieve the last element if the list is not empty.

	if ( Tail != NULL )
	{
		Temp = RetrieveElement ( NumElements - 1 );
	}

	// Return the retrieved value, or zero if the list was empty.

	return ( Temp );
}

//----------------------------------------------------------------------------
// Method: ListDouble::IsEmpty
//
// Description:
//
//   Determines whether the list is empty.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   result (int):
//   - Returns 1 if the list is empty.
//   - Returns 0 if the list is not empty.
//
//----------------------------------------------------------------------------

int ListDouble::IsEmpty ( void )
{
	// Check the element count and return the appropriate boolean result.

	if ( NumElements == 0 )
	{
		// The list contains no elements.

		return ( 1 );
	}
	else
	{
		// The list contains one or more elements.

		return ( 0 );
	}
}

//----------------------------------------------------------------------------
// Method: ListDouble::IsFull
//
// Description:
//
//   Determines whether the list is full. Only meaningful if the
//   maximum number of elements has been calculated via
//   MaxNumberElements.
//
// Parameters:
//
//   None.
//
// Return Values:
//
//   result (int):
//   - Returns  1 if the list is full.
//   - Returns  0 if the list is not full.
//   - Returns -1 if the maximum is unknown or infinite.
//
//----------------------------------------------------------------------------

int ListDouble::IsFull ( void )
{
	// Check the element count against the maximum and return the appropriate result.

	if ( MaxElements == -1 )
	{
		// Maximum number of elements is unknown or infinite.

		return ( -1 );
	}
	else if ( NumElements >= MaxElements )
	{
		// The list has reached or exceeded its maximum capacity.

		return ( 1 );
	}
	else
	{
		// The list still has room for more elements.

		return ( 0 );
	}
}

//----------------------------------------------------------------------------
// Method: ListDouble::MaxNumberElements
//
// Description:
//
//   Calculates the maximum number of elements that can be stored in
//   the list, based on the element size and the amount of available
//   far-heap memory.
//
// Parameters:
//
//   ElementSize (unsigned): The size of the element data in bytes.
//
// Return Values:
//
//   maxElements (long):
//   - The estimated maximum number of elements that can be stored.
//
//----------------------------------------------------------------------------

long ListDouble::MaxNumberElements ( unsigned ElementSize )
{
	// Query the amount of available far-heap memory.

	unsigned long FreeMem = farcoreleft ();

	// Estimate the maximum number of nodes that can fit in the available memory.

	MaxElements = (long) ( FreeMem / ( ElementSize + sizeof ( void * ) + sizeof ( NodeType ) ) );

	// Return the estimated maximum.

	return ( MaxElements );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
