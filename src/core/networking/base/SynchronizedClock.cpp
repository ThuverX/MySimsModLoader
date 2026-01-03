//
// Created by exozg on 22/12/2025.
//

#include "SynchronizedClock.h"

using namespace std::chrono_literals;

namespace Msml::Core::Networking
{
    SynchronizedClock::SynchronizedClock() noexcept
        : m_lastServerTick{0}
          , m_simulatedTick{0}
          , m_tickDelta{0}
          , m_lastSynchronizationTime{std::chrono::high_resolution_clock::now()}
    {
    }

    uint64_t SynchronizedClock::GetCurrentTick() const noexcept
    {
        return m_simulatedTick;
    }

    bool SynchronizedClock::IsSynchronized() const noexcept
    {
        return m_simulatedTick != 0;
    }

    void SynchronizedClock::Synchronize(const uint64_t aServerTick, const uint32_t tripTime) noexcept
    {
        if (aServerTick <= m_lastServerTick)
            return;

        m_lastServerTick = aServerTick;

        m_lastSynchronizationTime = std::chrono::high_resolution_clock::now();

        if (IsSynchronized())
        {
            m_previousSimulatedTick = std::chrono::milliseconds(m_simulatedTick);
            const auto estimatedServerTime = std::chrono::milliseconds(m_lastServerTick) +
                std::chrono::milliseconds(tripTime);
            m_tickDelta = estimatedServerTime - m_previousSimulatedTick;
        }
        else
        {
            m_previousSimulatedTick = std::chrono::milliseconds(m_lastServerTick) + std::chrono::milliseconds(tripTime);
            m_simulatedTick = std::chrono::duration_cast<std::chrono::milliseconds>(m_previousSimulatedTick).count();
            m_tickDelta = 0ns;
        }
    }

    void SynchronizedClock::Reset() noexcept
    {
        *this = SynchronizedClock{};
    }

    void SynchronizedClock::Update() noexcept
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = now - m_lastSynchronizationTime;
        const std::chrono::duration<double> syncDelta = now - m_lastSynchronizationTime;

        if (!IsSynchronized())
            return;

        const auto syncDeltaSeconds = syncDelta.count();
        const auto serverTickEstimate = m_previousSimulatedTick + delta;

        const auto diff = std::chrono::nanoseconds(
            static_cast<int64_t>(m_tickDelta.count() * std::min(1.0f, static_cast<float>(syncDeltaSeconds))));

        m_simulatedTick = std::chrono::duration_cast<std::chrono::milliseconds>(serverTickEstimate + diff).count();
    }
}
