#pragma once

#include <Modules/ECS/Objects/ECSystem.h>

#include <Modules/Editor/EditorElementContainer.h>

#include <unordered_map>

namespace DFW
{
	namespace DEditor
	{
		class EditorSystem final : public DECS::System::Registrar<EditorSystem>
		{
		public:
			EditorSystem() = default;
			~EditorSystem() = default;

			EditorElementContainer element_container;

		private:
			virtual void Init() override;
			virtual void Terminate() override;

			virtual void Update(DECS::Universe& a_universe) override;
			virtual void UpdateSystemImGui(DECS::Universe& a_universe) override;

		};

	} // End of namespace ~ DFW

} // End of namespace ~ DFW.
