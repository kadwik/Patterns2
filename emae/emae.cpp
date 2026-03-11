#include <iostream>
#include <string>

// Абстрактные продукты (семейство UI-элементов)
class Button {
public:
    virtual std::string render() = 0;
};

class ScrollBar {
public:
    virtual std::string render() = 0;
};

// Конкретные продукты для Windows
class WinButton : public Button {
public:
    std::string render() override { return "Windows Кнопка"; }
};

class WinScrollBar : public ScrollBar {
public:
    std::string render() override { return "Windows Скроллбар"; }
};

// Конкретные продукты для macOS
class MacButton : public Button {
public:
    std::string render() override { return "macOS Кнопка"; }
};

class MacScrollBar : public ScrollBar {
public:
    std::string render() override { return "macOS Скроллбар"; }
};

// Абстрактная фабрика
class UIFactory {
public:
    virtual Button* createButton() = 0;
    virtual ScrollBar* createScrollBar() = 0;
};

// Конкретные фабрики
class WinFactory : public UIFactory {
public:
    Button* createButton() override { return new WinButton(); }
    ScrollBar* createScrollBar() override { return new WinScrollBar(); }
};

class MacFactory : public UIFactory {
public:
    Button* createButton() override { return new MacButton(); }
    ScrollBar* createScrollBar() override { return new MacScrollBar(); }
};

int main() {
    // Выбираем фабрику по платформе
    UIFactory* factory = new WinFactory();  // Или MacFactory
    Button* btn = factory->createButton();
    ScrollBar* sb = factory->createScrollBar();
    std::cout << btn->render() << " и " << sb->render() << std::endl;  // Вывод: Windows Кнопка и Windows Скроллбар
    delete btn; delete sb; delete factory;
    return 0;
}