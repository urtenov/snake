#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cassert> 
using namespace std;

bool exit_game;
int width_window = 20;
int height_window = 15;
int x;
int y;
int target_X;
int target_Y;
int score;
int bonus_X;
int bonus_Y;
int tail_X[100];
int tail_Y[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
void BonusSet();
bool through_walls;

/**
* \brief Настраивает параметры игры такие как размеры поля, координаты змейки в начале игры, координаты цели в начале игры, количество очков.
*
* \param width_window Ширина поля игры
* \param height_window Высота поля игры
* \param x Координата змейки в начале игры по x
* \param y Координата змейки в начале игры по y
* \param target_X Координата цели по x
* \param target_Y Координата цели по y
* \param bonus_X Координата бонусной цели по x
* \param bonus_Y Координата бонусной цели по y
* \param score Количество очков
* \param mode 
*/

void Setup()
{
    exit_game = false;
    dir = STOP;
    x = width_window / 2 - 1;
    y = height_window / 2 - 1;
    target_X = rand() % width_window;
    target_Y = rand() % height_window;
    bonus_X = -1;
    bonus_Y = -1;
    score = 0;

    int mode;
    cout << "Выберите режим игры:\n";
    cout << "0 - Стандартный режим (столкновение со стенками)\n";
    cout << "1 - Режим прохождения сквозь стены\n";
    cout << "Выбор: ";
    cin >> mode;
    through_walls = (mode == 1);
}

/**
* \brief Отрисовывает поле игры,
*
* \param width_window Ширина поля игры
* \param height_window Высота поля игры
* \param target_X Координата цели по x
* \param target_Y Координата цели по y
* \param bonus_X Координата бонусной цели по x
* \param bonus_Y Координата бонусной цели по y
* \param score Количество очков
* \param ntail Длина хвоста змейки
*/

void Draw()
{
    system("cls");
    for (int i = 0; i < width_window + 1; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height_window; i++)
    {
        for (int j = 0; j < width_window; j++)
        {
            if (j == 0 || j == width_window - 1)
                cout << "#";
            if (i == y && j == x)
                cout << "O";
            else if (i == target_Y && j == target_X)
                cout << "F";
            else if (i == bonus_Y && j == bonus_X)
                cout << "B";
            else
            {
                bool print_tail = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tail_X[k] == j && tail_Y[k] == i)
                    {
                        cout << "o";
                        print_tail = true;
                    }
                }
                if (!print_tail)
                    cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < width_window + 1; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;

    cout << "Режим игры: ";
    if (through_walls)
        cout << "Прохождение сквозь стены" << endl;
    else
        cout << "Столкновение со стенами" << endl;
}

/**
* \brief Обрабатывает пользовательский ввод. Отвечает за напрвление змейки и возможность выхода из игры
*
* \param dir Направление движения змейки
* \param left Направление влево
* \param right Направление вправо
* \param up Направление вверх
* \param down Направление вниз
* \param x Выход из игры
*/

void Input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            exit_game = true;
            break;
        }
    }
}

/**
* \brief Обновляет состояние игры
*
* \param width_window Ширина поля игры
* \param height_window Высота поля игры
* \param x Координата змейки
* \param y Координата змейки
* \param target_X Координата цели
* \param target_Y Координата цели
* \param bonus_X Координата бонусной цели
* \param bonus_Y Координата бонусной цели
* \param tail_X Координаты элементов хвоста змейки
* \param tail_Y Координаты элементов хвоста змейки
* \param nTail Длина хвоста змейки
* \param score Количество очков
*/

