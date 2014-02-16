#include<iostream>
#include<fstream>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>                                                              
#include<time.h>  
#include<string.h>
#include<QApplication>
#include<vector>
#include<sstream>
#include<algorithm>
#include<QHBoxLayout>
#include<QObjectList>
#include<QDateTime>
#include <QPushButton>
#include <QButtonGroup>
#include <QMouseEvent>

#include"chartdir.h"
#include"FinanceChart.h"
#include"analysis.h"

using namespace std;

int
main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    //app.setStyleSheet("* {font-family:arial;font-size:11px}");
    Analysis demo;    
    demo.show();
    return app.exec();
}

Analysis::Analysis(QWidget *parent) :
    QDialog(parent, Qt::Window)
{
    //
    // Set up the GUI
    //

    setFixedSize(750, 500);
    setWindowTitle("Analysis by Navpreet Singh");

    // The frame on the left side
    QFrame *frame = new QFrame(this);
    frame->setGeometry(4, 4, 120, 500);
    frame->setFrameShape(QFrame::StyledPanel);
    
    // Ticker Symbol
    (new QLabel("Ticker Symbol", frame))->setGeometry(4, 8, 112, 28);
    m_TickerSymbol = new QLineEdit("AAPL", frame);
    m_TickerSymbol->setGeometry(4, 46, 112, 28);

    // Pointer push button
    QPushButton *pointerPB = new QPushButton(QIcon(":/pointer.png"), "Pointer");//, frame);
    /*pointerPB->setGeometry(4, 100, 112, 28);
    pointerPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
    pointerPB->setCheckable(true);*/

    // Zoom In push button
    QPushButton *zoomInPB = new QPushButton(QIcon(":/zoomin.png"), "Zoom In");//, frame);
    /*zoomInPB->setGeometry(4, 124, 112, 28);
    zoomInPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
    zoomInPB->setCheckable(true);*/

    // Zoom Out push button
    QPushButton *zoomOutPB = new QPushButton(QIcon(":/zoomout.png"), "Zoom Out");//, frame);
    /*zoomOutPB->setStyleSheet("QPushButton { text-align:left; padding:5px}");
    zoomOutPB->setGeometry(4, 148, 112, 28);
    zoomOutPB->setCheckable(true);*/
    
    // MOVING AVERAGE1
    (new QLabel("Moving Average 1", frame))->setGeometry(4, 180, 112, 28);
    m_MovingAvg1 = new QLineEdit("5", frame);
    m_MovingAvg1->setGeometry(4, 200, 112, 28);
    
    // MOVING AVERAGE 2
    (new QLabel("Moving Average 2", frame))->setGeometry(4, 230, 112, 28);
    m_MovingAvg2 = new QLineEdit("20", frame);
    m_MovingAvg2->setGeometry(4, 250, 112, 28);

    // The Pointer/Zoom In/Zoom Out buttons form a button group
    QButtonGroup *mouseUsage = new QButtonGroup(frame);
    mouseUsage->addButton(pointerPB, Chart::MouseUsageScroll);
    mouseUsage->addButton(zoomInPB, Chart::MouseUsageZoomIn);
    mouseUsage->addButton(zoomOutPB, Chart::MouseUsageZoomOut);
    connect(mouseUsage, SIGNAL(buttonPressed(int)), SLOT(onMouseUsageChanged(int)));

    m_ChartViewer = new QChartViewer(this);
    m_ChartViewer->setGeometry(128, 4, 630, 500);    
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)), SLOT(onMouseMovePlotArea(QMouseEvent*)));
    connect(m_ChartViewer, SIGNAL(mouseWheel(QWheelEvent*)), SLOT(onMouseWheelChart(QWheelEvent*)));
    drawChart(m_ChartViewer);
    connect(m_TickerSymbol, SIGNAL(editingFinished()), SLOT(onLineEditChanged()));
    connect(m_MovingAvg1, SIGNAL(editingFinished()), SLOT(onLineEditMAChanged()));
    connect(m_MovingAvg2, SIGNAL(editingFinished()), SLOT(onLineEditMAChanged()));
    drawChart(m_ChartViewer);

    // Horizontal scroll bar
    m_HScrollBar = new QScrollBar(Qt::Horizontal, this);
    m_HScrollBar->setGeometry(128, 485, 600, 17);
    connect(m_HScrollBar, SIGNAL(valueChanged(int)), SLOT(onHScrollBarChanged(int)));

    //
    // Initialize the chart
    //
	
    // Load the data
    char symbol[100] = "/home/navpreet/major_project/Yahoo/AAPL.csv" ;
    read_data(symbol);   

    // Initialize the QChartViewer
    initChartViewer(m_ChartViewer);

    // Initially set the mouse to drag to scroll mode
    pointerPB->click();

    // Trigger the ViewPortChanged event to draw the chart
    m_ChartViewer->updateViewPort(true, true);    
}



