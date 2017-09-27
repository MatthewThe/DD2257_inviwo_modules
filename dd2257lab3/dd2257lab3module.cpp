/*********************************************************************
 *  Author  : Himangshu Saikia, Wiebke Koepp
 *  Init    : Tuesday, September 19, 2017 - 15:07:48
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab3/dd2257lab3module.h>
#include <dd2257lab3/eulerrk4comparison.h>
#include <dd2257lab3/streamlineintegrator.h>
#include <dd2257lab3/utils/vectorfieldgenerator2d.h>
#include <modules/opengl/shader/shadermanager.h>

namespace inviwo
{

DD2257Lab3Module::DD2257Lab3Module(InviwoApplication* app) : InviwoModule(app, "DD2257Lab3")
{
    // Add a directory to the search path of the Shadermanager
    ShaderManager::getPtr()->addShaderSearchPath(getPath(ModulePath::GLSL));

    // Register processors
	registerProcessor<EulerRK4Comparison>();
	registerProcessor<StreamlineIntegrator>();
	registerProcessor<VectorFieldGenerator>();
}

} // namespace
