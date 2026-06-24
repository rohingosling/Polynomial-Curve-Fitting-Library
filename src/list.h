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

#ifndef LIST_DOUBLE
#define LIST_DOUBLE

#include <ALLOC.H>

//----------------------------------------------------------------------------
// Type Definitions
//----------------------------------------------------------------------------

typedef struct NodeType *NodePtrType;

struct NodeType
{
	double      Element;
	NodePtrType Next;
	NodePtrType Prev;
};

//----------------------------------------------------------------------------
// Class: ListDouble
//----------------------------------------------------------------------------

class ListDouble
{

	// Member variables.

	NodePtrType Node;
	NodePtrType Head;
	NodePtrType Tail;

public:

	long NumElements;
	long MaxElements;

	// Boolean functions

	int IsEmpty ( void );
	int IsFull  ( void );

	// Element handling functions

	NodeType *SearchElement   ( long Index );
	int       InsertElement   ( long Index, double Element );
	int       DeleteElement   ( long Index );
	double    RetrieveElement ( long Index );
	int       ModifyElement   ( long Index, double NewElement );
	void      DeleteAll       ( void );

	// Head and tail handling functions

	void   InsertAtHead  ( double Element );
	void   InsertAtTail  ( double Element );
	int    DeleteHead    ( void );
	int    DeleteTail    ( void );
	double RetrieveHead  ( void );
	double RetrieveTail  ( void );

	// Miscellaneous functions

	long MaxNumberElements ( unsigned ElementSize );

	// Constructor

	ListDouble ( void );
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif
