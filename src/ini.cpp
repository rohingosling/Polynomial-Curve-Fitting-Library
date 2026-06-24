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

#include <STDIO.H>
#include <STRING.H>
#include <ALLOC.H>
#include "ini.h"

//----------------------------------------------------------------------------
// Method: INI::INI ( Constructor )
//
// Description:
//
//   Default constructor for the INI file reader class. Initialises the
//   internal data pointers to NULL and the entry count to zero. The
//   object remains in an uninitialized state until Load is called.
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

INI::INI ()
{
	section_names = NULL;		// Section names
	key_names     = NULL;		// Key names
	key_values    = NULL;		// Values
	entry_count   = 0;			// Entries. 
};

//----------------------------------------------------------------------------
// Method: INI::~INI ( Destructor )
//
// Description:
//
//   Destructor for the INI file reader class. Releases all far-heap
//   memory allocated during Load.
//
//   - Deallocates each per-entry string buffer.
//
//   - Deallocates the top-level pointer arrays.
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

INI::~INI ()
{
	// Only deallocate if memory was previously allocated by Load.

	if ( section_names != NULL )
	{
		// Deallocate each per-entry string buffer.

		for ( long i = 0; i < INI_MAX_ENTRIES; i++ )
		{
			farfree ( section_names [ i ] );
			farfree ( key_names     [ i ] );
			farfree ( key_values    [ i ] );
		}

		// Deallocate the top-level pointer arrays.

		farfree ( section_names );
		farfree ( key_names );
		farfree ( key_values );
	}
};

//----------------------------------------------------------------------------
// Method: INI::TrimWhitespace
//
// Description:
//
//   Removes leading and trailing whitespace characters from a string
//   in place. Whitespace includes spaces, tabs, carriage returns, and
//   newline characters.
//
// Parameters:
//
//   string (char *): The string to trim.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void INI::TrimWhitespace ( char *string )
{
	// Initialise local variables. 

	char *start  = string;	// Pointer to the first non-whitespace character
	char *end    = NULL;	// Pointer to the last non-whitespace character
	long  length = 0;		// Length of the trimmed string
	long  i      = 0;		// Loop counter

	// Find first non-whitespace character.

	while ( *start == ' ' || *start == '\t' ) start++;

	// Handle empty or all-whitespace string.

	if ( *start == '\0' )
	{
		string [ 0 ] = '\0';
		return;
	}

	// Find last non-whitespace character.

	end = string + strlen ( string ) - 1;

	while ( end > start && ( *end == ' ' || *end == '\t' || *end == '\n' || *end == '\r' ) )
	{
		end--;
	}

	// Copy trimmed string back to original buffer.

	length = end - start + 1;

	for ( i = 0; i < length; i++ )
	{
		string [ i ] = start [ i ];
	}

	// Null-terminate the trimmed string.

	string [ length ] = '\0';
};

//----------------------------------------------------------------------------
// Method: INI::StripQuotes
//
// Description:
//
//   Removes surrounding double-quote characters from a string in
//   place.
//
//   - If the string begins and ends with a double-quote character, both 
//     quotes are removed and the inner content is preserved.
//
//   - If the string is not quoted, no modification is made.
//
// Parameters:
//
//   string (char *): The string to strip.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void INI::StripQuotes ( char *string )
{
	// Initialise local variables. 

	long length = strlen ( string );	// Length of the input string
	long i      = 0;					// Loop counter

	// Check if the string is enclosed in double quotes.

	if ( length >= 2 && string [ 0 ] == '"' && string [ length - 1 ] == '"' )
	{
		// Shift the inner content one position left to overwrite the opening quote.

		for ( i = 0; i < length - 2; i++ )
		{
			string [ i ] = string [ i + 1 ];
		}

		// Null-terminate at the position of the former closing quote.

		string [ length - 2 ] = '\0';
	}
};

//----------------------------------------------------------------------------
// Method: INI::Load
//
// Description:
//
//   Loads and parses an INI file. Allocates far-heap memory for up to
//   INI_MAX_ENTRIES entries, where each entry stores a section name,
//   key, and value. The parser supports:
//
//   - Line comments beginning with ;
//   - Section headers in the form [section]
//   - Key-value pairs in the form key = value
//   - Quoted values, where surrounding double quotes are stripped
//
//   Key-value pairs appearing before any section header are ignored.
//
// Parameters:
//
//   file_name (const char *): Path to the INI file to load.
//
// Return Values:
//
//   result (int):
//   - Returns 0 on success.
//   - Returns -1 if the file cannot be opened.
//
//----------------------------------------------------------------------------

