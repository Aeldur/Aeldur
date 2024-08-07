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

#include <Core/Core.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    class Stat final
    {
    public:

        // -=(Undocumented)=-
        enum class Action
        {
            // -=(Undocumented)=-
            Set,

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
        constexpr Stat()
            : mBase       { 0 },
              mAdditive   { 0 },
              mMultiplier { 0 },
              mEffective  { 0 }
        {
        }

        // -=(Undocumented)=-
        void Apply(Action Operation, Real32 Magnitude)
        {
            switch (Operation)
            {
            case Action::Set:
                mBase = Magnitude;
                break;
            case Action::Add:
                mAdditive += Magnitude;
                break;
            case Action::Sub:
                mAdditive -= Magnitude;
                break;
            case Action::Inc:
                mMultiplier += Magnitude;
                break;
            case Action::Dec:
                mMultiplier -= Magnitude;
                break;
            }
        }

        // -=(Undocumented)=-
        void Calculate()
        {
            mEffective = (mBase + mAdditive) + ((mBase + mAdditive) * mMultiplier);
        }

        // -=(Undocumented)=-
        Real32 GetBase() const
        {
            return mBase;
        }

        // -=(Undocumented)=-
        Real32 GetAdditive() const
        {
            return mAdditive;
        }

        // -=(Undocumented)=-
        Real32 GetMultiplier() const
        {
            return mMultiplier;
        }

        // -=(Undocumented)=-
        Real32 GetEffective() const
        {
            return mEffective;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Real32 mBase;
        Real32 mAdditive;
        Real32 mMultiplier;
        Real32 mEffective;
    };
}