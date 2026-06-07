#include "banking/Account.hpp"
#include <stdexcept>

namespace banking {

Account::Account(int id) : id(id), balance(0.0) {}

Account::Account(int id, double initial_balance) : id(id), balance(initial_balance) {
    if (initial_balance < 0) {
        throw std::invalid_argument("Initial balance cannot be negative");
    }
}

int Account::getId() const {
    return id;
}

double Account::getBalance() const {
    return balance;
}

void Account::deposit(double amount) {
    if (amount < 0) {
        throw std::invalid_argument("Deposit amount cannot be negative");
    }
    balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount < 0) {
        throw std::invalid_argument("Withdrawal amount cannot be negative");
    }
    if (amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}

void Account::lock() const {
    mtx.lock();
}

void Account::unlock() const {
    mtx.unlock();
}

} // namespace banking
