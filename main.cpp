#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <locale.h>
#include <thread>
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

const char PLAYER = '@';
const char WALL = '#';
const char FLOOR = '.';
const char EXIT = 'E';
const char FOG = '-';
const char DOOR = 'D';
const char KEY = 'K';
const char COIN = 'C';
const char MONSTER = 'X';
const char SWORD = 'S';

const int RADIUS = 2;

struct Position {
    int x, y;
};

string readFile(string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

class Game {
private:
    vector<vector<vector<char>>> levels;
    vector<string> leveltext;
    int currentLevel;
    Position playerPos;
    int stepsInLevel;
    int totalSteps;
    int coinsCollected;
    int keysCollected;
    int swordsCollected;
    int doorsopened; // there
    int killed_monsters; // there
    string status;

public:
    Game() {
        levels = {
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','E','#'},//l1, first check for user
                {'#','#','.','.','.','.','.','.','.','.','.','#','.','.','#'},
                {'#','#','#','#','.','#','#','#','.','#','.','#','.','.','#'},
                {'#','@','.','.','.','.','#','.','.','#','.','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}, //l2, stairts
                {'@','.','.','.','.','#','.','.','.','.','#','.','.','C','#'},
                {'#','.','#','.','#','.','.','#','.','#','.','.','#','.','#'},
                {'#','#','K','.','S','.','#','.','.','D','.','#','.','X','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','E','#','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}, //l3, looking for a torch
                {'#','K','.','.','D','.','#','K','.','D','C','S','#','.','E'},
                {'#','.','#','#','#','.','#','#','.','#','#','#','#','S','#'},
                {'#','.','@','.','#','.','.','.','.','.','.','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
            {
                {'#','#','#','@','#','#','#','#','#','#','#','#','#','E','#'}, //l4, first monsters, bats
                {'#','.','.','.','.','#','.','C','X','.','.','.','#','.','#'},
                {'#','.','#','#','.','#','#','#','.','#','#','.','.','D','#'},
                {'#','K','X','.','.','.','S','.','.','.','#','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','E','#'}
            },
            {
                 {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},//l5, many X, many S
                 {'#','X','@','.','#','K','.','S','.','.','X','.','.','.','#'},
                 {'#','.','#','.','.','.','#','.','#','.','C','.','E','X','#'},
                 {'#','S','.','S','#','X','.','D','.','#','.','#','.','.','#'},
                 {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},//l6
                {'#','X','S','@','#','K','X','.','X','.','.','D','C','.','#'},
                {'#','.','#','#','#','#','#','.','#','#','#','.','#','#','#'},
                {'#','K','.','.','X','.','D','S','#','X','.','.','.','C','#'},
                {'#','#','#','#','#','#','#','#','#','E','#','#','#','E','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},//l7
                {'#','.','C','.','.','.','.','X','.','S','C','.','K','.','#'},
                {'#','X','#','#','.','S','#','#','#','.','#','#','.','S','#'},
                {'#','.','K','@','.','.','S','D','.','.','.','.','E','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},//l8 with Ilya
                {'#','@','.','C','.','.','.','K','.','.','#','C','.','.','E'},
                {'#','X','#','#','.','#','S','#','.','.','#','X','#','C','#'},
                {'#','C','.','.','.','#','D','C','#','.','C','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
        };
        leveltext = { readFile("text1.txt"),
                    readFile("text2.txt"),
                    readFile("text3.txt"),
                    readFile("text4.txt"),
                    readFile("text5.txt"),
                    readFile("text6.txt"),
                    readFile("text7.txt"),
                    readFile("text8.txt")
        };

        currentLevel = 0;
        stepsInLevel = 0;
        totalSteps = 0;
        coinsCollected = 0;
        keysCollected = 0;
        swordsCollected = 0;
        killed_monsters = 0;
        doorsopened = 0; // здесь
        status = "Всё хорошо";
        findPlayerPosition();
    }
    void SetColor(int textColor, int bgColor) {  //функция для работы с цветом переднего, заднего фона
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
    } 
    void showleveltext() {
        system("cls");
        cout << leveltext[currentLevel] << endl;  // Вывод текста для текущего уровня
        string action;
        cout << "Далее? ";
        cin >> action;
        while ((action != "ok") && (action != "ок")) {
            std::getline(cin, action); 
            cout << "Была введена неверная команда. Введите ok или ок" << std::endl;
            cout << "Далее? ";
            cin >> action;
        }
    }
    void showInstructions() {
        SetColor(4, 0);
        system("cls");
        cout << "Добро пожаловать в игру-лабиринт!\n";
        cout << "Цель игры - собирать монеты и попытаться найти выход живым.. А еще смотрите на статус, там могут быть подсказки.\n";
        cout << "@ - игрок\n# - стена\n. - пустая клетка\nE - выход\nD - дверь (открывается ключом)\n";
        cout << "K - ключ\nC - монета\nS - меч\nX - монстр\n";
        cout << "Команды для передвижения: вверх/ц, вниз/ы, влево/ф, вправо/в\n";
        cout << "Команды для передвижения(Eng): вверх/w, вниз/s, влево/a, вправо/d\n";
        cout << "После прочтения текста: ок, ok\n";
        cout << "Нажмите ПРОБЕЛ, чтобы пропустить инструкцию...\n";

        auto start = std::chrono::steady_clock::now();
        while (true) {
            if (GetAsyncKeyState(VK_SPACE)) break;
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            if (duration >= 10) break;
        }
    }

    void findPlayerPosition() {
        for (int i = 0; i < levels[currentLevel].size(); i++) {
            for (int j = 0; j < levels[currentLevel][i].size(); j++) {
                if (levels[currentLevel][i][j] == PLAYER) {
                    playerPos = { i, j };
                    return;
                }
            }
        }
    }

    void render() {
        system("cls");
        for (int i = 0; i < levels[currentLevel].size(); i++) {
            for (int j = 0; j < levels[currentLevel][i].size(); j++) {
                if (abs(i - playerPos.x) <= RADIUS && abs(j - playerPos.y) <= RADIUS) {
                    cout << levels[currentLevel][i][j];
                } else {
                    cout << FOG;
                }
            }
            cout << endl;
        }
        SetColor(4, 0);
        cout << "Собрано монет: " << coinsCollected << " | Ключей: " << keysCollected << " | Дверей открыто: " << doorsopened; //здесь
        cout << " | Мечей: " << swordsCollected << " | Ходов в уровне: " << stepsInLevel; //
        cout << " | Всего ходов: " << totalSteps << " | Убитых монстров:  " << killed_monsters << endl; //
        cout << "Статус: " << status << endl;
    }

    void nextLevel() {
        if (currentLevel + 1 < levels.size()) {
            currentLevel++;
            stepsInLevel = 0;
            status = "Переход на следующий уровень.";
            showleveltext();
            findPlayerPosition();
        } else {
            if (coinsCollected == 14) {//
                cout << "Поздравляем! Вы прошли все уровни и собрали все монеты!" << endl;
                exit(0);
            } else {
                cout << "Поздравляем! Вы прошли все уровни, но не собрали все монеты((!" << endl;
                exit(0);
            }
        }
    }
    void eaten() { // eaten
        SetColor(4, 0);
        cout << "GAME OVER!У вас не было меча, чтобы защититься, и вас убил монстр :(";
        exit(0);
    }

    void movePlayer(int dx, int dy) {
        SetColor(2, 0);
        int newX = playerPos.x + dx;
        int newY = playerPos.y + dy;
        status = "Всё хорошо";

        if (levels[currentLevel][newX][newY] == WALL) {
            SetColor(4, 0);
            status = "Там стена.";
            return;
        }

        if (levels[currentLevel][newX][newY] == EXIT) {
            SetColor(4, 0);
            status = "Вы перешли на следующий уровень";
            nextLevel();
            return;
        }

        if (levels[currentLevel][newX][newY] == COIN) {
            coinsCollected++;
            levels[currentLevel][newX][newY] = FLOOR;
            status = "Монета собрана.";
        }

        if (levels[currentLevel][newX][newY] == KEY) {
            keysCollected++;
            levels[currentLevel][newX][newY] = FLOOR;
            status = "Ключ собран.";
        }
        if (levels[currentLevel][newX][newY] == DOOR and keysCollected > 0) { //если открыли дверь, то остается пол
            levels[currentLevel][newX][newY] = FLOOR;
            ++doorsopened;
            --keysCollected;
            status = "Дверь открыта, ключ использован.";
        } else if (levels[currentLevel][newX][newY] == DOOR and keysCollected == 0) { // не может пройти, если нет ключа
            SetColor(4, 0);
            status = "Вы не можете открыть, у вас нет ключа, вернитесь и найдите его, либо обойдите дверь.";
            return;
        }
        if (levels[currentLevel][newX][newY] == MONSTER and swordsCollected > 0) { //здесь
            levels[currentLevel][newX][newY] = FLOOR;
            ++killed_monsters;
            --swordsCollected;
            status = "Вы использовали меч и убили монстра.";
        } else if (levels[currentLevel][newX][newY] == MONSTER and swordsCollected == 0) { // новая функция, если человека съели
            SetColor(4, 0);
            eaten();
        }
        if (levels[currentLevel][newX][newY] == SWORD) { //здесь
            ++swordsCollected;
            levels[currentLevel][newX][newY] = FLOOR;
            status = "Вы забрали меч.";
        }

        std::swap(levels[currentLevel][playerPos.x][playerPos.y], levels[currentLevel][newX][newY]);
        playerPos = { newX, newY };

        stepsInLevel++;
        totalSteps++;
    }

    void run() {
        showInstructions();
        SetColor(4, 0);
        showleveltext();
        string command;
        while (true) {
            render();
            cout << "Введите команду: ";
            cin >> command;//
            if (command == "w" || command == "ц") movePlayer(-1, 0);
            else if (command == "s" || command == "ы") movePlayer(1, 0);
            else if (command == "a" || command == "ф") movePlayer(0, -1);
            else if (command == "d" || command == "в") movePlayer(0, 1);
            else {
                status = "Неверная команда!";
                SetColor(4, 0);
                cout << "Неверная команда!" << endl;
            }
        }
    }
};


int main() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Game game;
    game.run();
    return 0;
}
