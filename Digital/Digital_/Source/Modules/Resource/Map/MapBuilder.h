#pragma once

#include <Modules/Resource/Map/MapData.h>

#include <CoreSystems/Memory.h>

#include <unordered_map>
#include <string>

// FW Declare.
struct LMEntity;
class LMMapData;

namespace DFW
{
    namespace DResource
    {
        class MapBuilder
        {
        public:
            MapBuilder() = default;
            ~MapBuilder() = default;

            UniquePtr<MapData> CreateMap(std::string const& a_filepath);

        private:
            bool LoadMap();
            void BuildMap();

            void ParseEntity(int32 const a_entity_id, LMEntity& a_entity);

            void BuildEntityVisualMesh(int32 const a_entity_id, LMEntity& a_entity);
            void BuildEntityCollisionMesh(int32 const a_entity_id, LMEntity& a_entity);

            void LoadAndCacheTextures();
            void SetupSurfaceUVCalculation();

        private:
            std::unordered_map<std::string, SharedPtr<DRender::TextureData>> _cached_map_textures;
            SharedPtr<LMMapData> _loaded_libmap_data;

            UniquePtr<MapData> _map;
            std::string _map_filepath;

        };

    } // End of namespace ~ DResource.

} // End of namespace ~ DFW.
