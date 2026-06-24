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

#include <STDIO.H>
#include <STDLIB.H>
#include <STRING.H>
#include "csv.h"

//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------

#define CSV_MAX_LINE_SIZE 256

//----------------------------------------------------------------------------
// Constructor: CSVFile::CSVFile
//
// Description:
//
//   Default constructor for the CSVFile class. No initialisation is
//   required as the class holds no state.
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

CSVFile::CSVFile ( void )
{
}

//----------------------------------------------------------------------------
// Method: CSVFile::LoadCSVToList
//
// Description:
//
//   Reads a CSV file containing two columns of numeric data and loads
//   the values into a pair of ListDouble containers. The first row of
//   the file is assumed to be a header and is skipped. Each subsequent
//   row is expected to contain two comma-separated floating-point
//   values.
//
// Parameters:
//
//   FileName (char*): Path to the CSV file to read.
//
//   ListX (ListDouble*): Pointer to the list that will receive the
//     first column values.
//
//   ListY (ListDouble*): Pointer to the list that will receive the
//     second column values.
//
// Return Values:
//
//   result (int):
//   - Returns the number of data rows read on success.
//   - Returns -1 if the file could not be opened.
//
//----------------------------------------------------------------------------

int CSVFile::LoadCSVToList ( char *FileName, ListDouble *ListX, ListDouble *ListY )
{
	// Initialise local variables.

	FILE   *file;
	char   line [ CSV_MAX_LINE_SIZE ];
	char   *separator;
	double x = 0;
	double y = 0;
	int    count = 0;

	// Open the CSV file.

	file = fopen ( FileName, "r" );

	if ( file == NULL )
	{
		return ( -1 );
	}

	// Skip the header row.

	fgets ( line, CSV_MAX_LINE_SIZE, file );

	// Read data rows and insert values into the lists.

	while ( fgets ( line, CSV_MAX_LINE_SIZE, file ) != NULL )
	{
		// Locate the comma separator.

		separator = strchr ( line, ',' );

		if ( separator != NULL )
		{
			// Parse the x value from the text before the comma.

			x = atof ( line );

			// Parse the y value from the text after the comma.

			y = atof ( separator + 1 );

			// Insert the values into the lists.

			ListX->InsertAtTail ( x );
			ListY->InsertAtTail ( y );

			count++;
		}
	}

	// Close the file.

	fclose ( file );

	// Return the number of data rows read.

	return ( count );
}

//----------------------------------------------------------------------------
// Method: CSVFile::LoadCSVToList (3-column)
//
// Description:
//
//   Reads a CSV file containing three columns of numeric data and loads
//   the values into three ListDouble containers. The first row of the
//   file is assumed to be a header and is skipped. Each subsequent row
//   is expected to contain three comma-separated floating-point values.
//
// Parameters:
//
//   FileName (char*): Path to the CSV file to read.
//
//   ListA (ListDouble*): Pointer to the list that will receive the
//     first column values.
//
//   ListB (ListDouble*): Pointer to the list that will receive the
//     second column values.
//
//   ListC (ListDouble*): Pointer to the list that will receive the
//     third column values.
//
// Return Values:
//
//   result (int):
//   - Returns the number of data rows read on success.
//   - Returns -1 if the file could not be opened.
//
//----------------------------------------------------------------------------

int CSVFile::LoadCSVToList ( char *FileName, ListDouble *ListA, ListDouble *ListB, ListDouble *ListC )
{
	// Initialise local variables.

	FILE   *file;
	char   line [ CSV_MAX_LINE_SIZE ];
	char   *separator1;
	char   *separator2;
	double a     = 0;
	double b     = 0;
	double c     = 0;
	int    count = 0;

	// Open the CSV file.

	file = fopen ( FileName, "r" );

	if ( file == NULL )
	{
		return ( -1 );
	}

	// Skip the header row.

	fgets ( line, CSV_MAX_LINE_SIZE, file );

	// Read data rows and insert values into the lists.

	while ( fgets ( line, CSV_MAX_LINE_SIZE, file ) != NULL )
	{
		// Locate the first comma separator.

		separator1 = strchr ( line, ',' );

		if ( separator1 != NULL )
		{
			// Locate the second comma separator.

			separator2 = strchr ( separator1 + 1, ',' );

			if ( separator2 != NULL )
			{
				// Parse the first value from the text before the first comma.

				a = atof ( line );

				// Parse the second value from the text between the commas.

				b = atof ( separator1 + 1 );

				// Parse the third value from the text after the second comma.

				c = atof ( separator2 + 1 );

				// Insert the values into the lists.

				ListA->InsertAtTail ( a );
				ListB->InsertAtTail ( b );
				ListC->InsertAtTail ( c );

				// Increment the data row counter.

				count++;
			}
		}
	}

	// Close the file.

	fclose ( file );

	// Return the number of data rows read.

	return ( count );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
