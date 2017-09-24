/*********************************************************************
 *  Author  : Himangshu Saikia, Wiebke Koepp, ...
 *  Init    : Monday, September 11, 2017 - 12:58:42
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab2/dd2257lab2moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo
{

/** \docpage{org.inviwo.MarchingSquares, Marching Squares}
    ![](org.inviwo.MarchingSquares.png?classIdentifier=org.inviwo.MarchingSquares)

    Extraction of isocontours in 2D with the marching squares algorithm.
    
    ### Inports
      * __data__ The input here is 2-dimensional scalar field (thus a single
      value within each voxel) but it is represented by a 3-dimensional volume. 
      This processor deals with 2-dimensional data only, therefore it is assumed 
      the z-dimension will have size 1 otherwise the 0th slice of the volume 
      will be processed
    
    ### Outports
      * __mesh__ The output mesh contains (possibly multiple) iso contours as well as gridlines
    
    ### Properties
      * __propShowGrid__ Display grid lines if true, do not display grid lines if false.
      * __propGridColor__ Color of the grid lines
      * __propDeciderType__ Type of decider for ambiguities in marching squares
      * __propMultiple__ Display of one iso contour or multiple
      * __propIsoValue__ Iso value for one iso contour
      * __propIsoColor__ Color for iso contour(s)
      * __propNumContours__ Number of isocontours to be displayed between minimum and maximum data value
      * __propIsoTransferFunc__ Transfer function to be used to color those multiple contours
*/
class IVW_MODULE_DD2257LAB2_API MarchingSquares : public Processor
{ 
//Friends
//Types
public:

//Construction / Deconstruction
public:
    MarchingSquares();
    virtual ~MarchingSquares() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;

    // (TODO: Helper functions can be defined here and then implemented in the .cpp)

    double getInputValue(const VolumeRAM* data, size3_t dims, size_t x, size_t y);
  	float getIsoIntersection(float v1, float v2, float iso);
	//std::vector<BasicMesh::Vertex> solveAmbiguity(float iso, float tl, float bl, float br, float tr, float * v, std::vector<BasicMesh::Vertex> gridIso);

//Ports
public:
    // Input data
	VolumeInport inData;

    // Output mesh
	MeshOutport meshOut;

//Properties
public:
    // Basic settings
    BoolProperty propShowGrid;
    FloatVec4Property propGridColor;
    TemplateOptionProperty<int> propDeciderType;
    TemplateOptionProperty<int> propMultiple;
    // Properties for choosing a single iso contour by value
    FloatProperty propIsoValue;
    FloatVec4Property propIsoColor;
    // Properties for multiple iso contours 
    IntProperty propNumContours;
    TransferFunctionProperty propIsoTransferFunc;

//Attributes
private:

};

} // namespace
