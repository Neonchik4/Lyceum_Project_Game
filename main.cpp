#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;

const char PLAYER = '@';
const char WALL = '#';
const char FLOOR = '.';
const char EXIT = 'E';
const char FOG = '-';
const char DOOR = 'D';
const char KEY = 'K';
const char COIN = 'C';
const char MONSTER = 'X';

const int RADIUS = 2;

struct Position {
    int x, y;
};

class Game {
private:
    vector<vector<vector<char>>> levels;
    int current_level;
    Position player_pos;
    int steps_in_level;
    int total_steps;
    int coins_collected;
    int keys_collected;

public:
    Game() {
        levels = {
            {
                {'#', '#', '#', '#', '#', '#', '#'},
                {'#', '.', 'C', '.', '.', 'E', '#'},
                {'#', '.', '#', '#', 'D', '.', '#'},
                {'#', 'K', '.', '@', 'X', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#'}
            },
            {
                {'#', '#', '#', '#', '#', '#', '#'},
                {'#', '.', 'C', '.', '.', '.', '#'},
                {'#', '.', '#', '#', 'D', 'E', '#'},
                {'#', '.', '@', '.', 'X', '.', '#'},
                {'#', '#', '#', '#', '#', '#', '#'}
            }
        };
        current_level = 0;
        steps_in_level = 0;
        total_steps = 0;
        coins_collected = 0;
        keys_collected = 0;
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
        system("cls"); // Очистка экрана
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
        cout << "Собрано монет: " << coins_collected << endl;
        cout << "Ключей в инвентаре: " << keys_collected << endl;
        cout << "Ходов в уровне: " << steps_in_level << endl;
        cout << "Всего ходов: " << total_steps << endl;
    }

    void move_player(int dx, int dy) {
        int new_x = player_pos.x + dx;
        int new_y = player_pos.y + dy;

        if (levels[current_level][new_x][new_y] == WALL) return;

        // Если это монстр — проигрыш
        if (levels[current_level][new_x][new_y] == MONSTER) {
            cout << "Вы столкнулись с монстром! Игра окончена." << endl;
            cout << "Вы потеряли все монеты и ключи!" << endl;
            coins_collected = 0;
            keys_collected = 0;
            exit(0);
        }

        // Если это дверь — проверяем наличие ключей
        if (levels[current_level][new_x][new_y] == DOOR) {
            if (keys_collected > 0) {
                cout << "Вы открыли дверь!" << endl;
                --keys_collected;
            } else {
                cout << "Нужно найти ключ, чтобы открыть дверь!" << endl;
                return;
            }
        }

        // Если это выход — переход на следующий уровень
        if (levels[current_level][new_x][new_y] == EXIT) {
            cout << "Поздравляем! Вы прошли уровень за " << steps_in_level << " ходов." << endl;
            current_level++;
            steps_in_level = 0;
            if (current_level >= levels.size()) {
                cout << "Вы победили! Игра завершена!" << endl;
                cout << "Всего собрано монет: " << coins_collected << endl;
                cout << "Всего сделано ходов: " << total_steps << endl;
                exit(0);
            }
            find_player_position();
            return;
        }

        // Если это монета — добавляем к счетчику и заменяем на пол
        if (levels[current_level][new_x][new_y] == COIN) {
            coins_collected++;
            cout << "Монета собрана! Всего монет: " << coins_collected << endl;
            levels[current_level][new_x][new_y] = FLOOR;
        }

        // Если это ключ — добавляем к счетчику и заменяем на пол
        if (levels[current_level][new_x][new_y] == KEY) {
            keys_collected++;
            cout << "Ключ подобран! Всего ключей: " << keys_collected << endl;
            levels[current_level][new_x][new_y] = FLOOR;
        }

        // Перемещение игрока
        swap(levels[current_level][player_pos.x][player_pos.y], levels[current_level][new_x][new_y]);
        player_pos = { new_x, new_y };

        steps_in_level++;
        total_steps++;
    }

    void run() {
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
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Лабиринт" << endl;
    Game game;
    game.run();
    return 0;
}
