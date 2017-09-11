/*********************************************************************
 *  Author  : Wiebke 
 *  Init    : Wednesday, August 30, 2017 - 18:01:26
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab1/dd2257lab1moduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo
{

class IVW_MODULE_DD2257LAB1_API DD2257Lab1Module : public InviwoModule
{
public:
    DD2257Lab1Module(InviwoApplication* app);
    virtual ~DD2257Lab1Module() = default;
};

} // namespace
