#include <Modules/Rendering/ModelData.h>

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

        MeshData::MeshData()
            : file_name("")
            , source_file("")
        {
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
