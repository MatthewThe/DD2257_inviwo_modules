/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Wednesday, September 20, 2017 - 12:04:15
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab3/dd2257lab3moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo
{

class IVW_MODULE_DD2257LAB3_API Integrator
{ 
//Friends
//Types
public:

//Construction / Deconstruction
public:
    Integrator();
    virtual ~Integrator() = default;

//Methods
public:
    static vec2 sampleFromField(const VolumeRAM* vr, size3_t dims, const vec2& position);

    // TODO: Implement the methods below (one integration step with either Euler or 
    // Runge-Kutte of 4th order integration method)
    // Pass any other properties that influence the integration process
    static vec2 RK4(const VolumeRAM* vr, size3_t dims, const vec2& position, const float stepsize, bool normalized = false);
    static vec2 Euler(const VolumeRAM* vr, size3_t dims, const vec2& position, const float stepsize, bool normalized = false);

};

} // namespace
