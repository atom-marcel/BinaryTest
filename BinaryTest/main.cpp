// BinaryTest.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <iostream>
#include "NotationSystem.hpp"
#include "Node.hpp"

using namespace BaseNotation;

int TestPassed = 0;
int TestLength = 0;

template<typename T>
void TestValue(T value, T compare)
{
    TestLength++;
    if (value == compare) 
    {
        std::cout << TestLength << ". Test passed!\n";
        TestPassed++;
    }
    else std::cout << TestLength << ". Test NOT passed! Value '" << value << "' is not equal '" << compare << "'.\n";
}

void TestNode() 
{
    Node<int> nodes(10);
    nodes.add(15);
    nodes.add(20);
    TestValue<int>(nodes.value, 10);
    TestValue<int>(nodes.get(1), 15);
    TestValue<int>(nodes.get(2), 20);

    TestValue<std::string>(nodes.get_string(), "(10), (15), (20)\n");
    TestValue<int>(nodes.length(), 3);
}

void TestNotation()
{
    Notation<>* notation = new Notation(64);
    TestValue<std::string>(notation->notation, "64");
    TestValue<int>(notation->value, 64);
    notation->ConvertTo(Base::Hexadecimal);
    TestValue<std::string>(notation->notation, "40");
    TestValue<int>(notation->value, 64);
    notation->ConvertTo(Base::Binary);
    TestValue<std::string>(notation->notation, "1000000");
    notation->ConvertTo(Base::Octal);
    TestValue<std::string>(notation->notation, "100");
}

void TestByteAndByteList()
{
    Byte* b = new Byte((uint8_t)255);
    TestValue<std::string>(b->notation, "11111111");
    Byte* b2 = new Byte((uint8_t)7);
    TestValue<std::string>(b2->notation, "00000111");
    ByteList bl("Hallo");

    TestValue<std::string>(bl.To_Ascii(), "Hallo");
    TestValue<std::string>(bl.Get_Base64_String(), "SGFsbG8=");

    ByteList bl2("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam");
    TestValue<std::string>(bl2.Get_Base64_String(), "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNldGV0dXIgc2FkaXBzY2luZyBlbGl0ciwgc2VkIGRpYW0=");
    bl2.SetListFromBase64(bl2.Get_Base64_String());
    TestValue<std::string>(bl2.To_Ascii(), "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam");
    bl2.SaveFile("test.txt");
    
    ByteList bl3("");
    bl3.LoadFile("test.txt");
    TestValue < std::string>(bl3.To_Ascii(), "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam");
}

void Test()
{
    TestNode();
    TestNotation();
    TestByteAndByteList();

    std::cout << TestPassed << " out of " << TestLength << " Test(s) have passed.\n";
}

int main()
{
    Test();

    return 0;
}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
