#include <iostream>
#include <string>
#include <memory>
using namespace std;

// Абстрактные продукты
class Button {
public:
    virtual ~Button() = default;
    virtual void render() const = 0;
    virtual void interact() const = 0;  // Метод взаимодействия
};

class CheckBox {
public:
    virtual ~CheckBox() = default;
    virtual void render() const = 0;
    virtual void interact() const = 0;  // Метод взаимодействия
};

//  Конкретные продукты для Windows 
class WinButton : public Button {
public:
    void render() const override {
        cout << "Рендеринг кнопки в стиле Windows" << endl;
    }

    void interact() const override {
        cout << "Клик по Windows кнопке! Звук: 'динь'" << endl;
    }
};

class WinCheckBox : public CheckBox {
public:
    void render() const override {
        cout << "Рендеринг чекбокса в стиле Windows" << endl;
    }

    void interact() const override {
        cout << "Клик по Windows чекбоксу! Статус изменен" << endl;
    }
};

//  Конкретные продукты для Linux 
class LinuxButton : public Button {
public:
    void render() const override {
        cout << "Рендеринг кнопки в стиле Linux (GTK)" << endl;
    }

    void interact() const override {
        cout << "Клик по Linux кнопке! Звук: 'тук'" << endl;
    }
};

class LinuxCheckBox : public CheckBox {
public:
    void render() const override {
        cout << "Рендеринг чекбокса в стиле Linux (GTK)" << endl;
    }

    void interact() const override {
        cout << "Клик по Linux чекбоксу! Статус изменен" << endl;
    }
};

// Абстрактная фабрика 
class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual unique_ptr<Button> createButton() const = 0;
    virtual unique_ptr<CheckBox> createCheckBox() const = 0;
};

//  Конкретные фабрики 
class WinFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() const override {
        return make_unique<WinButton>();
    }

    unique_ptr<CheckBox> createCheckBox() const override {
        return make_unique<WinCheckBox>();
    }
};

class LinuxFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() const override {
        return make_unique<LinuxButton>();
    }

    unique_ptr<CheckBox> createCheckBox() const override {
        return make_unique<LinuxCheckBox>();
    }
};

//  Клиентский код 
void renderUI(const GUIFactory& factory) {
    auto button = factory.createButton();
    auto checkbox = factory.createCheckBox();

    button->render();
    checkbox->render();

    cout << "--- Взаимодействие с элементами ---" << endl;
    button->interact();
    checkbox->interact();
}

/*
 * Почему Abstract Factory лучше отдельных фабрик для каждого продукта:
 * 1. Согласованность семейств продуктов: Abstract Factory гарантирует,
 *    что создаваемые продукты совместимы друг с другом 
 * 2. Инкапсуляция: Логика создания целого семейства продуктов находится в одном
 *    месте, что упрощает поддержку и замену целых семейств.
 * 3. Масштабируемость: Добавление нового семейства продуктов 
 *    требует создания только одной новой фабрики, а не модификации множества
 *    отдельных фабрик.
 * 4. Единая точка конфигурации: Всё приложение можно легко переключить на другое
 *    семейство продуктов, изменив только выбранную фабрику.
 */

int main() {
    string osType;

    // Выбор фабрики по строке
    cout << "Выберите ОС (win/linux): ";
    getline(cin, osType);

    unique_ptr<GUIFactory> factory;

    if (osType == "win") {
        factory = make_unique<WinFactory>();
        cout << "\n=== Создание интерфейса для Windows ===\n" << endl;
    }
    else if (osType == "linux") {
        factory = make_unique<LinuxFactory>();
        cout << "\n=== Создание интерфейса для Linux ===\n" << endl;
    }
    else {
        cout << "Неизвестная ОС. Используется Windows по умолчанию." << endl;
        factory = make_unique<WinFactory>();
    }

    // Рендерим интерфейс с выбранной фабрикой
    renderUI(*factory);

    // Демонстрация переключения между разными фабриками
    cout << "\n=== Переключение на другую платформу ===\n" << endl;

    auto linuxUI = LinuxFactory();
    renderUI(linuxUI);

    return 0;
}