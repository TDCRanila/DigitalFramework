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
        uint64 GetElapsedCycleCount() const;
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

        void QueryCPUClockFreq(uint64& a_ref_value) const;
        void QueryCPUClockCounter(uint64& a_ref_value) const;

        TimeUnit CPUCyclesToSeconds(uint64 a_cpu_cycles) const;
        uint64 SecondsToCPUCycles(TimeUnit a_seconds) const;

        uint64          _elapsed_cpu_cycles;
        uint64          _start_frame_cycle_count;
        uint64          _end_frame_cycle_count;
        uint64          _cpu_cycles_per_second;

        TimeModifier    _timescale_modifier;
        TimeUnit        _last_frame_delta_time;

        bool            _is_clock_paused;
        bool            _has_requested_single_step;
    };

} // End of namspace ~ DFW.
