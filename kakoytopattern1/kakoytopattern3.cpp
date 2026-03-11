#include <iostream>
#include <string>

// Контекст: Автомат
class VendingMachine;

// Абстрактное состояние
class State {
public:
    virtual void insertCoin(VendingMachine* vm) = 0;
    virtual void dispense(VendingMachine* vm) = 0;
};

// Конкретные состояния
class NoCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override;
    void dispense(VendingMachine* vm) override { std::cout << "Вставьте монету сначала!" << std::endl; }
};

class HasCoinState : public State {
public:
    void insertCoin(VendingMachine* vm) override { std::cout << "Монета уже вставлена!" << std::endl; }
    void dispense(VendingMachine* vm) override;
};

class VendingMachine {
private:
    State* currentState;
public:
    VendingMachine() {
        currentState = new NoCoinState();
    }
    ~VendingMachine() { delete currentState; }
    void setState(State* s) { delete currentState; currentState = s; }
    void insertCoin() { currentState->insertCoin(this); }
    void dispense() { currentState->dispense(this); }
};

void NoCoinState::insertCoin(VendingMachine* vm) {
    std::cout << "Монета вставлена." << std::endl;
    vm->setState(new HasCoinState());
}

void HasCoinState::dispense(VendingMachine* vm) {
    std::cout << "Напиток выдан!" << std::endl;
    vm->setState(new NoCoinState());
}

int main() {
    VendingMachine vm;
    vm.dispense();  // Вывод: Вставьте монету сначала!
    vm.insertCoin();  // Вывод: Монета вставлена.
    vm.dispense();  // Вывод: Напиток выдан!
    return 0;
}