/*********************************************************************
 *  Author  : Wiebke Koepp, ... 
 *  Init    : Friday, September 01, 2017 - 12:13:44
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
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/ports/dataoutport.h>
#include <dd2257lab1/utils/dataframe.h>

namespace inviwo
{

/** \docpage{org.inviwo.Generate2DData, Generate2DData}
    ![](org.inviwo.Generate2DData.png?classIdentifier=org.inviwo.Generate2DData)

    Generates datapoints of certain shape.
    
    ### Outports
      * __data__ resulting DataFrame containing three columns where the first column contains indices 
      and the second and third column contain the x and y coordinate for each data point. Thus each row
      corresponds to one data point.
    
    ### Properties
      * __Data Shape__ shape of data that is created such as a line, circle or hyperbola.
*/
class IVW_MODULE_DD2257LAB1_API Generate2DData : public Processor
{ 
//Friends
//Types
public:
    enum class DataShape
    {
        Line,
        Circle,
        Hyperbola
    };

//Construction / Deconstruction
public:
    Generate2DData();
    virtual ~Generate2DData() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;

//Ports
public:
    DataOutport<DataFrame> outDataFrame;

//Properties
public:
    // A property where we want to choose different modes, specified through an enum
    TemplateOptionProperty<DataShape> propdataShape;

//Attributes
private:

};

} // namespace