int INI::Load ( const char *file_name )
{
	// Initialise local variables. 

	FILE *file   = NULL;	// File handle for the INI file
	char *equals = NULL;	// Position of the '=' delimiter in the current line
	char *key    = NULL;	// Pointer to the key portion of a key-value pair
	char *value  = NULL;	// Pointer to the value portion of a key-value pair
	char *end    = NULL;	// Position of the ']' delimiter in a section header
	long  i      = 0;		// Loop counter

	// Line buffer and current section name accumulator.

	char  line            [ INI_MAX_LINE_SIZE   ];
	char  current_section [ INI_MAX_STRING_SIZE ];

	// Allocate memory for entries.

	section_names = (char **) farmalloc ( INI_MAX_ENTRIES * sizeof ( char * ) );
	key_names     = (char **) farmalloc ( INI_MAX_ENTRIES * sizeof ( char * ) );
	key_values    = (char **) farmalloc ( INI_MAX_ENTRIES * sizeof ( char * ) );

	// Allocate per-entry string buffers for each of the three parallel arrays.

	for ( i = 0; i < INI_MAX_ENTRIES; i++ )
	{
		section_names [ i ] = (char *) farmalloc ( INI_MAX_STRING_SIZE );
		key_names     [ i ] = (char *) farmalloc ( INI_MAX_STRING_SIZE );
		key_values    [ i ] = (char *) farmalloc ( INI_MAX_STRING_SIZE );
	}

	// Reset entry count and clear the current section name.

	entry_count           = 0;
	current_section [ 0 ] = '\0';

	// Open file.

	file = fopen ( file_name, "r" );

	if ( file == NULL )
	{
		return -1;
	}

	// Parse file line by line.

	while ( fgets ( line, INI_MAX_LINE_SIZE, file ) != NULL )
	{
		TrimWhitespace ( line );

		// Skip empty lines and comments.

		if ( line [ 0 ] == '\0' || line [ 0 ] == ';' )
		{
			continue;
		}

		// Parse section header.

		if ( line [ 0 ] == '[' )
		{
			// Locate the closing bracket of the section header.

			end = strchr ( line, ']' );

			// Extract and store the section name if the closing bracket was found.

			if ( end != NULL )
			{
				*end = '\0';
				strcpy         ( current_section, line + 1 );
				TrimWhitespace ( current_section           );
			}

			// Advance to the next line after processing the section header.

			continue;
		}

		// Skip key-value pairs outside of a section.

		if ( current_section [ 0 ] == '\0' )
		{
			// Key-value pairs are only valid within a named section.

			continue;
		}

		// Parse key-value pair.

		equals = strchr ( line, '=' );

		if ( equals != NULL && entry_count < INI_MAX_ENTRIES )
		{
			// Split the line at the '=' delimiter into key and value substrings.

			*equals = '\0';
			key     = line;
			value   = equals + 1;

			// Clean up whitespace and remove any surrounding quotes from the value.

			TrimWhitespace ( key );
			TrimWhitespace ( value );
			StripQuotes    ( value );

			// Store the parsed entry in the parallel arrays.

			strcpy ( section_names [ entry_count ], current_section );
			strcpy ( key_names     [ entry_count ], key             );
			strcpy ( key_values    [ entry_count ], value           );

			// Advance the entry count to the next available slot.

			entry_count++;
		}
	}

	// Close file.

	fclose ( file );

	return 0;
};

//----------------------------------------------------------------------------
// Method: INI::GetValue
//
// Description:
//
//   Retrieves the value associated with a given section and key. The
//   search is case-sensitive. If the section and key combination is
//   not found, NULL is returned.
//
// Parameters:
//
//   section (const char *): The section name to search for.
//
//   key (const char *): The key name to search for within the section.
//
// Return Values:
//
//   value (char *):
//   - A pointer to the value string if found.
//   - NULL if the section and key combination does not exist.
//
//----------------------------------------------------------------------------

char *INI::GetValue ( const char *section, const char *key )
{
	// Linear search through all stored entries for a matching section and key.

	for ( long i = 0; i < entry_count; i++ )
	{
		// Compare both the section name and key name against the query.

		if ( strcmp ( section_names [ i ], section ) == 0 && strcmp ( key_names [ i ], key ) == 0 )
		{
			// Return a pointer to the matching value string.

			return key_values [ i ];
		}
	}

	// No matching section and key combination was found.

	return NULL;
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
