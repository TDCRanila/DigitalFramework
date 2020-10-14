#pragma once

#include <CoreSystems/TimeTracker.h>
#include <CoreSystems/WindowManagement.h>
#include <CoreSystems/InputManagement.h>
#include <CoreSystems/ImGui/ImGuiLayer.h>

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

        static WindowManagementSystem* ProvideWindowManagement();
        static InputManagementSystem* ProvideInputManagment();

    protected:

        virtual void PreApplicationLoad();
        virtual void ApplicationLoad();

    private:
        void InitApplication();
        void UpdateApplication();
        void TerminateApplication();

        static WindowManagementSystem _window_management;
        static InputManagementSystem _input_management;
        
        ImGuiLayer _imgui;
 
        TimeTracker _game_timer;

        std::string _application_name;
    };

} // End of namespace ~ DCore

#define DFW_APP_ENTRY()                         \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    DCore::ApplicationInstance app;             \
    app.RunApplication();                       \
    }                                           \

#define DFW_NAMED_APP_ENTRY(a_name)             \
    int main(int /*argc*/, char** /*argv*/)     \
    {                                           \
    DCore::ApplicationInstance app;             \
    app.RunApplication(#a_name);                \
    }    
