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

#include "UserRepository.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Aeldur::Server
{
    // -=(Undocumented)=-
    class UserService final
    {
    public:

        // -=(Undocumented)=-
        enum class Error
        {
            None,
            Invalid,
            Mismatch,
            Exist,
            Online,
        };

    public:

        // -=(Undocumented)=-
        UserService(Ref<Subsystem::Context> Context);

        // -=(Undocumented)=-
        Error Login(CStr Username, CStr Password);

        // -=(Undocumented)=-
        Error Logout(UInt ID);

        // -=(Undocumented)=-
        Error Create(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        Error Delete(CStr Username, CStr Password);

        // -=(Undocumented)=-
        SPtr<User> GetByID(UInt ID);

        // -=(Undocumented)=-
        SPtr<User> GetByUsername(CStr Username);

    private:

        // -=(Undocumented)=-
        Bool CheckUsername(CStr Username);

        // -=(Undocumented)=-
        Bool CheckEmail(CStr Email);

        // -=(Undocumented)=-
        Bool CheckPassword(CStr Password);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UserRepository          mRepository;
        Table<UInt, SPtr<User>> mRegistry;
    };
}