void Logic()
{
    int prev_X = tail_X[0];
    int prev_Y = tail_Y[0];
    int prev2_X, prev2_Y;
    tail_X[0] = x;
    tail_Y[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2_X = tail_X[i];
        prev2_Y = tail_Y[i];
        tail_X[i] = prev_X;
        tail_Y[i] = prev_Y;
        prev_X = prev2_X;
        prev_Y = prev2_Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    if (through_walls) // Режим прохождения сквозь стены
    {
        if (x >= width_window - 1)
            x = 0;
        else if (x < 0)
            x = width_window - 2;

        if (y >= height_window)
            y = 0;
        else if (y < 0)
            y = height_window - 1;
    }
    else // Стандартный режим со столкновением со стенами
    {
        if (x >= width_window - 1 || x < 0 || y >= height_window || y < 0)
            exit_game = true;
    }

    for (int i = 0; i < nTail; i++)
    {
        if (tail_X[i] == x && tail_Y[i] == y)
            exit_game = true;
    }

    if (x == target_X && y == target_Y)
    {
        score += 10;
        target_X = rand() % width_window;
        target_Y = rand() % height_window;
        nTail++;
    }

    if (x == bonus_X && y == bonus_Y)
    {
        score += 50;
        bonus_X = -1;
        bonus_Y = -1;
        nTail++;
    }

    if (bonus_X == -1 && bonus_Y == -1 && rand() % 100 < 5)
        BonusSet();
}

void run_tests()
{
    //test_snake_moves_up();
    //test_snake_moves_down();
    //test_snake_moves_left();
    //test_snake_moves_right();
    //test_snake_wraps_around_screen();
    //test_snake_eats_target();
    //test_snake_tail_grows_when_eats_target();
    //test_snake_hits_wall_in_standard_mode();
    //test_snake_does_not_die_when_hits_wall_in_through_walls_mode();
    //test_snake_eats_bonus();
}

/**
* \brief Устанавливает случайные координаты для бонусной цели
*
* \param width_window Ширина поля игры
* \param height_window Высота поля игры
* \param bonus_X Координата бонусной цели
* \param bonus_Y Координата бонусной цели
*/

void BonusSet()
{
    bonus_X = rand() % width_window;
    bonus_Y = rand() % height_window;
}



// Тесты

/**
* Проверяет, что змейка движется вверх при установленном направлении вверх
*/
void test_snake_moves_up()
{
    Setup();
    dir = UP;
    int initial_y = y;
    Logic();
    assert(y == initial_y - 1);
}

/**
* Проверяет, что змейка движется вниз при установленном направлении вниз
*/

void test_snake_moves_down()
{
    Setup();
    dir = DOWN;
    int initial_y = y;
    Logic();
    assert(y == initial_y + 1);
}

/**
* Проверяет, что змейка движется влево при установленном направлении влево
*/

void test_snake_moves_left()
{
    Setup();
    dir = LEFT;
    int initial_x = x;
    Logic();
    assert(x == initial_x - 1);
}

/**
* Проверяет, что змейка движется вправо при установленном направлении вправо
*/

void test_snake_moves_right()
{
    Setup();
    dir = RIGHT;
    int initial_x = x;
    Logic();
    assert(x == initial_x + 1);
}

/**
* Проверяет, что змейка не двигается, когда направление равно STOP.
*/
void test_snake_does_not_move_when_direction_is_STOP()
{
    Setup();
    dir = STOP;
    int initial_x = x;
    int initial_y = y;
    Logic();
    assert(x == initial_x);
    assert(y == initial_y);
}

/**
* Проверяет, что змейка не растет, когда не съедает цель.
*/
void test_snake_does_not_grow_when_does_not_eat_target()
{
    Setup();
    target_X = x + 1;
    target_Y = y;
    int initial_tail_length = nTail;
    Logic();
    assert(nTail == initial_tail_length);
}

/**
* Проверяет, что змейка не съедает бонус, когда бонус не сгенерирован.
*/
void test_snake_does_not_eat_bonus_when_not_generated()
{
    Setup();
    bonus_X = -1;
    bonus_Y = -1;
    int initial_score = score;
    Logic();
    assert(score == initial_score);
}

/**
* Проверяет, что змейка генерирует новый бонус после съедения предыдущего бонуса.
*/
void test_snake_generates_new_bonus_after_eating_previous_bonus()
{
    Setup();
    bonus_X = x;
    bonus_Y = y;
    Logic();
    assert(bonus_X != x || bonus_Y != y);
}

/**
* Проверяет, что змейка перемещается через стены по горизонтали в режиме прохождения сквозь стены.
*/
void test_snake_moves_through_walls_horizontally()
{
    Setup();
    through_walls = true;
    x = width_window - 2;
    dir = RIGHT;
    Logic();
    assert(x == 0);
}

/**
* Проверяет, что змейка умирает при столкновении со стеной в стандартном режиме игры.
*/
void test_snake_hits_wall_in_standard_mode()
{
    Setup();
    x = width_window - 2;
    dir = RIGHT;
    Logic();
    assert(exit_game);
}

/**
* Проверяет, что змейка не умирает при столкновении со стеной в режиме прохождения сквозь стены.
*/
void test_snake_does_not_die_when_hits_wall_in_through_walls_mode()
{
    Setup();
    through_walls = true;
    x = width_window - 2;
    dir = RIGHT;
    Logic();
    assert(!exit_game);
}

/**
* Проверяет, что змейка съедает бонусную цель и получает 50 очков.
*/
void test_snake_eats_bonus()
{
    Setup();
    bonus_X = x = 10;
    bonus_Y = y = 10;
    int initial_score = score;
    Logic();
    assert(score == initial_score + 50);
}
/**
* \brief Функция "main". Выводит приветственный текст и информацию об игре. Дает возможность выбора уровня и возможность выбора режима игры: 
* 1) возможность прохождения сквозь стены;
* 2) запретность этого действия;
* Объединяет все описанные ранее функции;
* 
* \param delay в зависимости от выбранного уровня сложности примает разные значения, от которых зависит замедоение игры;
* \param difficulty принимает уровень сложности;
*/
int main()
{
    run_tests();
    setlocale(LC_ALL, "Russian");
    cout << "Вас приветствует легендарная игра 'Змейка'!\n";
    cout << "За каждую съеденную цель вы получаете 10 очков, за бонус - 20 очков\n";
    cout << "Управление змейкой:\n";
    cout << "W - Вверх\n";
    cout << "S - Вниз\n";
    cout << "A - Влево\n";
    cout << "D - Вправо\n";
    cout << "X - Выход из игры\n";
    Setup();
    int delay;
    int difficulty;
    cout << "Выберите уровень сложности (1 - легкий, 2 - средний, 3 - сложный): ";
    cin >> difficulty;
    switch (difficulty)
    {
    case 1:
        delay = 100;  // Задержка для легкого уровня
        break;
    case 2:
        delay = 50;  // Задержка для среднего уровня
        break;
    case 3:
        delay = 10;  // Задержка для сложного уровня
        break;
    default:
        delay = 100;  // Значение по умолчанию
        break;
    }
    system("pause");

    while (!exit_game)
    {
        Draw();
        Input();
        Logic();
        Sleep(delay);
    }
    return 0;
}