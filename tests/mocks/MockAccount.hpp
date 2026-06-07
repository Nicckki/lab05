#ifndef MOCK_ACCOUNT_HPP
#define MOCK_ACCOUNT_HPP

#include <gmock/gmock.h>
#include "banking/Account.hpp"

namespace banking {

class MockAccount : public Account {
public:
    MockAccount(int id) : Account(id) {}
    MockAccount(int id, double balance) : Account(id, balance) {}
    
    MOCK_METHOD(int, getId, (), (const, override));
    MOCK_METHOD(double, getBalance, (), (const, override));
    MOCK_METHOD(void, deposit, (double amount), (override));
    MOCK_METHOD(bool, withdraw, (double amount), (override));
    MOCK_METHOD(void, lock, (), (const, override));
    MOCK_METHOD(void, unlock, (), (const, override));
};

} // namespace banking

#endif // MOCK_ACCOUNT_HPP
