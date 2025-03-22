/*#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void set_input_mode(void)
{
  struct termios tattr;

  // Получаем текущие настройки терминала
  tcgetattr(STDIN_FILENO, &tattr);

  // Отключаем канонический режим и эхо
  tattr.c_lflag &= ~(ICANON | ECHO);

  // Устанавливаем новые настройки терминала
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

int main()
{
  char input = 0; // Инициализация переменной input
  int paddle1_position = HEIGHT / 2;
  int paddle2_position = HEIGHT / 2;

  set_input_mode();

  // Отрисовка игрового поля
  while (input != 'q')
  {
    // Очистка экрана
    printf("\033[H\033[J");

    for (int y = 0; y < HEIGHT; y++)
    {
      for (int x = 0; x < WIDTH; x++)
      {
        if (x <= 1 || x >= WIDTH - 2)
        { // отрисовка крайних позиций поля по вертикали
          printf("|");
        }
        else if (x == 4 && (y >= paddle1_position - 1 && y <= paddle1_position +
1)) { // отрисовка позиций первой ракетки printf("|");
        }
        else if (x == WIDTH - 4 && (y >= paddle2_position - 1 && y <=
paddle2_position + 1)) { // отрисовка позиций второй ракетки printf("|");
        }
        else if (x == WIDTH / 2 && y == HEIGHT / 2)
        { // отрисовка позиции мяча
          printf("O");
        }
        else
        {
          printf(" ");
        }


      }
      printf("\n");
    }

    input = getchar();
    if (input != EOF)
    {
      if ((input == 'a' || input == 'A') && paddle1_position > 1)
      {
        paddle1_position--;
      }
      else if ((input == 'z' || input == 'Z') && paddle1_position < HEIGHT - 2)
      {
        paddle1_position++;
      }
      else if ((input == 'k' || input == 'K') && paddle2_position > 1)
      {
        paddle2_position--;
      }
      else if ((input == 'm' || input == 'M') && paddle2_position < HEIGHT - 2)
      {
        paddle2_position++;
      }
    }

    usleep(50000); // Задержкaа для замедления игры
  }

  return 0;
}*/

/*
//рабочая игра*/
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define ROCKET_HEIGHT 3

void set_input_mode(void)
{
  struct termios tattr;

  // Получаем текущие настройки терминала
  tcgetattr(STDIN_FILENO, &tattr);

  // Отключаем канонический режим и эхо
  tattr.c_lflag &= ~(ICANON | ECHO);

  // Устанавливаем новые настройки терминала
  tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}

void draw_field(int paddle1_position, int paddle2_position, float ball_x,
                float ball_y, int score1, int score2)
{
  for (int y = 0; y < HEIGHT; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      if (x <= 1 ||
          x >= WIDTH - 2)
      { // отрисовка крайних позиций поля по вертикали
        printf("|");
      }
      else if (x == 4 && (y >= paddle1_position - 1 &&
                          y <= paddle1_position +
                                   1))
      { // отрисовка позиций первой ракетки
        printf("|");
      }
      else if (x == WIDTH - 4 &&
               (y >= paddle2_position - 1 &&
                y <= paddle2_position +
                         1))
      { // отрисовка позиций второй ракетки
        printf("|");
      }
      else if (x == (int)ball_x &&
               y == (int)ball_y)
      { // отрисовка позиции мяча
        printf("O");
      }
      else
      {
        printf(" ");
      }
    }
    printf("\n");
  }
  printf("first player score: %d\n", score1);
  printf("second player score: %d\n", score2);
}

void update_paddle_positions(char input, int *paddle1_position,
                             int *paddle2_position)
{
  if ((input == 'a' || input == 'A') && *paddle1_position > 1)
  {
    (*paddle1_position)--;
  }
  else if ((input == 'z' || input == 'Z') && *paddle1_position < HEIGHT - 2)
  {
    (*paddle1_position)++;
  }
  else if ((input == 'k' || input == 'K') && *paddle2_position > 1)
  {
    (*paddle2_position)--;
  }
  else if ((input == 'm' || input == 'M') && *paddle2_position < HEIGHT - 2)
  {
    (*paddle2_position)++;
  }
}

