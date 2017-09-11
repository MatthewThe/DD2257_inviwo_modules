/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Tuesday, September 05, 2017 - 23:45:01
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab0/dd2257lab0moduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo
{

class IVW_MODULE_DD2257LAB0_API DD2257Lab0Module : public InviwoModule
{
public:
    DD2257Lab0Module(InviwoApplication* app);
    virtual ~DD2257Lab0Module() = default;
};

} // namespace
