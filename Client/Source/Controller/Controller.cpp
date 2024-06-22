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

#include "Controller.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Client
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::Initialize(Ref<Subsystem::Context> Context)
    {
        // Initialize our universe
        mWorld.Initialize(Context);

        // Initialize our default resource(s)
        mResources = Context.GetSubsystem<Content::Service>();
        mRenderer  = NewPtr<Graphic::Renderer>(Context);
        mPipeline  = mResources->Load<Graphic::Pipeline>("Resources://Pipeline/Sprite.effect");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::Tick(Real64 Time)
    {
        const Real64 Delta = Time - mTime;
        mTime = Time;

        mWorld.Tick(Time);
        
        OnUpdate(Delta);
        OnRender(Delta);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::OnUpdate(Real64 Delta)
    {
        // Update the camera and recalculate visible chunk(s)
        const Bool Dirty = mDirector.Compute(Delta);
        if (Dirty)
        {
            Ref<const Recti> Boundaries = Recti::Max(mDirector.GetBoundaries(), Recti(0, 0, 0, 0));
            Compute(Boundaries);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::OnRender(Real64 Delta)
    {
        // Draw Game (Entities, Particles, ...)
        mRenderer->Begin(mDirector.GetMatrix(), Delta);
        {
            // @TODO: Draw Foreground

            // Draw Middle (Entities)
            mWorld.GetEntities().Query(mDirector.GetBoundaries() * Tile::kDimension, [this](Ref<Entity> Actor)
            {
                DrawEntity(Actor);
            });
            mRenderer->Flush();

            // Draw Background (Floor, Decal, ...)
            for (Ref<const Chunk> Chunk : mTerrain)
            {
                const UInt32 MinLocalX = Chunk.Dimension.GetLeft();
                const UInt32 MinLocalY = Chunk.Dimension.GetTop();
                const UInt32 MaxLocalX = Chunk.Dimension.GetRight();
                const UInt32 MaxLocalY = Chunk.Dimension.GetBottom();
                DrawTiles(* Chunk.Region, MinLocalX, MinLocalY, MaxLocalX, MaxLocalY);
            }
        }
        mRenderer->End();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::Compute(Ref<const Recti> Viewport)
    {
        const UInt32 RegionX1 = std::floorf(Viewport.GetLeft() / Region::kTilesPerRow);
        const UInt32 RegionY1 = std::floorf(Viewport.GetTop()  / Region::kTilesPerColumn);
        const UInt32 RegionX2 = std::ceilf(Viewport.GetRight()  / Region::kTilesPerRow);
        const UInt32 RegionY2 = std::ceilf(Viewport.GetBottom() / Region::kTilesPerColumn);

        UInt32 MinLocalX = Viewport.GetLeft() % Region::kTilesPerRow;
        UInt32 MinLocalY = Viewport.GetTop()  % Region::kTilesPerColumn;

        mTerrain.clear();

        for (UInt32 RegionY = RegionY1; RegionY <= RegionY2; ++RegionY)
        {
            const UInt32 MaxLocalY = Core::Min(
                Region::kTilesPerColumn, Viewport.GetBottom() - (Region::kTilesPerColumn - 1) * RegionY);

            for (UInt32 RegionX = RegionX1; RegionX <= RegionX2; ++RegionX)
            {
                const UInt32 MaxLocalX = Core::Min(
                    Region::kTilesPerRow, Viewport.GetRight() - (Region::kTilesPerRow - 1) * RegionX);

                if (ConstSPtr<Region> Chunk = mWorld.Load(RegionX, RegionY))
                {
                    mTerrain.emplace_back(Recti(MinLocalX, MinLocalY, MaxLocalX, MaxLocalY), Chunk);
                }

                MinLocalX = 0;
            }

            MinLocalY = 0;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::DrawTiles(Ref<Region> Region, UInt32 MinX, UInt32 MinY, UInt32 MaxX, UInt32 MaxY)
    {
        for (UInt TileY = MinY; TileY < MaxY; ++TileY)
        {
            for (UInt TileX = MinX; TileX < MaxX; ++TileX)
            {
                const Vector2f Coordinates(
                    Region.GetPosition().GetX() + (TileX * Tile::kDimension),
                    Region.GetPosition().GetY() + (TileY * Tile::kDimension));

                Ref<Tile> Tile = Region.GetTile(TileX, TileY);
                DrawSprite(Tile.GetLayer(Tile::Layer::Floor), Coordinates, 0.0f, Drawable::Order::Background);
                DrawSprite(Tile.GetLayer(Tile::Layer::Decal), Coordinates, 1.0f, Drawable::Order::Background);
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::DrawEntity(Ref<Entity> Entity)
    {
        switch (Entity.GetType())
        {
        case Entity::Type::Object:
        {
            Ref<Aeldur::Object> Object = static_cast<Ref<Aeldur::Object>>(Entity);
            DrawSprite(Object.GetDrawable(), Object.GetPosition(), 2.0f, Drawable::Order::Middle);
        }
            break;
        case Entity::Type::Character:
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Controller::DrawSprite(Ref<Drawable> Drawable, Ref<const Vector2f> Position, Real32 Depth, Drawable::Order Order)
    {
        Ptr<const Animation> Animation = Drawable.GetAnimation();

        if (Animation)
        {
            ConstSPtr<Graphic::Material> Material = mResources->Retrieve<Graphic::Material>(
                Content::Uri(Format("Memory://Material/{}", Animation->File)), true);

            // @TODO: Remove this and have the content manager load material(s)
            if (Material->GetStatus() == Content::Resource::Status::None)
            {
                ConstSPtr<Graphic::Texture> Texture = mResources->Load<Graphic::Texture>(
                    Content::Uri(Format("Resources://Texture/{}.png", Animation->File)));

                const Graphic::Sampler Sampler(
                    Graphic::TextureEdge::Repeat, Graphic::TextureEdge::Repeat, Graphic::TextureFilter::Nearest);

                Material->SetTexture(0, Texture);
                Material->SetSampler(0, Sampler);

                mResources->Register(Material, false);
            }

            // @TODO: Check if is possible to have unique depth, if not move this to GPU
            // @TODO: Fixed depth for fixed sprites (Background and Foreground)
            const Vector2f Screen = mDirector.GetSpaceCoordinates(Position);
            Depth  = Director::CalculateDepth(
                CastEnum(Order),
                Screen.GetX(),
                Screen.GetY(),
                Depth);

            mRenderer->DrawTexture(
                Drawable::GetBoundaries(Drawable, Position),
                Drawable.GetFrame(mTime),
                Depth,
                Drawable.GetRotation(),
                Graphic::Renderer::Order::Opaque, // @TODO: Customizable
                Drawable.GetColor(),
                mPipeline,                        // @TODO: Customizable
                Material);
        }
    }
}