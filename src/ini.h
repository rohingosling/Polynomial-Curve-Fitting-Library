//****************************************************************************
// Program: INI File Reader Library
// Version: 1.0
// Date:    1992-03-21
// Author:  Rohin Gosling
//
// Description
//
//   Basic INI file reader library, that supports comments, sections,
//   and key-value pairs.
//
//****************************************************************************

#ifndef INI_H
#define INI_H

//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------

#define INI_MAX_ENTRIES     32		// Maximum number of key-value entries that can be stored
#define INI_MAX_STRING_SIZE 128		// Maximum length of a section name, key, or value string
#define INI_MAX_LINE_SIZE   256		// Maximum length of a single line read from the INI file

//----------------------------------------------------------------------------
// Class: INI
//----------------------------------------------------------------------------

class INI
{

	// Member variables.

	char **section_names;	// Array of section name strings, one per entry
	char **key_names;		// Array of key name strings, one per entry
	char **key_values;		// Array of value strings, one per entry
	long   entry_count;		// Number of key-value entries currently stored

	// Private methods.

	void TrimWhitespace ( char *string );
	void StripQuotes    ( char *string );

public:

	// Constructor(s)

	INI ();

	// Destructor(s)

	~INI ();

	// Methods

	int   Load     ( const char *file_name );
	char *GetValue ( const char *section, const char *key );
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
