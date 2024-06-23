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
    template<typename Attributes, typename Markers>
    class Effect final
    {
    public:

        // -=(Undocumented)=-
        using Modifier = StatModifier<Attributes>;

    public:

        // -=(Undocumented)=-
        Effect(UInt32 ID, CStr Name, Real32 Duration, UInt32 Stack)
            : mID       { ID },
              mName     { Name },
              mDuration { Duration },
              mStack    { Stack }
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
        Real32 GetDuration() const
        {
            return mDuration;
        }

        // -=(Undocumented)=-
        UInt32 GetStack() const
        {
            return mStack;
        }

        // -=(Undocumented)=-
        void AddModifier(Ref<const Modifier> Modifier)
        {
            mModifiers.emplace_back(Modifier);
        }

        // -=(Undocumented)=-
        CPtr<const Modifier> GetModifiers() const
        {
            return mModifiers;
        }

        // -=(Undocumented)=-
        void AddMarker(Markers Marker)
        {
            mMarkers.emplace_back(Marker);
        }

        // -=(Undocumented)=-
        CPtr<const Markers> GetMarkers() const
        {
            return mMarkers;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const UInt32     mID;
        const SStr       mName;
        const Real32     mDuration;
        const UInt32     mStack;
        Vector<Modifier> mModifiers;
        Vector<Markers>  mMarkers;
    };
}