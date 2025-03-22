#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define ROCKET_HEIGHT 3

void draw_field(int paddle1_position, int paddle2_position, int score1, int score2)
{
  for (int y = 0; y < HEIGHT; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      if (x <= 1 || x >= WIDTH - 2)
      { // отрисовка крайних позиций поля по вертикали
        printf("|");
      }
      else if (x == 4 && (y >= paddle1_position - 1 && y <= paddle1_position + 1))
      { // отрисовка позиций первой ракетки
        printf("|");
      }
      else if (x == WIDTH - 4 && (y >= paddle2_position - 1 && y <= paddle2_position + 1))
      { // отрисовка позиций второй ракетки
        printf("|");
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

void update_paddle_positions(char input, int *paddle1_position, int *paddle2_position)
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

// Печать победителя
void print_winner(int score1, int score2)
{
  printf("\033[H\033[J");
  if (score1 == 3)
  {
    printf("The winner is first player");
  }
  else if (score2 == 3)
  {
    printf("The winner is second player >>>");
  }
}

int update_paddle1_position(char input, int paddle1_position)
{
  if ((input == 'a' || input == 'A') && paddle1_position > 1)
  {
    return paddle1_position - 1;
  }
  else if ((input == 'z' || input == 'Z') && paddle1_position < HEIGHT - 2)
  {
    return paddle1_position + 1;
  }
  return paddle1_position;
}

int update_paddle2_position(char input, int paddle2_position)
{
  if ((input == 'k' || input == 'K') && paddle2_position > 1)
  {
    return paddle2_position - 1;
  }
  else if ((input == 'm' || input == 'M') && paddle2_position < HEIGHT - 2)
  {
    return paddle2_position + 1;
  }
  return paddle2_position;
}

int main()
{
  char input = 0; // Инициализация переменной input
  int paddle1_position = HEIGHT / 2;
  int paddle2_position = HEIGHT / 2;
  int score1 = 0;
  int score2 = 0;

  // Отрисовка игрового поля
  while (input != 'q' && score1 < 21 && score2 < 21)
  {
    // Очистка экрана
    printf("\033[H\033[J"); //очистка поля

    draw_field(paddle1_position, paddle2_position, score1, score2);

    input = getchar();
    if (input != EOF && input != ' ')
    {
      update_paddle_positions(input, &paddle1_position, &paddle2_position);
    } //вот здесь должно быть реализовано условие "Space Bar для пропуска действия на очередном шаге игры в пошаговом режиме."

    usleep(5000); // Задержка для замедления игры
  }
  print_winner(score1, score2);
  return 0;
}
