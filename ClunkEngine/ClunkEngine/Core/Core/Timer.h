#pragma once

#include <chrono>

namespace Clunk
{
    class Timer
    {
    public:

        /**
         * @brief Construct a new Timer object and starts the timer
         */
        Timer() {}
        ~Timer() {}

        /**
         * @brief Start() sets the time
         */
        void Start()
        {
            m_time = std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief Reset() sets the time again. Use for readability.
         */
        void Reset()
        {
            Start();
        }

        /**
         * @brief Elapsed() returns the elapsed time from when timer was last set.
         * @return float - Represents the time.
         */
        float Elaplsed()
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_time).count() * 0.001f * 0.001f * 0.001f;
        }

        /**
         * @brief ElapsedMillis() returns the elapsed time in milliseconds from when the time was last set.
         * @return float - Represents the time.
         */
        float ElapsedMillis()
        {
            return Elaplsed() * 1000.0f;
        }

    private:

        std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
    };
}