/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, September 19, 2017 - 15:08:33
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab3/streamlineintegrator.h>
#include <dd2257lab3/integrator.h>
#include <inviwo/core/util/utilities.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo StreamlineIntegrator::processorInfo_
{
    "org.inviwo.StreamlineIntegrator",      // Class identifier
    "Streamline Integrator",                // Display name
    "DD2257",                               // Category
    CodeState::Experimental,                // Code state
    Tags::None,                             // Tags
};

const ProcessorInfo StreamlineIntegrator::getProcessorInfo() const
{
    return processorInfo_;
}


StreamlineIntegrator::StreamlineIntegrator()
	:Processor()
	, inData("volIn")
	, outMesh("meshOut")
    , propStartPoint("startPoint", "Start Point", vec2(0.5, 0.5), vec2(0), vec2(1024), vec2(0.5))
    , propSeedMode("seedMode", "Seeds")
    // TODO: Initialize additional properties
    // propertyName("propertyIdentifier", "Display Name of the Propery", 
    // default value (optional), minimum value (optional), maximum value (optional), increment (optional));
    // propertyIdentifier cannot have spaces
    , mouseMoveStart("mouseMoveStart", "Move Start", [this](Event* e) { eventMoveStart(e); },
        MouseButton::Left, MouseState::Press | MouseState::Move)
{
    // Register Ports
	addPort(inData);
	addPort(outMesh);

    // Register Properties
    propSeedMode.addOption("one", "Single Start Point", 0);
    propSeedMode.addOption("multiple", "Multiple Seeds", 1);
    addProperty(propSeedMode);
    addProperty(propStartPoint);
    addProperty(mouseMoveStart);

    // TODO: Register additional properties
    // addProperty(propertyName);

    // Show properties for a single seed and hide properties for multiple seeds (TODO)
    propSeedMode.onChange([this]()
    {
        if (propSeedMode.get() == 0)
        {
            util::show(propStartPoint, mouseMoveStart);
            // util::hide(...)
        }
        else
        {
            util::hide(propStartPoint, mouseMoveStart);
            // util::show(...)
        }
    });

}

void StreamlineIntegrator::eventMoveStart(Event* event)
{
    // Handle mouse interaction only if we 
    // are in the mode with a single point
    if (propSeedMode.get() == 1) return;
    auto mouseEvent = static_cast<MouseEvent*>(event);
    vec2 mousePos = mouseEvent->posNormalized();
    // Denormalize to volume dimensions
    mousePos.x *= dims.x - 1;
    mousePos.y *= dims.y - 1;
    // Update starting point
    propStartPoint.set(mousePos);
    event->markAsUsed();
}

void StreamlineIntegrator::process()
{
    // Get input
	if (!inData.hasData()) {
		return;
	}
	auto vol = inData.getData();

    // Retreive data in a form that we can access it
    auto vr = vol->getRepresentation<VolumeRAM>();
    dims = vol->getDimensions();
    // The start point should be inside the volume (set maximum to the upper right corner)
    propStartPoint.setMaxValue(vec2(dims.x - 1, dims.y - 1));
    
	auto mesh = std::make_shared<BasicMesh>();
	std::vector<BasicMesh::Vertex> vertices;

    if (propSeedMode.get() == 0)
    {
        auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);
        // Draw start point
        vec2 startPoint = propStartPoint.get();
        vertices.push_back({ vec3(startPoint.x / (dims.x - 1), startPoint.y / (dims.y - 1), 0),
            vec3(0), vec3(0), vec4(0, 0, 0, 1) });
        indexBufferPoints->add(static_cast<std::uint32_t>(0));
        // TODO: Create one stream line from the given start point
    }
    else
    {
        // TODO: Seed multiple stream lines either randomly or using a uniform grid

        // (TODO: Bonus, sample randomly according to magnitude of the vector field)
    }

	mesh->addVertices(vertices);
	outMesh.setData(mesh);
}

} // namespace

