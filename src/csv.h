//****************************************************************************
// Program: CSV File Reader
// Version: 1.0
// Date:    1998-11-12
// Author:  Rohin Gosling
//
// Description
//
//   Simple CSV file reader class for loading comma-separated data into
//   doubly-linked list containers.
//
//****************************************************************************

#ifndef CSV_FILE
#define CSV_FILE

#include "list.h"

//----------------------------------------------------------------------------
// Class: CSVFile
//----------------------------------------------------------------------------

class CSVFile
{

public:

	// Methods

	int LoadCSVToList ( char *FileName, ListDouble *ListX, ListDouble *ListY );
	int LoadCSVToList ( char *FileName, ListDouble *ListA, ListDouble *ListB, ListDouble *ListC );

	// Constructor

	CSVFile ( void );
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif
