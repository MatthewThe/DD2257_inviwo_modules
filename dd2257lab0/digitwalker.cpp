/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Tuesday, September 05, 2017 - 23:46:16
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab0/digitwalker.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo DigitWalker::processorInfo_
{
    "org.inviwo.DigitWalker",      // Class identifier
    "Digit Walker",                // Display name
    "Undefined",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo DigitWalker::getProcessorInfo() const
{
    return processorInfo_;
}


DigitWalker::DigitWalker()
    :Processor()
    , outMesh("outMesh")
	, startPoint("start", "Starting Point", vec2(0.5, 0.5), vec2(0), vec2(1))
	, propMaxDigits("maxDigits", "Maximum Number of Digits", 3, 1, 150)
	, propLength("length", "Segment Length", 0.5, 0.01, 1, 0.01)
{
    // Register Ports

    addPort(outMesh);

    // Register Properties
	addProperty(startPoint);
	addProperty(propMaxDigits);
	addProperty(propLength);
}


void DigitWalker::process()
{
    auto mesh = std::make_shared<BasicMesh>();

	std::vector<BasicMesh::Vertex> vertices;

	auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

	vec3 start(startPoint.get(), 0.0f);
	int counter = 0;
	while (counter != propMaxDigits.get()) 
	{
		int digit = digitsPi[counter] - '0';

		vertices.push_back({ start, vec3(0), vec3(0), colors[digit] });
		indexBuffer->add(static_cast<std::uint32_t>(2 * counter));

		start = start + propLength.get() * directions[digit];
		vertices.push_back({ start, vec3(0), vec3(0), colors[digit] });
		indexBuffer->add(static_cast<std::uint32_t>(2 * counter + 1));
		counter++;
	}

	mesh->addVertices(vertices);

    // Push the mesh out
    outMesh.setData(mesh);
}

} // namespace

