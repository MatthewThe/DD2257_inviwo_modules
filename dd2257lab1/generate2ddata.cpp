/*********************************************************************
 *  Author  : Wiebke Koepp, Jiayao Yu
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
		//propdataShape.set(DataShape::Circle);
		//propdataShape.set(DataShape::Hyperbola);
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
			int step_line = 20;
			float stepLengthX_line = 0.5;
			float stepLengthY_line = 0.5;
			// TODO: Add points that lie on a line with negative slope
			//addPoint(0.3f, 0.4f);
			float x_line[20] = { 0. };
			float y_line[20] = { 20. };
			for (int i = 0; i < step_line; i++) {
				x_line[i] += i*stepLengthX_line;
				y_line[i] -= i*stepLengthY_line;
				addPoint(x_line[i], y_line[i]);
			}
			break;
		}
		case DataShape::Circle: {
			int step_circle = 36;
			float r = 10;
			int x_origin, y_origin = 5;
			float x_circle[36], y_circle[36] = { 0. };
			// TODO: Add points that lie on a circle
			for (int i = 0; i < step_circle; i++) {
				x_circle[i] = x_origin + r*cos(i * (360 / step_circle) * 2 * PI / 180);
				y_circle[i] = y_origin + r*sin(i * (360 / step_circle) * 2 * PI / 180);
				addPoint(x_circle[i], y_circle[i]);
			}
			//addPoint(0.24f, 0.4f);

			break;
		}
		case DataShape::Hyperbola: {
			int step_hyperbola = 36;
			int a, b = 5;
			float x_hyperbola[36], y_hyperbola[36] = { 0. };
			for (int i = 0; i < step_hyperbola; i++) {
				x_hyperbola[i] = a*cosh(i * 360 / step_hyperbola);
				y_hyperbola[i] = b*sinh(i * 360 / step_hyperbola);
				addPoint(x_hyperbola[i], y_hyperbola[i]);
			}

			// TODO: Add points that lie on a hyperbola
			//addPoint(0.3f, 0.4f);

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


