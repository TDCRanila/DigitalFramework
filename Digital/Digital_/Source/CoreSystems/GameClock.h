#pragma once

#include <Defines/TimeDefines.h>

namespace DFW
{
    // TODO Make GameClock an interface for multiplatform implementations later on.
    // TODO Make multi-platform, because this class now uses windows api to querry cpu cycles.
    // TODO Implement Minimum DeltaTime Setting.
    // TODO Implement SingleStep Frametime Setting.
    class GameClock
    {
    public:
        GameClock(TimeUnit a_start_time_in_seconds, bool a_start_paused);
        ~GameClock() = default;
        
        void BeginGameFrame();
        void EndGameFrame();
        void AdvanceSingleTick(bool a_override_pause);

        TimeUnit GetLastFrameDeltaTime() const;
        int64 GetElapsedCycleCount() const;
        TimeUnit GetElapsedTimeInSeconds() const;

        void Pause();
        void Unpause();
        bool IsPaused() const;

        void Reset();

        TimeModifier GetTimeModifier() const;
        void SetTimeModifier(TimeModifier a_new_time_modifier);

        void Debug_LogInfo() const;

    private:
        void UpdateClock();

        int64          _elapsed_cpu_cycles;
        int64          _start_frame_cycle_count;
        int64          _end_frame_cycle_count;

        TimeModifier    _timescale_modifier;
        TimeUnit        _last_frame_delta_time;

        bool            _is_clock_paused;
        bool            _has_requested_single_step;
    };

} // End of namspace ~ DFW.
