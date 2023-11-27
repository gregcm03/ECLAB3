#include <iostream>
#include <iomanip>
#include <string>

class BankAccount
{
protected:
  std::string accountID;
  std::string accountHolder;
  double accountBalance;

public:
  BankAccount(const std::string &id, const std::string &holder, double initialBalance)
      : accountID(id), accountHolder(holder), accountBalance(initialBalance) {}

  virtual void showDetails() const
  {
    std::cout << "Account Details for " << getType() << " (ID: " << accountID << "):" << std::endl;
    std::cout << "   Holder: " << accountHolder << std::endl;
    std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << accountBalance << std::endl;
  }

  virtual void deposit(double amount)
  {
    accountBalance += amount;
  }

  virtual void withdraw(double amount)
  {
    if (amount > accountBalance)
    {
      std::cout << "Withdrawal amount exceeds the balance!" << std::endl;
    }
    else
    {
      accountBalance -= amount;
    }
  }

  virtual std::string getType() const
  {
    return "Bank Account";
  }

  double getBalance() const
  {
    return accountBalance;
  }

  friend std::ostream &operator<<(std::ostream &os, const BankAccount &acc);
};

std::ostream &operator<<(std::ostream &os, const BankAccount &acc)
{
  acc.showDetails();
  return os;
}

class Savings : public BankAccount
{
private:
  double interestRate;

public:
  Savings(const std::string &id, const std::string &holder, double initialBalance, double rate)
      : BankAccount(id, holder, initialBalance), interestRate(rate) {}

  void showDetails() const override
  {
    BankAccount::showDetails();
    std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << (interestRate * 100) << "%" << std::endl;
  }

  void withdraw(double amount) override
  {
    const double minimumBalance = 100;
    if (accountBalance - amount < minimumBalance)
    {
      std::cout << "Withdrawal not possible. Minimum balance should be maintained." << std::endl;
    }
    else
    {
      accountBalance -= amount;
    }
  }

  std::string getType() const override
  {
    return "Savings Account";
  }
};

class Checking : public BankAccount
{
private:
  double overdraftLimit;

public:
  Checking(const std::string &id, const std::string &holder, double initialBalance, double limit)
      : BankAccount(id, holder, initialBalance), overdraftLimit(limit) {}

  void showDetails() const override
  {
    BankAccount::showDetails();
    std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
  }

  void withdraw(double amount) override
  {
    if (amount > (accountBalance + overdraftLimit))
    {
      std::cout << "Withdrawal not possible. Amount exceeds overdraft limit." << std::endl;
    }
    else
    {
      accountBalance -= amount;
    }
  }

  std::string getType() const override
  {
    return "Checking Account";
  }
};

Checking operator+(Checking &checking, Savings &savings)
{
  double transferAmount = 300;
  if (savings.getBalance() >= transferAmount)
  {
    checking.deposit(transferAmount);
    savings.withdraw(transferAmount);
  }
  else
  {
    std::cout << "Transfer not possible. Insufficient amount in savings account." << std::endl;
  }

  return checking;
}

int main()
{
  Savings savings("S123", "John Doe", 1000, 0.02);
  Checking checking("C456", "Jane Doe", 2000, 500);

  std::cout << savings << std::endl;
  std::cout << " " << std::endl;
  std::cout << checking << std::endl;
  std::cout << " " << std::endl;

  savings.deposit(500);
  checking.withdraw(1000);

  std::cout << "Account Details after deposit and withdrawal:" << std::endl;
  std::cout << savings << std::endl;
  std::cout << " " << std::endl;
  std::cout << checking << std::endl;
  std::cout << " " << std::endl;

  // Transfer 300 from savings to checking
  checking = checking + savings;

  std::cout << "Account Details after transfer:" << std::endl;
  std::cout << savings << std::endl;
  std::cout << " " << std::endl;
  std::cout << checking << std::endl;
  std::cout << " " << std::endl;

  return 0;
}