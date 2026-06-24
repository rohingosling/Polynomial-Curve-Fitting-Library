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

#ifndef GRAPH_H
#define GRAPH_H

#include "list.h"

//----------------------------------------------------------------------------
// Boolean Constants
//----------------------------------------------------------------------------

#define FALSE 0
#define TRUE  1

//----------------------------------------------------------------------------
// BGI Font Constants
//----------------------------------------------------------------------------

#define BGI_DEFAULT_FONT      0
#define BGI_TRIPLEX_FONT      1
#define BGI_SMALL_FONT        2
#define BGI_SANSSERIF_FONT    3
#define BGI_GOTHIC_FONT       4
#define BGI_SCRIPT_FONT       5
#define BGI_SIMPLEX_FONT      6
#define BGI_TRIPLEX_SCR_FONT  7
#define BGI_COMPLEX_FONT      8
#define BGI_EUROPEAN_FONT     9
#define BGI_BOLD_FONT        10

//----------------------------------------------------------------------------
// Class: Graph
//----------------------------------------------------------------------------

class Graph
{

public:

	// Axis range

	double VerticalAxisRangeMin;
	double VerticalAxisRangeMax;
	double VerticalAxisRangeStep;
	double HorizontalAxisRangeMin;
	double HorizontalAxisRangeMax;
	double HorizontalAxisRangeStep;

	// Colours

	int ColorBackground;
	int ColorChartArea;
	int ColorChartBorder;
	int ColorDataPoints;
	int ColorDataLine;
	int ColorText;
	int ColorOriginAxis;

	// Visibility

	int OriginAxisVisible;
	int TickMarkVerticalVisible;
	int TickMarkHorizontalVisible;
	int DataPointVisible;
	int DataLineVisible;

	// Tick marks

	int TickMarkVerticalSize;
	int TickMarkHorizontalSize;
	int TickMarkFont;
	int TickMarkFontSize;

	// Chart margins

	int ChartMarginLeft;
	int ChartMarginBottom;
	int ChartMarginTop;
	int ChartMarginRight;

	// Data styling

	int DataPointSize;
	int DataLineThickness;

	// State

	long PlotCount;

	// Methods

	long PlotLineChart ( ListDouble *DataX, ListDouble *DataY );
	void Reset ( void );

	// Constructor

	Graph ( void );
};

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

#endif
