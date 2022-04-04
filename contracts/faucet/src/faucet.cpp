#include <eosio/system.hpp>

#include "../include/faucet.hpp"

namespace faucets {
  void contract::addfaucet(name account, eosio::time_point_sec interval, uint32_t max_tokens_per_interval) {
    require_auth(get_self());

    faucet_table _faucet(get_self(), get_self().value);
    auto faucet_itr = _faucet.find(account.value);

    if(faucet_itr == _faucet.end()) {
      _faucet.emplace(get_self(), [&](auto& row) {
        row.account = account;
        row.interval = interval;
        row.max_tokens_per_interval = max_tokens_per_interval;
        row.is_active = true;
        row.started_at = current_time_point();
        row.transferred_tokens = 0;
      });

      return;
    }

    check(faucet_itr != _faucet.end() && !faucet_itr->is_active, "Faucet account already exist and is active");

    _faucet.modify(faucet_itr, get_self(), [&]( auto& row ) {
      row.interval = interval;
      row.max_tokens_per_interval = max_tokens_per_interval;
      row.is_active = true;
      row.started_at = current_time_point();
      row.transferred_tokens = 0;
    });
  }

  void contract::rmfaucet(name account) {
    require_auth(get_self());

    faucet_table _faucet(get_self(), get_self().value);
    auto faucet_itr = _faucet.find(account.value);

    check(faucet_itr != _faucet.end() && faucet_itr->is_active, "Faucet does not exist");

    _faucet.modify(faucet_itr, get_self(), [&]( auto& row ) {
      row.is_active = false;
    });
  }

  void contract::givetokens(name faucet, name to) {
    require_auth(faucet);

    faucet_table _faucet(get_self(), get_self().value);
    auto faucet_itr = _faucet.find(faucet.value);

    check(faucet_itr != _faucet.end() && faucet_itr->is_active, "Faucet does not exist or is not active");

    if(current_time_point() - faucet_itr->started_at >= seconds(faucet_itr->interval.sec_since_epoch())) {
      _faucet.modify(faucet_itr, get_self(), [&]( auto& row ) {
        row.started_at = current_time_point();
        row.transferred_tokens = 0;
      });
    }

    check(
      current_time_point() - faucet_itr->started_at <= 
        seconds(faucet_itr->interval.sec_since_epoch()) &&
      faucet_itr->transferred_tokens < faucet_itr->max_tokens_per_interval, "Not so fast...");

    _faucet.modify(faucet_itr, get_self(), [&]( auto& row ) {
      row.transferred_tokens = faucet_itr->transferred_tokens + 10;
    });

    asset amount = asset(10, symbol("EOS", 4));
    string memo = "Faucet transfer";

    eosio::action(
      permission_level {get_self(), "active"_n},
      "eosio.token"_n,
      "transfer"_n,
      std::make_tuple(get_self(), to, amount, memo)
    ).send();
  }
} // namespace faucets

EOSIO_ACTION_DISPATCHER(faucets::actions)
EOSIO_ABIGEN(actions(faucets::actions),
             ricardian_clause("datastorage", faucets::datastorage_clause),
             ricardian_clause("datausage", faucets::datausage_clause),
             ricardian_clause("dataownership",
                              faucets::dataownership_clause),
             ricardian_clause("datadistribution",
                              faucets::datadistribution_clause),
             ricardian_clause("datafuture", faucets::datafuture_clause))