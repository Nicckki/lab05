#include <gtest/gtest.h>
#include "banking/Transaction.hpp"
#include "banking/Account.hpp"
#include <memory>

using namespace banking;

TEST(TransactionTest, TransferBetweenAccounts) {
    auto from = std::make_shared<Account>(1, 100.0);
    auto to = std::make_shared<Account>(2, 50.0);
    
    Transaction tx(1, from, to, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
    EXPECT_DOUBLE_EQ(from->getBalance(), 70.0);
    EXPECT_DOUBLE_EQ(to->getBalance(), 80.0);
}

TEST(TransactionTest, InsufficientFunds) {
    auto from = std::make_shared<Account>(1, 100.0);
    auto to = std::make_shared<Account>(2, 50.0);
    
    Transaction tx(1, from, to, 150.0);
    bool result = tx.execute();
    
    EXPECT_FALSE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::FAILED);
    EXPECT_DOUBLE_EQ(from->getBalance(), 100.0);
    EXPECT_DOUBLE_EQ(to->getBalance(), 50.0);
}

TEST(TransactionTest, DepositOnly) {
    auto to = std::make_shared<Account>(2, 50.0);
    
    Transaction tx(1, nullptr, to, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
    EXPECT_DOUBLE_EQ(to->getBalance(), 80.0);
}

TEST(TransactionTest, WithdrawOnly) {
    auto from = std::make_shared<Account>(1, 100.0);
    
    Transaction tx(1, from, nullptr, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
    EXPECT_DOUBLE_EQ(from->getBalance(), 70.0);
}

TEST(TransactionTest, CancelPendingTransaction) {
    auto from = std::make_shared<Account>(1, 100.0);
    auto to = std::make_shared<Account>(2, 50.0);
    
    Transaction tx(1, from, to, 30.0);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::PENDING);
    
    tx.cancel();
    EXPECT_EQ(tx.getStatus(), TransactionStatus::CANCELLED);
}

TEST(TransactionTest, ExecuteAfterCancelDoesNothing) {
    auto from = std::make_shared<Account>(1, 100.0);
    auto to = std::make_shared<Account>(2, 50.0);
    
    Transaction tx(1, from, to, 30.0);
    tx.cancel();
    
    bool result = tx.execute();
    EXPECT_FALSE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::CANCELLED);
    EXPECT_DOUBLE_EQ(from->getBalance(), 100.0);
    EXPECT_DOUBLE_EQ(to->getBalance(), 50.0);
}
