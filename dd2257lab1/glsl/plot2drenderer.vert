/*********************************************************************
 *  Author  : Wiebke Koepp, ...
 *  Init    : Thursday, September 07, 2017 - 08:09:49
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include "utils/structs.glsl"


uniform GeometryParameters geometry_;

uniform mat4 projectionMatrix;

out vec4 color_;
out vec3 texCoord_;
 
void main() {
    color_ = in_Color;
    texCoord_ = in_TexCoord;

    gl_Position = projectionMatrix * in_Vertex;
}
