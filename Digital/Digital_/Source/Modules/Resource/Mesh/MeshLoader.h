#pragma once

#include <Modules/Rendering/MeshData.h>

#include <CoreSystems/Memory.h>

namespace DFW
{
    namespace DResource
    {
        UniquePtr<DRender::MeshData> LoadMesh(std::string const& a_filepath);

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
