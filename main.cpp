#include <qapplication.h>
#include <QVBoxLayout>
#include "CHR_6dm.h"
#include "plot.h"

using namespace CHR;

CHR_6dm device("/dev/ttyUSB0");
SensorData sensor;

double sensor_callback(void)
{
	device.getData(&sensor);
	return sensor.datafield.accel_y()/1000;
}

int main( int argc, char **argv )
{
	if (device.open(20) != CHR_OK) {
		fprintf(stderr, "Error Open");
		return -1;
	}
	sensor.enableChannel(Accelerometer);
	
	if (device.setActiveChannels(&sensor) != CHR_OK) {
		fprintf(stderr, "Error set active channel");
		return -1;
	}

	if (device.gotoMeasurementMode(&sensor, 300) != CHR_OK) {
		fprintf(stderr, "Error goto measurement mode");
		return -1;
	}
	
	QApplication a( argc, argv );
	
	QWidget *window = new QWidget;
	window->setWindowTitle("Multiple Plot");


	Plot *plot1 = new Plot;
	plot1->resize( 600, 400 );

	Settings setting1;
	setting1.sensor_callback = sensor_callback;
	setting1.updateInterval = 20;
	plot1->setSettings(setting1);

	Plot *plot2 = new Plot;
	plot2->resize( 600, 400 );

	Settings setting2;
	setting2.sensor_callback = sensor_callback;
	setting2.updateInterval = 20;
	plot2->setSettings(setting2);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(plot1);
	layout->addWidget(plot2);
	window->setLayout(layout);

	window->resize(600, 800);
	window->show();

	return a.exec();
}
