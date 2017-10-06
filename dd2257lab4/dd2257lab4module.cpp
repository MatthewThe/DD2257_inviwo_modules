/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Friday, September 29, 2017 - 15:50:00
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab4/dd2257lab4module.h>
#include <dd2257lab4/licprocessor.h>
#include <dd2257lab4/noisetexturegenerator.h>

namespace inviwo
{

DD2257Lab4Module::DD2257Lab4Module(InviwoApplication* app) : InviwoModule(app, "DD2257Lab4")
{
	registerProcessor<LICProcessor>();
	registerProcessor<NoiseTextureGenerator>();
}

} // namespace
