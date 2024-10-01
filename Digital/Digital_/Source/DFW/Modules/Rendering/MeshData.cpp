#include <Modules/Rendering/MeshData.h>

namespace DFW
{
    namespace DRender
    {
        SubMeshData::SubMeshData()
            : num_vertices(0)
            , num_faces(0)
        {
            vbh.idx = bgfx::kInvalidHandle;
            ibh.idx = bgfx::kInvalidHandle;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
