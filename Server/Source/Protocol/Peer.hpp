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

#include "User/User.hpp"
#include <Network/Client.hpp>
#include <Protocol/Protocol.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Server
{
    // -=(Undocumented)=-
    class Peer final
    {
    public:

        // -=(Undocumented)=-
        Peer(ConstSPtr<Network::Client> Connection);

        // -=(Undocumented)=-
        UInt GetID() const
        {
            return mConnection->GetID();
        }

        // -=(Undocumented)=-
        void SetUser(ConstSPtr<User> User)
        {
            mUser = User;
        }

        // -=(Undocumented)=-
        ConstSPtr<User> GetUser() const
        {
            return mUser;
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Close(Message && Packet)
        {
            mConnection->Write(Packet, false);
            mConnection->Close(false);
        }

        // -=(Undocumented)=-
        void Close()
        {
            mConnection->Close(true);
        }

        // -=(Undocumented)=-
        template<typename Message>
        void Write(Message && Packet, Bool Unreliable = false)
        {
            if constexpr (std::is_base_of_v<Writer, Message>)
            {
                mConnection->Write(Packet.GetData(), Unreliable);
            }
            else
            {
                mConnection->Write(Packet, Unreliable);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Network::Client> mConnection;
        SPtr<User>            mUser;
    };
}