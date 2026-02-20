#include <DFW/CoreSystems/GameClock.h>

#include <DFW/CoreSystems/Logging/Logger.h>

#ifdef DFW_PLATFORM_WINDOWS
#include <Windows.h>
#elif DFW_PLATFORM_LINUX
#include <chrono>
#include <ratio>
#include <type_traits>
#else
#error DFW_PLATFORM_UNSUPPORTED
#endif

namespace DFW
{
    namespace Detail
    {
        int64 cycles_per_second;

        #ifdef DFW_PLATFORM_WINDOWS
            void QueryClockFreq(int64& a_ref_value)
            {
                LARGE_INTEGER cpu_freq;
                if (!QueryPerformanceFrequency(&cpu_freq))
                    DFW_ERRORLOG("Couldn't determine CPU frequency for the game clock.");
                else
                    a_ref_value = static_cast<int64>(cpu_freq.QuadPart);
            }

            void QueryClockCounter(int64& a_ref_value)
            {
                LARGE_INTEGER measurement;
                if (!QueryPerformanceCounter(&measurement))
                    DFW_ERRORLOG("Couldn't query CPU Cycle Counter for the game clock.");
                else
                    a_ref_value = static_cast<int64>(measurement.QuadPart);
            }
        #elif DFW_PLATFORM_LINUX
            void QueryClockFreq(clock_t& a_ref_value)
            {
                using ClockType = std::conditional_t<
                  std::chrono::high_resolution_clock::is_steady
                , std::chrono::high_resolution_clock
                , std::chrono::steady_clock>;

                using ClockFrequency = std::ratio_divide<std::ratio<1>, ClockType::duration::period>;

                static_assert(ClockFrequency::den == 1);

                a_ref_value = ClockFrequency::num;
            }

            void QueryClockCounter(clock_t& a_ref_value)
            {
                using ClockType = std::conditional_t<
                  std::chrono::high_resolution_clock::is_steady
                , std::chrono::high_resolution_clock
                , std::chrono::steady_clock>;

                a_ref_value = ClockType::now().time_since_epoch().count();
            }
        #endif

        TimeUnit CyclesToSeconds(int64 a_cpu_cycles)
        {
            return static_cast<TimeUnit>(a_cpu_cycles / static_cast<float64>(cycles_per_second));
        }

        int64 SecondsToCycles(TimeUnit a_seconds)
        {
            return static_cast<int64>(a_seconds * static_cast<float64>(cycles_per_second));
        }

    } // End of namespace ~ DFW.

    GameClock::GameClock(TimeUnit a_start_time_in_seconds, bool a_start_paused)
        : _start_frame_cycle_count(0)
        , _end_frame_cycle_count(0)

        , _elapsed_cycles(0)

        , _timescale_modifier(1.0f)
        , _last_frame_delta_time(0.0f)
        , _is_clock_paused(a_start_paused)
        , _has_requested_single_step(false)
    {
        Detail::QueryClockFreq(Detail::cycles_per_second);
        DFW_ASSERT(Detail::cycles_per_second != 0);

        if (a_start_time_in_seconds > 0)
            _elapsed_cycles = Detail::SecondsToCycles(a_start_time_in_seconds);
    }

    void GameClock::BeginGameFrame()
    {
        Detail::QueryClockCounter(_start_frame_cycle_count);
    }

    void GameClock::EndGameFrame()
    {
        Detail::QueryClockCounter(_end_frame_cycle_count);

        UpdateClock();
    }

    TimeUnit GameClock::GetLastFrameDeltaTime() const
    {
        return _last_frame_delta_time;
    }

    int64 GameClock::GetElapsedCycleCount() const
    {
        return _elapsed_cycles;
    }

    TimeUnit GameClock::GetElapsedTimeInSeconds() const
    {
        return Detail::CyclesToSeconds(_elapsed_cycles);
    }

    void GameClock::UpdateClock()
    {
        if (_has_requested_single_step)
        {
            TimeUnit const single_step_time = (1.0f / 60.0f) * _timescale_modifier;
            _last_frame_delta_time = single_step_time;
            _elapsed_cycles += Detail::SecondsToCycles(single_step_time);
            
            _has_requested_single_step = false;
        }
        else if (_is_clock_paused)
        {
            _last_frame_delta_time = 0.0f;
        }
        else
        {
            int64 delta_counter = static_cast<int64>((_end_frame_cycle_count - _start_frame_cycle_count) * _timescale_modifier);
            _last_frame_delta_time = delta_counter / static_cast<TimeUnit>(Detail::cycles_per_second);
            _elapsed_cycles += delta_counter;
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

        _elapsed_cycles         = 0;

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
