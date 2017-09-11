/*********************************************************************
 *  Author  : Wiebke Koepp
 *  Init    : Wednesday, August 30, 2017 - 18:01:26
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab1/dd2257lab1module.h>
#include <dd2257lab1/utils/csvsource.h>
#include <dd2257lab1/parallelcoordinates.h>
#include <dd2257lab1/scatterplot.h>
#include <dd2257lab1/generate2ddata.h>
#include <dd2257lab1/utils/plot2drenderer.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo
{

DD2257Lab1Module::DD2257Lab1Module(InviwoApplication* app) : InviwoModule(app, "DD2257Lab1")
{
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register objects that can be shared with the rest of inviwo here:
    
    // Processors
    registerProcessor<CSVSource>();
    registerProcessor<ParallelCoordinates>();
    registerProcessor<ScatterPlot>();
    registerProcessor<Generate2DData>();
    registerProcessor<Plot2DRenderer>();

    // Properties
    // registerProperty<DD2257Lab1Property>();
    
    // Readers and writes
    // registerDataReader(util::make_unique<DD2257Lab1Reader>());
    // registerDataWriter(util::make_unique<DD2257Lab1Writer>());
    
    // Data converters
    // registerRepresentationConverter(util::make_unique<DD2257Lab1Disk2RAMConverter>());

    // Ports
    // registerPort<DD2257Lab1Outport>("DD2257Lab1Outport");
    // registerPort<DD2257Lab1Inport>("DD2257Lab1Inport");

    // PropertyWidgets
    // registerPropertyWidget<DD2257Lab1PropertyWidget, DD2257Lab1Property>("Default");
    
    // Dialogs
    // registerDialog<DD2257Lab1Dialog>(DD2257Lab1Outport);
    
    // Other varius things
    // registerCapabilities(util::make_unique<DD2257Lab1Capabilities>());
    // registerSettings(util::make_unique<DD2257Lab1Settings>());
    // registerMetaData(util::make_unique<DD2257Lab1MetaData>());   
    // registerPortInspector("DD2257Lab1Outport", "path/workspace.inv");
    // registerProcessorWidget(std::string processorClassName, std::unique_ptr<ProcessorWidget> processorWidget);
    // registerDrawer(util::make_unique_ptr<DD2257Lab1Drawer>());  
}

} // namespace
