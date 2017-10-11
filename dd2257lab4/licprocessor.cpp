/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Monday, October 02, 2017 - 13:31:17
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab4/licprocessor.h>
#include <dd2257lab4/integrator.h>
// #include <dd2257lab3/integrator.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo LICProcessor::processorInfo_
{
    "org.inviwo.LICProcessor",      // Class identifier
    "LICProcessor",                // Display name
    "DD2257",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo LICProcessor::getProcessorInfo() const
{
    return processorInfo_;
}


LICProcessor::LICProcessor()
    :Processor()
	, volumeIn_("volIn")
	, noiseTexIn_("noiseTexIn")
	, licOut_("licOut")
	, licType_("licType", "LIC type")
	, kernelSize_("kernelLength", "Kernel Length", 50, 10, 100)
	//, kernelLength_(50) /* symmetric around base pixel, i.e. the full kernel is of size (2*kernelLength_+1) */
    // TODO: Register properties
{
    // Register ports
	
	addPort(volumeIn_);
	addPort(noiseTexIn_);
	addPort(licOut_);

	// TODO: Register properties
	licType_.addOption("fast", "fast LIC", 0);
    licType_.addOption("base", "base LIC", 1);
    addProperty(licType_);
	addProperty(kernelSize_);
}


void LICProcessor::process()
{
	LogProcessorInfo("Process started");
	// Get input
	if (!volumeIn_.hasData()) {
		return;
	}

	if (!noiseTexIn_.hasData()) {
		return;
	}
	
	auto vol = volumeIn_.getData();
	vectorFieldDims_ = vol->getDimensions();
	auto vr = vol->getRepresentation<VolumeRAM>();
	
	LogProcessorInfo("Vector field dims: " << vectorFieldDims_);
	
	// An accessible form of on image is retrieved analogous to a volume
	auto tex = noiseTexIn_.getData();
	texDims_ = tex->getDimensions();
	auto tr = tex->getRepresentation<ImageRAM>();
	
	LogProcessorInfo("Texture field dims: " << texDims_);

	// Prepare the output, it has the same dimensions and datatype as the output
	// and an editable version is retrieved analogous to a volume
	auto outImage = tex->clone();
	auto outLayer = outImage->getColorLayer();
	auto lr = outLayer->getEditableRepresentation<LayerRAM>();

	// TODO: Implement LIC and FastLIC
	int kernelLength_ = kernelSize_.get();
	// This code instead sets all pixels to the same gray value
	std::vector<std::vector<double> > licTexture;
	if (licType_.get() == 0) {
		licTexture = fastLic(vr, tr, kernelLength_);
	} else {
		licTexture = slowLic(vr, tr, kernelLength_);
	}
	
	for (auto j = 0u; j < texDims_.y; j++)
	{
		//LogProcessorInfo(j<< "/" <<texDims_.y);
		for (auto i = 0u; i < texDims_.x; i++)
		{
			int val = int(licTexture[i][j]);
			lr->setFromDVec4(size2_t(i, j), dvec4(val, val, val, 255));
		}

	}
	
	licOut_.setData(outImage);

}

std::vector<std::vector<double> > LICProcessor::slowLic(const VolumeRAM* vr, const ImageRAM* ir, int kernelLength_)
{
	//vector field 
	
	LogProcessorInfo("base lic started");
	float stepsize = 0.01; // for integrator
	uint32_t maxsteps = 10000; // for integrator
	std::vector<std::vector<double> > licTexture(texDims_.x, std::vector<double>(texDims_.y, 0));
	//choose smaller ratio for equidistant stepsize
	float xratio = static_cast<float>(texDims_.x) / (vectorFieldDims_.x - 1);
	float yratio = static_cast<float>(texDims_.y) / (vectorFieldDims_.y - 1);
	float minSizePixel = 0;
	if (xratio <= yratio) 
	{
		minSizePixel = xratio;
	}
	else 
	{
		minSizePixel = yratio;
	}
	//LogProcessorInfo("minSizePixel:"<< minSizePixel);
	
	 // kernel lenght = minSizePixel * kernelLength_

	
	for (auto pixelj = 0u; pixelj < texDims_.y; pixelj++)
	{
		//LogProcessorInfo(j<< "/" <<texDims_.y);
		for (auto pixeli = 0u; pixeli < texDims_.x; pixeli++)
		{
			/*make streamline forward and backward*/

			vec2 startPos = vec2(pixeli / minSizePixel, pixelj / minSizePixel);

			std::vector<vec2> streamlineForward = getStreamLine(vr, stepsize, maxsteps, kernelLength_, minSizePixel, startPos);
			std::vector<vec2> streamlineBackward = getStreamLine(vr, -1*stepsize, maxsteps, kernelLength_, minSizePixel, startPos);
		
			
			//equidistant point along the given streamline forward
			std::vector<vec2> equidistantPointsF = equidistantPos(vr, kernelLength_, minSizePixel, startPos, streamlineForward);
			std::vector<vec2> equidistantPointsB = equidistantPos(vr, kernelLength_, minSizePixel, startPos, streamlineBackward);
			
			dvec4 bufferpixel = dvec4(0,0,0,0);
			float sumKernel = 0;
			
			float bufferXpixel = 0;
			float bufferYpixel = 0;
			float denominator = 0;
			vec2 bufferPopVector = vec2(0, 0);
			
			// for each equidistant point, get pixel pos, get pixel value, plus to sum
			equidistantPointsF.insert(equidistantPointsF.end(), equidistantPointsB.begin(), equidistantPointsB.end());
			
			while (!equidistantPointsF.empty())
			{
				bufferPopVector = equidistantPointsF.back();
				equidistantPointsF.pop_back();
				bufferXpixel = floor(minSizePixel*bufferPopVector.x);
				bufferYpixel = floor(minSizePixel*bufferPopVector.y);
				if (bufferXpixel < 0 || bufferYpixel < 0 || bufferXpixel > (texDims_.x - 1) || bufferYpixel > (texDims_.y - 1))
				{
					//LogProcessorInfo("pixel outside" << bufferXpixel << " " << bufferYpixel);
				}
				else
				{
					//LogProcessorInfo("pixel" << bufferXpixel << " " << bufferYpixel);

					bufferpixel = ir->readPixel(size2_t(bufferXpixel, bufferYpixel), inviwo::LayerType('0'));
					//LogProcessorInfo("pixel" << bufferpixel);
					sumKernel += bufferpixel.x + bufferpixel.y + bufferpixel.z;
					denominator++;
				}
				

			}
			float pixelValue = sumKernel / (denominator*3);
			licTexture[pixeli][pixelj] = pixelValue;
			//LogProcessorInfo("pixel value" << pixelValue); 
			
		}
	}
	
	LogProcessorInfo("base lic finished");

	return licTexture;
}

std::vector<std::vector<double> > LICProcessor::fastLic(const VolumeRAM* vr, const ImageRAM* ir, int kernelLength_)
{
	//vector field 
	
	LogProcessorInfo("fast lic started");
	float stepsize = 0.01; // for integrator
	uint32_t maxsteps = 10000; // for integrator
	std::vector<std::vector<double> > licTexture(texDims_.x, std::vector<double>(texDims_.y, 0));
	//choose smaller ratio for equidistant stepsize
	float xratio = static_cast<float>(texDims_.x) / (vectorFieldDims_.x - 1);
	float yratio = static_cast<float>(texDims_.y) / (vectorFieldDims_.y - 1);
	float minSizePixel = 0; // 1.0 / (size of 1 pixel in vectorField units)
	if (xratio <= yratio) 
	{
		minSizePixel = xratio;
	}
	else 
	{
		minSizePixel = yratio;
	}
	//LogProcessorInfo("minSizePixel:"<< minSizePixel);
	
	uint32_t maxArcLength = texDims_.x * texDims_.y;
	std::vector<std::vector<bool> > visited(texDims_.x, std::vector<bool>(texDims_.y, false));
	
	 // kernel lenght = minSizePixel * kernelLength_

	size_t skipped = 0u;
	for (auto pixelj = 0u; pixelj < texDims_.y; pixelj++)
	{
		//LogProcessorInfo(j<< "/" <<texDims_.y);
		for (auto pixeli = 0u; pixeli < texDims_.x; pixeli++)
		{			
			/*make streamline forward and backward*/
			if (visited[pixeli][pixelj]) {
				++skipped;
				continue;
			}
			
			vec2 startPos = vec2(pixeli / minSizePixel, pixelj / minSizePixel);

			std::vector<vec2> streamlineForward = getStreamLine(vr, stepsize, maxsteps, maxArcLength, minSizePixel, startPos);
			std::vector<vec2> streamlineBackward = getStreamLine(vr, -1*stepsize, maxsteps, maxArcLength, minSizePixel, startPos);
			
			//LogProcessorInfo("1 backward, forward = " << streamlineBackward.size() << ", "  << streamlineForward.size());
			
			//equidistant point along the given streamline forward
			std::vector<vec2> equidistantPointsF = equidistantPos(vr, maxArcLength, minSizePixel, startPos, streamlineForward);
			std::vector<vec2> equidistantPointsB = equidistantPos(vr, maxArcLength, minSizePixel, startPos, streamlineBackward);
			
			//LogProcessorInfo("backward, forward = " << equidistantPointsB.size() << ", "  << equidistantPointsF.size());
			
			
			dvec4 bufferpixel = dvec4(0,0,0,0);
			float sumKernel = 0;
			
			float bufferXpixel = 0;
			float bufferYpixel = 0;
			vec2 bufferPopVector = vec2(0, 0);
			
			// for each equidistant point, get pixel pos, get pixel value, plus to sum
			std::reverse(equidistantPointsB.begin(), equidistantPointsB.end());
			equidistantPointsB.insert(equidistantPointsB.end(), equidistantPointsF.begin(), equidistantPointsF.end());
			//LogProcessorInfo("total = " << equidistantPointsB.size());
			
			std::list<float> kernelValues;
			size_t kernelIndex = 0u;
			for (size_t index = 0u; index < equidistantPointsB.size(); ++index)
			{
				vec2 currentPos = equidistantPointsB.at(index);
				while (kernelIndex - index < kernelLength_ && kernelIndex < equidistantPointsB.size()) {
					bufferPopVector = equidistantPointsB.at(kernelIndex++);
					bufferXpixel = floor(minSizePixel*bufferPopVector.x);
					bufferYpixel = floor(minSizePixel*bufferPopVector.y);
					if (bufferXpixel < 0 || bufferYpixel < 0 || bufferXpixel > (texDims_.x - 1) || bufferYpixel > (texDims_.y - 1))
					{
						//LogProcessorInfo("pixel outside" << bufferXpixel << " " << bufferYpixel);
					}
					else
					{
						//LogProcessorInfo("pixel" << bufferXpixel << " " << bufferYpixel);

						bufferpixel = ir->readPixel(size2_t(bufferXpixel, bufferYpixel), inviwo::LayerType('0'));
						//LogProcessorInfo("pixel" << bufferpixel);
						float bufferPixelValue = bufferpixel.x + bufferpixel.y + bufferpixel.z;
						kernelValues.push_back(bufferPixelValue);
						
						sumKernel += bufferPixelValue;
					}
				}
				
				if (kernelValues.size() >= kernelLength_ * 2 || kernelIndex == equidistantPointsB.size() - 1) {
					sumKernel -= kernelValues.front();
					kernelValues.pop_front();
				}
				
				float currentXpixel = minSizePixel * currentPos.x;
				float currentYpixel = minSizePixel * currentPos.y;
				
				size_t currentXpixelInt = std::min(static_cast<size_t>(std::floor(currentXpixel)), texDims_.x - 1);
				size_t currentYpixelInt = std::min(static_cast<size_t>(std::floor(currentYpixel)), texDims_.y - 1);
				
				
				//LogProcessorInfo(minSizePixel << " "  << currentPos << " " << vectorFieldDims_ << " " << currentXpixel << " "  << currentYpixel);
				
				float pixelValue = sumKernel / (kernelValues.size() * 3);
				licTexture.at(currentXpixelInt).at(currentYpixelInt) = pixelValue;
				//LogProcessorInfo("visited " << currentXpixelInt << " "  << currentYpixelInt);
				visited.at(currentXpixelInt).at(currentYpixelInt) = true;

			}			
			//LogProcessorInfo("pixel value" << pixelValue);
		}
	}
	
	LogProcessorInfo("fast lic finished; could skip " << skipped << "/" << texDims_.x*texDims_.y << " pixels" );
	
	return licTexture;
}

std::vector<vec2> LICProcessor::getStreamLine(const VolumeRAM* vr, float stepsize, uint32_t maxstep, uint32_t maxPoints, float minSizePixel, vec2 startpos)
{
	vec2 buffertvector1 = vec2(startpos.x, startpos.y);
	vec2 buffertvector2 = vec2(startpos.x, startpos.y);
	float cumulativeDistance = 0.0f;
	uint32_t stepCounter = 0u;

	std::vector<vec2> streamvector;
	
	streamvector.push_back(startpos);

	//Forward streamline
	while (cumulativeDistance < maxPoints / minSizePixel && stepCounter++ < maxstep)
	{
		//integration step and store pos
		buffertvector2 = buffertvector1;
		buffertvector1 = Integrator::RK4(vr, vectorFieldDims_, buffertvector1, stepsize, false);
		float distance = glm::length(buffertvector1 - buffertvector2);
		if (distance == 0)
		{
			//LogProcessorInfo("no change " << Integrator::sampleFromField(vr, vectorFieldDims_, buffertvector1));
			// no change, give up 
			break;
		} else if (buffertvector1.x < 0 || buffertvector1.x > vectorFieldDims_.x 
						|| buffertvector1.y < 0 || buffertvector1.y > vectorFieldDims_.y )
		{
			// outside of canvas, give up
			//LogProcessorInfo("outside canvas");
			break;
		}
		streamvector.push_back(buffertvector1);
		cumulativeDistance += distance;
	}
	//LogProcessorInfo("integrated point: " << buffertvector1);
	return streamvector;
}

std::vector<vec2> LICProcessor::equidistantPos(const VolumeRAM * vr, uint32_t maxPoints, float minSizePixel, vec2 startpos, std::vector<vec2> inputPos)
{
	std::vector<vec2> equidistantPoints;
	vec2 bufferpos = 1.0f*startpos;
	float bufferLength = 0;
	float equidistance = 1.0 / minSizePixel;
	equidistantPoints.push_back(startpos);
	for (auto currentPos : inputPos) {
		float bufferLineSegmentLength = glm::length(bufferpos - currentPos);
		while (bufferLength + bufferLineSegmentLength > equidistance * equidistantPoints.size()) {
			float a = equidistance * equidistantPoints.size() - bufferLength;
			equidistantPoints.push_back(bufferpos + (a / bufferLineSegmentLength)*(currentPos - bufferpos));
			if (equidistantPoints.size() == maxPoints) break;
		}
		if (equidistantPoints.size() == maxPoints) break;
		bufferLength += bufferLineSegmentLength;
		bufferpos = currentPos;
	}
	return equidistantPoints;
}


} // namespace

