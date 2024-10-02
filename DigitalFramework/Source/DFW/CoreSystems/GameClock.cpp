#include <CoreSystems/GameClock.h>

#include <CoreSystems/Logging/Logger.h>
#include <Defines/MathDefines.h>

// TODO : Adapt code for mutli platform.
#include <Windows.h>

#include <chrono>

namespace DFW
{
    namespace Detail
    {
        int64 cpu_cycles_per_second;

        void QueryCPUClockFreq(int64& a_ref_value)
        {
            LARGE_INTEGER cpu_freq;
            if (!QueryPerformanceFrequency(&cpu_freq))
                DFW_ERRORLOG("Couldn't determine CPU frequency for the game clock.");
            else
                a_ref_value = static_cast<int64>(cpu_freq.QuadPart);
        }

        void QueryCPUClockCounter(int64& a_ref_value)
        {
            LARGE_INTEGER measurement;
            if (!QueryPerformanceCounter(&measurement))
                DFW_ERRORLOG("Couldn't query CPU Cycle Counter for the game clock.");
            else
                a_ref_value = static_cast<int64>(measurement.QuadPart);
        }

        TimeUnit CPUCyclesToSeconds(int64 a_cpu_cycles)
        {
            return static_cast<TimeUnit>(a_cpu_cycles / static_cast<float64>(cpu_cycles_per_second));
        }

        int64 SecondsToCPUCycles(TimeUnit a_seconds)
        {
            return static_cast<int64>(a_seconds * static_cast<float64>(cpu_cycles_per_second));
        }

    } // End of namespace ~ DFW.

    GameClock::GameClock(TimeUnit a_start_time_in_seconds, bool a_start_paused)
        : _start_frame_cycle_count(0)
        , _end_frame_cycle_count(0)

        , _elapsed_cpu_cycles(0)

        , _timescale_modifier(1.0f)
        , _last_frame_delta_time(0.0f)
        , _is_clock_paused(a_start_paused)
        , _has_requested_single_step(false)
    {
        Detail::QueryCPUClockFreq(Detail::cpu_cycles_per_second);
        DFW_ASSERT(Detail::cpu_cycles_per_second != 0);

        if (a_start_time_in_seconds > 0)
            _elapsed_cpu_cycles = Detail::SecondsToCPUCycles(a_start_time_in_seconds);
    }

    void GameClock::BeginGameFrame()
    {
        Detail::QueryCPUClockCounter(_start_frame_cycle_count);
    }

    void GameClock::EndGameFrame()
    {
        Detail::QueryCPUClockCounter(_end_frame_cycle_count);

        UpdateClock();
    }

    TimeUnit GameClock::GetLastFrameDeltaTime() const
    {
        return _last_frame_delta_time;
    }

    int64 GameClock::GetElapsedCycleCount() const
    {
        return _elapsed_cpu_cycles;
    }

    TimeUnit GameClock::GetElapsedTimeInSeconds() const
    {
        return Detail::CPUCyclesToSeconds(_elapsed_cpu_cycles);
    }

    void GameClock::UpdateClock()
    {
        if (_has_requested_single_step)
        {
            TimeUnit const single_step_time  = (1.0f / 60.0f) * _timescale_modifier;
            _last_frame_delta_time          = single_step_time;
            _elapsed_cpu_cycles             += Detail::SecondsToCPUCycles(single_step_time);
            
            _has_requested_single_step = false;
        }
        else if (_is_clock_paused)
        {
            _last_frame_delta_time = 0.0f;
        }
        else
        {
            int64 delta_counter = static_cast<int64>((_end_frame_cycle_count - _start_frame_cycle_count) * _timescale_modifier);
            _last_frame_delta_time = delta_counter / static_cast<TimeUnit>(Detail::cpu_cycles_per_second);
            _elapsed_cpu_cycles += delta_counter;
        }
    }

    void GameClock::AdvanceSingleTick(bool a_override_pause)
    {
        if (a_override_pause || !_is_clock_paused)
            _has_requested_single_step = true;
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

} // End of namespace ~ DFW.
