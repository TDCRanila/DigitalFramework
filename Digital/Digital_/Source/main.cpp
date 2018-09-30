#include <iostream>

#include <ECS/ECSystem.h>
#include <ECS/ECSEntity.h>

#include <AutoFactory/AutoFactory.h>

#include <Defines/Defines.h>

class A : public DECS::ECSComponent {
public:
    A() {}
    ~A() {}

	float a = 5;

    void call() {
        std::cout << "calling" << std::endl;
    }
};

class B : public DECS::ECSComponent {
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

int main(int /*argc*/, char** /*argv*/) {

}