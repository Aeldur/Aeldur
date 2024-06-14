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

#include "Engine/Kernel.hpp"
#include "Options.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Client
{
    // -=(Undocumented)=-
    class Application final : public Engine::Kernel, public Network::Protocol
    {
    public:

        // -=(Undocumented)=-
        void SavePreferences();

        // -=(Undocumented)=-
        Ref<Options> GetPreferences()
        {
            return mPreferences;
        }

        // -=(Undocumented)=-
        void Goto(ConstSPtr<class Activity> Foreground);

        // -=(Undocumented)=-
        void Back();

        // -=(Undocumented)=-
        template<typename Type = Activity>
        SPtr<Type> GetForeground()
        {
            return mStack.empty() ? nullptr : CastPtr<Type>(mStack.front());
        }

        // -=(Undocumented)=-
        void Connect(CStr Address, UInt32 Port);

        // -=(Undocumented)=-
        template<typename Message>
        void Send(Message && Packet, Bool Unreliable = false)
        {
            if (mConnection)
            {
                mConnection->Write(Packet, Unreliable);
            }
        }

        // -=(Undocumented)=-
        void Disconnect();

    private:

        // \see Kernel::OnInitialize
        Bool OnInitialize() override;

        // \see Kernel::OnDestroy
        void OnDestroy() override;

        // \see Kernel::OnTick
        void OnTick(Real64 Time) override;

        // \see Protocol::OnConnect
        void OnConnect(ConstSPtr<Network::Client> Connection) override;

        // \see Protocol::OnDisconnect
        void OnDisconnect(ConstSPtr<Network::Client> Connection) override;

        // \see Protocol::OnRead
        void OnRead(ConstSPtr<Network::Client> Connection, CPtr<UInt08> Bytes) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Options                mPreferences;
        Vector<SPtr<Activity>> mStack;
        SPtr<Network::Client>  mConnection;
    };
}