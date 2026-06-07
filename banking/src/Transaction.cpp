#include "banking/Transaction.hpp"

namespace banking {

Transaction::Transaction(int id, std::shared_ptr<Account> from, 
                         std::shared_ptr<Account> to, double amount)
    : id(id), fromAccount(from), toAccount(to), amount(amount), 
      status(TransactionStatus::PENDING) {
    timestamp = std::chrono::system_clock::now();
}

bool Transaction::execute() {
    if (status != TransactionStatus::PENDING) {
        return false;
    }
    
    // Блокируем оба счёта для потокобезопасности
    if (fromAccount) fromAccount->lock();
    if (toAccount) toAccount->lock();
    
    bool success = false;
    
    if (fromAccount && toAccount) {
        // Перевод между счетами
        if (fromAccount->withdraw(amount)) {
            toAccount->deposit(amount);
            success = true;
        }
    } else if (fromAccount && !toAccount) {
        // Только списание
        success = fromAccount->withdraw(amount);
    } else if (!fromAccount && toAccount) {
        // Только пополнение
        toAccount->deposit(amount);
        success = true;
    }
    
    if (fromAccount) fromAccount->unlock();
    if (toAccount) toAccount->unlock();
    
    status = success ? TransactionStatus::COMPLETED : TransactionStatus::FAILED;
    return success;
}

void Transaction::cancel() {
    if (status == TransactionStatus::PENDING) {
        status = TransactionStatus::CANCELLED;
    }
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

double Transaction::getAmount() const {
    return amount;
}

int Transaction::getId() const {
    return id;
}

} // namespace banking
