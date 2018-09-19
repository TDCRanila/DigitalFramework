#include <iostream>

#include <ECS/ECSKeyLockSystem.h>

#include <AutoFactory/AutoFactory.h>

#include <Defines/Defines.h>

class A {
public:
    A() {}
    ~A() {}

	float a = 5;

    void call() {
        std::cout << "calling" << std::endl;
    }
};

class B {
public:
	B() {}
	~B() {}

	float a = 5;

	void call() {
		std::cout << "calling" << std::endl;
	}
};

REGISTER_TYPE(A, ComponentListName)
REGISTER_TYPE(B, ComponentListName)

int main(int argc, char** argv) {

	auto a = DFactory::GetFactories().size();

	auto vec = DFactory::GetFactories();

	auto map = DFactory::GetFactoryList(ComponentListName);
	auto item = DFactory::GetFactoryOfType<A>(ComponentListName);
	auto object = DFactory::ConstructFromFactory<A>(ComponentListName);
	 
	for (auto const&[type, fac] : *map) {
		std::cout << type.name();
	}

	//DECS::ECSKeyLockSystem sys;
	//sys.GenerateComponentKeys();

    system("PAUSE");			

    //DECS::ECSStorageManager man;

    //float* new_float = new float();
    //*new_float = 1;
    //man.Subscribe<float>(new_float);
    //man.Subscribe<float>(new_float);

    //A* new_a = nullptr;
    //for (int i = 0; i < 10; ++i) {
    //    new_a = new A();
    //    man.Subscribe<A>(new_a);
    //}

    //auto test1 = man.GetComponentVector<float>();
    //auto test2 = man.GetComponentVector<A>();
    //auto test3 = man.GetComponentVector<bool>();

    //for (auto t : test1->component_vector) {
    //    *t = 5;
    //}

    //for (auto t : test2->component_vector) {
    //    t->call();
    //}
}