#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "banking/Transaction.hpp"
#include "../mocks/MockAccount.hpp"

using namespace banking;
using ::testing::_;
using ::testing::Return;
using ::testing::Exactly;

class TransactionMockTest : public ::testing::Test {
protected:
    void SetUp() override {
        fromMock = std::make_shared<MockAccount>(1, 100.0);
        toMock = std::make_shared<MockAccount>(2, 50.0);
    }
    
    std::shared_ptr<MockAccount> fromMock;
    std::shared_ptr<MockAccount> toMock;
};

TEST_F(TransactionMockTest, TransferUsesWithdrawAndDeposit) {
    // Ожидаем, что withdraw будет вызван с 30.0 и вернёт true
    EXPECT_CALL(*fromMock, withdraw(30.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    // Ожидаем, что deposit будет вызван с 30.0
    EXPECT_CALL(*toMock, deposit(30.0))
        .Times(Exactly(1));
    
    Transaction tx(1, fromMock, toMock, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
}

TEST_F(TransactionMockTest, TransferFailsWhenWithdrawFails) {
    // Ожидаем, что withdraw вернёт false (недостаточно средств)
    EXPECT_CALL(*fromMock, withdraw(30.0))
        .Times(Exactly(1))
        .WillOnce(Return(false));
    
    // Ожидаем, что deposit НЕ будет вызван
    EXPECT_CALL(*toMock, deposit(30.0))
        .Times(0);
    
    Transaction tx(1, fromMock, toMock, 30.0);
    bool result = tx.execute();
    
    EXPECT_FALSE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::FAILED);
}

TEST_F(TransactionMockTest, WithdrawOnlyUsesWithdraw) {
    EXPECT_CALL(*fromMock, withdraw(30.0))
        .Times(Exactly(1))
        .WillOnce(Return(true));
    
    Transaction tx(1, fromMock, nullptr, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
}

TEST_F(TransactionMockTest, DepositOnlyUsesDeposit) {
    EXPECT_CALL(*toMock, deposit(30.0))
        .Times(Exactly(1));
    
    Transaction tx(1, nullptr, toMock, 30.0);
    bool result = tx.execute();
    
    EXPECT_TRUE(result);
    EXPECT_EQ(tx.getStatus(), TransactionStatus::COMPLETED);
}
