#pragma once
#include "Rank.h"

struct RawFormsAnswer;

struct Player final
{
    Player() = default;
    Player(const RawFormsAnswer* answer, Rank::Tier _rank = Rank::Tier::Silver4) 
        : formsAnswer{ answer }
        , rank{ _rank }
    {
    }

    const RawFormsAnswer* formsAnswer;
    Rank::Tier rank;
};