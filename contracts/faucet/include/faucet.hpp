#pragma once

#include <eosio/eosio.hpp>
#include "../ricardian/faucet_ricardian.hpp"

using namespace std;
using namespace eosio;

namespace faucets {
  struct faucet {
    name account;
    eosio::time_point_sec interval;
    uint32_t max_tokens_per_interval;
    bool is_active;

    uint64_t primary_key() const { return account.value; }
  };
  EOSIO_REFLECT(
    faucet,
    account,
    interval,
    max_tokens_per_interval,
    is_active
  )
  typedef eosio::multi_index<"faucets"_n, faucet > faucet_table;

  class contract : public eosio::contract {
    public:
      using eosio::contract::contract;

      // Admin only
      void addfaucet(name account, eosio::time_point_sec interval, uint32_t max_tokens_per_interval);
      void rmfaucet(name account);

      // Faucet only
      void givetokens(name faucet, name to);
  };

  EOSIO_ACTIONS(contract, "faucet"_n,
                action(addfaucet, account, max_tokens_per_interval),
                action(rmfaucet, account),
                action(givetokens, faucet, to))
} // namespace faucets