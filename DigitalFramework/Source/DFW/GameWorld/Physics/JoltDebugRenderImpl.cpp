#include <DFW/GameWorld/Physics/JoltDebugRenderImpl.h>

#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>

#include <unordered_map>

namespace DFW
{
    void JoltDebugRenderer::Init()
    {
        JPH::DebugRenderer::Initialize();
    }

    void JoltDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor)
    {
        DebugDrawEncoder* dde = DebugRenderSystem::_dde;
        dde->setColor(inColor.GetUInt32());
        dde->moveTo(inFrom.GetX(), inFrom.GetY(), inFrom.GetZ());
        dde->lineTo(inTo.GetX(), inTo.GetY(), inTo.GetZ());
        dde->pop();
    }

    void JoltDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow /*inCastShadow*/)
    {
        DebugDrawEncoder* dde = DebugRenderSystem::_dde;
        std::array<DdVertex, 3> vertices;
        vertices[0].x = inV1.GetX();
        vertices[0].y = inV1.GetY();
        vertices[0].z = inV1.GetZ();

        vertices[1].x = inV1.GetX();
        vertices[1].y = inV1.GetY();
        vertices[1].z = inV1.GetZ();

        vertices[2].x = inV1.GetX();
        vertices[2].y = inV1.GetY();
        vertices[2].z = inV1.GetZ();

        dde->setColor(inColor.GetUInt32());
        dde->drawTriList(3, vertices.data());
    }

    JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch(const Triangle* inTriangles, int inTriangleCount)
    {
        BatchImpl* batch = new BatchImpl();

        if (inTriangles == nullptr || inTriangleCount == 0)
            return batch;

        std::unordered_map<JPH::Float3, int32> unique_vertices_and_index;
        unique_vertices_and_index.reserve(inTriangleCount * 2); // Use inTriangleCount to guestimate the amount of unique vertices that might be used.
        int32 index_counter(0);
        for (int32 triangle_index(0); triangle_index < inTriangleCount; triangle_index++)
        {
            Triangle const& triangle = inTriangles[triangle_index];
            {
                auto const [it, emplaced] = unique_vertices_and_index.try_emplace(triangle.mV[0].mPosition);
                if (emplaced)
                    it->second = index_counter++;
            }
            {
                auto const [it, emplaced] = unique_vertices_and_index.try_emplace(triangle.mV[1].mPosition);
                if (emplaced)
                    it->second = index_counter++;
            }
            {
                auto const [it, emplaced] = unique_vertices_and_index.try_emplace(triangle.mV[2].mPosition);
                if (emplaced)
                    it->second = index_counter++;
            }
        }
        
        std::vector<int32> indicies;
        int32 constexpr indicies_per_triangle(3);
        indicies.reserve(inTriangleCount * indicies_per_triangle);
        for (int32 triangle_index(0); triangle_index < inTriangleCount; triangle_index++)
        {
            Triangle const& triangle = inTriangles[triangle_index];

            auto const& [vertex1, index1] = *unique_vertices_and_index.find(triangle.mV[0].mPosition);
            auto const& [vertex2, index2] = *unique_vertices_and_index.find(triangle.mV[1].mPosition);
            auto const& [vertex3, index3] = *unique_vertices_and_index.find(triangle.mV[2].mPosition);

            indicies.emplace_back(index1);
            indicies.emplace_back(index2);
            indicies.emplace_back(index3);
        }

        std::vector<DdVertex> vertices;
        vertices.reserve(unique_vertices_and_index.size());
        for (auto const& [vertex, index] : unique_vertices_and_index)
        {
            vertices.emplace_back(vertex.x, vertex.y, vertex.z);
        }

        batch->geometry_handle = ddCreateGeometry(vertices.size(), vertices.data(), indicies.size(), indicies.data(), true);
        return batch;
    }

    JPH::DebugRenderer::Batch JoltDebugRenderer::CreateTriangleBatch(const Vertex* inVertices, int inVertexCount, const uint32* inIndices, int inIndexCount)
    {
        BatchImpl* batch = new BatchImpl();

        if (inVertices == nullptr || inVertexCount == 0 || inIndices == nullptr || inIndexCount == 0)
            return batch;

        std::vector<DdVertex> vertices;
        vertices.reserve(inVertexCount);
        for (int32 index(0); index < inVertexCount; index++)
        {
            Vertex const& v = inVertices[index];
            vertices.emplace_back(v.mPosition.x, v.mPosition.y, v.mPosition.z);
        }

        batch->geometry_handle = ddCreateGeometry(vertices.size(), vertices.data(), inIndexCount, inIndices, true);
        return batch;        
    }

    void JoltDebugRenderer::DrawGeometry(JPH::RMat44Arg inModelMatrix, const JPH::AABox& inWorldSpaceBounds, float inLODScaleSq, JPH::ColorArg inModelColor, const GeometryRef& inGeometry, ECullMode inCullMode, ECastShadow inCastShadow, EDrawMode inDrawMode)
    {
        BatchImpl* batch = static_cast<BatchImpl*>(inGeometry->mLODs[0].mTriangleBatch.GetPtr());
        
        DebugDrawEncoder* dde = DebugRenderSystem::_dde;
        dde->setTransform(&inModelMatrix);
        dde->setColor(inModelColor.GetUInt32());
        dde->setWireframe(inDrawMode == EDrawMode::Wireframe ? true : false);
        dde->setState(true, false, inCullMode == ECullMode::CullFrontFace ? true : false);

        dde->draw(batch->geometry_handle);
    }

    void  JoltDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view& inString, JPH::ColorArg inColor, float inHeight)
    {
        // No implementation as of right now.
    }

} // End of namespace ~ DFW.
