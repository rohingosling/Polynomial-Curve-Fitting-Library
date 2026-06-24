//****************************************************************************
// Program: Graph Plotting Library
// Version: 1.0
// Date:    1992-12-20
// Author:  Rohin Gosling
//
// Description
//
//   Graph plotting class for rendering line charts using BGI graphics.
//   Plots data from doubly-linked list containers.
//
//****************************************************************************

#include <GRAPHICS.H>
#include <STDIO.H>
#include "graph.h"

//----------------------------------------------------------------------------
// Constructor: Graph::Graph
//
// Description:
//
//   Default constructor for the Graph class. Initialises all member
//   variables to their default values.
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

Graph::Graph ( void )
{
	// Axis range defaults.

	VerticalAxisRangeMin    = 0;
	VerticalAxisRangeMax    = 1;
	VerticalAxisRangeStep   = 0.1;
	HorizontalAxisRangeMin  = 0;
	HorizontalAxisRangeMax  = 1;
	HorizontalAxisRangeStep = 0.1;

	// Colour defaults.

	ColorBackground  = LIGHTGRAY;
	ColorChartArea   = WHITE;
	ColorChartBorder = BLACK;
	ColorDataPoints  = BLACK;
	ColorDataLine    = BLACK;
	ColorText        = BLACK;
	ColorOriginAxis  = LIGHTGRAY;

	// Visibility defaults.

	OriginAxisVisible         = 1;
	TickMarkVerticalVisible   = 1;
	TickMarkHorizontalVisible = 1;
	DataPointVisible          = 1;
	DataLineVisible           = 1;

	// Tick mark defaults.

	TickMarkVerticalSize   = 3;
	TickMarkHorizontalSize = 3;
	TickMarkFont           = DEFAULT_FONT;
	TickMarkFontSize       = 1;

	// Chart margin defaults.

	ChartMarginLeft   = 16;
	ChartMarginBottom = 16;
	ChartMarginTop    = 4;
	ChartMarginRight  = 4;

	// Data styling defaults.

	DataPointSize     = 3;
	DataLineThickness = 1;

	// State defaults.

	PlotCount = 0;
}

//----------------------------------------------------------------------------
// Method: Graph::Reset
//
// Description:
//
//   Resets the plot counter to zero so that the next call to PlotLineChart
//   will perform a full render including background, axes, and labels.
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

void Graph::Reset ( void )
{
	PlotCount = 0;
}

//----------------------------------------------------------------------------
// Method: Graph::PlotLineChart
//
// Description:
//
//   Plots a line chart using the data from two ListDouble containers,
//   one for x-coordinates and one for y-coordinates. The chart is
//   rendered within the current BGI graphics mode using the configured
//   axis ranges, colours, margins, and styling options.
//
//   Rendering order:
//
//     1. Screen background fill.
//     2. Chart area fill.
//     3. Origin axis lines (if visible).
//     4. Data lines (if visible).
//     5. Data points (if visible).
//     6. Chart area border.
//     7. Tick marks.
//     8. Tick value labels.
//
// Parameters:
//
//   DataX (ListDouble*): Pointer to the list of x-coordinates.
//
//   DataY (ListDouble*): Pointer to the list of y-coordinates.
//
// Return Values:
//
//   count (long):
//   - The number of data elements plotted.
//
//----------------------------------------------------------------------------

