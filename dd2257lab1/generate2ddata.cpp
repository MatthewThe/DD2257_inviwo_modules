/*********************************************************************
 *  Author  : Wiebke Koepp, ... 
 *  Init    : Friday, September 01, 2017 - 12:13:44
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab1/generate2ddata.h>

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
            addPoint(0.3f, 0.4f);

            break;
        }
        case DataShape::Circle: {

            // TODO: Add points that lie on a circle
            addPoint(0.24f, 0.4f);
            break;
        }
        case DataShape::Hyperbola: {

            // TODO: Add points that lie on a hyperbola
            addPoint(0.3f, 0.4f);

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

