//
// Created by exozg on 22/12/2025.
//

#ifndef MYSIMSMODLOADER_SYNCHRONIZEDCLOCK_H
#define MYSIMSMODLOADER_SYNCHRONIZEDCLOCK_H
#include <chrono>
#include <cstdint>

namespace Msml::Core::Networking
{
    class SynchronizedClock
    {
    public:
        SynchronizedClock() noexcept;
        [[nodiscard]] uint64_t GetCurrentTick() const noexcept;
        [[nodiscard]] bool IsSynchronized() const noexcept;
        void Synchronize(uint64_t aServerTick, uint32_t tripTime) noexcept;
        void Reset() noexcept;
        void Update() noexcept;

    private:
        uint64_t m_lastServerTick;
        uint64_t m_simulatedTick;
        std::chrono::nanoseconds m_previousSimulatedTick;
        std::chrono::nanoseconds m_tickDelta;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_lastSynchronizationTime{};
    };
}

#endif //MYSIMSMODLOADER_SYNCHRONIZEDCLOCK_H