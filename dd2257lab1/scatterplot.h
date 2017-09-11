/*********************************************************************
 *  Author  : Wiebke 
 *  Init    : Wednesday, August 30, 2017 - 18:02:35
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

/** \docpage{org.inviwo.ScatterPlot, Scatter Plot}
    ![](org.inviwo.ScatterPlot.png?classIdentifier=org.inviwo.ScatterPlot)

    Creates a point mesh and a line mesh that resemble the points and axes of a scatter plot.
    
    ### Inports
      * __data__ DataFrame containing a number columns where the first column contains indices 
      and subsequent columns contain values for each dimension. Thus each row corresponds to 
      one data point.
    
    ### Outports
      * __Point Mesh__ Mesh resembling the points in the scatter plot.
      * __Line Mesh__ Mesh resembling the axes in the scatter plot.
    
    ### Properties
      * __propColorPoint__ Color for points.
      * __propColorPoint__ Color for axes.
      * __propXAxis__ Currently chosen dimension (column) for the x axis.
      * __propYAxis__ Currently chosen dimension (column) for the y axis.
*/


class IVW_MODULE_DD2257LAB1_API ScatterPlot : public Processor
{ 
//Friends
//Types
public:

//Construction / Deconstruction
public:
    ScatterPlot();
    virtual ~ScatterPlot() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;  
    void updateAxisLabels();

//Ports
public:
    DataInport<DataFrame> inData;
    MeshOutport outMeshPoints;
    MeshOutport outMeshLines;

//Properties
public:
    // Color properties for points and axis
    FloatVec4Property propColorPoint;
    FloatVec4Property propColorAxes;
    // Properties capturing the currently chosen columns of the input data
    OptionPropertyInt propXAxis;
    OptionPropertyInt propYAxis;

//Attributes
private:

};

} // namespace
