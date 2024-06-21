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

#include "Gateway.hpp"
#include "Lobby.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Client
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Gateway::Gateway(Ref<Application> Application)
        : Activity(Application)
    {
        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAttach()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        Browser->Register("onGatewayLogin", [this](CPtr<const UI::Value> Parameters)
        {
            OnAccountLogin(Parameters[0], Parameters[1]);
            return UI::Value();
        });
        Browser->Register("onGatewayCreate", [this](CPtr<const UI::Value> Parameters)
        {
            OnAccountCreate(Parameters[0], Parameters[1], Parameters[2]);
            return UI::Value();
        });
        Browser->Register("onGatewayExit", [this](CPtr<const UI::Value>) -> UI::Value {
            GetApplication().Exit();
            return UI::Value();
        });
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnDetach()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();
        Browser->Unregister("onGatewayLogin");
        Browser->Unregister("onGatewayCreate");
        Browser->Unregister("onGatewayExit");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnPause()
    {
        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnResume()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();
        Browser->Call("setMainScreen", "templates/connect.html");

        // Use the username that we stored when we logged the first time.
        const SStr Username(GetApplication().GetOptions().GetUsername());
        Browser->Call("setAccountUsername", Username);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnConnect()
    {
        switch (mState)
        {
        case State::Idle:
            break;
        case State::Authenticate:
            GetApplication().Send(GatewayAccountLogin(mUsername, mPassword), false);
            break;
        case State::Create:
            GetApplication().Send(GatewayAccountRegister(mUsername, mPassword, mEmail), false);
            break;
        case State::Wait:
            break;
        }

        mState = State::Wait;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnMessage(Ref<Reader> Archive)
    {
        switch (Archive.ReadInt<UInt>())
        {
        case GatewayAccountResult::kID:
            OnAccountResult(GatewayAccountResult(Archive));
            break;
        case GatewayAccountData::kID:
            OnAccountAuthorized(GatewayAccountData(Archive));
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnDisconnect()
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        switch (mState)
        {
        case State::Authenticate:
            Browser->Call("showAccountLoginError", "global_error_offline");
            break;
        case State::Create:
            Browser->Call("showAccountCreateError", "global_error_offline");
            break;
        default:
            break;
        }

        mState = State::Idle;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountLogin(CStr Username, CStr Password)
    {
        if (mState != State::Idle)
        {
            return;
        }

        mState    = State::Authenticate;
        mUsername = Username;
        mPassword = Password;

        GetApplication().Connect(kRemoteAddress, kRemotePort);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountCreate(CStr Username, CStr Password, CStr Email)
    {
        if (mState != State::Idle)
        {
            return;
        }

        mState    = State::Create;
        mUsername = Username;
        mPassword = Password;
        mEmail    = Email;

        GetApplication().Connect(kRemoteAddress, kRemotePort);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountResult(Ref<const GatewayAccountResult> Message)
    {
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        switch (Message.Type)
        {
        case GatewayAccountResult::ID::OK:
            break;
        case GatewayAccountResult::ID::Unknown:
            Browser->Call("showAccountLoginError", "connect_error_unknown");
            break;
        case GatewayAccountResult::ID::Mismatch:
            Browser->Call("showAccountLoginError", "connect_error_password_mismatch");
            break;
        case GatewayAccountResult::ID::Exist:
            Browser->Call("showAccountCreateError", "create_account_error_username_taken");
            break;
        case GatewayAccountResult::ID::Online:
            Browser->Call("showAccountLoginError", "connect_error_user_already_connected");
            break;
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Gateway::OnAccountAuthorized(Ref<const GatewayAccountData> Message)
    {
        // Store the username for next time
        GetApplication().GetOptions().SetUsername(mUsername);

        // Switch to the next activity (Lobby)
        GetApplication().Goto(NewPtr<Lobby>(GetApplication()));

        // Display the character(s) information
        ConstSPtr<UI::Service> Browser = GetApplication().GetSubsystem<UI::Service>();

        for (Ref<const GatewayAccountData::Entity> Entity : Message.Entities)
        {
            Browser->Call("setCharacterInLobby", Entity.ID, Entity.Level, Entity.Name, Entity.Class);
        }
    }
}