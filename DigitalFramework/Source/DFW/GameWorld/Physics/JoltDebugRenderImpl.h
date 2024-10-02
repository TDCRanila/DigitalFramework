#pragma once

#include <Modules/Rendering/DebugDraw/debugdraw.h>

#include <Jolt/Jolt.h>
#include <Jolt/Renderer/DebugRenderer.h>

#include <atomic>

namespace DFW
{
    class JoltDebugRenderer final : public JPH::DebugRenderer
    {
    public:
        JoltDebugRenderer() = default;
        virtual ~JoltDebugRenderer() = default;

        void Init();

        virtual void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
        virtual void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow = ECastShadow::Off) override;
        virtual JPH::DebugRenderer::Batch CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount) override;
        virtual JPH::DebugRenderer::Batch CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const uint32* inIndices, int inIndexCount) override;
        virtual void DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode) override;
        virtual void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight) override;

    private:
        class BatchImpl : public JPH::RefTargetVirtual
        {
        public:
            BatchImpl() 
                : _ref_count(0)
                , geometry_handle() 
            {}

            GeometryHandle geometry_handle;

        private:
            virtual void AddRef() override { ++_ref_count; }
            virtual void Release() override { if (--_ref_count == 0) delete this; }

            std::atomic<uint32> _ref_count;
        };
    };

} // End of namespace ~ DFW.
