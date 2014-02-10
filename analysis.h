#ifndef ANALYSIS_H
#define ANALYSIS_H

#include<QDialog>
#include<QLineEdit>
#include<QComboBox>
#include<QCheckBox>
#include<iostream>
#include <QTimer>
#include<fstream>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>                                                              
#include<time.h>  
#include<string.h>
#include<QApplication>
#include <QDateEdit>
#include <QScrollBar>
#include <QDateTime>
#include<vector>
#include<sstream>
#include<algorithm>
#include<QHBoxLayout>
#include<QObjectList>
#include<QDateTime>

#include"qchartviewer.h"
#include"chartdir.h"

using namespace std;

class Analysis : public QDialog 
{
    Q_OBJECT
	public:
		Analysis(QWidget *parent = 0);
		~Analysis();
		
	private:
		QChartViewer *m_ChartViewer;
		QDateEdit *m_StartDate;
    	QDateEdit *m_EndDate;
    	QScrollBar *m_HScrollBar;
    	QLineEdit *m_TickerSymbol;
    	QLineEdit *m_MovingAvg1;
    	QLineEdit *m_MovingAvg2;
    	
		
		double date[5000];
		double open[5000];
		double high[5000];
		double low[5000];
		double close[5000];
		double volume[5000];
		double adjclose[5000];
		int data_len;    	
	
		void drawChart(QChartViewer *viewer);            // Draw chart
		void analysis(MultiChart *m, int mouseX);    // Draw Track Cursor
		void initChartViewer(QChartViewer *viewer);     // Initialize the QChartViewer
		void read_data(char* );
		void updateControls(QChartViewer *viewer);      // Update other controls
		void updateImageMap(QChartViewer *viewer);      // Update the image map	



	private slots:
		void onMouseUsageChanged(int mouseUsage);
		void onViewPortChanged();	
		void onMouseMovePlotArea(QMouseEvent *event);
		void onHScrollBarChanged(int value);
		void onMouseWheelChart(QWheelEvent *event);	
		void onLineEditChanged();
		void onLineEditMAChanged();		
};

#endif // ANALYSIS_H
