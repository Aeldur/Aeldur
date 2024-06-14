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

#include "AccountRepository.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Server
{
    // -=(Undocumented)=-
    class AccountService final : public Subsystem
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
        AccountService(Ref<Context> Context);

        // -=(Undocumented)=-
        Error Login(CStr Username, CStr Password);

        // -=(Undocumented)=-
        Error Logout(UInt ID);

        // -=(Undocumented)=-
        Error Create(CStr Username, CStr Password, CStr Email);

        // -=(Undocumented)=-
        Error Delete(CStr Username, CStr Password);

        // -=(Undocumented)=-
        SPtr<Account> GetByID(UInt ID);

        // -=(Undocumented)=-
        SPtr<Account> GetByUsername(CStr Username);

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

        AccountRepository          mRepository;
        Table<UInt, SPtr<Account>> mRegistry;
    };
}