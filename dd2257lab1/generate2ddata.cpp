/*********************************************************************
 *  Author  : Wiebke Koepp, Pontus Karl Georg Strandberg, Matthew The, Jiayao Yu
 *  Init    : Friday, September 01, 2017 - 12:13:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab1/generate2ddata.h>
#include <math.h>

#define PI 3.14159265

namespace inviwo
{

	// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
	const ProcessorInfo Generate2DData::processorInfo_
	{
		"org.inviwo.Generate2DData",      // Class identifier
		"Generate 2D Data",               // Display name
		"DD2257",                         // Category
		CodeState::Experimental,          // Code state
		"CPU, DataFrame"                  // Tags
	};

	const ProcessorInfo Generate2DData::getProcessorInfo() const
	{
		return processorInfo_;
	}

	Generate2DData::Generate2DData()
		:Processor()
		, outDataFrame("outDataFrame")
		, propdataShape("dataShape", "Data Shape")
	{
		//Register ports

		addPort(outDataFrame);

		// Register properties

		addProperty(propdataShape);
		propdataShape.addOption("line", "Line", DataShape::Line);
		propdataShape.addOption("circle", "Circle", DataShape::Circle);
		propdataShape.addOption("hyperbola", "Hyperbola", DataShape::Hyperbola);
		propdataShape.set(DataShape::Line);
		propdataShape.setCurrentStateAsDefault();
	}


	void Generate2DData::process()
	{
		// Initialize the dataframe
		auto dataframe = std::make_shared<DataFrame>();

		// Create data container with two columns
		auto colX = dataframe->addColumn<float>("x");
		auto colY = dataframe->addColumn<float>("y");

		// Lamda function for adding a single data point
		auto addPoint = [&](float x, float y)
		{
			colX->add(x);
			colY->add(y);
		};

		switch (propdataShape.get())
		{
		case DataShape::Line: {
			// TODO: Add points that lie on a line with negative slope
			int steps_line = 100;			// steps is equivalent to data points amount
			float stepLengthX_line = 1.0;
			float stepLengthY_line = 0.5;
			float originX_line = 1.0;
			float originY_line = 20.0;
			float x_line[100], y_line[100] = { 0. };
			for (int i = 0; i < steps_line; i++) {
				x_line[i] = originX_line + i*stepLengthX_line;
				y_line[i] = originY_line - i*stepLengthY_line;
				addPoint(x_line[i], y_line[i]);
			}
			break;
		}
		case DataShape::Circle: {
			// TODO: Add points that lie on a circle
			int steps_circle = 100;
			float r = 10.0;
			float originX_circle = 5.0;
			float originY_circle = 5.0;
			float x_circle[100], y_circle[100] = { 0. };
			for (int i = 0; i < steps_circle; i++) {
				x_circle[i] = originX_circle + r*cos(i * 2 * PI / steps_circle);
				y_circle[i] = originY_circle + r*sin(i * 2 * PI / steps_circle);
				addPoint(x_circle[i], y_circle[i]);
			}
			break;
		}
		case DataShape::Hyperbola: {
			// TODO: Add points that lie on a hyperbola
			float steps_hyperbola = 100.0;
			float a = 1.0;
			float b = 1.0;
			float originX_hyperbola = 5.0;
			float originY_hyperbola = 5.0;
			float x_hyperbola[100], y_hyperbola[100] = { 0. };
			for (int i = 0; i < steps_hyperbola; i++) {
				x_hyperbola[i] = a*coshf(-3 + i * (6 / steps_hyperbola)) + originX_hyperbola;
				y_hyperbola[i] = b*sinhf(-3 + i * (6 / steps_hyperbola)) + originY_hyperbola;
				addPoint(x_hyperbola[i], y_hyperbola[i]);
				addPoint(-x_hyperbola[i], y_hyperbola[i]);
			}
			break;
		}
		default: {
			break;
		}
		}

		// We did not take care of the indices (first column of the dataframe)
		// so we will do this now
		dataframe->updateIndexBuffer();

		// Push the created data out
		outDataFrame.setData(dataframe);
	}

} // namespace