long Graph::PlotLineChart ( ListDouble *DataX, ListDouble *DataY )
{
	// Initialise local variables.

	int    screenWidth  = getmaxx () + 1;
	int    screenHeight = getmaxy () + 1;
	int    chartLeft    = ChartMarginLeft;
	int    chartTop     = ChartMarginTop;
	int    chartRight   = screenWidth  - 1 - ChartMarginRight;
	int    chartBottom  = screenHeight - 1 - ChartMarginBottom;
	double rangeX       = HorizontalAxisRangeMax - HorizontalAxisRangeMin;
	double rangeY       = VerticalAxisRangeMax   - VerticalAxisRangeMin;
	double scaleX       = (double) ( chartRight  - chartLeft ) / rangeX;
	double scaleY       = (double) ( chartBottom - chartTop  ) / rangeY;
	long   N            = DataX->NumElements;
	long   i            = 0;
	double dataX        = 0;
	double dataY        = 0;
	double tickValue    = 0;
	int    pixelX       = 0;
	int    pixelY       = 0;
	char   text [ 16 ];

	// Use the smaller of the two list lengths.

	if ( DataY->NumElements < N )
	{
		N = DataY->NumElements;
	}

	if ( PlotCount == 0 )
	{
		// 1. Render the screen background.

		setfillstyle ( SOLID_FILL, ColorBackground );
		bar          ( 0, 0, screenWidth - 1, screenHeight - 1 );

		// 2. Render the chart area.

		setfillstyle ( SOLID_FILL, ColorChartArea );
		bar          ( chartLeft, chartTop, chartRight, chartBottom );

		// Render the origin axes if visible.

		if ( OriginAxisVisible )
		{
			setcolor     ( ColorOriginAxis );
			setlinestyle ( SOLID_LINE, 0, NORM_WIDTH );

			// Vertical origin axis at x = 0.

			if ( HorizontalAxisRangeMin <= 0 && HorizontalAxisRangeMax >= 0 )
			{
				pixelX = chartLeft + (int) ( ( 0 - HorizontalAxisRangeMin ) * scaleX );
				line ( pixelX, chartTop, pixelX, chartBottom );
			}

			// Horizontal origin axis at y = 0.

			if ( VerticalAxisRangeMin <= 0 && VerticalAxisRangeMax >= 0 )
			{
				pixelY = chartBottom - (int) ( ( 0 - VerticalAxisRangeMin ) * scaleY );
				line ( chartLeft, pixelY, chartRight, pixelY );
			}
		}
	}

	// 3. Render the data lines.

	if ( DataLineVisible && N > 1 )
	{
		setcolor     ( ColorDataLine );
		setlinestyle ( SOLID_LINE, 0, DataLineThickness );

		for ( i = 0; i < N; i++ )
		{
			dataX = DataX->RetrieveElement ( i );
			dataY = DataY->RetrieveElement ( i );

			pixelX = chartLeft   + (int) ( ( dataX - HorizontalAxisRangeMin ) * scaleX );
			pixelY = chartBottom - (int) ( ( dataY - VerticalAxisRangeMin   ) * scaleY );

			if ( i == 0 )
			{
				moveto ( pixelX, pixelY );
			}
			else
			{
				lineto ( pixelX, pixelY );
			}
		}
	}

	// 4. Render the data points.

	if ( DataPointVisible )
	{
		setcolor     ( ColorDataPoints );
		setfillstyle ( SOLID_FILL, ColorDataPoints );

		for ( i = 0; i < N; i++ )
		{
			dataX = DataX->RetrieveElement ( i );
			dataY = DataY->RetrieveElement ( i );

			pixelX = chartLeft   + (int) ( ( dataX - HorizontalAxisRangeMin ) * scaleX );
			pixelY = chartBottom - (int) ( ( dataY - VerticalAxisRangeMin   ) * scaleY );

			fillellipse ( pixelX, pixelY, DataPointSize, DataPointSize );
		}
	}

	// 5. Render the chart area border.

	setcolor     ( ColorChartBorder );
	setlinestyle ( SOLID_LINE, 0, NORM_WIDTH );
	rectangle    ( chartLeft, chartTop, chartRight, chartBottom );

	if ( PlotCount == 0 )
	{
		// 6. Render the tick marks.

		setcolor ( ColorChartBorder );

		// Vertical axis tick marks.

		if ( TickMarkVerticalVisible )
		{
			for
			(
				tickValue = VerticalAxisRangeMin;
				tickValue <= VerticalAxisRangeMax + VerticalAxisRangeStep * 0.01;
				tickValue += VerticalAxisRangeStep
			)
			{
				pixelY = chartBottom - (int) ( ( tickValue - VerticalAxisRangeMin ) * scaleY );

				line ( chartLeft - TickMarkVerticalSize, pixelY, chartLeft, pixelY );
			}
		}

		// Horizontal axis tick marks.

		if ( TickMarkHorizontalVisible )
		{
			for
			(
				tickValue = HorizontalAxisRangeMin;
				tickValue <= HorizontalAxisRangeMax + HorizontalAxisRangeStep * 0.01;
				tickValue += HorizontalAxisRangeStep
			)
			{
				pixelX = chartLeft + (int) ( ( tickValue - HorizontalAxisRangeMin ) * scaleX );

				line ( pixelX, chartBottom, pixelX, chartBottom + TickMarkHorizontalSize );
			}
		}

		// 7. Render the tick value labels.

		settextstyle ( TickMarkFont, HORIZ_DIR, TickMarkFontSize );
		setcolor     ( ColorText );

		// Vertical axis tick values.

		if ( TickMarkVerticalVisible )
		{
			settextjustify ( RIGHT_TEXT, CENTER_TEXT );

			for ( tickValue = VerticalAxisRangeMin;
				  tickValue <= VerticalAxisRangeMax + VerticalAxisRangeStep * 0.01;
				  tickValue += VerticalAxisRangeStep )
			{
				pixelY = chartBottom - (int) ( ( tickValue - VerticalAxisRangeMin ) * scaleY );

				sprintf   ( text, "%.1f", tickValue );
				outtextxy ( chartLeft - TickMarkVerticalSize - 2, pixelY, text );
			}
		}

		// Horizontal axis tick values.

		if ( TickMarkHorizontalVisible )
		{
			settextjustify ( CENTER_TEXT, TOP_TEXT );

			for
			(
				tickValue = HorizontalAxisRangeMin;
				tickValue <= HorizontalAxisRangeMax + HorizontalAxisRangeStep * 0.01;
				tickValue += HorizontalAxisRangeStep
			)
			{
				pixelX = chartLeft + (int) ( ( tickValue - HorizontalAxisRangeMin ) * scaleX );

				sprintf   ( text, "%.1f", tickValue );
				outtextxy ( pixelX, chartBottom + TickMarkHorizontalSize + 4, text );
			}
		}
	}

	// Increment the plot counter.

	PlotCount++;

	// Return the number of data elements plotted.

	return ( N );
}

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
