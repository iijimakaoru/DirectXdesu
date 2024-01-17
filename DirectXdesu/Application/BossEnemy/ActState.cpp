#include "ActState.h"

ActState* ActState::nowActState = nullptr;

ActState::~ActState()
{
}

const bool ActState::GetIsFinish() const
{
    return isFinish;
}
