#pragma once

#include <Defines/TimeDefines.h>

namespace DCore
{
    class GameClock
    {
    public:

        GameClock(float32 a_start_time_in_seconds = 0.0f, bool a_start_paused = false);
        ~GameClock() = default;

        void BeginGameFrame();
        void EndGameFrame();

        float32 GetLastFrameDeltaTime() const;
        uint64 GetElapsedCycleCount() const;

        void AdvanceSingleTick(bool a_override_pause = true);

        void Pause();
        void Unpause();
        void Reset();

        bool IsPaused() const;

        float32 GetTimeModifier() const;
        void SetTimeModifier(float32 a_new_time_modifier);

    private:

        void UpdateClock();

        float32 CPUCyclesToSeconds(uint64 a_cpu_cycles) const;
        uint64 SecondsToCPUCycles(float32 a_seconds) const;

        uint64 _elapsed_cpu_cycles;
        uint64 _start_frame_cycle_count;
        uint64 _end_frame_cycle_count;
        uint64 _cpu_cycles_per_second;

        float32 _timescale_modifier;
        float32 _last_frame_delta_time;

        bool _is_clock_paused;
        bool _has_requested_single_step;
    };


} // End of namspace ~ DCore.
