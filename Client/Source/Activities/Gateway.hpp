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

#include "Core/Activity.hpp"
#include <Network/Packets.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Client
{
    // -=(Undocumented)=-
    class Gateway final : public Activity
    {
    public:

        // -=(Undocumented)=-
        static constexpr CStr kRemoteAddress = "127.0.0.1";

        // -=(Undocumented)=-
        static constexpr UInt kRemotePort    = 7667;

    public:

        // -=(Undocumented)=-
        enum class State
        {
            Idle,
            Authenticate,
            Create,
            Wait,
        };

    public:

        // -=(Undocumented)=-
        Gateway(Ref<Application> Application);

        // \see Activity::OnAttach
        void OnAttach() override;

        // \see Activity::OnDetach
        void OnDetach() override;

        // \see Activity::OnPause
        void OnPause() override;

        // \see Activity::OnResume
        void OnResume() override;

        // \see Activity::OnConnect
        void OnConnect() override;

        // \see Activity::OnMessage
        void OnMessage(Ref<Reader> Archive) override;

        // \see Activity::OnDisconnect
        void OnDisconnect() override;

    private:

        // -=(Undocumented)=-
        void OnAccountLogin(CStr Username, CStr Password);

        // -=(Undocumented)=-
        void OnAccountCreate(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        void OnAccountResult(Ref<const GatewayAccountResult> Message);

        // -=(Undocumented)=-
        void OnAccountAuthorized(Ref<const GatewayAccountData> Message);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        State mState;
        SStr  mUsername;
        SStr  mPassword;
        SStr  mEmail;
    };
}