/*********************************************************************
 *  Author  : Himangshu Saikia, Wiebke Koepp
 *  Init    : Tuesday, September 19, 2017 - 15:08:24
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab3/eulerrk4comparison.h>
#include <dd2257lab3/integrator.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo EulerRK4Comparison::processorInfo_
{
    "org.inviwo.EulerRK4Comparison",      // Class identifier
    "Euler RK4 Comparison",               // Display name
    "DD2257",                             // Category
    CodeState::Experimental,              // Code state
    Tags::None,                           // Tags
};

const ProcessorInfo EulerRK4Comparison::getProcessorInfo() const
{
    return processorInfo_;
}

EulerRK4Comparison::EulerRK4Comparison()
	:Processor()
	, outMesh("meshOut")
	, inData("inData")
	, propStartPoint("startPoint", "Start Point", vec2(0.5, 0.5), vec2(0), vec2(1024), vec2(0.5))
	// TODO: Initialize additional properties
	// default value (optional), minimum value (optional), maximum value (optional), increment (optional));
	// propertyIdentifier cannot have spaces
	, propSteps("steps", "Steps",  50, 0, 10000)
	, propStepsize("stepsize", "Stepsize", 0.2f, 0.0f)
    , mouseMoveStart("mouseMoveStart", "Move Start", [this](Event* e) { eventMoveStart(e); },
        MouseButton::Left, MouseState::Press | MouseState::Move)
{
	// Register Ports
    addPort(outMesh);
    addPort(inData);

    // Register Properties
    addProperty(propStartPoint);
    addProperty(mouseMoveStart);

    // TODO: Register additional properties
    addProperty(propSteps);
	addProperty(propStepsize);

}

void EulerRK4Comparison::eventMoveStart(Event* event)
{
    auto mouseEvent = static_cast<MouseEvent*>(event);
    vec2 mousePos = mouseEvent->posNormalized();
    // Denormalize to volume dimensions
    mousePos.x *= dims.x - 1;
    mousePos.y *= dims.y - 1;
    // Update starting point
    propStartPoint.set(mousePos);
    event->markAsUsed();
}

void EulerRK4Comparison::process()
{
    // Get input
    if (!inData.hasData())
    {
        return;
    }
    auto vol = inData.getData();

    // Retreive data in a form that we can access it
    const VolumeRAM* vr = vol->getRepresentation< VolumeRAM >();
    dims = vol->getDimensions();
    // The start point should be inside the volume (set maximum to the upper right corner)
    propStartPoint.setMaxValue(vec2(dims.x - 1, dims.y - 1));

    // Initialize mesh, vertices and index buffers for the two streamlines and the 
	auto mesh = std::make_shared<BasicMesh>();
    std::vector<BasicMesh::Vertex> vertices;
	auto indexBufferEuler = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
    auto indexBufferRK = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
    auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);

    // Draw start point
    vec2 startPoint = propStartPoint.get();
    vertices.push_back({ vec3(startPoint.x / (dims.x -1), startPoint.y / (dims.y-1), 0), 
        vec3(0), vec3(0), vec4(0, 0, 0, 1)});
    indexBufferPoints->add(static_cast<std::uint32_t>(0));
    indexBufferEuler->add(static_cast<std::uint32_t>(0));
    indexBufferRK->add(static_cast<std::uint32_t>(0));

    // TODO: Implement the Euler and Runge-Kutta of 4th order integration schemes
    // and then integrate forward for a specified number of integration steps and a given stepsize 
    // (these should be additional properties of the processor)

	//init - first value
	vec2 buffertVec2 = startPoint;
	uint32_t IndexBuffOffset = 0;
	//Eulerboy
	
	for (IndexBuffOffset = 0;IndexBuffOffset <= propSteps.get(); IndexBuffOffset++)
	{
		
		//calculation
		buffertVec2 = Integrator::Euler(vr, dims, buffertVec2, propStepsize.get());
		
		
		//drawing
		vertices.push_back({ vec3(buffertVec2.x / (dims.x - 1), buffertVec2.y / (dims.y - 1), 0),
			vec3(0), vec3(0), vec4(0, 0, 1, 1) });

		indexBufferEuler->add(static_cast<std::uint32_t>(IndexBuffOffset));
		indexBufferPoints->add(static_cast<std::uint32_t>(IndexBuffOffset));
	}
	
	// my man Runga von kutta the fourth. 
	//init - first value
	

	buffertVec2 = startPoint;
	
	for (uint32_t i = 0; i <= propSteps.get(); i++)
	{

		//calculation
		buffertVec2 = Integrator::RK4(vr, dims, buffertVec2, propStepsize.get());


		//drawing
		vertices.push_back({ vec3(buffertVec2.x / (dims.x - 1), buffertVec2.y / (dims.y - 1), 0),
			vec3(0), vec3(0), vec4(0, 1, 0, 1) });

		indexBufferRK->add(static_cast<std::uint32_t>(IndexBuffOffset+1+i));
		indexBufferPoints->add(static_cast<std::uint32_t>(IndexBuffOffset+1+i));

	}

    

	mesh->addVertices(vertices);
	outMesh.setData(mesh);
}

} // namespace