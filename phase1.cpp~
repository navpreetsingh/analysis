#include<iostream>
#include<fstream>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>                                                              
#include<time.h>  
#include<string.h>

#include"chartdir.h"

using namespace std;

class Symbol
{
	private:
		string date[5000];
		double open[5000];
		double high[5000];
		double low[5000];
		double close[5000];
		int volume[5000];
		double adjclose[5000];
		int data_len;
	public:
		//PHASE 1
		void read_data(char*);
		
		//PHASE 2
		void candlestick();

};


int
main()
{
	Symbol sym;
	
	//PHASE 1
	char symbol[100] = "/home/navpreet/major_project/Python27/Lib/site-packages/QSTK/QSData/Yahoo/A.csv" ;
	sym.read_data(symbol);
	
	//PHASE 2
	sym.candlestick();
	return 0;
}

void Symbol :: candlestick()
{
	int data_len = 20;
	const char* a[data_len];
	for(int i = 0; i < data_len; i++)
		a[i] = date[i].c_str();
	
	
	
	// Create a XYChart object of size 600 x 350 pixels
    XYChart *c = new XYChart(600, 350);

    // Set the plotarea at (50, 25) and of size 500 x 250 pixels. Enable both the
    // horizontal and vertical grids by setting their colors to grey (0xc0c0c0)
    c->setPlotArea(50, 25, 500, 250)->setGridColor(0xc0c0c0, 0xc0c0c0);

    // Add a title to the chart
    c->addTitle("Universal Stock Index on Jan 2001");

    // Add a custom text at (50, 25) (the upper left corner of the plotarea). Use 12
    // pts Arial Bold/blue (4040c0) as the font.
    c->addText(50, 25, "(c) Global XYZ ABC Company", "arialbd.ttf", 12, 0x4040c0);

    // Add a title to the x axis
    c->xAxis()->setTitle("Jan 2001");

    // Set the labels on the x axis. Rotate the labels by 45 degrees.
    c->xAxis()->setLabels(StringArray(a, data_len))->setFontAngle(45);
    cout << date[5];
	


    // Add a title to the y axis
    c->yAxis()->setTitle("Universal Stock Index");

    // Draw the y axis on the right hand side of the plot area
    c->setYAxisOnRight(true);

    // Add a CandleStick layer to the chart using green (00ff00) for up candles and
    // red (ff0000) for down candles
    CandleStickLayer *layer = c->addCandleStickLayer(DoubleArray(high, data_len
        ), DoubleArray(low, data_len), DoubleArray(open, data_len), DoubleArray(close, data_len), 0x00ff00, 0xff0000);

    // Set the line width to 2 pixels
    layer->setLineWidth(2);

    // Output the chart
    c->makeChart("candlestick.png");

    //free up resources
    delete c;	
}	
	
void Symbol :: read_data(char *symbol)
{
	ifstream file (symbol);
	string value;
	
	//ELIMINATE FIRST ROW
	int i;
	for(i = 0; i < 6; i++)
		getline(file, value, ',');
	getline(file, value);							 
	
	//GETTING DATA
	i = 0;
	while ( getline(file, date[i], ',') )
	{
		//date[i] = value.c_str();
		cout << "date: " << date[i] << '\n';
		
		getline(file, value, ',');
		open[i] = atof(value.c_str());
		cout << "open: " << open[i] << '\n';
		
		getline(file, value, ',');
		high[i] = atof(value.c_str());
		cout << "high: " << high[i] << '\n';
		
		getline(file, value, ',');
		low[i] = atof(value.c_str());
		cout << "low: " << low[i] << '\n';
		
		getline(file, value, ',');
		close[i] = atof(value.c_str());
		cout << "close: " << close[i] << '\n';
		
		getline(file, value, ',');
		volume[i] = atoi(value.c_str());
		cout << "vol: " << volume[i] << '\n';
		
		getline(file, value);
		adjclose[i] = atof(value.c_str());
		cout << "adj-close: " << adjclose[i] << '\n';
		
		i++;	
	}	
	data_len = i - 1;
}
