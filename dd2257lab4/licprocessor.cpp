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
    // TODO: Register properties
{
    // Register ports
	
	addPort(volumeIn_);
	addPort(noiseTexIn_);
	addPort(licOut_);

// TODO: Register properties
}


void LICProcessor::process()
{
	LogProcessorInfo("Proccess started");
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

	// An accessible form of on image is retrieved analogous to a volume
	auto tex = noiseTexIn_.getData();
	texDims_ = tex->getDimensions();
	auto tr = tex->getRepresentation<ImageRAM>();

	// Prepare the output, it has the same dimensions and datatype as the output
	// and an editable version is retrieved analogous to a volume
	auto outImage = tex->clone();
	auto outLayer = outImage->getColorLayer();
	auto lr = outLayer->getEditableRepresentation<LayerRAM>();

	// TODO: Implement LIC and FastLIC
	// This code instead sets all pixels to the same gray value
	std::vector<std::vector<double> > licTexture = slowLic(vr, vectorFieldDims_, tr, texDims_);
	
	for (auto j = 0; j < texDims_.y; j++)
	{
		//LogProcessorInfo(j<< "/" <<texDims_.y);
		for (auto i = 0; i < texDims_.x; i++)
		{
			
			
			int val = int(licTexture[i][j]);
			lr->setFromDVec4(size2_t(i, j), dvec4(val, val, val, 255));
		}

	}
	
	licOut_.setData(outImage);

}
std::vector<std::vector<double> > LICProcessor::slowLic(const VolumeRAM* vr, vec3 vectordims,const ImageRAM* ir, vec2 texDims)
{
	//vector field 
	
	//LogProcessorInfo("simple lic started");
	float stepsize = 0.01; // for integrator
	uint32_t maxsteps = 300; // for integrator
	std::vector<std::vector<double> > licTexture(texDims.x, std::vector<double>(texDims.y, 0));
	//choose smaller ratio for ekvidistant stepsize
	float xratio = texDims.x/ vectordims.x;
	float yratio = texDims.y/vectordims.y;
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
	
	 // kernel lenght = minSizePixel * kernelLenght
	uint32_t kernelLenght = 51; // num of eqvidistant steps 

	
	for (auto pixelj = 0; pixelj <(texDims.y-1); pixelj++)
	{
		//LogProcessorInfo(j<< "/" <<texDims_.y);
		for (auto pixeli = 0; pixeli < (texDims.x - 1); pixeli++)
		{
			/*make streamline forward and backward*/

			vec2 startPos = vec2(pixeli/ minSizePixel, pixelj/ minSizePixel);

			vec2 backwardVec2 = vec2(startPos.x, startPos.y);
			vec2 buffertVec2 = vec2(startPos.x, startPos.y);
			float forwardDistance = 0;
			float backwardDistance = 0;
			uint32_t Stepcounter = 0;

			std::vector<vec2> streamlineForward = LICProcessor::getStremlinePos(vr,  vectordims, stepsize,  maxsteps,  kernelLenght,minSizePixel, startPos);
			std::vector<vec2> streamlineBackward = LICProcessor::getStremlinePos(vr, vectordims, -1*stepsize, maxsteps, kernelLenght, minSizePixel, startPos);
		
			
			//ekvidistant point along the given streamline  forward
			std::vector<vec2> EkvidistantPointsF = LICProcessor::equidistantPos( vr, vectordims,  kernelLenght,  minSizePixel,  startPos, streamlineForward);
			std::vector<vec2> EkvidistantPointsB = LICProcessor::equidistantPos(vr, vectordims, kernelLenght, minSizePixel, startPos, streamlineBackward);
			
			dvec4 bufferpixel = dvec4(0,0,0,0);
			float sumKernel = 0;
			
			float bufferXpixel = 0;
			float bufferYpixel = 0;
			float denominator = 0;
			vec2 bufferPopVector = vec2(0, 0);
			
			// for each ekvidistantpoint, get pixel pos, get pixel value, plus to sum
			EkvidistantPointsF.insert(EkvidistantPointsF.end(), EkvidistantPointsB.begin(), EkvidistantPointsB.end());

			while (EkvidistantPointsF.empty() == false)
			{
				bufferPopVector = EkvidistantPointsF.back();
				EkvidistantPointsF.pop_back();
				bufferXpixel = floor(minSizePixel*bufferPopVector.x);
				bufferYpixel = floor(minSizePixel*bufferPopVector.y);
				if (bufferXpixel < 0 || bufferYpixel < 0 || bufferXpixel > 500 || bufferYpixel > 500)
				{
					//LogProcessorInfo("pixel outside" << bufferXpixel << " " << bufferYpixel);
				}
				else
				{
					//LogProcessorInfo("pixel" << bufferXpixel << " " << bufferYpixel);

					bufferpixel = ir->readPixel(size2_t(bufferXpixel, bufferYpixel), inviwo::LayerType('0'));
					//LogProcessorInfo("pixel" << bufferpixel);
					sumKernel = sumKernel + bufferpixel.x + bufferpixel.y + bufferpixel.z;
					denominator++;
				}
				

			}
			float pixelValue = sumKernel / (denominator*3);
			licTexture[pixeli][pixelj] = pixelValue;
			//LogProcessorInfo("pixel value" << pixelValue); 
			
		}
	}

	return licTexture;
}

