/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Thursday, September 07, 2017 - 08:09:49
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include "utils/shading.glsl"


in vec3 texCoord_;
in vec4 color_;

void main() {
    FragData0 = color_;
}
