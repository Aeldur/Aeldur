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

#include "Drawable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    class Tile final
    {
    public:

        // -=(Undocumented)=-
        enum class Layer
        {
            Floor,
            Decal,
        };

        // -=(Undocumented)=-
        enum class Property
        {
            // -=(Undocumented)=-
            Block  = 0b00000001,
        };

        // -=(Undocumented)=-
        static constexpr SInt32 kDimension = 32;

    public:

        // -=(Undocumented)=-
        void SetProperty(Property Mask)
        {
            mProperties |= CastEnum(Mask);
        }

        // -=(Undocumented)=-
        void UnsetProperty(Property Mask)
        {
            mProperties &= ~CastEnum(Mask);
        }

        // -=(Undocumented)=-
        Bool HasProperty(Property Mask) const
        {
            return mProperties & CastEnum(Mask);
        }

        // -=(Undocumented)=-
        Ref<Drawable> GetLayer(Layer Type)
        {
            return mLayers[CastEnum(Type)];
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt08             mProperties;
        Array<Drawable, 2> mLayers;
    };
}