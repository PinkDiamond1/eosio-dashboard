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
   t.create_code_account("test"_n);
   t.create_code_account("faucet"_n);
   t.set_code("faucet"_n, "faucet.wasm");
}

void setup(test_chain& t)
{
   setup_chain(t);
}

TEST_CASE("add faucet") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>(
      "faucet1"_n,
      123,
      200000);

   CHECK(true);
}

TEST_CASE("add duplicated faucet") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>(
      "faucet1"_n,
      123,
      200000);

   expect(chain.as("faucet"_n).trace<faucets::actions::addfaucet>(
              "faucet1"_n,
              1234,
              200000),
         "Faucet account already exist and is active");
}
