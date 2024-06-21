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

#include "Entity.hpp"
#include "Drawable.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    class Partitioner final
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt32 kDimension = 16;

    public:

        // -=(Undocumented)=-
        void Insert(Ref<Entity> Actor);

        // -=(Undocumented)=-
        void Update(Ref<Entity> Actor);

        // -=(Undocumented)=-
        void Remove(Ref<Entity> Actor);

        // -=(Undocumented)=-
        void Prune(Ref<const Rectf> Area);

        // -=(Undocumented)=-
        template<typename Function>
        void Query(Ref<const Rectf> Area, Function Executor) const
        {
            const Rectf Boundaries = Rectf::Max(Area, Rectf(0, 0, 0, 0)) / kDimension;

            for (UInt32 CellY = Boundaries.GetTop(); CellY <= Boundaries.GetBottom(); ++CellY)
            {
                for (UInt32 CellX = Boundaries.GetLeft(); CellX <= Boundaries.GetRight(); ++CellX)
                {
                    const auto Iterator = mCells.find(GetCell(CellX, CellY));
                    if (Iterator != mCells.end())
                    {
                        for (Ptr<Entity> Actor : Iterator->second)
                        {
                            if (Actor->GetBoundaries().Intersects(Area))
                            {
                                Executor(* Actor);
                            }
                        }
                    }
                }
            }
        }

    private:

        // -=(Undocumented)=-
        using Cell = Vector<Ptr<Entity>>;

        // -=(Undocumented)=-
        static constexpr UInt32 GetCell(UInt32 X, UInt32 Y)
        {
            return Y * UINT16_MAX + X;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

       Table<UInt32, Cell>  mCells;
       Table<UInt32, Rectf> mBoundaries;
    };
}