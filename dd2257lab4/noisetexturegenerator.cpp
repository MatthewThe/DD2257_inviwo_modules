/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Monday, October 02, 2017 - 13:31:36
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab4/noisetexturegenerator.h>
#include <inviwo/core/datastructures/image/layerram.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo NoiseTextureGenerator::processorInfo_
{
    "org.inviwo.NoiseTextureGenerator",      // Class identifier
    "Noise Texture Generator",                // Display name
    "DD2257",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo NoiseTextureGenerator::getProcessorInfo() const
{
    return processorInfo_;
}


NoiseTextureGenerator::NoiseTextureGenerator()
    :Processor()
	, texOut_("texOut")
    , texSize_("texSize", "Texture Size", vec2(512, 512), vec2(1, 1), vec2(2048, 2048), vec2(1, 1))
    , randomSeed_("randSeed", "Random seed",  1, 0, 10000)
    , blackWhiteOrGrayscale_("grayOrBw", "Texture Type")
    // TODO: Register additional properties
{
    //Register ports
	addPort(texOut_);

    //Register properties
	addProperty(texSize_);

    // TODO: Register additional properties
    blackWhiteOrGrayscale_.addOption("gray", "Grayscale", 0);
    blackWhiteOrGrayscale_.addOption("bw", "Black and white", 1);
    addProperty(blackWhiteOrGrayscale_);
    addProperty(randomSeed_);
}


void NoiseTextureGenerator::process()
{
    // The output of the generation process is an Image
	auto outImage = std::make_shared<Image>();
    
    // In Inviwo, images consist of multiple layers, but only the
    // color layer is relevant for us
	auto outLayer = outImage->getColorLayer();
	
    outLayer->setDimensions(size2_t(texSize_.get().x, texSize_.get().y));
    // With the data format DataVec4UInt8 values for RGB-alpha range between 0 and 255
	outLayer->setDataFormat(DataVec4UInt8::get());

    // Just like we did with the volume in other assignments we need to retrieve an editable
    // representation of the object we want to modify (here a layer) 
	auto lr = outLayer->getEditableRepresentation<LayerRAM>();
	
	srand(randomSeed_);
	for (int j = 0; j < texSize_.get().y; j++) {
		for (int i = 0; i < texSize_.get().x; i++) {
			
			int val = rand() % 256;
			// TODO: Randomly sample values for the texture
			if (blackWhiteOrGrayscale_.get() == 1) {
				if (val > 127) {
					val = 255;
				} else {
					val = 0;
				}
			}
			
			// A value within the ouput image is set by specifying pixel position and color
			lr->setFromDVec4(size2_t(i, j), dvec4(val, val, val, 255));
		}
	}

	texOut_.setData(outImage);
}

} // namespace