Analysis::~Analysis()
{
    delete m_ChartViewer->getChart();
}



void Analysis::drawChart(QChartViewer *viewer)
{
    
    // Get the start date and end date that are visible on the chart.
    double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
    double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
        viewer->getViewPortWidth());

    // Get the array indexes that corresponds to the visible start and end dates
    int startIndex = (int)floor(Chart::bSearch(DoubleArray(date, data_len), viewPortStartDate));
    int endIndex = (int)ceil(Chart::bSearch(DoubleArray(date, data_len), viewPortEndDate));
    int noOfPoints = endIndex - startIndex + 1;
    
    
    char symbol[150];
    sprintf(symbol,"/home/navpreet/major_project/Yahoo/%s.csv", m_TickerSymbol->text().toLocal8Bit().data());
    cout<<"symbol"<<symbol << "\n";
     read_data(symbol);
     
    
    // Create a FinanceChart object of width 720 pixels
    FinanceChart *c = new FinanceChart(610);
    
    // Add a title to the chart
    if(data_len <= 0)
    	c->addTitle("INVALID SYMBOL");    
    else
    	c->addTitle(Chart::TopCenter, m_TickerSymbol->text().toLocal8Bit().data());

    // Disable default legend box, as we are using dynamic legend
    c->setLegendStyle("normal", 8, Chart::Transparent, Chart::Transparent);
	
    // Set the data into the finance chart object
    c->setData(DoubleArray(date + startIndex, noOfPoints), DoubleArray(high + startIndex, noOfPoints), DoubleArray(low + startIndex, noOfPoints), DoubleArray(open + startIndex, noOfPoints), DoubleArray(close + startIndex, noOfPoints), DoubleArray(volume + startIndex, noOfPoints), 30);

    // Add the main chart with 240 pixels in height
    c->addMainChart(280);

    // Add a 10 period simple moving average to the main chart, using brown color
    int a = m_MovingAvg1->text().toInt();
    c->addSimpleMovingAvg(a, 0x663300);

    // Add a 20 period simple moving average to the main chart, using purple color
    int b = m_MovingAvg2->text().toInt();
    c->addSimpleMovingAvg(b, 0x9900ff);

    // Add candlestick symbols to the main chart, using green/red for up/down days
    c->addCandleStick(0x00ff00, 0xff0000);

    // Add 20 days bollinger band to the main chart, using light blue (9999ff) as the border and
    // semi-transparent blue (c06666ff) as the fill color
    c->addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);

    // Add a 75 pixels volume bars sub-chart to the bottom of the main chart, using green/red/grey for
    // up/down/flat days
    c->addVolBars(75, 0x99ff99, 0xff9999, 0x808080);

    // Append a 14-days RSI indicator chart (75 pixels high) after the main chart. The main RSI line
    // is purple (800080). Set threshold region to +/- 20 (that is, RSI = 50 +/- 25). The upper/lower
    // threshold regions will be filled with red (ff0000)/blue (0000ff).
    c->addRSI(75, 14, 0x800080, 20, 0xff0000, 0x0000ff);

    // Append a MACD(26, 12) indicator chart (75 pixels high) after the main chart, using 9 days for
    // computing divergence.
    c->addMACD(75, 26, 12, 9, 0x0000ff, 0xff00ff, 0x008000);

    // Include track line with legend for the latest data values
    analysis(c, ((XYChart *)c->getChart(0))->getPlotArea()->getRightX());
    //cout << "track875: " << ((XYChart *)c->getChart(0))->getPlotArea()->getRightX();
    
    
    ///////////////////////////////////////////////////////////////////////////////////////
    // Output the chart
    ///////////////////////////////////////////////////////////////////////////////////////

    viewer->setChart(c);
        
}


void Analysis::onLineEditChanged()
{
    char symbol[150];
    sprintf(symbol,"/home/navpreet/major_project/Yahoo/%s.csv", m_TickerSymbol->text().toLocal8Bit().data());
    cout<<"symbol"<<symbol << "\n";
    read_data(symbol);
    drawChart(m_ChartViewer);
}

