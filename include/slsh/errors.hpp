// Stefan Olivier
// Description: Error handling

# pragma once

///////////////////////////////////////////////////////////////////////////

// Standard headers
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

// Project headers
#include "traits.hpp"

///////////////////////////////////////////////////////////////////////////

namespace slsh::errors {

// @SECTION Error Signals

using error_t = int;
enum Signal : error_t
{
    success,
    fail,
    fail_panic,
    fail_assert,
    unknown
};

} // @END OF namespace slsh::errors

///////////////////////////////////////////////////////////////////////////

namespace slsh::errors::internal
{

// @SECTION Error Functions

template<typename Tstring>
inline auto signal_msg(error_t errcode) -> std::remove_cvref_t<Tstring>
{
    constexpr std::string_view default_msg{ "unknown program failure" };
    switch (errcode) {
        case Signal::success:     return "exited successfully";
        case Signal::fail:        return "program execution failed";
        case Signal::fail_panic:  return "program panicked during execution";
        case Signal::fail_assert: return "failed assertion caused execution to be terminated";
        default:                  return default_msg;
    }

    return default_msg;
}

} // @END OF namespace slsh::errors::internal

///////////////////////////////////////////////////////////////////////////

// @SECTION Error and Diagnostic Macros
#ifndef VTM_ERROR_MACROS
#define VTM_ERROR_MACROS

#define __SIGNAL_MSG(s) slsh::errors::internal::signal_msg<std::string_view>(s)

#define __DIAGMSG_PREQUIT(program, signal) (std::cerr << "\n" << program                                    \
                                                      << ": program exited with signal: " << signal << "\n" \
                                                      << "DESCRIPTION: " <<  __SIGNAL_MSG(signal) << "\n\n")


#define __DIAGMSG_NOQUIT(prefix, msg) (std::cerr << prefix << ": "                        \
                                                 << __FILE__ << ":(" << __LINE__ << "): " \
                                                 << __func__ << ": " << msg << "\n")

#define __DIAGMSG_QUIT(prefix, msg, signal) __DIAGMSG_NOQUIT(prefix, msg); \
                                            (std::exit(signal))

#define __DIAGMSG_PANIC(msg) __DIAGMSG_NOQUIT("FATAL", msg); \
                             (std::abort())
 
#define __DIAGMSG_ASSERT(prefix, expr, msg) if (!(expr)) { __DIAGMSG_PREQUIT("APPLICATION", slsh::errors::Signal::fail_assert); \
                                                           __DIAGMSG_NOQUIT(prefix, "Assertion failed with: "#expr"\n"#msg);   \
                                                           std::abort();                                                       \
                                                         }

#define __DIAGMSG_NOQUIT_IF(prefix, expr, msg) if (!(expr)) { __DIAGMSG_NOQUIT(prefix, msg); }

#define VTM_ASSERT(expr, msg)   __DIAGMSG_ASSERT("FATAL", expr, msg)
#define VTM_ERROR(msg)          __DIAGMSG_NOQUIT("ERROR", msg)
#define VTM_ERROR_IF(expr, msg) __DIAGMSG_NOQUIT_IF("ERROR", expr, msg)
#define VTM_PANIC(msg)          __DIAGMSG_PREQUIT("APPLICATION", slsh::errors::Signal::fail_panic); __DIAGMSG_PANIC(msg)
#define VTM_TODO(msg)           __DIAGMSG_PREQUIT("APPLICATION", slsh::errors::Signal::fail); __DIAGMSG_QUIT("TODO", msg, slsh::errors::Signal::fail)
#define VTM_WARN(msg)           __DIAGMSG_NOQUIT("WARNING", msg)
#define VTM_WARN_IF(expr, msg)  __DIAGMSG_NOQUIT_IF("WARNING", expr, msg)

#endif // @END OF VTM_ERROR_MACROS

///////////////////////////////////////////////////////////////////////////
