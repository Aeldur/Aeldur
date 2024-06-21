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

#include "Animator.hpp"
#include "Partitioner.hpp"
#include "Object.hpp"
#include "Character.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    // @TODO: Entity Component (Not ECS shit) DOD system.
    class Entities final
    {
    public:

        // -=(Undocumented)=-
        Entities(Ref<Animator> Animator);

        // -=(Undocumented)=-
        SPtr<Entity> Load(Ref<Reader> Reader);

        // -=(Undocumented)=-
        void Save(Ref<Writer> Writer, Ref<Entity> Actor);

        // -=(Undocumented)=-
        SPtr<Entity> Create(UInt32 ID, Entity::Type Type, Ref<const Vector2f> Position);

        // -=(Undocumented)=-
        void Update(Ref<Entity> Actor);

        // -=(Undocumented)=-
        void Remove(Ref<Entity> Actor);

        // -=(Undocumented)=-
        SPtr<Entity> Find(UInt32 ID)
        {
            const auto Iterator = mRegistry.find(ID);
            return (Iterator != mRegistry.end() ? Iterator->second : nullptr);
        }

        // -=(Undocumented)=-
        void Prune(Ref<const Rectf> Area);

        // -=(Undocumented)=-
        template<typename Function>
        void Query(Ref<const Rectf> Area, Function Executor)
        {
            mPartitioner.template Query<Function>(Area, Executor);
        }

    private:

        // -=(Undocumented)=-
        void OnDecode(Ref<Reader> Reader, Ref<Object> Actor);

        // -=(Undocumented)=-
        void OnEncode(Ref<Writer> Writer, Ref<Object> Actor);

        // -=(Undocumented)=-
        void OnDecode(Ref<Reader> Reader, Ref<Character> Actor);

        // -=(Undocumented)=-
        void OnEncode(Ref<Writer> Writer, Ref<Character> Actor);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Partitioner                 mPartitioner;
        Ref<Animator>               mAnimator;
        Table<UInt32, SPtr<Entity>> mRegistry;
    };
}