void Analysis::onLineEditMAChanged()
{
    drawChart(m_ChartViewer);
}


//
// Initialize the QChartViewer
//
void Analysis::initChartViewer(QChartViewer *viewer)
{
    // Set the full x range to be the duration of the data
    viewer->setFullRange("x", date[0], date[data_len]);

    // Initialize the view port to show the latest 20% of the time range
    viewer->setViewPortWidth(0.04);
    viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

    // Set the maximum zoom to 10 points
    viewer->setZoomInWidthLimit(50.0 / data_len);
}

//
// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
//

void Analysis::onViewPortChanged()
{
	updateControls(m_ChartViewer);
    
    if (m_ChartViewer->needUpdateChart())
        drawChart(m_ChartViewer);
    // We need to update the track line too. If the mouse is moving on the chart (eg. if
    // the user drags the mouse on the chart to scroll it), the track line will be updated
    // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
    if (m_ChartViewer->needUpdateImageMap())
        updateImageMap(m_ChartViewer);
}

void Analysis::updateImageMap(QChartViewer *viewer)
{
    // Include tool tip for the chart
    if (0 == viewer->getImageMapHandler())
    {
        viewer->setImageMap(viewer->getChart()->getHTMLImageMap("", "",
            "title='[{dataSetName}] {x|mmm dd, yyyy}: USD {value|2}'"));
    }
}

//
// The Pointer, Zoom In or Zoom out button is pressed
//
void Analysis::onMouseUsageChanged(int mouseUsage)
{
    m_ChartViewer->setMouseUsage(mouseUsage);
}


//
// Update controls in the user interface when the view port changed
//
void Analysis::updateControls(QChartViewer *viewer)
{
    // The logical length of the scrollbar. It can be any large value. The actual value does
    // not matter.
    const int scrollBarLen = 1000000000;

    // Update the horizontal scroll bar
    m_HScrollBar->setEnabled(viewer->getViewPortWidth() < 1);
    m_HScrollBar->setPageStep((int)ceil(viewer->getViewPortWidth() * scrollBarLen));
    m_HScrollBar->setSingleStep(min(scrollBarLen / 100, m_HScrollBar->pageStep()));
    m_HScrollBar->setRange(0, scrollBarLen - m_HScrollBar->pageStep());
    m_HScrollBar->setValue((int)(0.5 + viewer->getViewPortLeft() * scrollBarLen));

}

//
// User clicks on the the horizontal scroll bar
//
void Analysis::onHScrollBarChanged(int value)
{
    if (!m_ChartViewer->isInViewPortChangedEvent())
    {
        // Set the view port based on the scroll bar
        int scrollBarLen = m_HScrollBar->maximum() + m_HScrollBar->pageStep();
        m_ChartViewer->setViewPortLeft(value / (double)scrollBarLen);

        // Update the chart display without updating the image maps. (We can delay updating
        // the image map until scrolling is completed and the chart display is stable.)
        m_ChartViewer->updateViewPort(true, false);
    }
}

//
// When the mouse enters the chart, we will generate an image map for hot spots and tooltips
// support if it has not already been generated.
//
void Analysis::onMouseWheelChart(QWheelEvent *event)
{
    // Process the mouse wheel only if the mouse is over the plot area
    if (!m_ChartViewer->isMouseOnPlotArea())
    {
        event->ignore();
        return;
    }

    // We zoom in or out by 10% depending on the mouse wheel direction.
    double newVpWidth = m_ChartViewer->getViewPortWidth() * (event->delta() > 0 ? 0.9 : 1 / 0.9);
    double newVpHeight = m_ChartViewer->getViewPortHeight() * (event->delta() > 0 ? 0.9 : 1 / 0.9);

    // We do not zoom beyond the zoom width or height limits.
    newVpWidth = max(m_ChartViewer->getZoomInWidthLimit(), min(newVpWidth,
        m_ChartViewer->getZoomOutWidthLimit()));
    newVpHeight = max(m_ChartViewer->getZoomInHeightLimit(), min(newVpWidth,
        m_ChartViewer->getZoomOutHeightLimit()));

    if ((newVpWidth != m_ChartViewer->getViewPortWidth()) ||
            (newVpHeight != m_ChartViewer->getViewPortHeight()))
    {
        // Set the view port position and size so that the point under the mouse remains under
        // the mouse after zooming.

        double deltaX = (m_ChartViewer->getPlotAreaMouseX() - m_ChartViewer->getPlotAreaLeft()) *
            (m_ChartViewer->getViewPortWidth() - newVpWidth) / m_ChartViewer->getPlotAreaWidth();
        m_ChartViewer->setViewPortLeft(m_ChartViewer->getViewPortLeft() + deltaX);
        m_ChartViewer->setViewPortWidth(newVpWidth);

        double deltaY = (m_ChartViewer->getPlotAreaMouseY() - m_ChartViewer->getPlotAreaTop()) *
            (m_ChartViewer->getViewPortHeight() - newVpHeight) / m_ChartViewer->getPlotAreaHeight();
        m_ChartViewer->setViewPortTop(m_ChartViewer->getViewPortTop() + deltaY);
        m_ChartViewer->setViewPortHeight(newVpHeight);

        m_ChartViewer->updateViewPort(true, false);
    }
}


