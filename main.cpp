#include <iostream>
#include <vector>
#include <string>
#include <locale>
#include <windows.h>

using namespace std;

const char PLAYER = '@';
const char WALL = '#';
const char FLOOR = '.';
const char EXIT = 'E';
const char FOG = '-';
const int RADIUS = 2;

struct Position {
    int x, y;
};

class Game {
private:
    vector<vector<vector<char>>> levels;
    int current_level;
    Position player_pos;
public:
    Game() {
        levels = {
            {
                {'#', '#', '#', '#', '#', '#', '#'},
                {'#', '.', '.', '.', '.', 'E', '#'},
                {'#', '.', '#', '#', '#', '.', '#'},
                {'#', '.', '.', '@', '.', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#'}
            },
            {
                {'#', '#', '#', '#', '#', '#', '#'},
                {'#', '.', '.', '.', '.', '.', '#'},
                {'#', '.', '#', '#', '#', 'E', '#'},
                {'#', '.', '@', '.', '.', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#'}
            },
            {
                {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
                {'#', '.', '.', '#', '.', '.', '.', 'E', '#'},
                {'#', '.', '#', '#', '#', '#', '#', '.', '#'},
                {'#', '.', '@', '.', '.', '.', '.', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#', '#', '#'}
            },
            {
                {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
                {'#', '#', 'E', '#', '.', '.', '#', 'E', '#'},
                {'#', '.', '#', '#', '#', '#', '#', '.', '#'},
                {'#', '.', '@', '.', '.', '.', '.', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#', '#', '#'}
            },
            {
                {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
                {'#', '#', 'E', '#', '.', '.', '.', '.', '.', '#'},
                {'#', '.', '#', '#', '.', '#', '#', '#', '.', '#'},
                {'#', '.', '@', '.', '.', '#', '.', 'E', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
            }
        };
        current_level = 0;
        find_player_position();
    }

    void find_player_position() {
        for (int i = 0; i < levels[current_level].size(); i++) {
            for (int j = 0; j < levels[current_level][i].size(); j++) {
                if (levels[current_level][i][j] == PLAYER) {
                    player_pos = { i, j };
                    return;
                }
            }
        }
    }

    void render() {
        for (int i = 0; i < levels[current_level].size(); i++) {
            for (int j = 0; j < levels[current_level][i].size(); j++) {
                if (abs(i - player_pos.x) <= RADIUS && abs(j - player_pos.y) <= RADIUS) {
                    cout << levels[current_level][i][j] << ' ';
                } else {
                    cout << FOG << ' ';
                }
            }
            cout << endl;
        }
    }

    void move_player(int dx, int dy) {
        int new_x = player_pos.x + dx;
        int new_y = player_pos.y + dy;

        if (levels[current_level][new_x][new_y] == EXIT) {
            cout << "Поздравляем! Вы прошли уровень!" << endl;
            current_level++;
            if (current_level >= levels.size()) {
                cout << "Вы победили! Игра завершена!" << endl;
                exit(0);
            }
            find_player_position();
        } else if (levels[current_level][new_x][new_y] != WALL) {
            swap(levels[current_level][player_pos.x][player_pos.y], levels[current_level][new_x][new_y]);
            player_pos = { new_x, new_y };
        }
    }

    void run() {
        setlocale(LC_ALL, "Russian");
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        string command;
        while (true) {
            render();
            cout << "Введите команду (вверх/вв, вниз/вн, влево/вл, вправо/вп):" << endl;
            getline(cin, command);
            if (command == "вверх" || command == "вв") move_player(-1, 0);
            else if (command == "вниз" || command == "вн") move_player(1, 0);
            else if (command == "влево" || command == "вл") move_player(0, -1);
            else if (command == "вправо" || command == "вп") move_player(0, 1);
            else cout << "Неверная команда! Попробуйте снова." << endl;
        }
    }
};

int main() {
    std::cout << "Hello World!\n";
    int a = 10;
    std::cout << a;
}
