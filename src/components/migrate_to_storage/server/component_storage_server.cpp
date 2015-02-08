//  Copyright (c) 2015 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/components/migrate_to_storage/server/component_storage.hpp>

namespace hpx { namespace components { namespace server
{
    naming::gid_type component_storage::migrate_to_here(
        std::vector<char> const& data, naming::id_type id,
        naming::address const& current_lva)
    {
        data_[id.get_gid()] = data;

        // rebind the object to this storage locality
        naming::address addr(current_lva);
        addr.address_ = 0;       // invalidate lva
        if (!agas::bind_sync(id.get_gid(), addr, this->gid_))
        {
            std::ostringstream strm;
            strm << "failed to rebind id " << id
                 << "to storage locality: " << gid_;

            HPX_THROW_EXCEPTION(duplicate_component_address,
                "component_storage::migrate_to_here",
                strm.str());
            return naming::invalid_gid;
        }

        id.make_unmanaged();            // we can now release the object
        return naming::invalid_gid;
    }

    std::vector<char> component_storage::migrate_from_here(naming::gid_type id)
    {
        // return the stored data and erase it from the map
        return data_.get_value_sync(id, true);
    }
}}}

HPX_REGISTER_UNORDERED_MAP(hpx::naming::gid_type, hpx_component_storage_data_type)
