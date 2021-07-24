#include <CoreSystems/GameClock.h>

#include <CoreSystems/Logging/Logger.h>
#include <Defines/MathDefines.h>

// TODO : Adapt code for mutli platform.
#include <Windows.h>

#include <chrono>

namespace DCore
{

    GameClock::GameClock(float32 a_start_time_in_seconds, bool a_start_paused)
        : _start_frame_cycle_count(0)
        , _end_frame_cycle_count(0)

        , _cpu_cycles_per_second(0)
        , _elapsed_cpu_cycles(0)

        , _timescale_modifier(1.0f)
        , _last_frame_delta_time(0.0f)
        , _is_clock_paused(a_start_paused)
        , _has_requested_single_step(false)
    {
        LARGE_INTEGER cpu_freq;
        if (!QueryPerformanceFrequency(&cpu_freq))
            DFW_ERRORLOG("Couldn't determine CPU frequency.");
        else
            _cpu_cycles_per_second = cpu_freq.QuadPart;

        _elapsed_cpu_cycles = SecondsToCPUCycles(a_start_time_in_seconds);

    }

    float32 GameClock::CPUCyclesToSeconds(uint64 a_cpu_cycles) const
    {
        return static_cast<float32>(a_cpu_cycles / static_cast<float64>(_cpu_cycles_per_second));
    }
    
    uint64 GameClock::SecondsToCPUCycles(float32 a_seconds) const
    {
        return static_cast<uint64>(a_seconds * static_cast<float64>(_cpu_cycles_per_second));
    }


    void GameClock::BeginGameFrame()
    {
        LARGE_INTEGER measurement;
        QueryPerformanceCounter(&measurement);
        _start_frame_cycle_count = measurement.QuadPart;
    }

    void GameClock::EndGameFrame()
    {
        LARGE_INTEGER measurement;
        QueryPerformanceCounter(&measurement);
        _end_frame_cycle_count = measurement.QuadPart;

        UpdateClock();
    }

    float32 GameClock::GetLastFrameDeltaTime() const
    {
        return _last_frame_delta_time;
    }

    uint64 GameClock::GetElapsedCycleCount() const
    {
        return _elapsed_cpu_cycles;
    }

    float32 GameClock::GetElapsedTimeInSeconds() const
    {
        return CPUCyclesToSeconds(_elapsed_cpu_cycles);
    }

    void GameClock::UpdateClock()
    {
        if      (_has_requested_single_step)
        {
            /* TODO : Store this elsewhere, must change depending on settings? */
            const float32 ideal_frame_time  = (1.0f / 60.0f) * _timescale_modifier;
            _last_frame_delta_time          = ideal_frame_time;
            _elapsed_cpu_cycles             += SecondsToCPUCycles(ideal_frame_time);
            
            _has_requested_single_step = false;
        }
        else if (!_is_clock_paused)
        {
            uint64 delta_counter    = (_end_frame_cycle_count - _start_frame_cycle_count) * _timescale_modifier;
            _last_frame_delta_time  = delta_counter / static_cast<float64>(_cpu_cycles_per_second);
            _elapsed_cpu_cycles     += delta_counter;
        }
        else
        {
            _last_frame_delta_time = 0.0f;
        }
    }

    void GameClock::AdvanceSingleTick(bool a_override_pause)
    {
        if (a_override_pause || !_is_clock_paused)
        {
            _has_requested_single_step = true;
        }
    }

    void GameClock::Pause()
    {
        _is_clock_paused = true;
    }

    void GameClock::Unpause()
    {
        _is_clock_paused = false;
    }

    void GameClock::Reset()
    {
        _start_frame_cycle_count    = 0;
        _end_frame_cycle_count      = 0;

        _elapsed_cpu_cycles         = 0;

        _timescale_modifier         = 1.0f;
        _last_frame_delta_time      = 0.0f;
        _is_clock_paused            = false;
        _has_requested_single_step  = false;
    }

    bool GameClock::IsPaused() const
    {
        return _is_clock_paused;
    }

    float32 GameClock::GetTimeModifier() const
    {
        return _timescale_modifier;
    }

    void GameClock::SetTimeModifier(float32 a_new_time_modifier)
    {
        _timescale_modifier = a_new_time_modifier;
    }

    void GameClock::DebugLog() const
    {
        auto& dt = _last_frame_delta_time;
        DFW_INFOLOG("DeltaTime(s): {} - FPS: {} - Cycles: {} - Sec: {}"
            , dt, (1 / dt), GetElapsedCycleCount(), GetElapsedTimeInSeconds());
    }

} // End of namespace ~ DCore.
