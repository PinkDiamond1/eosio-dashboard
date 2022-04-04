#include "include/tester-base.hpp"

TEST_CASE("add faucet") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>("faucet1"_n, 5, 200000);

   CHECK(get_table_size<faucets::faucet_table>() == 1);
}

TEST_CASE("add duplicated faucet") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>(
      "faucet1"_n,
      5,
      200000);

   expect(chain.as("faucet"_n).trace<faucets::actions::addfaucet>(
              "faucet1"_n,
              6,
              200000),
         "Faucet account already exist and is active");
}

TEST_CASE("remove existing faucet") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>(
      "faucet1"_n,
      5,
      200000);
   chain.as("faucet"_n).act<faucets::actions::rmfaucet>("faucet1"_n);

   CHECK(true);
}

TEST_CASE("remove unexisting faucet") {
   test_chain chain;
   setup(chain);

   expect(chain.as("faucet"_n).trace<faucets::actions::rmfaucet>("faucet1"_n),
         "Faucet does not exist");
}

TEST_CASE("send tokens with wrong authority") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>("faucet1"_n, 5, 200000);

   expect(chain.as("faucet"_n).trace<faucets::actions::givetokens>("faucet1"_n, "testacc"_n), "missing authority of faucet1");
}

TEST_CASE("error sending max tokens per interval") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>("faucet1"_n, 5, 20);
   chain.as("faucet1"_n).act<faucets::actions::givetokens>("faucet1"_n, "testacc"_n);
   chain.as("faucet1"_n).act<faucets::actions::givetokens>("faucet1"_n, "testacc2"_n);
   expect(chain.as("faucet1"_n).trace<faucets::actions::givetokens>("faucet1"_n, "testacc3"_n), "Not so fast...");
}

TEST_CASE("send max tokens within the interval") {
   test_chain chain;
   setup(chain);

   chain.as("faucet"_n).act<faucets::actions::addfaucet>("faucet1"_n, 5, 30);
   chain.as("faucet1"_n).act<faucets::actions::givetokens>("faucet1"_n, "testacc"_n);
   chain.as("faucet1"_n).act<faucets::actions::givetokens>("faucet1"_n, "testacc2"_n);
   chain.as("faucet1"_n).act<faucets::actions::givetokens>("faucet1"_n, "testacc3"_n);

   CHECK(true);
}
