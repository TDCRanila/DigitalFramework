#pragma once

// systems of a component type

// when updating, systems requests its components from the storage manager. 
// Storage managers gives a pointer to the storage, containg the components
// System can then do its thing.
//
//namespace DECS {
//
//    class ECSystem {
//    public:
//        ECSystem();
//        virtual ~ECSystem();
//
//        virtual void Init();
//        virtual void Terminate();
//
//        virtual void OnEntry();
//        virtual void OnUpdate() = 0;
//        virtual void OnExit();
//
//    };
//
//} // End of namespace ~ DECS