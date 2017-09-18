/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Monday, September 11, 2017 - 12:57:56
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab2/dd2257lab2module.h>
#include <dd2257lab2/marchingsquares.h>
#include <dd2257lab2/utils/amirameshvolumereader.h>

namespace inviwo
{

DD2257Lab2Module::DD2257Lab2Module(InviwoApplication* app) : InviwoModule(app, "DD2257Lab2")
    
{
	registerProcessor<MarchingSquares>();
	registerDataReader(util::make_unique<AmiraMeshVolumeReader>());
}

} // namespace
