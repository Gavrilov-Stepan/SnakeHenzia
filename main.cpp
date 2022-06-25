#include <iostream>
#include <conio.h>
using namespace std;
bool gameover;
int width;
int height;
int snake_x;
int snake_y;
int fruit_x;
int fruit_y;
int score = 0;
int movements = 0;
int tail_x[1000];
int tail_y[1000];
int tail_lenght;
int blockade[50];
int no_fruits_moves = 0;
string cause_of_death = "";
enum direction {
    direction_left,
    direction_up,
    direction_down,
    direction_right,
    stop
};
direction dir;
void Barriers_Generation() {
/*
функция автоматически вызывается до старта игры для задания начальных координат по X препятствиям.
вызывается в main.
*/
    for (int i = 0; i < 50; i++) {
        blockade[i] = rand() % 50;
    }
}
void Setup() {
/*
включение змейки. функция вызывается автоматически 1 раз после включение программы и устанавливает начальные координаты змее и фрукту, а также счет = 0. на вход элементов не поступает.
gameover - служебная переменная, которая используется для определения проигрыша игрока (при значении true). начальное значение - false.
dir отражает состояние змеи. изначально змея покоится
snake_x и snake_y тут задают начальные координаты головы змейки (изначально только голова и присутствует).
fruit_x и fruit_y задают начальные координаты фрукта.
score - переменная, показывающая количество заработанных игроком очков. значение растет при съедении фрукта.
вызывается в main.
*/
    gameover = false;
    dir = stop;
    snake_x = width / 2 - 1;
    snake_y = height / 2 - 1;
    fruit_x = 1 + rand() % (width - 3);
    fruit_y = 1 + rand() % (height - 3);
    score = 0;
}
void Map_Building() {
/*
создание карты и обновление консоли. вызывается автоматически каждый раз, когда змея совершает движение. в процессе действия функции очищается терминал, отображающий предыдущую расстановку на карте, после чего выводится в консоль обновленная после совершения перемещения змеи карта. аналогично выводится счет игрока, количество нажатий на задействуемые для игры клавиши и количество доступных движений.
вызывается в main.
*/
    system("cls");
    for (int i = 0; i <= width; i++) {
        cout << "#";
    }
    cout << "   gamefield size:" << endl;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (i == 0 or i == width - 1) {
                cout << "#";
            }
            if (i == snake_x and j == snake_y) {
                cout << "Q";
            }
            else if (i == fruit_x and j == fruit_y) {
                cout << "F";
            }
            else if (i == blockade[j]) {
                cout << "X";
            }
            else {
                bool print_necessity = false;
                for (int k = 0; k < tail_lenght; k++) {
                    if (tail_x[k] == i and tail_y[k] == j) {
                        cout << "O";
                        print_necessity = true;
                    }
                }
                if (not print_necessity) {
                    cout << " ";
                }
            }
        }
        if (j == 0) {
            cout << "  " << width << "x" << height;
        }
        cout << endl;
    }
    for (int i = 0; i <= width; i++) {
        cout << "#";
    }
    cout << endl;
    cout << "   your score: " << score << endl;
    cout << "   your movements: " << movements << endl;
    cout << "   you need to eat a fruit in next " << 15 - no_fruits_moves << " movements" << endl;
}
void Input() {
/*
функция меняет значения некоторых переменных в зависимости от действий игрока.
при нажатии на "a" направление змейки изменяется на "влево";
при нажатии на "w" направление змейки изменяется на "вверх";
при нажатии на "s" направление змейки изменяется на "вниз";
при нажатии на "d" направление змейки изменяется на "вправо";
при нажатии на "p" игра прекращается.
вызывается в main.
*/
    if (_kbhit()) { //_kbhit проверяет, было ли произведено пользователем нажатие на кнопку
        switch (_getch()) { //_getch передает кнопку, на которую было произведено нажатие
            case 'a':
                if (tail_x[0] != snake_x - 1) {
                    dir = direction_left;
                    no_fruits_moves++;
                    movements++;
                }
                break;
            case 'w':
                if (tail_y[0] != snake_y - 1) {
                    dir = direction_up;
                    no_fruits_moves++;
                    movements++;
                }
                break;
            case 's':
                if (tail_y[0] != snake_y + 1) {
                    dir = direction_down;
                    no_fruits_moves++;
                    movements++;
                }
                break;
            case 'd':
                if (tail_x[0] != snake_x + 1) {
                    dir = direction_right;
                    no_fruits_moves++;
                    movements++;
                }
                break;
            case 'p':
                cause_of_death = "SURRENDER";
                gameover = true;
                movements++;
                break;
        }
    }
}
void Logic() { //логические операции
/*
функция отвечает за события в игре. также тут изменяются координаты для хвоста змеи каждый раз, когда она двигается.
если направление змеи определено как "влево", то при следующем обновлении терминала (ф-ция Map_Building) голова змейки двинется влево, 1-я часть ее хвоста встанет на место головы, 2-я часть хвоста на место первой и т.д. . аналогично для направлений "вверх", "вправо", "вниз".
если координаты головы змеи совпадают с координатами препятствия, то игра заканчивается.
если координаты головы части змеи совпадают с координатами края карты по X, то эту часть змеи кидает на другой конец карты по X. аналогично для Y.
если количество свободных движений будет <1, то игра заканчивается.
вызывается в main.
*/
    int previous_x = tail_x[0];
    int previous_y = tail_y[0];
    int previous_2x;
    int previous_2y;
    tail_x[0] = snake_x;
    tail_y[0] = snake_y;
    for (int i = 1; i < tail_lenght; i++) {
        previous_2x = tail_x[i];
        previous_2y = tail_y[i];
        tail_x[i] = previous_x;
        tail_y[i] = previous_y;
        previous_x = previous_2x;
        previous_y = previous_2y;
    }
    switch (dir) {
        case direction_left:
            snake_x -= 1;
            break;
        case direction_up:
            snake_y -= 1;
            break;
        case direction_down:
            snake_y += 1;
            break;
        case direction_right:
            snake_x += 1;
            break;
    }
    if (snake_x >= width - 1) {
        snake_x = 0;
    }
    if (snake_x < 0) {
        snake_x = width - 1;
    }
    if (snake_y < 0) {
        snake_y = height - 1;
    }
    if (snake_y >= height) {
        snake_y = 0;
    }
    for (int k = 0; k < tail_lenght; k++) {
        if (snake_x == tail_x[k] and snake_y == tail_y[k]) {
            cause_of_death = "SUICIDE";
            gameover = true;
        }
        for (int i = 0; i < 50; i++) {
            if (snake_x == blockade[i] and snake_y == i) {
                cause_of_death = "ACCIDENT";
                gameover = true;
            }
        }
    }
    if (snake_x == fruit_x and snake_y == fruit_y) {
        score += 10;
        fruit_x = 1 + rand() % (width-3);
        fruit_y = 1 + rand() % (height-3);
        no_fruits_moves = 0;
        tail_lenght++;
    }
    if (no_fruits_moves > 15) {
        cause_of_death = "HUNGER";
        gameover = true;
    }
}
int main() {
/*
функция, запускающая игру.
до начала игры пользователь устанавливает ширину и высоту карты (вводит дважды в терминал число от 4 до 50)
если произошла ошибка генерации препятствий (ф-ция Barriers_Generation), то игра заканчивается сразу после начала.
по окончании игры в последний раз обновляется терминал, где выводится уведомление о завершении игры, итоговый счет, количество нажатый на клавиши и причина конца игры.
*/
    bool incorrect = false;
    cout << "Enter your gamefield width(natural number from 4 to 50): ";
    cin >> width;
    if (width > 50 or width < 4) {
        cout << endl << "Incorrect gamefield width";
        incorrect = true;
    }
    cout << "Enter your gamefield height(natural number from 4 to 50): ";
    cin >> height;
    if (height > 50 or width < 4) {
        cout << endl << "Incorrect gamefield height";
        incorrect = true;
    }
    Setup();
    Barriers_Generation();
    if (incorrect) {
        cause_of_death = "INCORRECT";
        gameover = true;
    }
    while (gameover != true) {
        Map_Building();
        Input();
        Logic();
    }
    system("cls");
    cout << "Game Over" << endl;
    cout << "You finished with score: " << score << endl;
    cout << "You made " << movements << " movements" << endl;
    cout << "Died because of: " << cause_of_death << endl;
    return 0;
}