/* Функция для обновления позиции мяча */
//.                    (&ball_x, &ball_y,           &ball_dir_x, &ball_dir_y,
// paddle1_position, paddle2_position);
void updateBallPosition(float *movex, float *movey, float *reversex,
                        float *reversey, int spos_r1, int spos_r2, int *score1,
                        int *score2)
{
  // Проверка отражения от горизонтальных бортов
  if (((int)(*movey + *reversey) == 1) || ((int)(*movey + *reversey) == HEIGHT))
    // Условие ((int)(*movey + *reversey) == 1) ...проверяет, достиг ли мяч
    // верхней или нижней границы игрового поля. Если это так, то мяч начинает
    // двигаться в противоположном направлении по оси Y.
    *reversey = *reversey * (-1);

  // Проверка отражения от ракетки 1
  for (int z = spos_r1; z <= spos_r1 + ROCKET_HEIGHT; z++)
  {
    if ((int)*movex == 5 && ((int)*movey == z))
    {
      *reversex = *reversex * (-1);
    }
  }

  // Проверка отражения от ракетки 2
  for (int z = spos_r2; z <= spos_r2 + ROCKET_HEIGHT; z++)
  {
    if ((int)*movex == WIDTH - 5 && ((int)*movey == z))
    {
      *reversex = *reversex * (-1);
    }
  }

  // Проверка выхода мяча за ракетку 1
  if ((int)*movex < 5)
  {
    *movex = WIDTH / 2;
    *movey = HEIGHT / 2;
    *reversex = 1.0;
    *reversey = 1.0;
    *score2 = *score2 + 1;
  }

  // Проверка выхода мяча за ракетку 2
  if ((int)*movex > WIDTH - 5)
  {
    *movex = WIDTH / 2;
    *movey = HEIGHT / 2;
    *reversex = -1.0;
    *reversey = 1.0;
    *score1 = *score1 + 1;
  }

  // Обновление позиции мяча
  *movex = *movex + *reversex;
  *movey = *movey + *reversey;
}

// Печать победителя
void print_winner(int score1, int score2)
{
  printf("\033[H\033[J");
  if (score1 == 2)
  {
    printf("The winner is over there <<<");
  }
  else if (score2 == 2)
  {
    printf("The winner is over there >>>");
  }
}

int main()
{
  char input = 0; // Инициализация переменной input
  int paddle1_position = HEIGHT / 2;
  int paddle2_position = HEIGHT / 2;
  int score1 = 0;
  int score2 = 0;
  float ball_x = WIDTH / 2;
  float ball_y = HEIGHT / 2;
  float ball_dir_x = 1.0;
  float ball_dir_y = 1.0;

  /*Значение float ball_dir_x = 1.0 в коде означает начальное направление
  движения мяча по оси X. В данном контексте это значение указывает на то, что
  мяч будет двигаться вправо с шагом 1.0 единицы за каждый цикл обновления.

  Объяснение
  Положительное значение ball_dir_x:

  Если ball_dir_x положительное (например, 1.0), мяч будет двигаться вправо.
  Если мяч сталкивается с правой границей или ракеткой, значение ball_dir_x
  меняется на отрицательное, и мяч начинает двигаться влево. Отрицательное
  значение ball_dir_x:

  Если ball_dir_x отрицательное (например, -1.0), мяч будет двигаться влево.
  Если мяч сталкивается с левой границей или ракеткой, значение ball_dir_x
  меняется на положительное, и мяч начинает двигаться вправо. Пример Начальное
  значение:

  float ball_dir_x = 1.0; // Мяч движется вправо
  Обновление позиции мяча:

  ball_x += ball_dir_x; // Обновление позиции мяча по оси X
  Отражение мяча от границы или ракетки:

  if (ball_x >= WIDTH - 1 || ball_x <= 0) {
      ball_dir_x *= -1; // Изменение направления движения мяча
  }
  Итог
  Значение float ball_dir_x = 1.0 указывает на начальное направление движения
  мяча вправо. Если мяч сталкивается с границей или ракеткой, направление
  меняется на противоположное.*/

  set_input_mode();

  // Отрисовка игрового поля
  while (input != 'q' && score1 < 3 && score2 < 3)
  {
    // Очистка экрана
    printf("\033[H\033[J");

    draw_field(paddle1_position, paddle2_position, ball_x, ball_y, score1,
               score2);

    input = getchar();
    if (input != EOF)
    {
      update_paddle_positions(input, &paddle1_position, &paddle2_position);
    }
    updateBallPosition(&ball_x, &ball_y, &ball_dir_x, &ball_dir_y,
                       paddle1_position, paddle2_position, &score1, &score2);
    usleep(50000); // Задержка для замедления игры
  }
  print_winner(score1, score2);
  return 0;
}

//