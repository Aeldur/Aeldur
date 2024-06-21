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

#include "Application.hpp"
#include "Database/Database.hpp"
#include "Protocol/Endpoint.hpp"
#include <Content/Locator/SystemLocator.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Server
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Application::OnInitialize()
    {
        // Initialize resources
        ConstSPtr<Content::Service> Resources = GetSubsystem<Content::Service>();
        Resources->AddLocator("Root", NewPtr<Content::SystemLocator>("."));
        Resources->AddLocator("Resources", NewPtr<Content::SystemLocator>("Resources"));

        // Load preferences
        mOptions.Load(Resources->Find("Root://Server.toml").GetText());

        // Initialize database
        ConstSPtr<Database> Database = AddSubsystem<Server::Database>();
        if (!Database->Connect(
            mOptions.GetDatabaseAddress(),
            mOptions.GetDatabasePort(),
            mOptions.GetDatabaseUser(),
            mOptions.GetDatabasePassword(),
            mOptions.GetDatabaseDB()))
        {
            LOG_ERROR("Application: Failed to connect to database {}:{}",
                      mOptions.GetDatabaseAddress(),
                      mOptions.GetDatabasePort());
            return false;
        }

        // Initialize endpoint
        ConstSPtr<Endpoint> Endpoint = AddSubsystem<Server::Endpoint>();
        if (!Endpoint->Listen(mOptions.GetEndpointPort(), mOptions.GetEndpointCapacity()))
        {
            LOG_ERROR("Application: Failed to listen to port {}", mOptions.GetEndpointPort());
            return false;
        }
        else
        {
            LOG_INFO("Application: Listening to port {}", mOptions.GetEndpointPort());
        }

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnDestroy()
    {
        // TODO
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Application::OnTick(Real64 Time)
    {
        // TODO
    }
}

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   MAIN   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef    EA_PLATFORM_WINDOWS
    #include <Windows.h>
#endif // EA_PLATFORM_WINDOWS

int main([[maybe_unused]] int Argc, [[maybe_unused]] Ptr<Char> Argv[])
{
#ifdef    EA_PLATFORM_WINDOWS
    ::CoInitialize(nullptr);
#endif // EA_PLATFORM_WINDOWS

    // Initialize 'Aurora Engine' and enter main loop
    UPtr<Aeldur::Server::Application> Server = NewUniquePtr<Aeldur::Server::Application>();
    Server->Initialize(System<Subsystem>::Mode::Server, Engine::Properties());
    Server->Run();

    return 0;
}