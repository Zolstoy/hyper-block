#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "boost/asio/io_context.hpp"
#include "common.hpp"
#include "../instance.hpp"

using namespace boost;

TEST(Instance, InstanciationNominal)
{
    asio::io_context ioc;
    ASSERT_NO_THROW(hyper_block::instance(ioc, 0, CERT, KEY));
}
