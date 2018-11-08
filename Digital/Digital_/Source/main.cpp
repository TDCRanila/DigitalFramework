#include <iostream>

#include <ECS/Objects/ECSystem.h>
//#include <ECS/ECSEntity.h>

#include <AutoFactory/AutoFactory.h>

#include <Defines/Defines.h>

//class SisterSystem : public DECS::ECSystem::Registrar<SisterSystem> {
//public:
//	SisterSystem() {
//
//	}
//
//	~SisterSystem() {
//
//	}
//};
//
//class KappaSystem : public DECS::ECSystem::Registrar<KappaSystem> {
//public:
//	KappaSystem() {
//
//	}
//
//	~KappaSystem() {
//
//	}
//};

int main(int /*argc*/, char** /*argv*/) {

	auto factories = DECS::ECSystem::GetFactories();

	auto blop1 = DECS::ECSystem::Construct("KappaSystem");
	auto blop2 = DECS::ECSystem::Construct("SisterSystem");

}