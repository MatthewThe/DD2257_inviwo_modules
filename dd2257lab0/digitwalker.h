/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Tuesday, September 05, 2017 - 23:46:16
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#pragma once

#include <dd2257lab0/dd2257lab0moduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/meshport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperty.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo
{

/** \docpage{org.inviwo.DigitWalker, Digit Walker}
    ![](org.inviwo.DigitWalker.png?classIdentifier=org.inviwo.DigitWalker)

    Visualization of the digits of the floating point number,
    inspired by Nadieh Bremer
    https://www.visualcinnamon.com/portfolio/the-art-in-pi
    
    ### Outports
      * __Out Mesh__ Mesh where each segment represents a digit
    
    ### Properties
      * __Max Digits__ maximum number of digits to visualize
      * __Stop After__ stop showing digits after a certain pattern appears
*/

class IVW_MODULE_DD2257LAB0_API DigitWalker : public Processor
{ 
//Friends
//Types
public:
    enum class DigitsType
    {
        Pi,
        E,
        Phi
    };


//Construction / Deconstruction
public:
    DigitWalker();
    virtual ~DigitWalker() = default;

//Methods
public:
    virtual const ProcessorInfo getProcessorInfo() const override;
    static const ProcessorInfo processorInfo_;

protected:
    ///Our main computation function
    virtual void process() override;

//Ports
public:
    MeshOutport outMesh;

//Properties
public:
	FloatVec2Property startPoint;

	// After how many digits to we stop
	IntSizeTProperty propMaxDigits;

	// How long are the segments for each digit
	FloatProperty propLength;

//Attributes
private:
    const std::string digitsPi =  "141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128"; 
    const std::string digitsE =   "718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427427466391932003059921817413596629043572900334295260"; 
    const std::string digitsPhi = "618033988749894848204586834365638117720309179805762862135448622705260462818902449707207204189391137484754088075386891752126633862223536931793180060766";
    const std::vector<vec4> colors{
        vec4(0.937255f, 0.713725f, 0.019608f, 1.0f),
        vec4(0.925490f, 0.603922f, 0.054902f, 1.0f),
        vec4(0.913725f, 0.239216f, 0.156863f, 1.0f),
        vec4(0.819608f, 0.0f, 0.239216f, 1.0f),
        vec4(0.682353f, 0.019608f, 0.376471f, 1.0f),
        vec4(0.529412f, 0.180392f, 0.537255f, 1.0f),
        vec4(0.309804f, 0.325490f, 0.666667f, 1.0f),
        vec4(0.066667f, 0.505882f, 0.584314f, 1.0f),
        vec4(0.133333f, 0.682353f, 0.435294f, 1.0f),
        vec4(0.494118f, 0.721569f, 0.321569f, 1.0f),
    };
    const std::vector<vec3> directions{
        vec3(cosf(M_PI / 2),sinf(M_PI / 2), 0.0f),
        vec3(cosf(M_PI / 2 - 2 * M_PI / 10), sinf(M_PI / 2 - 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 2 * 2 * M_PI / 10), sinf(M_PI / 2 - 2 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 3 * 2 * M_PI / 10), sinf(M_PI / 2 - 3 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 4 * 2 * M_PI / 10), sinf(M_PI / 2 - 4 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 5 * 2 * M_PI / 10), sinf(M_PI / 2 - 5 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 6 * 2 * M_PI / 10), sinf(M_PI / 2 - 6 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 7 * 2 * M_PI / 10), sinf(M_PI / 2 - 7 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 8 * 2 * M_PI / 10), sinf(M_PI / 2 - 8 * 2 * M_PI / 10), 0.0f),
        vec3(cosf(M_PI / 2 - 9 * 2 * M_PI / 10), sinf(M_PI / 2 - 9 * 2 * M_PI / 10), 0.0f)
    };
};

} // namespace