//
// Draw track cursor when mouse is moving over plotarea
//
void Analysis::onMouseMovePlotArea(QMouseEvent *)
{
    analysis((MultiChart *)m_ChartViewer->getChart(), m_ChartViewer->getPlotAreaMouseX());
    m_ChartViewer->updateDisplay();
}


//
// Draw finance chart track line with legend
//
void Analysis::analysis(MultiChart *m, int mouseX)
{
    //cout << "mouseX" << mouseX << "\n";
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = m->initDynamicLayer();

    // It is possible for a FinanceChart to be empty, so we need to check for it.
    if (m->getChartCount() == 0)
        return ;
	//cout << "Chart count: " << m->getChartCount() << "\n" ;
    // Get the data x-value that is nearest to the mouse
    int xValue = (int)(((XYChart *)m->getChart(0))->getNearestXValue(mouseX));//layer
    //cout << "(XYChart *)m->getChart(0): " << (XYChart *)m->getChart(0) << "\n";
    //cout << "xValue: " << xValue << "\n";
	//cout << "xValue: " << xValue << "\n";
    // Iterate the XY charts (main price chart and indicator charts) in the FinanceChart
    XYChart *c = 0;
    for(int i = 0; i < m->getChartCount(); ++i) 
    {
        //cout << "IIIIiiii" << i << "\n";
        c = (XYChart *)m->getChart(i);
        //cout << "XYCHART: " << c << "\n";

        // Variables to hold the legend entries
        ostringstream ohlcLegend;
        vector<string> legendEntries;

        // Iterate through all layers to find the highest data point
        for(int j = 0; j < c->getLayerCount(); ++j) 
        {
            //cout << "JJJJjjjj" << j << "\n";
            //cout << "layer count: " << c->getLayerCount() << "\n";
            Layer *layer = c->getLayerByZ(j);
            //cout << "Layer: " << layer << "\n";
            int xIndex = layer->getXIndexOf(xValue);
            //cout << "xIndex: " << xIndex << "\n";
            int dataSetCount = layer->getDataSetCount();
            //cout << "data set count: " << dataSetCount << "\n";

            // In a FinanceChart, only layers showing OHLC data can have 4 data sets
            if (dataSetCount == 4) 
            {
                double highValue = layer->getDataSet(0)->getValue(xIndex);
                //cout << "high: " << highValue << "\n";
                double lowValue = layer->getDataSet(1)->getValue(xIndex);
                //cout << "low: " << lowValue << "\n";
                double openValue = layer->getDataSet(2)->getValue(xIndex);
                //cout << "open: " << openValue << "\n";
                double closeValue = layer->getDataSet(3)->getValue(xIndex);
                //cout << "close: " << closeValue << "\n";
				
				//cout << "Chart noValue: " << Chart::NoValue << "\n";
                if (closeValue != Chart::NoValue) 
                {
                    // Build the OHLC legend
					ohlcLegend << "      <*block*>";
					ohlcLegend << "Open: " << c->formatValue(openValue, "{value|P4}");
					ohlcLegend << ", High: " << c->formatValue(highValue, "{value|P4}"); 
					ohlcLegend << ", Low: " << c->formatValue(lowValue, "{value|P4}"); 
					ohlcLegend << ", Close: " << c->formatValue(closeValue, "{value|P4}");
					//cout << "Display open: " << c->formatValue(openValue, "{value|P4}") << "\n";
					//cout << "Display high: " << c->formatValue(highValue, "{value|P4}") << "\n";
					//cout << "Display low: " << c->formatValue(lowValue, "{value|P4}") << "\n";
					//cout << "Display close: " << c->formatValue(closeValue, "{value|P4}") << "\n";
					
                    // We also draw an upward or downward triangle for up and down days and the %
                    // change
                    double lastCloseValue = layer->getDataSet(3)->getValue(xIndex - 1);
                    //cout << "Last Close Value: " << lastCloseValue << "\n";
                    if (lastCloseValue != Chart::NoValue) 
                    {
                        double change = closeValue - lastCloseValue;
                        double percent = change * 100 / closeValue;
                        //cout << "Percent" << percent << "\n";
                        string symbol = (change >= 0) ?
                            "<*font,color=008800*><*img=@triangle,width=8,color=008800*>" :
                            "<*font,color=CC0000*><*img=@invertedtriangle,width=8,color=CC0000*>";

                        ohlcLegend << "  " << symbol << " " << c->formatValue(change, "{value|P4}");
                    }

					ohlcLegend << "<*/*>";
                }
            } 
            else 
            {
                // Iterate through all the data sets in the layer
                for(int k = 0; k < layer->getDataSetCount(); ++k)
                {
                    DataSet *dataSet = layer->getDataSetByZ(k);
					//cout << "Dataset by Z: " << dataset << "\n";
                    string name = dataSet->getDataName();
                    //cout << "Dataset Name: " << name << "\n";
                    double value = dataSet->getValue(xIndex);
                    //cout << "Data set Value: " << value << "\n";
                    //cout << "name size: " << name.size() << "\n";
                    if ((0 != name.size()) && (value != Chart::NoValue)) 
                    {
                        // In a FinanceChart, the data set name consists of the indicator name and its
                        // latest value. It is like "Vol: 123M" or "RSI (14): 55.34". As we are
                        // generating the values dynamically, we need to extract the indictor name
                        // out, and also the volume unit (if any).

						// The volume unit
						string unitChar;
						//cout << "unit char: " << unitChar << "\n";

                        // The indicator name is the part of the name up to the colon character.
						int delimiterPosition = (int)name.find(':');
						//cout << "delimiter position " << delimiterPosition << "\n";
						//cout << "name nPOS: " << name.npos << "\n";
                        if ((int)name.npos != delimiterPosition) 
                        {							
							// The unit, if any, is the trailing non-digit character(s).
							int lastDigitPos = (int)name.find_last_of("0123456789");
							//cout << "Last digit position: " << lastDigitPos << "\n";
                            if (((int)name.npos != lastDigitPos) && (lastDigitPos + 1 < (int)name.size())
                                && (lastDigitPos > delimiterPosition))
								unitChar = name.substr(lastDigitPos + 1);
								//cout << "unit char 2: " << unitChar << "\n";

							name.resize(delimiterPosition);
                        }

                        // In a FinanceChart, if there are two data sets, it must be representing a
                        // range.
                        if (dataSetCount == 2) 
                        {
                            // We show both values in the range in a single legend entry
                            value = layer->getDataSet(0)->getValue(xIndex);
                            //cout << "value22: " << value << "\n";
                            double value2 = layer->getDataSet(1)->getValue(xIndex);
                            //cout << "value2: " << value2 << "\n";
                            name = name + ": " + c->formatValue(min(value, value2), "{value|P3}");
							name = name + " - " + c->formatValue(max(value, value2), "{value|P3}");
                        } 
                        else 
                        {
                            // In a FinanceChart, only the layer for volume bars has 3 data sets for
                            // up/down/flat days
                            if (dataSetCount == 3) 
                            {
                                // The actual volume is the sum of the 3 data sets.
                                value = layer->getDataSet(0)->getValue(xIndex) + layer->getDataSet(1
                                    )->getValue(xIndex) + layer->getDataSet(2)->getValue(xIndex);
                                //cout << "value in if1: " << value << "\n";
                                //cout << "layer->getDataSet(0)->getValue(xIndex): " << layer->getDataSet(0)->getValue(xIndex) << "\n";
                                //cout << "layer->getDataSet(1)->getValue(xIndex): " << layer->getDataSet(1)->getValue(xIndex) << "\n";
                                //cout << "layer->getDataSet(2)->getValue(xIndex): " << layer->getDataSet(1)->getValue(xIndex) << "\n";                                    
                            }

                            // Create the legend entry
                            name = name + ": " + c->formatValue(value, "{value|P3}") + unitChar;
                            //cout << "name legend entry: " << name << "\n";
                        }

                        // Build the legend entry, consist of a colored square box and the name (with
                        // the data value in it).
						ostringstream legendEntry;
						legendEntry << "<*block*><*img=@square,width=8,edgeColor=000000,color=" 
							<< hex << dataSet->getDataColor() << "*> " << name << "<*/*>";
						//cout << "legend Entry: " << legendEntry << "\n";	
                        legendEntries.push_back(legendEntry.str());
                    }
                }
            }
        }

        // Get the plot area position relative to the entire FinanceChart
        PlotArea *plotArea = c->getPlotArea();
        //cout << "plot area: " << plotArea << "\n";
        int plotAreaLeftX = plotArea->getLeftX() + c->getAbsOffsetX();
        //cout << "plot area leftx: " << plotAreaLeftX << "\n";
        int plotAreaTopY = plotArea->getTopY() + c->getAbsOffsetY();
        //cout << "plot area topy: " << plotAreaTopY << "\n";

		// The legend begins with the date label, then the ohlcLegend (if any), and then the
		// entries for the indicators.
		ostringstream legendText;
		legendText << "<*block,valign=top,maxWidth=" << (plotArea->getWidth() - 5) 
			<< "*><*font=arialbd.ttf*>[" << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
			<< "]<*/font*>" << ohlcLegend.str();
		for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
		{
			legendText << "      " << legendEntries[i];
			//cout << "legend text: " << legendText << "\n";
		}
		legendText << "<*/*>";

        // Draw a vertical track line at the x-position
        d->vline(plotAreaTopY, plotAreaTopY + plotArea->getHeight(), c->getXCoor(xValue) +
            c->getAbsOffsetX(), d->dashLineColor(0x000000, 0x0101));

        // Display the legend on the top of the plot area
        TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
        t->draw(plotAreaLeftX + 5, plotAreaTopY + 3, 0x000000, Chart::TopLeft);
		t->destroy();
    }
}
	
