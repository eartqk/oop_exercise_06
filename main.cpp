// Артамонов Олег, 208Б группа
// Вариант 18
// квадрат - очередь - список
#include <iostream>
#include <algorithm>
#include <string>

#include "Square.hpp"
#include "Allocator.hpp"
#include "Queue.hpp"

using namespace std;

void help() {
    cout <<
        "Справка: \n"
        "push   - вставить элемент в начало очереди.\n"
        "pop    - удалить последний элемент из очереди.\n"
        "top    - напечатать последний элемент очереди.\n"
        "print  - печать информации о всех квадратах.\n"
        "help   - помощь.\n"
        "quit   - выход из программы.\n\n";
}


int main() {
    string command;
    Queue<Square<double>, TAllocator<Square<double>, 100>> queue_of_squares;
    help();
    while (true) {
        cin >> command;
        if (command == "push") {
            Square<double> p;
            cin >> p;
            queue_of_squares.push(p);
            cout << "Успешно.\n";
        }
        else if (command == "pop") {
            queue_of_squares.pop();
            cout << "Успешно.\n";
        }
        else if (command == "top") {
            auto figure = queue_of_squares.top();
            cout << figure << "\n";
        }
        else if (command == "print") {
            for_each(queue_of_squares.begin(), queue_of_squares.end(), [](Square<double> &P) {
                auto figure = P;
                cout << figure;
            }
            );
            cout << "\n";
        }
        else if (command == "help") {
            help();
        }
        else if (command == "quit") {
            return 0;
        }
        else { cout << "Неверный ввод!\n"; }
    }
    return 0;
}
