#include<iostream>
#include<vector>

class BankAccount {
private:
  int balance = 0;
  int overdraft_limit = -500;
public:
  void deposit(int amount) {
    balance += amount;
    std::cout << "deposited " << amount << 
      ", balance now " << balance << std::endl;
  }

  bool withdraw(int amount) {
    if (balance - amount >= overdraft_limit) {
      balance -= amount;
      std::cout << "withdraw " << amount << 
        ", balance is now " << balance << std::endl;
      return true;
    }
    return false;
  }
};

// Now we implement this using the Command Pattern

// First create an interface:
struct Command {
  bool success = false;
  virtual void call() /*const*/ = 0;
  // implementing undo operation
  virtual void undo() = 0;
};

// Next we create commands for BankAccount
struct BankAccountCommand : public Command {
  BankAccount& account;
  enum Action {deposit, withdraw} action;
  int amount;
  bool withdraw_succeeded;

  BankAccountCommand(BankAccount& _account, const Action _action,
    const int _amount) : account(_account), action(_action),
      amount(_amount), withdraw_succeeded(false) {}
  
  void call() /*const*/ override {
    switch (action)
    {
    default : break;
    case deposit:
      account.deposit(amount);
      break;
    case withdraw:
      withdraw_succeeded = account.withdraw(amount);
      break;
    }
  }

  void undo() /*const*/ override {
    switch (action)
    {
    default : break;
    case deposit:
      account.withdraw(amount);
      break;
    case withdraw:
      if(withdraw_succeeded)
        account.deposit(amount);
      break;
    }
  }

};

// Composite Bank Account Command

struct CompositeBankAccountCommand : std::vector<BankAccountCommand>, 
                                     Command
{
  CompositeBankAccountCommand(const std::initializer_list<value_type>& items)
    : std::vector<BankAccountCommand>(items) {}
  

  void call() override {
    bool ok = true;
    for(auto& cmd : *this) {
      if(ok) {
        cmd.call();
        ok = cmd.success;
      }
      else
        cmd.success = false;
    }
  }
  
  void undo() override {
    for(auto it = rbegin(); it!= rend(); ++it)
      it->undo();
  }
                                    
};

struct MoneyTransferCommand : CompositeBankAccountCommand{
  MoneyTransferCommand(BankAccount& from, BankAccount& to, 
        int amount) : CompositeBankAccountCommand {
          BankAccountCommand {from, BankAccountCommand::withdraw, amount},
          BankAccountCommand {to, BankAccountCommand::deposit, amount},
        } {}
};

int main() {
  // BankAccount b1;
  // b1.deposit(100);
  // b1.withdraw(50);

  // Using Command pattern
  // BankAccount b2;
  // BankAccountCommand cmd{b2,BankAccountCommand::deposit, 100};
  // cmd.call();

  BankAccount b1, b2;
  BankAccountCommand init {b1, BankAccountCommand::deposit, 1000};
  init.call();
  MoneyTransferCommand m1{b1,b2,100};
  m1.call();
}