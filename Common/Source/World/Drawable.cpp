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

#include "Drawable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace World
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Drawable::Drawable()
        : mRotation  { 0.0f },
          mColor     { 1.0f, 1.0f, 1.0f, 1.0f },
          mPivot     { Pivot::TopLeft },
          mAnimation { nullptr }
    {
        SetState(State::Stopped);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<const Rectf> Drawable::GetFrame(Real64 Tick)
    {
        static constexpr Rectf kEmptyRect(0, 0, 0, 0);

        if (mAnimation == nullptr || mAnimation->Frames.empty() || mState == State::Stopped)
        {
            return kEmptyRect;
        }

        const UInt32 Count = mAnimation->Frames.size();
        UInt32 Offset = 0;

        if (Count > 1)
        {
            if (mTick == 0)
            {
                mTick = Tick;
            }

            Offset = static_cast<UInt32>((Tick - mTick) * (Count / mAnimation->Duration));

            if (Offset >= Count)
            {
                if (mState == State::Repeat)
                {
                    Offset %= Count;
                    mTick   = (Tick - Offset * mAnimation->Duration);
                }
                else
                {
                    Offset = Count - 1;
                    mState = State::Stopped;
                }
            }
        }
        return mAnimation->Frames[Offset];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Drawable::GetBoundaries(Ref<const Drawable> Drawable, Ref<const Vector2f> Position)
    {
        if (Ptr<const Animation> Animation = Drawable.GetAnimation())
        {
            return GetBoundaries(Drawable.GetPivot(), Position, Vector2f(Animation->Width, Animation->Height));
        }
        return Rectf();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Rectf Drawable::GetBoundaries(Pivot Pivot, Ref<const Vector2f> Position, Ref<const Vector2f> Size)
    {
        static constexpr Rectf kMultiplier[] = {
            Rectf( 0.0f,  0.0f, 1.0f, 1.0f),  // TopLeft
            Rectf(-0.5f,  0.0f, 0.5f, 1.0f),  // TopCenter
            Rectf(-1.0f,  0.0f, 0.0f, 1.0f),  // TopRight
            Rectf( 0.0f, -0.5f, 1.0f, 0.5f),  // MiddleLeft
            Rectf(-0.5f, -0.5f, 0.5f, 0.5f),  // MiddleCenter
            Rectf(-1.0f, -0.5f, 0.0f, 0.5f),  // MiddleRight
            Rectf( 0.0f, -1.0f, 1.0f, 0.0f),  // BottomLeft
            Rectf(-0.5f, -1.0f, 0.5f, 0.0f),  // BottomCenter
            Rectf(-1.0f, -1.0f, 0.0f, 0.0f),  // BottomRight
        };
        return kMultiplier[CastEnum(Pivot)] * Size + Position;
    }
}