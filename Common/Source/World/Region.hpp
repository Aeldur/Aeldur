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

#include "Tile.hpp"
#include <Content/Resource.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=(Undocumented)=-
    class Region final : public Content::AbstractResource<Hash("Region")>
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32   kTilesPerRow    = 32;

        // -=(Undocumented)=-
        static constexpr UInt32   kTilesPerColumn = 32;

        // -=(Undocumented)=-
        static constexpr UInt32   kTilesPerRegion = kTilesPerRow * kTilesPerColumn;

    public:

        // -=(Undocumented)=-
        Region(Ref<const Content::Uri> Key);

        // -=(Undocumented)=-
        void SetPosition(Vector2i Position)
        {
            mPosition = Position;
        }

        // -=(Undocumented)=-
        Vector2i GetPosition() const
        {
            return mPosition;
        }

        // -=(Undocumented)=-
        Ref<Tile> GetTile(UInt32 X, UInt32 Y)
        {
            return mTiles[Y * kTilesPerColumn + X];
        }

    public:

        // -=(Undocumented)=-
        static constexpr UInt32 GetID(UInt32 X, UInt32 Y)
        {
            return Y * UINT16_MAX + X;
        }

    private:

        // \see Resource::OnCreate
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UInt32                       mID;
        Vector2i                     mPosition;
        Array<Tile, kTilesPerRegion> mTiles;
    };
}