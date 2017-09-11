/*********************************************************************
 *  Author  : Wiebke 
 *  Init    : Wednesday, August 30, 2017 - 18:02:18
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab1/dd2257lab1moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <dd2257lab1/utils/dataframe.h>
#include <inviwo/core/ports/meshport.h>

namespace inviwo
{

/** \docpage{org.inviwo.ParallelCoordinates, Parallel Coordinates}
    ![](org.inviwo.ParallelCoordinates.png?classIdentifier=org.inviwo.ParallelCoordinates)

    Creates two line meshes that resemble the axes and line segments of a parallel coordinates plot.

    ### Inports
    * __data__ DataFrame containing a number columns where the first column contains indices
    and subsequent columns contain values for each dimension. Thus each row corresponds to
    one data point.

    ### Outports
    * __Line Mesh__ Mesh resembling the line segment in the scatter plot.
    * __Axis Mesh__ Mesh resembling the axes in the scatter plot.

    ### Properties
    * __Point Color__ Color for points.
    * __Axes Color__ Color for axes.
*/
class IVW_MODULE_DD2257LAB1_API ParallelCoordinates : public Processor
{ 
//Friends
//Types
public:

//Construction / Deconstruction
public:
    ParallelCoordinates();
    virtual ~ParallelCoordinates() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;

//Ports
public:
    DataInport<DataFrame> inData;
    MeshOutport outMeshAxis;
    MeshOutport outMeshLines;

//Properties
public:
    FloatVec4Property propColorLines;
    FloatVec4Property propColorAxes;

//Attributes
private:

};

} // namespace
