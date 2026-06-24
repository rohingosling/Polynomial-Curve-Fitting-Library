//****************************************************************************
// Program: Cubic Polynomial Curve Fitting Library - 2D Test Program
// Version: 1.0
// Date:    1998-11-18
// Author:  Rohin Gosling
//
// Description
//
//   Test program to demonstrate 2D parametric curve fitting using the
//   polynomial curve fitting library. 
//
//   - Loads sample data from a CSV file with columns (t, x, y), fits two 
//     polynomial curves f_0(t) = x and f_1(t) = y.
//
//   - Generates interpolated points f(t) = (x, y), and plots the result 
//     using the Graph class.
//
//****************************************************************************

// C++ headers

#include <GRAPHICS.H>
#include <CONIO.H>
#include <STDIO.H>
#include <STDLIB.H>
#include <BIOS.H>
#include <MATH.H>
#include <ALLOC.H>

// Local headers

#include "curve.h"
#include "list.h"
#include "csv.h"
#include "graph.h"
#include "ini.h"

//----------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

//----------------------------------------------------------------------------
// Function Prototypes
//----------------------------------------------------------------------------

int  InitGraphics    ( char *bgiPath );
void LoadGraphConfig ( INI *settings, Graph *graph );
void PlotGraph       ( char *csvFileName, INI *settings );
void CopyListToArray ( ListDouble *listX, ListDouble *listY, CALC_TYPE (*transferArray) [ 2 ], long count );
void CopyArrayToList ( CALC_TYPE (*transferArray) [ 2 ], long count, ListDouble *listX, ListDouble *listY );

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void main ( int argc, char *argv [] )
{
	// Initialise local variables.

	INI  settings;
	char *bgiPath;
	char *csvFileName;

	// Get the CSV file name from the command line, or use the default.

	if ( argc > 1 )
	{
		csvFileName = argv [ 1 ];
	}
	else
	{
		csvFileName = "data-2.csv";
	}

	// Load the BGI path from the settings file, defaulting to empty if not found.

	settings.Load ( "config-2.ini" );
	bgiPath = settings.GetValue ( "global", "bgi_path" );
	if ( bgiPath == NULL ) bgiPath = "";

	// Initialise graphics, plot the graph, wait for a keypress, then shut down.

	clrscr       ();
	InitGraphics ( bgiPath );
	PlotGraph    ( csvFileName, &settings );
	bioskey      ( 0 );
	closegraph   ();
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void PlotGraph ( char *csvFileName, INI *settings )
{
	// Initialise local variables.

	CSVFile    csvFile;
	Cubic      curveX;
	Cubic      curveY;
	Graph      graph;
	ListDouble sampleT;
	ListDouble sampleX;
	ListDouble sampleY;
	ListDouble plotX;
	ListDouble plotY;

	char     *value              = NULL;
	long      interpolationCount = 5;
	long      N                  = 0;
	long      i                  = 0;
	CALC_TYPE t                  = 0;
	CALC_TYPE t0                 = 0;
	CALC_TYPE t1                 = 0;
	CALC_TYPE step               = 0;

	CALC_TYPE (*p)[2] = NULL;

	//------------------------------------------------------------------------
	// Load Data
	//------------------------------------------------------------------------

	// Load curve settings from the INI file.

	value = settings->GetValue ( "curve", "interpolation_count" );
	if ( value ) interpolationCount = atol ( value );

	// Load sample data from the CSV file.

	csvFile.LoadCSVToList ( csvFileName, &sampleT, &sampleX, &sampleY );

	//------------------------------------------------------------------------
	// Fit Curve
	//------------------------------------------------------------------------

	// Allocate the transfer array for curve fitting.

	N = sampleT.NumElements;
	p = (CALC_TYPE (*) [ 2 ]) farmalloc ( N * 2 * sizeof ( CALC_TYPE ) );

	// Build the transfer array and fit the x-component curve: f_0(t) = x.

	CopyListToArray ( &sampleT, &sampleX, p, N );

	curveX.Init       ( N, p );		// Load sample points into the x-component spline.
	curveX.AutoSlopes ();			// Estimate derivatives using central finite differences.

	// Build the transfer array and fit the y-component curve: f_1(t) = y.

	CopyListToArray ( &sampleT, &sampleY, p, N );

	curveY.Init       ( N, p );		// Load sample points into the y-component spline.
	curveY.AutoSlopes ();			// Estimate derivatives using central finite differences.

	// Release the transfer array.

	farfree ( p );

	//------------------------------------------------------------------------
	// Generate Interpolated Data
	//------------------------------------------------------------------------

	// Generate interpolated data using the fitted curves.

	for ( i = 0; i < N - 1; i++ )
	{
		t0   = sampleT.RetrieveElement ( i );
		t1   = sampleT.RetrieveElement ( i + 1 );
		step = ( t1 - t0 ) / ( interpolationCount + 1 );

		// Add the current sample point.

		plotX.InsertAtTail ( curveX.f ( t0 ) );
		plotY.InsertAtTail ( curveY.f ( t0 ) );

		// Add interpolated points between this sample and the next.

		for ( long k = 1; k <= interpolationCount; k++ )
		{
			t = t0 + k * step;
			plotX.InsertAtTail ( curveX.f ( t ) );
			plotY.InsertAtTail ( curveY.f ( t ) );
		}
	}

	// Add the last sample point.

	t = sampleT.RetrieveTail ();
	plotX.InsertAtTail ( curveX.f ( t ) );
	plotY.InsertAtTail ( curveY.f ( t ) );

	//------------------------------------------------------------------------
	// Plot Graph
	//------------------------------------------------------------------------

	// Configure the graph from the INI file.

	LoadGraphConfig ( settings, &graph );

	// Plot fitted polynomial data.

	graph.DataPointVisible = FALSE;
	graph.DataLineVisible  = TRUE;
	graph.ColorDataPoints  = atof ( settings->GetValue ( "curve", "color_fitted_curve" ) );

	graph.PlotLineChart ( &plotX, &plotY );

	// Plot original sample data points over the fitted polynomial curve.

	graph.DataPointVisible = TRUE;
	graph.DataLineVisible  = FALSE;
	graph.DataPointSize    = atof ( settings->GetValue ( "curve", "color_sample_point_size" ) );
	graph.ColorDataPoints  = atof ( settings->GetValue ( "curve", "color_sample_point" ) );

	graph.PlotLineChart ( &sampleX, &sampleY );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

void LoadGraphConfig ( INI *settings, Graph *graph )
{
	// Initialise local variables.

	char *value = NULL;

	// Axis range.

	value = settings->GetValue ( "graph", "vertical_axis_range_min"    ); if ( value ) graph->VerticalAxisRangeMin    = atof ( value );
	value = settings->GetValue ( "graph", "vertical_axis_range_max"    ); if ( value ) graph->VerticalAxisRangeMax    = atof ( value );
	value = settings->GetValue ( "graph", "vertical_axis_range_step"   ); if ( value ) graph->VerticalAxisRangeStep   = atof ( value );
	value = settings->GetValue ( "graph", "horizontal_axis_range_min"  ); if ( value ) graph->HorizontalAxisRangeMin  = atof ( value );
	value = settings->GetValue ( "graph", "horizontal_axis_range_max"  ); if ( value ) graph->HorizontalAxisRangeMax  = atof ( value );
	value = settings->GetValue ( "graph", "horizontal_axis_range_step" ); if ( value ) graph->HorizontalAxisRangeStep = atof ( value );

	// Colours.

	value = settings->GetValue ( "graph", "color_background"   ); if ( value ) graph->ColorBackground  = atoi ( value );
	value = settings->GetValue ( "graph", "color_chart_area"   ); if ( value ) graph->ColorChartArea   = atoi ( value );
	value = settings->GetValue ( "graph", "color_chart_border" ); if ( value ) graph->ColorChartBorder = atoi ( value );
	value = settings->GetValue ( "graph", "color_data_points"  ); if ( value ) graph->ColorDataPoints  = atoi ( value );
	value = settings->GetValue ( "graph", "color_data_line"    ); if ( value ) graph->ColorDataLine    = atoi ( value );
	value = settings->GetValue ( "graph", "color_text"         ); if ( value ) graph->ColorText        = atoi ( value );
	value = settings->GetValue ( "graph", "color_origin_axis"  ); if ( value ) graph->ColorOriginAxis  = atoi ( value );

	// Visibility.

	value = settings->GetValue ( "graph", "origin_axis_visible"          ); if ( value ) graph->OriginAxisVisible         = atoi ( value );
	value = settings->GetValue ( "graph", "tick_mark_vertical_visible"   ); if ( value ) graph->TickMarkVerticalVisible   = atoi ( value );
	value = settings->GetValue ( "graph", "tick_mark_horizontal_visible" );	if ( value ) graph->TickMarkHorizontalVisible = atoi ( value );
	value = settings->GetValue ( "graph", "data_point_visible"           );	if ( value ) graph->DataPointVisible          = atoi ( value );
	value = settings->GetValue ( "graph", "data_line_visible"            );	if ( value ) graph->DataLineVisible           = atoi ( value );

	// Tick marks.

	value = settings->GetValue ( "graph", "tick_mark_vertical_size"   ); if ( value ) graph->TickMarkVerticalSize   = atoi ( value );
	value = settings->GetValue ( "graph", "tick_mark_horizontal_size" ); if ( value ) graph->TickMarkHorizontalSize = atoi ( value );
	value = settings->GetValue ( "graph", "tick_mark_font"            ); if ( value ) graph->TickMarkFont           = atoi ( value );
	value = settings->GetValue ( "graph", "tick_mark_font_size"       ); if ( value ) graph->TickMarkFontSize       = atoi ( value );

	// Chart margins.

	value = settings->GetValue ( "graph", "chart_margin_left"   ); if ( value ) graph->ChartMarginLeft   = atoi ( value );
	value = settings->GetValue ( "graph", "chart_margin_bottom" ); if ( value ) graph->ChartMarginBottom = atoi ( value );
	value = settings->GetValue ( "graph", "chart_margin_top"    ); if ( value ) graph->ChartMarginTop    = atoi ( value );
	value = settings->GetValue ( "graph", "chart_margin_right"  ); if ( value ) graph->ChartMarginRight  = atoi ( value );

	// Data styling.

	value = settings->GetValue ( "graph", "data_point_size"     ); if ( value ) graph->DataPointSize     = atoi ( value );
	value = settings->GetValue ( "graph", "data_line_thickness" ); if ( value ) graph->DataLineThickness = atoi ( value );
}

//----------------------------------------------------------------------------
// Function: CopyListToArray
//
// Description:
//
//   Copies data from two ListDouble containers into a transfer array
//   compatible with the Curve class. Each row of the transfer array
//   stores an (x, y) pair.
//
// Parameters:
//
//   listX         (ListDouble*):    Source list of x-coordinates.
//   listY         (ListDouble*):    Source list of y-coordinates.
//   transferArray (CALC_TYPE(*)[2]): Destination transfer array.
//   count         (long):           Number of elements to copy.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void CopyListToArray ( ListDouble *listX, ListDouble *listY, CALC_TYPE (*transferArray)[2], long count )
{
	// Copy each (x, y) pair from the lists into the transfer array.

	for ( long i = 0; i < count; i++ )
	{
		transferArray [ i ][ 0 ] = listX->RetrieveElement ( i );
		transferArray [ i ][ 1 ] = listY->RetrieveElement ( i );
	}
}

//----------------------------------------------------------------------------
// Function: CopyArrayToList
//
// Description:
//
//   Copies data from a transfer array into two ListDouble containers.
//   Each row of the transfer array is expected to store an (x, y) pair.
//   Elements are appended to the tail of each list.
//
// Parameters:
//
//   transferArray (CALC_TYPE(*)[2]): Source transfer array.
//   count         (long):           Number of elements to copy.
//   listX         (ListDouble*):    Destination list for x-coordinates.
//   listY         (ListDouble*):    Destination list for y-coordinates.
//
// Return Values:
//
//   None.
//
//----------------------------------------------------------------------------

void CopyArrayToList ( CALC_TYPE (*transferArray)[2], long count, ListDouble *listX, ListDouble *listY )
{
	// Copy each (x, y) pair from the transfer array into the lists.

	for ( long i = 0; i < count; i++ )
	{
		listX->InsertAtTail ( transferArray [ i ][ 0 ] );
		listY->InsertAtTail ( transferArray [ i ][ 1 ] );
	}
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

int InitGraphics ( char *bgiPath )
{
	// Request auto detection.

	int gdriver = VGA, gmode = VGAHI, errorcode;

	// Initialize graphics mode.

	initgraph ( &gdriver, &gmode, bgiPath );

	// Read result of initialization.

	errorcode = graphresult ();

	// Display error code.

	if ( errorcode != grOk )
	{
		printf ( "Graphics error: %s\n", grapherrormsg ( errorcode ) );
		printf ( "Press any key to halt:" );
		getch  ();
		exit   ( 1 );
	}

	// Return success.

	return 0;
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
