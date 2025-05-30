#pragma once
#include <variant>

namespace hyper_block {

namespace step {

struct events {
    events() = delete;
    struct server {
        struct accept {
        };
    };
    struct session {
        struct handshake {
        };
    };
};

using event    = std::variant<events::server::accept, events::session::handshake>;
using callback = bool(event&&);

}   // namespace step
}   // namespace hyper_block

#define HYPERBLOCK_STEP(event_type)                                 \
    do                                                              \
    {                                                               \
        if (!step_callback_(step::event{step::events::event_type})) \
            return;                                                 \
    } while (0)