std::vector<vec2> LICProcessor::getStremlinePos(const VolumeRAM* vr, vec3 vectordims, float stepsize, uint32_t maxstep, uint32_t kernelLenght, float minSizePixel,vec2 startpos)
{
	vec2 buffertvector1 = vec2(startpos.x, startpos.y);
	vec2 buffertvector2 = vec2(startpos.x, startpos.y);
	float cumulativDistance = 0;
	uint32_t Stepcounter = 0;

	std::vector<vec2> streamvector;
	
	streamvector.push_back(startpos);


	//Forward streamline
	while (cumulativDistance < kernelLenght / minSizePixel && Stepcounter < maxstep)
	{
		//integration step and store pos
		buffertvector2 = buffertvector1;
		buffertvector1 = Integrator::RK4(vr, vectordims, buffertvector1, stepsize, false);
		if (buffertvector2.x == buffertvector1.x && buffertvector2.y == buffertvector1.y)
		{
			//no change give up 
			break;
		}
		streamvector.push_back(buffertvector1);
		//distans calc
		float deltax = (buffertvector1.x - buffertvector2.x)*(buffertvector1.x - buffertvector2.x);
		float deltay = (buffertvector1.y - buffertvector2.y)*(buffertvector1.y - buffertvector2.y);
		cumulativDistance = cumulativDistance + sqrt(deltax + deltay);

		Stepcounter = Stepcounter + 1;

	}
	return streamvector;
}

std::vector<vec2> LICProcessor::equidistantPos(const VolumeRAM * vr, vec3 vectordims, uint32_t kernelLenght, float minSizePixel, vec2 startpos, std::vector<vec2> inputPos)
{
	std::vector<vec2> EkvidistantPoints;
	vec2 bufferpos = 1.0f*startpos;
	float bufferlenght = 0;
	float bufferLinesegmentLenght = 0;
	EkvidistantPoints.push_back(startpos);
	float a = 0;
	float m = 0;
	uint32_t index = 0;
	vec2 boundrycheck = vec2(0, 0);
	//LogProcessorInfo("started ekvi:" << backwardVec2);
	for (uint32_t i = 0; i < (kernelLenght - 1) / 2; i++)
	{

		while (bufferlenght < 1 / minSizePixel)
		{
			//LogProcessorInfo("bufferlenght" << bufferlenght)
			if ((bufferpos.x == boundrycheck.x && bufferpos.y == boundrycheck.y) || index >= (inputPos.size()))
			{
				//LogProcessorInfo("reached 0 0 " << bufferpos << (streamlineForward.size() - 1));
				EkvidistantPoints.push_back(inputPos[index + 1]);
				break;

			}
			//LogProcessorInfo("stuck in a loop with me " << bufferlenght)
			float deltax = (bufferpos.x - inputPos[index].x)*(bufferpos.x - inputPos[index].x);
			float deltay = (bufferpos.y - inputPos[index].y)*(bufferpos.y - inputPos[index].y);

			bufferLinesegmentLenght = sqrt(deltax + deltay);

			if (bufferlenght + bufferLinesegmentLenght <= 1 / minSizePixel)
			{
				// distans not reached on this intervall, check next points
				bufferlenght = bufferlenght + bufferLinesegmentLenght;
				bufferpos = inputPos[index];
				index = index + 1;
			}
			else
			{
				// distans in between points
				a = 1 / minSizePixel - bufferlenght;
				
				m = ( inputPos[index].x) / ( inputPos[index].y);

				if ((inputPos[index].y) == 0)
				{
					//only x
					bufferpos.x = bufferpos.x + a;
					bufferlenght = 1 / minSizePixel;

				}
				else if ( inputPos[index].x == 0)
				{
					//only y
					bufferpos.y = bufferpos.y + a;
					bufferlenght = 1 / minSizePixel;
				}
				else
				{
					float buffline2 = 0;
					buffline2 = sqrt(a / (1 + m*m));
					bufferpos = bufferpos + (a / bufferLinesegmentLenght)*(bufferpos - inputPos[index]);
					//LogProcessorInfo("calc new point with magic: "<<"value"<< bufferLinesegmentLenght)
					bufferlenght = 1 / minSizePixel;

				}

			}

		}
		/*
		LogProcessorInfo("bufferlenght" << bufferlenght)
		if ((bufferpos.x == boundrycheck.x && bufferpos.y == boundrycheck.y) || indexForward >= (streamlineForward.size() - 1))
		{
		LogProcessorInfo("reached 0 0 " << bufferpos << (streamlineForward.size() - 1));
		EkvidistantPoints.push_back(streamlineForward[indexForward + 1]);
		break;

		}
		*/
		EkvidistantPoints.push_back(bufferpos);
		//LogProcessorInfo("added point approx" << bufferpos);
		bufferlenght = 0;

	}
	return EkvidistantPoints;
}


} // namespace

