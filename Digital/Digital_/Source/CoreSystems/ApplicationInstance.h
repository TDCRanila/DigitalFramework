#pragma once

#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/WindowManagement.h>
#include <CoreSystems/InputManagement.h>

#include <string>

namespace DCore
{
    class ApplicationInstance
    {
    public:
        ApplicationInstance();
        virtual ~ApplicationInstance();

        void RunApplication(const char* a_name);
        void RunApplication(const std::string a_name = "DIGITAL");

    protected:

        virtual void PreApplicationLoad();
        virtual void ApplicationLoad();

    private:
        void InitApplication();
        void UpdateApplication();
        void TerminateApplication();

        TimeTracker _game_timer;
        WindowManagementSystem _window_management;
        InputManagementSystem _input_management;

        std::string _application_name;
    };

} // End of namespace ~ DCore

#define DIGIAPP_ENTRY()                         \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    DCore::ApplicationInstance app;             \
    app.RunApplication();                       \
    }                                           \

#define DIGIAPP_NAMED_ENTRY(a_name)             \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    DCore::ApplicationInstance app;             \
    app.RunApplication(#a_name);                \
    }    
