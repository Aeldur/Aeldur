// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 Aeldur contributors (see AUTHORS.md)
//
// This work is licensed under the terms of the AGPL-V3 license.
//
// For a copy, see <https://opensource.org/license/agpl-v3>
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "StatFlyweight.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    template<typename Attributes>
    class StatModifier final
    {
    public:

        // -=(Undocumented)=-
        enum class Action
        {
            // -=(Undocumented)=-
            Add,

            // -=(Undocumented)=-
            Sub,

            // -=(Undocumented)=-
            Inc,

            // -=(Undocumented)=-
            Dec,
        };

    public:

        // -=(Undocumented)=-
        constexpr StatModifier(Attributes Target, Action Action, Real32 Magnitude)
            : mTarget    { Target },
              mAction    { Action },
              mMagnitude { Magnitude }
        {
        }

        // -=(Undocumented)=-
        Attributes GetTarget() const
        {
            return mTarget;
        }

        // -=(Undocumented)=-
        Action GetAction() const
        {
            return mAction;
        }

        // -=(Undocumented)=-
        Real32 GetMagnitude() const
        {
            return mMagnitude;
        }

        // -=(Undocumented)=-
        void Apply(Ref<StatFlyweight> Stat, Real32 Intensity) const
        {
            const Real32 Value = (mMagnitude * Intensity);

            switch (mAction)
            {
            case Action::Add:
                Stat.SetAdditive(Stat.GetAdditive() + Value);
                break;
            case Action::Sub:
                Stat.SetAdditive(Stat.GetAdditive() - Value);
                break;
            case Action::Inc:
                Stat.SetMultiplier(Stat.GetMultiplier() + Value);
                break;
            case Action::Dec:
                Stat.SetMultiplier(Stat.GetMultiplier() - Value);
                break;
            }
        }

        // -=(Undocumented)=-
        void Erase(Ref<StatFlyweight> Stat, Real32 Intensity) const
        {
            const Real32 Value = (mMagnitude * Intensity);

            switch (mAction)
            {
            case Action::Add:
                Stat.SetAdditive(Stat.GetAdditive() - Value);
                break;
            case Action::Sub:
                Stat.SetAdditive(Stat.GetAdditive() + Value);
                break;
            case Action::Inc:
                Stat.SetMultiplier(Stat.GetMultiplier() - Value);
                break;
            case Action::Dec:
                Stat.SetMultiplier(Stat.GetMultiplier() + Value);
                break;
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const Attributes mTarget;
        const Action     mAction;
        const Real32     mMagnitude;
    };
}