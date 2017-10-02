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
    , propSteps("steps", "Steps",  50, 0, 10000)
	, propStepsize("stepsize", "Stepsize", 0.2f, 0.0f)
	, propIntegrationDirection("integrationDirection", "Integration Direction")
	, propStreamLineColor("streamLineColor", "Stream Lines Color", vec4(0.0f, 0.0f, 0.0f, 1.0f),
			vec4(0.0f), vec4(1.0f), vec4(0.1f),
			InvalidationLevel::InvalidOutput, PropertySemantics::Color)
	, directionField("directionField", "Direction Field", false)
    , numSeeds("numSeeds", "Num Seeds",  50, 0, 10000)
    , seedPlacement("seedPlacement", "Seed Placement Strategy")
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
    seedPlacement.addOption("random", "Random", 0);
    seedPlacement.addOption("uniform", "Uniform", 1);
    seedPlacement.addOption("magnitude", "Magnitude", 2);
    addProperty(seedPlacement);
	addProperty(numSeeds);
    
    propIntegrationDirection.addOption("forward", "Forward", 1);
    propIntegrationDirection.addOption("backward", "Backward", -1);
    addProperty(propIntegrationDirection);
    addProperty(propSteps);
	addProperty(propStepsize);
	addProperty(propStreamLineColor);
	
	addProperty(directionField);
	
    // Show properties for a single seed and hide properties for multiple seeds (TODO)
    propSeedMode.onChange([this]()
    {
        if (propSeedMode.get() == 0)
        {
            util::show(propStartPoint, mouseMoveStart);
            util::hide(numSeeds, seedPlacement);
        }
        else
        {
            util::hide(propStartPoint, mouseMoveStart);
            util::show(numSeeds, seedPlacement);
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
        //auto indexBufferPoints = mesh->addIndexBuffer(DrawType::Points, ConnectivityType::None);
        auto indexBufferRK = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
        
        // Draw start point
        vec2 startPoint = propStartPoint.get();
                
        // TODO: Create one stream line from the given start point
        drawSingleStreamLine(startPoint, vr, dims, vertices, indexBufferRK);
    }
    else
    {
        // TODO: Seed multiple stream lines either randomly or using a uniform grid
        srand(1);
        std::vector<vec2> seeds;
        auto getRandFloat = []() { return static_cast<double>(rand()) / RAND_MAX; };
        if (seedPlacement.get() == 0) // random seeding
        {
            for (int i = 0; i < numSeeds.get(); ++i)
            {
                seeds.push_back(vec2(getRandFloat()*(dims.x - 1), getRandFloat()*(dims.y - 1)));
                //LogProcessorInfo("Random seed at (" << seeds.back().x << ","  << seeds.back().y << ")")
            }
        } 
        else if (seedPlacement.get() == 1) // uniform seeding
        {
        
        }
        else if (seedPlacement.get() == 2) // magnitude based seeding
        {
            // (TODO: Bonus, sample randomly according to magnitude of the vector field)
			// makegrid, calc values for each cell save to vector
			// normalize,  sum of all cell values save 
			// prob is rand * sum, while greater then v(i) + commulativa; next one? 
			int numofcellsX = 20;
			int numofcellsY = 20;
			std::vector<float> cellValues;
			std::vector<float> cellseeds;
			float xCellLenght = static_cast<float>(dims.x) / numofcellsX;
			float YCellLenght = static_cast<float>(dims.y) / numofcellsY;

			
			//LogProcessorInfo(xCellLenght);

			//making cells and getting mean lenght values
			float buffer1 = 0;
			float buffer2 = 0;
			float buffer3 = 0;
			float buffer4 = 0;
			float sum = 0;
			float buffertMean = 0;

			for (int i = 0; i < numofcellsX; i++) {
				for (int j = 0; j < numofcellsY; j++) {

					buffer1 = glm::length(Integrator::sampleFromField(vr, dims, vec2(i*xCellLenght, j*YCellLenght)));
					buffer2 = glm::length(Integrator::sampleFromField(vr, dims, vec2((i + 1)*xCellLenght, j*YCellLenght)));
					buffer3 = glm::length(Integrator::sampleFromField(vr, dims, vec2(i*xCellLenght, (j + 1)*YCellLenght)));
					buffer4 = glm::length(Integrator::sampleFromField(vr, dims, vec2((i + 1) *xCellLenght, (j + 1)*YCellLenght)));

					buffertMean = (buffer1 + buffer2 + buffer3 + buffer4) / 4;
					cellValues.push_back(buffertMean);
					sum = sum + buffertMean;
					//LogProcessorInfo(sum);
					cellseeds.push_back(0);


				}
			}
			// assign seeds randomly with weights to cells
			float randSeed = 0;
			float cumulative = 0;
			for (int i = 0; i < numSeeds.get(); ++i) {
				
				cumulative = 0;
				randSeed = getRandFloat()*sum;

				for (int c = 0; c < numofcellsX*numofcellsY; c++) {
					if (randSeed <= (cellValues[c]+ cumulative)) {
						cellseeds[c] += 1;
						LogProcessorInfo(c);
						break;
					}
					else {
						cumulative += cellValues[c];
					}


				}

			}
			//set seeds depending on cell 
			float bufferX = 0;
			float bufferY = 0;

			for (int c = 0; c < numofcellsX*numofcellsY; c++) {
				bufferY = c % numofcellsX;
				bufferX = (c- bufferY) / numofcellsX;
				LogProcessorInfo(bufferX <<" "<< bufferY);
				for (int s = 0; s < cellseeds[c];s++){
					//LogProcessorInfo(vec2((bufferX*xCellLenght) + getRandFloat()*xCellLenght, (bufferY*YCellLenght) + getRandFloat()*YCellLenght));
					seeds.push_back(vec2((bufferX*xCellLenght) +getRandFloat()*xCellLenght, (bufferY*YCellLenght) + getRandFloat()*YCellLenght));
				}
			}


        }
        
        for (auto seed : seeds) {
            auto indexBufferRK = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::Strip);
            drawSingleStreamLine(seed, vr, dims, vertices, indexBufferRK);
        }
        
    }

	mesh->addVertices(vertices);
	outMesh.setData(mesh);
}

void StreamlineIntegrator::drawSingleStreamLine(vec2 startPoint, const VolumeRAM* vr, vec3 dims, 
        std::vector<BasicMesh::Vertex>& vertices, IndexBufferRAM *indexBufferLine) {
    vertices.push_back({ vec3(startPoint.x / (dims.x - 1), startPoint.y / (dims.y - 1), 0),
            vec3(0), vec3(0), propStreamLineColor.get() });
    //indexBufferPoints->add(static_cast<std::uint32_t>(0));        
    indexBufferLine->add(static_cast<std::uint32_t>(vertices.size() - 1));
    
    vec2 buffertVec2 = startPoint;
    for (uint32_t i = 0; i < propSteps.get(); i++)
    {
	    //calculation
	    buffertVec2 = Integrator::RK4(vr, dims, buffertVec2, propIntegrationDirection.get() * propStepsize.get(), directionField.get());

	    //drawing
	    vertices.push_back({ vec3(buffertVec2.x / (dims.x - 1), buffertVec2.y / (dims.y - 1), 0),
		    vec3(0), vec3(0), propStreamLineColor.get() });

	    indexBufferLine->add(static_cast<std::uint32_t>(vertices.size() - 1));
	    //indexBufferPoints->add(static_cast<std::uint32_t>(vertices.size() - 1));
    }
}

} // namespace

