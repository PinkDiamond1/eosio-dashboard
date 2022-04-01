// cltester definitions
#include <eosio/tester.hpp>

// contract definitions
#include "include/faucet.hpp"

// Catch2 unit testing framework. https://github.com/catchorg/Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

using namespace eosio;

void setup_chain(test_chain& t)
{
   t.create_code_account("faucet"_n);
   t.set_code("faucet"_n, "faucet.wasm");
}

void setup(test_chain& t)
{
   setup_chain(t);
}

// TEST_CASE("Wrong uuid length")
// {
//    test_chain chain;
//    setup(chain);

//    expect(chain.as("faucet"_n).trace<faucet::actions::addsub>(
//               "5d05a2a0-96a3-4b8e-b6e8-bdf30e76957",
//               "33799d59a1bcc308325d3e767e8718e479a5493b8c57d655feaf1dc8e51aa723",
//               "9714f9d908e887e8f71b1c3a82a795dbc2f4b1a10752ae38f02923c18546ab8f"),
//          "The uuid length must be exactly 36 characters");
// }
