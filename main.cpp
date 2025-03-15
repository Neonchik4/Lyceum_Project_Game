#include <iostream>
#include <vector>
#include <string>
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

class Game {
private:
    vector<vector<vector<char>>> levels;
    int currentLevel;
    Position playerPos;
    int stepsInLevel;
    int totalSteps;
    int coinsCollected;
    int keysCollected;
    int swordsCollected;
    string status;

public:
    Game() {
        levels = {
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
                {'#','.','C','.','.','E','.','X','.','#','C','.','K','.','#'},
                {'#','X','#','#','D','.','#','#','.','#','.','#','.','S','#'},
                {'#','.','K','@','.','.','#','D','.','#','.','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            },
            {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
                {'#','@','.','C','.','D','.','K','.','.','.','.','E','.','#'},
                {'#','X','#','#','.','.','#','S','.','#','X','#','.','C','#'},
                {'#','.','K','.','.','.','#','D','.','#','.','.','.','.','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
            }
        };

        currentLevel = 0;
        stepsInLevel = 0;
        totalSteps = 0;
        coinsCollected = 0;
        keysCollected = 0;
        swordsCollected = 0;
        status = "Всё хорошо";
        findPlayerPosition();
    }

    void showInstructions() {
        system("cls");
        cout << "Добро пожаловать в игру-лабиринт!\n";
        cout << "Цель игры - собрать все предметы и найти выход.\n";
        cout << "@ - игрок\n# - стена\n. - пустая клетка\nE - выход\nD - дверь (открывается с ключом)\n";
        cout << "K - ключ\nC - монета\nS - меч\nX - монстр\n";
        cout << "Команды для передвижения: вверх/вв, вниз/вн, влево/вл, вправо/вп\n";
        cout << "Нажмите ПРОБЕЛ, чтобы пропустить инструкцию...\n";

        auto start = std::chrono::steady_clock::now();
        while (true) {
            if (GetAsyncKeyState(VK_SPACE)) break;
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            if (duration >= 15) break;
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
                }
                else {
                    cout << FOG;
                }
            }
            cout << endl;
        }
        cout << "Собрано монет: " << coinsCollected << " | Ключей: " << keysCollected;
        cout << " | Мечей: " << swordsCollected << " | Ходов в уровне: " << stepsInLevel;
        cout << " | Всего ходов: " << totalSteps << endl;
        cout << "Статус: " << status << endl;
    }

    void nextLevel() {
        if (currentLevel + 1 < levels.size()) {
            currentLevel++;
            stepsInLevel = 0;
            status = "Переход на следующий уровень.";
            findPlayerPosition();
        }
        else {
            cout << "Поздравляем! Вы прошли все уровни!" << endl;
            exit(0);
        }
    }

    void movePlayer(int dx, int dy) {
        int newX = playerPos.x + dx;
        int newY = playerPos.y + dy;
        status = "Всё хорошо";

        if (levels[currentLevel][newX][newY] == WALL) {
            status = "Там стена.";
            return;
        }

        if (levels[currentLevel][newX][newY] == EXIT) {
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

        std::swap(levels[currentLevel][playerPos.x][playerPos.y], levels[currentLevel][newX][newY]);
        playerPos = { newX, newY };

        stepsInLevel++;
        totalSteps++;
    }

    void run() {
        showInstructions();
        string command;
        while (true) {
            render();
            cout << "Введите команду: ";
            getline(cin, command);
            if (command == "вверх" || command == "вв") movePlayer(-1, 0);
            else if (command == "вниз" || command == "вн") movePlayer(1, 0);
            else if (command == "влево" || command == "вл") movePlayer(0, -1);
            else if (command == "вправо" || command == "вп") movePlayer(0, 1);
            else {
				status = "Неверная команда!";
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
