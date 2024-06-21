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

#include <Network/Packet.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur
{
    // -=(Undocumented)=-
    struct GatewayAccountResult
        : public Network::Packet<GatewayAccountResult, 0>
    {
        // -=(Undocumented)=-
        enum class ID
        {
            OK,
            Unknown,
            Mismatch,
            Exist,
            Online,
        };

        ID Type;

        // -=(Undocumented)=-
        GatewayAccountResult() = default;

        // -=(Undocumented)=-
        GatewayAccountResult(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountResult(ID Type)
            : Type { Type }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeEnum(Type);
        }
    };

    // -=(Undocumented)=-
    struct GatewayAccountLogin
        : public Network::Packet<GatewayAccountLogin, 1>
    {
        SStr Username;
        SStr Password;

        // -=(Undocumented)=-
        GatewayAccountLogin() = default;

        // -=(Undocumented)=-
        GatewayAccountLogin(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountLogin(CStr Username, CStr Password)
            : Username { Username },
              Password { Password }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
            Archive.SerializeString8(Password);
        }
    };

    // -=(Undocumented)=-
    struct GatewayAccountRegister
        : public Network::Packet<GatewayAccountRegister, 2>
    {
        SStr Username;
        SStr Password;
        SStr Email;

        // -=(Undocumented)=-
        GatewayAccountRegister() = default;

        // -=(Undocumented)=-
        GatewayAccountRegister(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountRegister(CStr Username, CStr Password, CStr Email)
            : Username { Username },
              Password { Password },
              Email    { Email }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
            Archive.SerializeString8(Password);
            Archive.SerializeString8(Email);
        }
    };

    // -=(Undocumented)=-
    struct GatewayAccountDelete
        : public Network::Packet<GatewayAccountDelete, 3>
    {
        SStr Username;
        SStr Password;

        // -=(Undocumented)=-
        GatewayAccountDelete() = default;

        // -=(Undocumented)=-
        GatewayAccountDelete(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        GatewayAccountDelete(CStr Username, CStr Password)
            : Username { Username },
              Password { Password }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeString8(Username);
            Archive.SerializeString8(Password);
        }
    };

    // -=(Undocumented)=-
    struct GatewayAccountData
        : public Network::Packet<GatewayAccountData, 4>
     {
        // -=(Undocumented)=-
        struct Entity : public Serializable<Entity>
        {
            UInt ID;
            UInt Level;
            SStr Name;
            SStr Class;

            // -=(Undocumented)=-
            Entity() = default;

            // -=(Undocumented)=-
            Entity(UInt ID, UInt Level, CStr Name, CStr Class)
                : ID    { ID },
                  Level { Level },
                  Name  { Name },
                  Class { Class }
            {
            }

            // -=(Undocumented)=-
            template<typename Stream>
            void OnSerialize(Stream Archive)
            {
                Archive.SerializeInt(ID);
                Archive.SerializeInt(Level);
                Archive.SerializeString8(Name);
                Archive.SerializeString8(Class);
            }
        };

        Vector<Entity> Entities;

        // -=(Undocumented)=-
        GatewayAccountData() = default;

        // -=(Undocumented)=-
        GatewayAccountData(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        void Add(Ref<const Entity> Entity)
        {
            Entities.emplace_back(Entity);
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeList(Entities);
        }
    };

    // -=(Undocumented)=-
    struct LobbyCharacterEnter
        : public Network::Packet<LobbyCharacterEnter, 10>
    {
        UInt Character;

        // -=(Undocumented)=-
        LobbyCharacterEnter() = default;

        // -=(Undocumented)=-
        LobbyCharacterEnter(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        LobbyCharacterEnter(UInt Character)
            : Character { Character }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeInt(Character);
        }
    };

    // -=(Undocumented)=-
    struct LobbyCharacterDelete
        : public Network::Packet<LobbyCharacterDelete, 11>
    {
        UInt Character;

        // -=(Undocumented)=-
        LobbyCharacterDelete() = default;

        // -=(Undocumented)=-
        LobbyCharacterDelete(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        LobbyCharacterDelete(UInt Character)
            : Character { Character }
        {
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
            Archive.SerializeInt(Character);
        }
    };

    // -=(Undocumented)=-
    struct LobbyCharacterCreate
        : public Network::Packet<LobbyCharacterCreate, 12>
    {
        // TODO

        // -=(Undocumented)=-
        LobbyCharacterCreate() = default;

        // -=(Undocumented)=-
        LobbyCharacterCreate(Ref<Reader> Archive)
        {
            OnSerialize(Stream<Reader>(Archive));
        }

        // -=(Undocumented)=-
        template<typename Stream>
        void OnSerialize(Stream Archive)
        {
        }
    };
}