#include <CoreSystems/ApplicationInstance.h>

#include <CoreSystems/TimeTracker.h>

namespace DCore
{
    ApplicationInstance::ApplicationInstance()
    {
    }

    ApplicationInstance::~ApplicationInstance()
    {
    }

    void ApplicationInstance::RunApplication()
    {
        TimeTracker application_timer;
        INFOLOG("DIGITAL - Init Application.");
        application_timer.StartTimer();
        InitApplication();
        const TimeUnit elapsed_init_time = application_timer.FetchAndResetTime(false);
        INFOLOG("DIGITAL - Init Application Complete. - Elapsed Time: " << elapsed_init_time);

        INFOLOG("DIGITAL - Running Application.");
        UpdateApplication();
        
        INFOLOG("DIGITAL - Terminating Application.");
        application_timer.StartTimer();
        TerminateApplication();
        const TimeUnit elapsed_termination_time = application_timer.FetchAndResetTime(false);
        INFOLOG("DIGITAL - Terminating Application Complete - Elapsed Time: " << elapsed_termination_time);
    }

    void ApplicationInstance::PreApplicationLoad()
    {
    }

    void ApplicationInstance::ApplicationLoad()
    {
    }

    void ApplicationInstance::InitApplication()
    {
        PreApplicationLoad();
        
        ApplicationLoad();
    }

    void ApplicationInstance::UpdateApplication()
    {
        bool shouldRun(true);
        TimeTracker game_timer;
        game_timer.StartTimer();
        while (shouldRun)
        {
            // Update Game Instance(s)
            if (game_timer.FetchTime() > TimeUnit(5.f))
                shouldRun = false;
        }
    }

    void ApplicationInstance::TerminateApplication()
    {

    }

} // End of namespace ~ DCore
