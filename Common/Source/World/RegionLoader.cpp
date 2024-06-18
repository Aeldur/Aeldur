// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2024 Aeldur contributors (see AUTHORS.md)
//
// This work is licensed under the terms of the AGPL-V3 license.
//
// For a copy, see <https://opensource.org/license/agpl-v3>
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "RegionLoader.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    RegionLoader::RegionLoader(Ref<Animator> Animator, Ref<Entities> Entities)
        : mAnimator { Animator },
          mEntities { Entities }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool RegionLoader::Load(Ref<class Content::Service> Service, Ref<Chunk> Data, ConstSPtr<Region> Asset)
    {
        Reader Input(Data.GetSpan<UInt08>());

        constexpr UInt32 kMaskBlock = 0b00000001;
        constexpr UInt32 kMaskDecal = 0b00000010;

        const UInt32   RegionX = Input.ReadInt<UInt16>();
        const UInt32   RegionY = Input.ReadInt<UInt16>();
        const Vector2f Coordinates(
            RegionX * Region::kTilesPerRow * Tile::kDimension,
            RegionY * Region::kTilesPerColumn * Tile::kDimension);

        Asset->SetPosition(Coordinates);

        for (UInt32 Y = 0; Y < Region::kTilesPerColumn; ++Y)
        {
            for (UInt32 X = 0; X < Region::kTilesPerRow; ++X)
            {
                Ref<Tile> Tile = Asset->GetTile(X, Y);

                const UInt08 Flags = Input.ReadUInt8();

                ReadLayer(Input, Tile, Tile::Layer::Floor);

                if (Flags & kMaskBlock)
                {
                    Tile.SetProperty(Tile::Property::Block);
                }

                if (Flags & kMaskDecal)
                {
                    ReadLayer(Input, Tile, Tile::Layer::Decal);
                }
            }
        }

        for (UInt32 Element = Input.ReadInt<UInt32>(); Element > 0; --Element)
        {
            ConstSPtr<Entity> Actor = mEntities.Load(Input);
            if (Actor)
            {
                Actor->SetPosition(Actor->GetPosition() + Coordinates);

                mEntities.Update(* Actor);
            }
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void RegionLoader::ReadLayer(Ref<Reader> Input, Ref<Tile> Tile, Tile::Layer Type)
    {
        const UInt32 ID = Input.ReadInt<UInt32>();
        if (ID)
        {
            Ref<Drawable> Drawable = Tile.GetLayer(Type);
            Drawable.SetAnimation(mAnimator.GetAnimation(ID));
            Drawable.SetState(Drawable::State::Repeat);
        }
    }
}