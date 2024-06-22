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

#include "StatModifier.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    template<typename Attribute, typename Marker>
    class Effect final
    {
    public:

        // -=(Undocumented)=-
        using Modifier = StatModifier<Attribute>;

    public:

        // -=(Undocumented)=-
        Effect(UInt32 ID, CStr Name, CStr Description, Real32 Duration, UInt32 Stack)
            : mID          { ID },
              mName        { Name },
              mDescription { Description },
              mDuration    { Duration },
              mStack       { Stack }
        {
        }

        // -=(Undocumented)=-
        UInt32 GetID() const
        {
            return mID;
        }

        // -=(Undocumented)=-
        CStr GetName() const
        {
            return mName;
        }

        // -=(Undocumented)=-
        CStr GetDescription() const
        {
            return mDescription;
        }

        // -=(Undocumented)=-
        Real32 GetDuration() const
        {
            return mDuration;
        }

        // -=(Undocumented)=-
        UInt32 GetStack() const
        {
            return mStack;
        }

        // @TODO: Modifiers and Tags

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const UInt32     mID;
        const SStr       mName;
        const SStr       mDescription;
        const Real32     mDuration;
        const UInt32     mStack;
        Vector<Modifier> mModifiers;
        Vector<Marker>   mTags;
    };
}