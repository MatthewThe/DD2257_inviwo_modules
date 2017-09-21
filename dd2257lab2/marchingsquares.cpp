/*********************************************************************
 *  Author  : Himangshu Saikia, Wiebke Koepp, ...
 *  Init    : Monday, September 11, 2017 - 12:58:42
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab2/marchingsquares.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/util/utilities.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo MarchingSquares::processorInfo_
{
    "org.inviwo.MarchingSquares",      // Class identifier
    "Marching Squares",                // Display name
    "DD2257",                          // Category
    CodeState::Experimental,           // Code state
    Tags::None,                        // Tags
};

const ProcessorInfo MarchingSquares::getProcessorInfo() const
{
    return processorInfo_;
}


MarchingSquares::MarchingSquares()
	:Processor()
	, inData("volumeIn")
	, meshOut("meshOut")
	, propShowGrid("showGrid", "Show Grid")
	, propDeciderType("deciderType", "Decider Type")
    , propMultiple("multiple", "Iso Levels")
	, propIsoValue("isovalue", "Iso Value")
    , propGridColor("gridColor", "Grid Lines Color", vec4(0.0f, 0.0f, 0.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
    , propIsoColor("isoColor", "Color", vec4(0.0f, 0.0f, 1.0f, 1.0f),
        vec4(0.0f), vec4(1.0f), vec4(0.1f),
        InvalidationLevel::InvalidOutput, PropertySemantics::Color)
	, propNumContours("numContours", "Number of Contours", 1, 1, 50, 1)
    , propIsoTransferFunc("isoTransferFunc", "Colors", &inData)
{
    // Register ports
	addPort(inData);
	addPort(meshOut);
	
    // Register properties
	addProperty(propShowGrid);
    addProperty(propGridColor);
	
	addProperty(propDeciderType);
	propDeciderType.addOption("midpoint", "Mid Point", 0);
	propDeciderType.addOption("asymptotic", "Asymptotic", 1);

	addProperty(propMultiple);
    
    propMultiple.addOption("single", "Single", 0);
    addProperty(propIsoValue);
    addProperty(propIsoColor);

	propMultiple.addOption("multiple", "Multiple", 1);
	addProperty(propNumContours);
    addProperty(propIsoTransferFunc);

    // The default transfer function has just two blue points
    propIsoTransferFunc.get().clearPoints();
    propIsoTransferFunc.get().addPoint(vec2(0.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
    propIsoTransferFunc.get().addPoint(vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f));
    propIsoTransferFunc.setCurrentStateAsDefault();

    util::hide(propGridColor, propNumContours, propIsoTransferFunc);

    // Show the grid color property only if grid is actually displayed
    propShowGrid.onChange([this]()
    {
        if (propShowGrid.get())
        {
            util::show(propGridColor);
        }
        else
        {
            util::hide(propGridColor);
        }
    });

    // Show options based on display of one or multiple iso contours
    propMultiple.onChange([this]()
    {
        if (propMultiple.get() == 0)
        {
            util::show(propIsoValue, propIsoColor);
            util::hide(propNumContours, propIsoTransferFunc);
        }
        else
        {
            //util::hide(propIsoValue);
            //util::show(propIsoColor, propNumContours);
            // TODO (Bonus): Comment out above if you are using the transfer function
            // and comment in below instead
            util::hide(propIsoValue, propIsoColor);
            util::show(propNumContours, propIsoTransferFunc);
        }
    });

}

double MarchingSquares::getInputValue(const VolumeRAM* data, size3_t dims, size_t x, size_t y)
{
    if (x < dims.x && y < dims.y)
    {
        return data->getAsDouble(size3_t(x, y, 0));
    }
    else
    {
        LogProcessorError("Attempting to access data outside the boundaries of the volume, value is set to 0");
        return 0;
    }
}

void MarchingSquares::process()
{
	if (!inData.hasData()) {
		return;
	}

    // This results in a shared pointer to a volume
	auto vol = inData.getData();

    // Extract the minimum and maximum value from the input data
    const double minValue = vol->dataMap_.valueRange[0];
    const double maxValue = vol->dataMap_.valueRange[1];

    // Set the range for the isovalue to that minimum and maximum
    propIsoValue.setMinValue(minValue);
	propIsoValue.setMaxValue(maxValue);

    // You can print to the Inviwo console with Log-commands:
    LogProcessorInfo("This scalar field contains values between " << minValue << " and " << maxValue << ".");
    // You can also inform about errors and warnings:
    // LogProcessorWarn("I am warning about something"); // Will print warning message in yellow
    // LogProcessorError("I am letting you know about an error"); // Will print error message in red
    // (There is also LogNetwork...() and just Log...(), these display a different source,
    // LogProcessor...() for example displays the name of the processor in the workspace while
    // Log...() displays the identifier of the processor (thus with multiple processors of the
    // same kind you would not know which one the information is coming from
    
    // Retreive data in a form that we can access it
    const VolumeRAM* vr = vol->getRepresentation< VolumeRAM >();
    const size3_t dims = vol->getDimensions();

    // Initialize mesh and vertices
	auto mesh = std::make_shared<BasicMesh>();
	std::vector<BasicMesh::Vertex> vertices;

    auto indexBufferLines =
        mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);
	
    // Values within the input data are accessed by the function below
    // It's input is the VolumeRAM from above, the dimensions of the volume
    // and the x- and y- index of the position to be accessed where
    // x is in [0, dims.x-1] and y is in [0, dims.y-1]
    float valueat00 = getInputValue(vr, dims, 0, 0);
    LogProcessorInfo("Value at (0,0) is: " << valueat00);
	LogProcessorInfo("Value at (" << dims.x-1 << "," << dims.y-1 << ") is: " << getInputValue(vr, dims, dims.x-1, dims.y-1));
    // You can assume that dims.z = 1 and do not need to consider others cases

    // Grid
	
	float padding = 0.05f;
    if (propShowGrid.get())
    {
        // TODO: Add grid lines of the given color
    }

    // Iso contours

    if (propMultiple.get() == 0)
    {
        // TODO: Draw a single isoline at the specified isovalue (propIsoValue) 
        // and color it with the specified color (propIsoColor)
		
		auto transformX = [dims, padding](auto x) { return padding + static_cast<float>(x)/(dims.x-1)*(1.0 - 2*padding); };
		auto transformY = [dims, padding](auto y) { return padding + static_cast<float>(y)/(dims.y-1)*(1.0 - 2*padding); };
		
		auto isoValue = propIsoValue.get();
		for (auto c = 0ul; c < dims.x - 1; ++c) {
			for (auto r = 0ul; r < dims.y - 1; ++r) {
				float bottomLeft = getInputValue(vr, dims, c, r);
				float bottomRight = getInputValue(vr, dims, c+1, r);
				float topLeft = getInputValue(vr, dims, c, r+1);
				float topRight = getInputValue(vr, dims, c+1, r+1);
				
				std::vector<BasicMesh::Vertex> localVertices;
				float intersection = getIsoIntersection(bottomLeft, bottomRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + intersection), transformY(r), 0), vec3(0), vec3(0.2f, 0.5f, 0), propIsoColor.get() });
				}
				
				intersection = getIsoIntersection(bottomLeft, topLeft, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c), transformY(r+intersection), 0), vec3(0), vec3(0.2f, 0.5f, 0), propIsoColor.get() });
				}
				
				intersection = getIsoIntersection(bottomRight, topRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + 1), transformY(r+intersection), 0), vec3(0), vec3(0.2f, 0.5f, 0), propIsoColor.get() });
				}
				
				intersection = getIsoIntersection(topLeft, topRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + intersection), transformY(r+1), 0), vec3(0), vec3(0.2f, 0.5f, 0), propIsoColor.get() });
				}
				
				std::sort(localVertices.begin(), localVertices.end(), 
				    [](const BasicMesh::Vertex & a, const BasicMesh::Vertex & b) -> bool
				{ 
				    return a.pos.x < b.pos.x; 
				});
				
				//LogProcessorInfo("IV: " << isoValue << " BL: " << bottomLeft << " BR: " << bottomRight << " TL: " << topLeft << " TR: " << topRight);
				//LogProcessorInfo("Number of local vertices: " << localVertices.size());
				for (auto vertex : localVertices) {
					vertices.push_back(vertex);

					//LogProcessorInfo("Vertex (" << vertex.pos.x << "," << vertex.pos.y << ")");
					indexBufferLines->add(static_cast<std::uint32_t>(vertices.size()-1));
				}
			}
		}
    }
    else
    {
		
        // TODO: Draw a the given number (propNumContours) of isolines between 
        // the minimum and maximum value
        // Hint: If the number of contours to be drawn is 1, the iso value for
        // that contour would be half way between maximum and minimum
        
        // TODO (Bonus): Use the transfer function property to assign a color
        // The transfer function normalizes the input data and sampling colors
        // from the transfer function assumes normalized input, that means
        // vec4 color = propIsoTransferFunc.get().sample(0.0f);
        // is the color for the minimum value in the data
        // vec4 color = propIsoTransferFunc.get().sample(1.0f);
        // is the color for the maximum value in the data
		auto transformX = [dims, padding](auto x) { return padding + static_cast<float>(x) / (dims.x - 1)*(1.0 - 2 * padding); };
		auto transformY = [dims, padding](auto y) { return padding + static_cast<float>(y) / (dims.y - 1)*(1.0 - 2 * padding); };

		auto isoValue = propIsoValue.get();
		auto isoStepSize = (maxValue - minValue )/ (propNumContours.get()+1);
		auto color = propIsoTransferFunc.get();
		for (auto n = 1ul; n <= propNumContours.get(); n++){
			isoValue = minValue+ (isoStepSize * n);
			
			auto color1 = color.sample((isoValue-minValue)/(maxValue-minValue));

		for (auto c = 0ul; c < dims.x - 1; ++c) {
			for (auto r = 0ul; r < dims.y - 1; ++r) {
				float bottomLeft = getInputValue(vr, dims, c, r);
				float bottomRight = getInputValue(vr, dims, c + 1, r);
				float topLeft = getInputValue(vr, dims, c, r + 1);
				float topRight = getInputValue(vr, dims, c + 1, r + 1);

				std::vector<BasicMesh::Vertex> localVertices;
				float intersection = getIsoIntersection(bottomLeft, bottomRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + intersection), transformY(r), 0), vec3(0), vec3(0.2f, 0.5f, 0), color1 });
				}

				intersection = getIsoIntersection(bottomLeft, topLeft, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c), transformY(r + intersection), 0), vec3(0), vec3(0.2f, 0.5f, 0),color1 });
				}

				intersection = getIsoIntersection(bottomRight, topRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + 1), transformY(r + intersection), 0), vec3(0), vec3(0.2f, 0.5f, 0), color1 });
				}

				intersection = getIsoIntersection(topLeft, topRight, isoValue);
				if (intersection >= 0) {
					localVertices.push_back({ vec3(transformX(c + intersection), transformY(r + 1), 0), vec3(0), vec3(0.2f, 0.5f, 0), color1 });
				}

				std::sort(localVertices.begin(), localVertices.end(),
					[](const BasicMesh::Vertex & a, const BasicMesh::Vertex & b) -> bool
				{
					return a.pos.x < b.pos.x;
				});

				//LogProcessorInfo("IV: " << isoValue << " BL: " << bottomLeft << " BR: " << bottomRight << " TL: " << topLeft << " TR: " << topRight);
				//LogProcessorInfo("Number of local vertices: " << localVertices.size());
				for (auto vertex : localVertices) {
					vertices.push_back(vertex);

					//LogProcessorInfo("Vertex (" << vertex.pos.x << "," << vertex.pos.y << ")");
					indexBufferLines->add(static_cast<std::uint32_t>(vertices.size() - 1));
				}
			}
		}
		}
    }

    // Note: It is possible to add multiple index buffers to the same mesh,
    // thus you could for example add one for the grid lines and one for
    // each isoline
    // Also, consider to write helper functions to avoid code duplication
    // e.g. for the computation of a single iso contour

	mesh->addVertices(vertices);
	meshOut.setData(mesh);
}

float MarchingSquares::getIsoIntersection(float v1, float v2, float iso) {
	if (std::min(v1, v2) <= iso && iso < std::max(v1, v2)) {
		auto diff0 = std::abs(v1 - iso);
		auto diff1 = std::abs(v2 - iso);
		if (diff0 + diff1 != 0) { // handle the special case that the iso line is on an edge
			//LogProcessorInfo("Intersection: " << diff0 / (diff0 + diff1) << " v1 " << v1 << " v2 " << v2);
			return diff0 / (diff0 + diff1);
		}
	}
	return -1.0f;
}

} // namespace
