#include <Modules/Rendering/ModelData.h>

namespace DFW
{
    namespace DRender
    {
        Primitive::Primitive()
            : start_vertices(0)
            , start_index(0)
            , num_vertices(0)
            , num_indices(0)
        {
        }

        SubModelData::SubModelData()
            : vertices(nullptr)
            , indices(nullptr)
            , num_vertices(0)
            , num_indices(0)  
        {
            vbh.idx = bgfx::kInvalidHandle;
            ibh.idx = bgfx::kInvalidHandle;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
