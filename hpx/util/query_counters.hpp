//  Copyright (c) 2007-2012 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_UTIL_QUERY_COUNTERS_SEP_27_2011_0255PM)
#define HPX_UTIL_QUERY_COUNTERS_SEP_27_2011_0255PM

#include <hpx/config.hpp>
#include <hpx/exception_fwd.hpp>
#include <hpx/lcos/local/mutex.hpp>
#include <hpx/performance_counters/counters.hpp>
#include <hpx/util/interval_timer.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include <hpx/config/warnings_prefix.hpp>

namespace hpx { namespace util
{
    ///////////////////////////////////////////////////////////////////////////
    class HPX_EXPORT query_counters
    {
        // avoid warning about using this in member initializer list
        query_counters* this_() { return this; }

    public:
        query_counters(std::vector<std::string> const& names,
            std::int64_t interval, std::string const& dest,
            std::string const& form, std::vector<std::string> const& shortnames,
            bool csv_header);

        void start();
        void stop_evaluating_counters();
        bool evaluate();
        void terminate();

        void start_counters(error_code& ec = throws);
        void stop_counters(error_code& ec = throws);
        void reset_counters(error_code& ec = throws);
        bool evaluate_counters(bool reset = false,
            char const* description = nullptr, error_code& ec = throws);

    protected:
        void find_counters();
        bool find_counter(performance_counters::counter_info const& info,
            error_code& ec);

        bool print_raw_counters(bool destination_is_cout, bool reset,
            char const* description, std::vector<id_type> const& ids,
            error_code& ec);
        bool print_array_counters(bool destination_is_cout, bool reset,
            char const* description, std::vector<id_type> const& ids,
            error_code& ec);

        template <typename Stream>
        void print_headers(Stream& output);

        template <typename Stream, typename Future>
        void print_values(Stream& output, std::vector<Future> &&,
            std::vector<std::size_t> && indicies);

        template <typename Stream>
        void print_value(Stream& out, std::string const& name,
            performance_counters::counter_value const& value,
            std::string const& uom);
        template <typename Stream>
        void print_value(Stream& out, std::string const& name,
            performance_counters::counter_values_array const& value,
            std::string const& uom);

        template <typename Stream>
        void print_name_csv(Stream& out,
            std::string const& name);

        template <typename Stream>
        void print_value_csv(Stream& out,
            performance_counters::counter_value const& value);
        template <typename Stream>
        void print_value_csv(Stream& out,
            performance_counters::counter_values_array const& value);

        template <typename Stream>
        void print_name_csv_short(Stream& out,
            std::string const& name);

    private:
        typedef lcos::local::mutex mutex_type;

        mutex_type mtx_;

        std::vector<std::string> names_;      // counter instance names
        std::vector<naming::id_type> ids_;    // gids of counter instances
        std::vector<std::string> uoms_;       // units of measure
        std::vector<performance_counters::counter_type> types_; // counter type

        std::string destination_;
        std::string format_;
        std::vector<std::string> counter_shortnames_;
        bool csv_header_;

        interval_timer timer_;
    };
}}

#include <hpx/config/warnings_suffix.hpp>

#endif
