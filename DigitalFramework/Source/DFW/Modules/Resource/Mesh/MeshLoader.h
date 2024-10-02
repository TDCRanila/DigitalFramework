#pragma once

#include <DFW/Modules/Rendering/MeshData.h>

#include <DFW/CoreSystems/Memory.h>

namespace DFW
{
    namespace DResource
    {
        UniquePtr<DRender::MeshData> LoadMesh(FilePath const& a_filepath);

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
