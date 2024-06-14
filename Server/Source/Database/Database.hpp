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

#include <Core/Core.hpp>
#include <pqxx/pqxx>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Server
{
    // -=(Undocumented)=-
    // @TODO: Connection Pool + Async
    class Database final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        Database(Ref<Context> Context);

        // -=(Undocumented)=-
        Bool Connect(CStr Hostname, UInt16 Port, CStr Username, CStr Password, CStr Database);

        // -=(Undocumented)=-
        template<typename... Args>
        pqxx::result Query(CStr Statement, Args ... Values)
        {
            pqxx::result Result;
            try
            {
                pqxx::work Transaction(* mConnection);
                Result = Transaction.exec_params(pqxx::zview(Statement), Values...);
                Transaction.commit();
            }
            catch (Ref<const pqxx::sql_error> Exception)
            {
                Result = pqxx::result();
            }
            return Result;
        }

        // -=(Undocumented)=-
        template<typename Type, typename Factory, typename... Args>
        Vector<Type> Query(CStr Statement, Factory OnRead, Args ... Values)
        {
            const pqxx::result Result = Query(Statement, Values...);

            Vector<Type> Collection;
            Collection.reserve(Result.size());

            for (Ref<const pqxx::row> Entry : Result)
            {
                Collection.emplace_back(OnRead(Entry));
            }
            return Collection;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<pqxx::connection> mConnection;
    };
}
