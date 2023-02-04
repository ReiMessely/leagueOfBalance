#pragma once

class Rank final
{
public:
    Rank() = delete;

    enum class Tier
    {
        Iron4, Iron3, Iron2, Iron1,
        Bronze4, Bronze3, Bronze2, Bronze1,
        Silver4, Silver3, Silver2, Silver1,
        Gold4, Gold3, Gold2, Gold1,
        Plat4, Plat3, Plat2, Plat1,
        Dia4, Dia3, Dia2, Dia1,
        Master,
        GrandMaster,
        Challenger
    };

    static inline Tier GetRandomRankTier()
    {
        return static_cast<Rank::Tier>(rand() % (static_cast<int>(Rank::Tier::Challenger) + 1));
    }

    static inline UINT GetMMR(Tier rankTier)
    {
        if (m_Initialised)
        {
            return m_RankToMMR[rankTier];
        }

        // MMR Graph
        for (size_t i = static_cast<size_t>(Rank::Tier::Iron4); i != static_cast<size_t>(Rank::Tier::Challenger); ++i)
        {
            Rank::Tier rank = static_cast<Rank::Tier>(i);
            m_RankToMMR[rank] = i * i;
        }
        //

        m_Initialised = true;
        return m_RankToMMR[rankTier];
    }

private:
    static std::unordered_map<Tier, UINT> m_RankToMMR;
    static bool m_Initialised;
};

std::unordered_map<Rank::Tier, UINT> Rank::m_RankToMMR;
bool Rank::m_Initialised{ false };