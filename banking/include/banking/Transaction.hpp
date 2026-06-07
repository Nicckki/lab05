#ifndef BANKING_TRANSACTION_HPP
#define BANKING_TRANSACTION_HPP

#include "Account.hpp"
#include <memory>
#include <chrono>

namespace banking {

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    CANCELLED
};

class Transaction {
private:
    int id;
    std::shared_ptr<Account> fromAccount;
    std::shared_ptr<Account> toAccount;
    double amount;
    TransactionStatus status;
    std::chrono::system_clock::time_point timestamp;

public:
    Transaction(int id, std::shared_ptr<Account> from, std::shared_ptr<Account> to, double amount);
    
    bool execute();
    void cancel();
    TransactionStatus getStatus() const;
    double getAmount() const;
    int getId() const;
};

} // namespace banking

#endif // BANKING_TRANSACTION_HPP
