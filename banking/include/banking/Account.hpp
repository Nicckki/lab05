#ifndef BANKING_ACCOUNT_HPP
#define BANKING_ACCOUNT_HPP

#include <mutex>

namespace banking {

class Account {
private:
    int id;
    double balance;
    mutable std::mutex mtx;

public:
    explicit Account(int id);
    Account(int id, double initial_balance);
    
    int getId() const;
    double getBalance() const;
    void deposit(double amount);
    bool withdraw(double amount);
    
    void lock() const;
    void unlock() const;
};

} // namespace banking

#endif // BANKING_ACCOUNT_HPP