void Analysis :: read_data(char *symbol)
{
	ifstream file (symbol);
	string value;
	struct tm tm1;
	
	//ELIMINATE FIRST ROW
	int i;
	for(i = 0; i < 6; i++)
		getline(file, value, ',');
	getline(file, value);							 
	
	//GETTING DATA
	i = 0;
	while ( getline(file, value, ',') )
	{
		
		sscanf(value.c_str(),"%4d-%2d-%2d",&tm1.tm_year,&tm1.tm_mon,&tm1.tm_mday);
		//cout << "date: " <<tm1.tm_year << "-" << tm1.tm_mon << "-" << tm1.tm_mday <<"\n";
		date[i] = Chart::chartTime(tm1.tm_year , tm1.tm_mon, tm1.tm_mday);
		
		getline(file, value, ',');
		open[i] = atof(value.c_str());
		//cout << "open: " << open[i] << '\n';
		
		getline(file, value, ',');
		high[i] = atof(value.c_str());
		//cout << "high: " << high[i] << '\n';
		
		getline(file, value, ',');
		low[i] = atof(value.c_str());
		//cout << "low: " << low[i] << '\n';
		
		getline(file, value, ',');
		close[i] = atof(value.c_str());
		//cout << "close: " << close[i] << '\n';
		
		getline(file, value, ',');
		volume[i] = atoi(value.c_str());
		//cout << "vol: " << volume[i] << '\n';
		
		getline(file, value);
		adjclose[i] = atof(value.c_str());
		//cout << "adj-close: " << adjclose[i] << '\n';
		
		i++;	
	}	
	data_len = i - 1 ;
	
	for(i=0; i < data_len/2; i++)
	{
		double d_temp;		
		
		d_temp = date[i];
		date[i] = date[data_len - i];
		date[data_len - i] = d_temp;
		
		d_temp = open[i];
		open[i] = open[data_len - i];
		open[data_len - i] = d_temp;
		
		d_temp = high[i];
		high[i] = high[data_len - i];
		high[data_len - i] = d_temp;
		
		d_temp = low[i];
		low[i] = low[data_len - i];
		low[data_len - i] = d_temp;
		
		d_temp = close[i];
		close[i] = close[data_len - i];
		close[data_len - i] = d_temp;
		
		d_temp = adjclose[i];
		adjclose[i] = adjclose[data_len - i];
		adjclose[data_len - i] = d_temp;
		
		d_temp = volume[i];
		volume[i] = volume[data_len - i];
		volume[data_len - i] = d_temp;		
	}	
	cout<<"data len" << data_len << "\n";	
}
