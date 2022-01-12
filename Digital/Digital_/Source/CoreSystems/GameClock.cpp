#include <CoreSystems/GameClock.h>

#include <CoreSystems/Logging/Logger.h>
#include <Defines/MathDefines.h>

// TODO : Adapt code for mutli platform.
#include <Windows.h>

#include <chrono>

namespace DFW
{
    GameClock::GameClock(TimeUnit a_start_time_in_seconds, bool a_start_paused)
        : _start_frame_cycle_count(0)
        , _end_frame_cycle_count(0)

        , _cpu_cycles_per_second(0)
        , _elapsed_cpu_cycles(0)

        , _timescale_modifier(1.0f)
        , _last_frame_delta_time(0.0f)
        , _is_clock_paused(a_start_paused)
        , _has_requested_single_step(false)
    {
        QueryCPUClockFreq(_cpu_cycles_per_second);
        DFW_ASSERT(_cpu_cycles_per_second != 0);

        _elapsed_cpu_cycles = SecondsToCPUCycles(a_start_time_in_seconds);
    }

    TimeUnit GameClock::CPUCyclesToSeconds(uint64 a_cpu_cycles) const
    {
        return static_cast<TimeUnit>(a_cpu_cycles / static_cast<float64>(_cpu_cycles_per_second));
    }
    
    uint64 GameClock::SecondsToCPUCycles(TimeUnit a_seconds) const
    {
        return static_cast<uint64>(a_seconds * static_cast<float64>(_cpu_cycles_per_second));
    }

    void GameClock::BeginGameFrame()
    {
        QueryCPUClockCounter(_start_frame_cycle_count);
    }

    void GameClock::EndGameFrame()
    {
        QueryCPUClockCounter(_end_frame_cycle_count);

        UpdateClock();
    }

    TimeUnit GameClock::GetLastFrameDeltaTime() const
    {
        return _last_frame_delta_time;
    }

    uint64 GameClock::GetElapsedCycleCount() const
    {
        return _elapsed_cpu_cycles;
    }

    TimeUnit GameClock::GetElapsedTimeInSeconds() const
    {
        return CPUCyclesToSeconds(_elapsed_cpu_cycles);
    }

    void GameClock::UpdateClock()
    {
        if (_has_requested_single_step)
        {
            TimeUnit const single_step_time  = (1.0f / 60.0f) * _timescale_modifier;
            _last_frame_delta_time          = single_step_time;
            _elapsed_cpu_cycles             += SecondsToCPUCycles(single_step_time);
            
            _has_requested_single_step = false;
        }
        else if (_is_clock_paused)
        {
            _last_frame_delta_time = 0.0f;
        }
        else
        {
            uint64 delta_counter = (_end_frame_cycle_count - _start_frame_cycle_count) * _timescale_modifier;
            _last_frame_delta_time = delta_counter / static_cast<float64>(_cpu_cycles_per_second);
            _elapsed_cpu_cycles += delta_counter;
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

    bool GameClock::IsPaused() const
    {
        return _is_clock_paused;
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

    TimeModifier GameClock::GetTimeModifier() const
    {
        return _timescale_modifier;
    }

    void GameClock::SetTimeModifier(TimeModifier a_new_time_modifier)
    {
        _timescale_modifier = a_new_time_modifier;
    }

    void GameClock::Debug_LogInfo() const
    {
        auto const& dt = _last_frame_delta_time;
        DFW_INFOLOG("DeltaTime(s): {} - FPS: {} - Cycles: {} - Sec: {}"
            , dt, (1 / dt), GetElapsedCycleCount(), GetElapsedTimeInSeconds());
    }

    void GameClock::QueryCPUClockFreq(uint64& a_ref_value) const
    {
        LARGE_INTEGER cpu_freq;
        if (!QueryPerformanceFrequency(&cpu_freq))
            DFW_ERRORLOG("Couldn't determine CPU frequency for the game clock.");
        else
            a_ref_value = static_cast<uint64>(cpu_freq.QuadPart);
    }

    void GameClock::QueryCPUClockCounter(uint64& a_ref_value) const
    {
        LARGE_INTEGER measurement;
        if (!QueryPerformanceCounter(&measurement))
            DFW_ERRORLOG("Couldn't query CPU Cycle Counter for the game clock.");
        else
            a_ref_value = static_cast<uint64>(measurement.QuadPart);
    }

} // End of namespace ~ DFW.
