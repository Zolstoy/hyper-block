#pragma once

namespace hyper_block {

enum class error
{
    invalid_certificate      = 1,
    invalid_private_key      = 2,
    instance_already_running = 3,
    acceptor_failed          = 4
};

}
