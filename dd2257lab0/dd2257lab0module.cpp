/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Tuesday, September 05, 2017 - 23:45:01
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <dd2257lab0/dd2257lab0module.h>
#include <dd2257lab0/digitwalker.h>

namespace inviwo
{

DD2257Lab0Module::DD2257Lab0Module(InviwoApplication* app) : InviwoModule(app, "DD2257Lab0")
{   
    // Processors
    registerProcessor<DigitWalker>();
}

} // namespace
