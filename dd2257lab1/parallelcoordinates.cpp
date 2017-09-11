/*********************************************************************
 *  Author  : Wiebke 
 *  Init    : Wednesday, August 30, 2017 - 18:02:19
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab1/parallelcoordinates.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo ParallelCoordinates::processorInfo_
{
    "org.inviwo.ParallelCoordinatesDD2257", // Class identifier
    "Parallel Coordinates",                 // Display name
    "DD2257",                               // Category
    CodeState::Experimental,                // Code state
    "Plotting",                             // Tags
};

const ProcessorInfo ParallelCoordinates::getProcessorInfo() const
{
    return processorInfo_;
}

ParallelCoordinates::ParallelCoordinates()
    :Processor()
    , inData("indata")
    , outMeshLines("outMeshLines")
    , outMeshAxis("outMeshAxis")
    , propColorLines("linesColor", "Line Color", vec4(0.0f, 0.0f, 1.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    , propColorAxes("axisColor", "Axis Color", vec4(1.0f, 1.0f, 1.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
{
    // Register ports
    addPort(inData);
    addPort(outMeshAxis);
    addPort(outMeshLines);

    // Register properties
    addProperty(propColorLines);
    addProperty(propColorAxes);
}


void ParallelCoordinates::process()
{
    auto dataFrame = inData.getData();

    auto meshPoints = std::make_shared<BasicMesh>();
    auto meshAxes = std::make_shared<BasicMesh>();

    size_t numberOfRows = dataFrame->getNumberOfRows();
    size_t numberOfColumns = dataFrame->getNumberOfColumns();

    // TODO: Create a parallel coordinates plot 
    // You have to create both an axis mesh and a line mesh
    // Examples on how to create a mesh and how to fill it
    // are given in the Scatter plot processor

    std::vector<BasicMesh::Vertex> verticesAxis;
    auto indexBufferLines =
        meshAxes->addIndexBuffer(DrawType::Lines, ConnectivityType::None);
    verticesAxis.push_back({ vec3(0.2f, 0.5f, 0), vec3(0), vec3(0.2f, 0.5f, 0), propColorAxes.get() });
    indexBufferLines->add(static_cast<std::uint32_t>(0));
    verticesAxis.push_back({ vec3(0.1f, 0.9f, 0), vec3(0), vec3(0.2f, 0.5f, 0), propColorAxes.get() });
    indexBufferLines->add(static_cast<std::uint32_t>(1));
    meshAxes->addVertices(verticesAxis);

    // Push the mesh out
    outMeshLines.setData(meshPoints);
    outMeshAxis.setData(meshAxes);
}

} // namespace

