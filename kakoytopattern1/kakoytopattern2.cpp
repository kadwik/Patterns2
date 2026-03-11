#include <iostream>
#include <string>

using namespace std;

//  Целевые интерфейсы 
class USBPrinter {
public:
    virtual ~USBPrinter() = default;
    virtual string printUSB(const string& data, int quality) = 0;  // Добавлен quality
};

class USBScanner {
public:
    virtual ~USBScanner() = default;
    virtual string scanUSB(const string& data, int quality) = 0;   // Добавлен quality
};

//  Старое оборудование с LPT-портом 
class OldLPTPrinter {
public:
    string printLPT(const string& data) {
        return "Старый принтер печатает по LPT: " + data;
    }
};

class OldLPTScanner {
public:
    string scanLPT(const string& data) {
        return "Старый сканер сканирует по LPT: " + data;
    }
};

//  Object-Adapter для принтера 
class LPTToUSBPrinterAdapter : public USBPrinter {
private:
    OldLPTPrinter* oldPrinter;
public:
    LPTToUSBPrinterAdapter(OldLPTPrinter* p) : oldPrinter(p) {}

    string printUSB(const string& data, int quality) override {
        // Адаптируем вызов, преобразуя quality в текстовое описание
        string qualityStr;
        if (quality <= 3) qualityStr = " (черновик)";
        else if (quality <= 7) qualityStr = " (обычное качество)";
        else qualityStr = " (высокое качество)";

        return oldPrinter->printLPT(data + " [качество: " + to_string(quality) + "]" + qualityStr + " (адаптировано для USB)");
    }
};

class LPTToUSBScannerAdapter : public USBScanner {
private:
    OldLPTScanner* oldScanner;
public:
    LPTToUSBScannerAdapter(OldLPTScanner* s) : oldScanner(s) {}

    string scanUSB(const string& data, int quality) override {
        string resolution;
        if (quality <= 3) resolution = " 72dpi";
        else if (quality <= 7) resolution = " 150dpi";
        else resolution = " 300dpi";

        return oldScanner->scanLPT(data + " [разрешение: " + to_string(quality) + " ->" + resolution + "] (адаптировано для USB)");
    }
};


class LPTToUSBMultiPrinterAdapter : public USBPrinter, private OldLPTPrinter {
public:
    string printUSB(const string& data, int quality) override {
        string qualityStr;
        if (quality <= 3) qualityStr = " (черновик)";
        else if (quality <= 7) qualityStr = " (обычное качество)";
        else qualityStr = " (высокое качество)";

        return printLPT(data + " [качество: " + to_string(quality) + "]" + qualityStr + " (class-adapter)");
    }
};

/*
 * Почему Adapter лучше изменения старого класса:
 * 1. Принцип открытости/закрытости (Open/Closed): Старый класс не модифицируется,
 *    он закрыт для изменений, но открыт для расширения через адаптер.
 * 2. Сохранение обратной совместимости: Старый код, использующий OldLPTPrinter,
 *    продолжает работать без изменений. Если бы мы изменили старый класс,
 *    пришлось бы обновлять весь существующий код.
 * 3. Разделение ответственности: Адаптер отвечает только за преобразование интерфейсов,
 *    старая логика остается неизменной и не смешивается с кодом адаптации.
 * 4. Тестирование: Старый класс уже протестирован, адаптер можно тестировать отдельно
 * 5. Единообразное расширение: Можно создать несколько разных адаптеров для одного
 *    старого класса (например, USBAdapter, WiFiAdapter, BluetoothAdapter) без изменения
 *    исходного класса.
 * 6. Работа с чужим/сторонним кодом: Если старый класс находится в сторонней библиотеке,
 *    у нас может не быть возможности его изменить. Adapter решает эту проблему.
 */

int main() {
    setlocale(LC_ALL, "rus");
    cout << "=== Демонстрация Object-Adapter для принтера и сканера ===" << endl;

    OldLPTPrinter oldPrinter;
    OldLPTScanner oldScanner;

    USBPrinter* printerAdapter = new LPTToUSBPrinterAdapter(&oldPrinter);
    USBScanner* scannerAdapter = new LPTToUSBScannerAdapter(&oldScanner);

    cout << printerAdapter->printUSB("Документ", 10) << endl;
    cout << printerAdapter->printUSB("Черновик", 2) << endl;
    cout << scannerAdapter->scanUSB("Фото", 5) << endl;
    cout << scannerAdapter->scanUSB("Документ", 9) << endl;

    delete printerAdapter;
    delete scannerAdapter;

    cout << "\n=== Демонстрация Class-Adapter ===" << endl;
    LPTToUSBMultiPrinterAdapter multiAdapter;
    cout << multiAdapter.printUSB("Важный документ", 8) << endl;
    cout << "\n=== Преимущества Adapter перед изменением старого класса ===" << endl;
    cout << "1. Старый класс OldLPTPrinter не изменен и продолжает работать:" << endl;
    cout << oldPrinter.printLPT("Прямой вызов LPT") << endl;
    cout << "\n2. Можно создать несколько разных адаптеров:" << endl;
    cout << "   - LPTToUSBAdapter (USB интерфейс)" << endl;
    cout << "   - LPTToWiFiAdapter (WiFi интерфейс)" << endl;
    cout << "   - LPTToBluetoothAdapter (Bluetooth интерфейс)" << endl;
    cout << "\n3. Если бы мы изменили OldLPTPrinter, пришлось бы править:" << endl;
    cout << "   - Все места, где используется printLPT()" << endl;
    cout << "   - Добавлять параметр quality во все вызовы" << endl;
    cout << "   - Переписывать тесты для старого класса" << endl;
    return 0;
}