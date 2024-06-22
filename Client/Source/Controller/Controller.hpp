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

#include "Director.hpp"
#include <World/World.hpp>

#include <Graphic/Service.hpp>
#include <Graphic/Renderer.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Client
{
    // -=(Undocumented)=-
    class Controller final
    {
    public:

        // -=(Undocumented)=-
        void Initialize(Ref<Subsystem::Context> Context);

        // -=(Undocumented)=-
        void Tick(Real64 Time);

        // -=(Undocumented)=-
        Ref<Director> GetDirector()
        {
            return mDirector;
        }

    private:

        // -=(Undocumented)=-
        struct Chunk
        {
            // -=(Undocumented)=-
            Recti        Dimension;

            // -=(Undocumented)=-
            SPtr<Region> Region;
        };

    private:

        // -=(Undocumented)=-
        void OnUpdate(Real64 Delta);

        // -=(Undocumented)=-
        void OnRender(Real64 Delta);

        // -=(Undocumented)=-
        void Compute(Ref<const Recti> Viewport);

        // -=(Undocumented)=-
        void DrawTiles(Ref<Region> Region, UInt32 MinX, UInt32 MinY, UInt32 MaxX, UInt32 MaxY);

        // -=(Undocumented)=-
        void DrawEntity(Ref<Entity> Entity);

        // -=(Undocumented)=-
        void DrawSprite(Ref<Drawable> Drawable, Ref<const Vector2f> Position, Real32 Depth, Drawable::Order Order);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Content::Service>  mResources;
        SPtr<Graphic::Renderer> mRenderer;
        SPtr<Graphic::Pipeline> mPipeline;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        World                   mWorld;
        Director                mDirector;
        Real64                  mTime;
        Vector<Chunk>           mTerrain;
    };
}