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
    
    virtual ~Account() = default;
    
    virtual int getId() const;
    virtual double getBalance() const;
    virtual void deposit(double amount);
    virtual bool withdraw(double amount);
    
    virtual void lock() const;
    virtual void unlock() const;
};

} // namespace banking

#endif // BANKING_ACCOUNT_HPP
