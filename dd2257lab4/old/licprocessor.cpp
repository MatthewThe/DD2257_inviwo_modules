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
	// integrate streamline forward until half of kernel size
	// integrate stremline backwards unbtil half of the kernel size
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
			vec2 forwardVec2 = vec2(startPos.x, startPos.y);
			vec2 backwardVec2 = vec2(startPos.x, startPos.y);
			vec2 buffertVec2 = vec2(startPos.x, startPos.y);
			float forwardDistance = 0;
			float backwardDistance = 0;
			uint32_t Stepcounter = 0;

			std::vector<vec2> streamlineForward;
			std::vector<vec2> streamlineBackward;
			streamlineForward.push_back(startPos);
			streamlineBackward.push_back(startPos);

			//Forward streamline
			while (forwardDistance < kernelLenght / minSizePixel && Stepcounter < maxsteps)
			{
				//integration step and store pos
				buffertVec2 = forwardVec2;
				forwardVec2 = Integrator::RK4(vr, vectordims, forwardVec2, stepsize, false);
				streamlineForward.push_back(forwardVec2);
				//distans calc
				float deltax = (buffertVec2.x - forwardVec2.x)*(buffertVec2.x - forwardVec2.x);
				float deltay = (buffertVec2.y - forwardVec2.y)*(buffertVec2.y - forwardVec2.y);
				forwardDistance = forwardDistance + sqrt(deltax + deltay);

				Stepcounter = Stepcounter + 1;
				
			}
			
			//backward streamline
			Stepcounter = 0;
			
			while (backwardDistance < kernelLenght / minSizePixel && Stepcounter < maxsteps)
			{
				//integration step and store pos
				buffertVec2 = backwardVec2;
				backwardVec2 = Integrator::RK4(vr, vectordims, backwardVec2, -stepsize, false);
				streamlineBackward.push_back(backwardVec2);
				//distans calc
				float deltax = (buffertVec2.x - backwardVec2.x)*(buffertVec2.x - backwardVec2.x);
				float deltay = (buffertVec2.y - backwardVec2.y)*(buffertVec2.y - backwardVec2.y);
				backwardDistance = backwardDistance + sqrt(deltax + deltay);

				Stepcounter = Stepcounter + 1;
				//LogProcessorInfo("added backward:" << backwardVec2);
			}

			//ekvidistant point along the given streamline  forward
			std::vector<vec2> EkvidistantPoints;
			vec2 bufferpos = 1.0f*startPos;
			float bufferlenght = 0;
			float bufferLinesegmentLenght = 0;
			EkvidistantPoints.push_back(startPos);
			float a = 0;
			float b = 0;
			float c = 0;
			float m = 0;
			uint32_t indexForward = 0;
			vec2 boundrycheck = vec2(0, 0);
			//LogProcessorInfo("started ekvi:" << backwardVec2);
			for (uint32_t i = 0; i < (kernelLenght - 1) / 2; i++)
			{

				while (bufferlenght < 1/minSizePixel)
				{
					//LogProcessorInfo("bufferlenght" << bufferlenght)
						if ((bufferpos.x == boundrycheck.x && bufferpos.y == boundrycheck.y) || indexForward >= (streamlineForward.size() ))
						{
							//LogProcessorInfo("reached 0 0 " << bufferpos << (streamlineForward.size() - 1));
							EkvidistantPoints.push_back(streamlineForward[indexForward + 1]);
							break;

						}
						//LogProcessorInfo("stuck in a loop with me " << bufferlenght)
					float deltax = (bufferpos.x - streamlineForward[indexForward].x)*(bufferpos.x - streamlineForward[indexForward].x);
					float deltay = (bufferpos.y - streamlineForward[indexForward].y)*(bufferpos.y - streamlineForward[indexForward].y);

					bufferLinesegmentLenght = sqrt(deltax + deltay);

					if (bufferlenght + bufferLinesegmentLenght <= 1/minSizePixel)
					{
						// distans not reached on this intervall, check next points
					bufferlenght = bufferlenght + bufferLinesegmentLenght;
					bufferpos = streamlineForward[indexForward];
					indexForward = indexForward + 1;
					}
					else
					{
						// distans in between points
						a = 1/minSizePixel - bufferlenght;
						b = 0;
						c = 0;
						m = (b - streamlineForward[indexForward].x) / (c - streamlineForward[indexForward].y);

						if ((c - streamlineForward[indexForward].y) == 0)
						{
							//only x
							bufferpos.x = bufferpos.x + a;
							bufferlenght = 1 / minSizePixel;

						}
						else if ((b - streamlineForward[indexForward].x) == 0)
						{
							//only y
							bufferpos.y = bufferpos.y + a;
							bufferlenght = 1 / minSizePixel;
						}else
						{
							float buffline2 = 0;
							buffline2 = sqrt(a / (1 + m*m));
							bufferpos = bufferpos+ (a/bufferLinesegmentLenght)*(bufferpos- streamlineForward[indexForward]);
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
			//backward
			
			 bufferpos = 1.0f*startPos;
			 bufferlenght = 0;
			 bufferLinesegmentLenght = 0;
			 a = 0;
			 b = 0;
			 c = 0;
			 m = 0;
			 indexForward = 0;
			 boundrycheck = vec2(0, 0);
			//LogProcessorInfo("started ekvi:" << backwardVec2);
			for (uint32_t i = 0; i < (kernelLenght - 1) / 2; i++)
			{

				while (bufferlenght < 1 / minSizePixel)
				{
					//LogProcessorInfo("bufferlenght" << bufferlenght)
					if ((bufferpos.x == boundrycheck.x && bufferpos.y == boundrycheck.y) || indexForward >= (streamlineBackward.size()))
					{
						//LogProcessorInfo("reached 0 0 " << bufferpos << (streamlineForward.size() - 1));
						EkvidistantPoints.push_back(streamlineBackward[indexForward + 1]);
						break;

					}
					//LogProcessorInfo("stuck in a loop with me " << bufferlenght)
					float deltax = (bufferpos.x - streamlineBackward[indexForward].x)*(bufferpos.x - streamlineBackward[indexForward].x);
					float deltay = (bufferpos.y - streamlineBackward[indexForward].y)*(bufferpos.y - streamlineBackward[indexForward].y);

					bufferLinesegmentLenght = sqrt(deltax + deltay);

					if (bufferlenght + bufferLinesegmentLenght <= 1 / minSizePixel)
					{
						// distans not reached on this intervall, check next points
						bufferlenght = bufferlenght + bufferLinesegmentLenght;
						bufferpos = streamlineBackward[indexForward];
						indexForward = indexForward + 1;
					}
					else
					{
						// distans in between points
						a = 1 / minSizePixel - bufferlenght;
						b = 0;
						c = 0;
						m = (b - streamlineBackward[indexForward].x) / (c - streamlineBackward[indexForward].y);

						if ((c - streamlineBackward[indexForward].y) == 0)
						{
							//only x
							bufferpos.x = bufferpos.x + a;
							bufferlenght = 1 / minSizePixel;

						}
						else if ((b - streamlineBackward[indexForward].x) == 0)
						{
							//only y
							bufferpos.y = bufferpos.y + a;
							bufferlenght = 1 / minSizePixel;
						}
						else
						{
							float buffline2 = 0;
							buffline2 = sqrt(a / (1 + m*m));
							bufferpos = bufferpos + (a / bufferLinesegmentLenght)*(bufferpos - streamlineBackward[indexForward]);
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
			dvec4 bufferpixel = dvec4(0,0,0,0);
			float sumKernel = 0;
			
			float bufferXpixel = 0;
			float bufferYpixel = 0;
			float denominator = 0;
			vec2 bufferPopVector = vec2(0, 0);
			
			// for each ekvidistantpoint, get pixel pos, get pixel value, plus to sum

			while (EkvidistantPoints.empty() == false)
			{
				bufferPopVector = EkvidistantPoints.back();
				EkvidistantPoints.pop_back();
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

std::vector<vec2> LICProcessor::getStremlinePos(const VolumeRAM* vr, vec3 vectordims, float stepsize, uint32_t maxstep, float distanse, uint32_t kernelLenght, float minSizePixel)
{

	return std::vector<vec2>();
}


} // namespace

