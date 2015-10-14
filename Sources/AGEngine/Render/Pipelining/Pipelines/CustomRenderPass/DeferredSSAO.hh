#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm/glm.hpp>

#define KERNEL_SIZE 16

namespace AGE
{
    class Texture2D;
    class Program;

    class DeferredSSAO : public FrameBufferRender
    {
    public:
        DeferredSSAO(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager);
        virtual ~DeferredSSAO() = default;

    protected:
        virtual void renderPass(const DRBCameraDrawableList &infos);

        bool _is_initialized;
        glm::vec3 *_kernels;
        Key<Vertices> _quadVertices;
        std::shared_ptr<Painter> _quadPainter;
    };
}
