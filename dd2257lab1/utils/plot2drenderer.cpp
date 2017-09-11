/*********************************************************************************
*
* Inviwo - Interactive Visualization Workshop
*
* Copyright (c) 2015-2017 Inviwo Foundation
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*********************************************************************************/

#include <dd2257lab1/utils/plot2drenderer.h>

#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/meshdrawerfactory.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/rendering/meshdrawergl.h>
#include <modules/opengl/shader/shader.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/shader/shaderutils.h>
#include <modules/opengl/openglutils.h>
#include <inviwo/core/datastructures/coordinatetransformer.h>

namespace inviwo
{

    // The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
    const ProcessorInfo Plot2DRenderer::processorInfo_
    {
        "org.inviwo.Plot2DRendererDD2257",      // Class identifier
        "2D Plot Renderer",                     // Display name
        "DD2257",                               // Category
        CodeState::Experimental,                // Code state
        Tags::None,                             // Tags
    };

    const ProcessorInfo Plot2DRenderer::getProcessorInfo() const
    {
        return processorInfo_;
    }

    Plot2DRenderer::Plot2DRenderer()
        : Processor()
        , inport_("inputMesh")
        , imageInport_("imageInport")
        , outport_("outputImage")
        , shader_("plot2drenderer.vert", "plot2drenderer.frag")
        , enableDepthTest_("enableDepthTest", "Enable Depth Test", true)
        , pointSize_("pointSize", "PointSize", 5.0, 1.0, 20.0)
        , top_("top", "Top", 1, -5, 5)
        , bottom_("bottom", "Bottom", 0, -5, 5)
        , left_("left", "Left", 0, -5, 5)
        , right_("right", "Right", 1, -5, 5)
    {
        addPort(inport_);
        addPort(imageInport_);
        addPort(outport_);

        imageInport_.setOptional(true);

        //addProperty(enableDepthTest_);

        addProperty(left_);
        addProperty(right_);
        addProperty(bottom_);
        addProperty(top_);
        addProperty(pointSize_);

        inport_.onChange(this, &Plot2DRenderer::updateDrawers);
    }

    Plot2DRenderer::~Plot2DRenderer() {}

    void Plot2DRenderer::process()
    {
        if (imageInport_.isConnected())
        {
            utilgl::activateTargetAndCopySource(outport_, imageInport_, ImageType::ColorDepth);
        }
        else
        {
            utilgl::activateAndClearTarget(outport_, ImageType::ColorDepth);
        }
        shader_.activate();

        mat4 proj = glm::ortho(left_.get(), right_.get(), bottom_.get(), top_.get(), -200.0f, 100.0f);
        shader_.setUniform("projectionMatrix", proj);
        utilgl::GlBoolState depthTest(GL_DEPTH_TEST, enableDepthTest_);
        utilgl::GlBoolState nvPointSize(GL_VERTEX_PROGRAM_POINT_SIZE_NV, true);
        utilgl::GlBoolState pointSprite(GL_POINT_SPRITE, true);

        glPointSize(pointSize_.get());

        for (auto& drawer : drawers_)
        {
            utilgl::setShaderUniforms(shader_, *(drawer.second->getMesh()), "geometry_");
            drawer.second->draw();
        }

        shader_.deactivate();
        utilgl::deactivateCurrentTarget();
    }

    void Plot2DRenderer::updateDrawers()
    {
        auto changed = inport_.getChangedOutports();
        DrawerMap temp;
        std::swap(temp, drawers_);

        std::map<const Outport*, std::vector<std::shared_ptr<const Mesh>>> data;
        for (auto& elem : inport_.getSourceVectorData())
        {
            data[elem.first].push_back(elem.second);
        }

        for (auto elem : data)
        {
            auto ibegin = temp.lower_bound(elem.first);
            auto iend = temp.upper_bound(elem.first);

            if (util::contains(changed, elem.first) || ibegin == temp.end() ||
                static_cast<long>(elem.second.size()) !=
                std::distance(ibegin, iend))
            {  // data is changed or new.

                for (auto geo : elem.second)
                {
                    auto factory = getNetwork()->getApplication()->getMeshDrawerFactory();
                    if (auto renderer = factory->create(geo.get()))
                    {
                        drawers_.emplace(std::make_pair(elem.first, std::move(renderer)));
                    }
                }
            }
            else
            {  // reuse the old data.
                drawers_.insert(std::make_move_iterator(ibegin), std::make_move_iterator(iend));
            }
        }
    }

}  // namespace