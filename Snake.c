/* Bismillahir Rahmanir Raheem
   Author: Farid Islam
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

//  2 : 5
#define board_size 10
#define row (2 * board_size)
#define column (5 * board_size)

typedef struct snake Snake;
struct snake
{
    int r, c;
};

enum direction
{
    LEFT = 75,
    RIGHT = 77,
    UP = 72,
    DOWN = 80
};

char wall = '|';
char body = ':';
char fruit = '1';
char big_fruit = '5';
char head_sign = '>';
int score, high_score = 0;

char board[row][column + 1];
int body_length = 5;
Snake snake_body[(row - 2) * (column - 2)];

void wall_maker();
void display_board();
void snake_run();
void create_snake();
void play();
void snake_forward(Snake *head, char input);
void fruit_taken(char input);
void data_save_to_file();
void data_read_from_file();

int main()
{
    data_read_from_file();

    wall_maker();
    create_snake();
    play();

    _getch();
    _getch();
    _getch();
    _getch();
    _getch();

    return 0;
}

void play()
{
    Snake head;
    int fruit_exist = 1;
    int fruit_count = 0;
    score = 0;

    char direction;

    while (1)
    {
        if (fruit_exist)
        {
            if (fruit_count == 3)
            {
                fruit_taken(big_fruit);
                fruit_count = 0;
            }
            else
            {
                fruit_taken(fruit);
                fruit_count++;
            }

            fruit_exist = !fruit_exist;
        }
        system("cls");
        display_board();

        while (1)
        {
            direction = _getch();

            if (direction == 0)
            {
                //puts("Contunue...");
                continue;
            }
            else
            {
                //printf("direction = %d\n", direction);
                break;
            }
            
        }

        head = snake_body[body_length - 1];

        if (direction == LEFT)
        {
            head.c--;
        }
        else if (direction == RIGHT)
        {
            head.c++;
        }
        else if (direction == UP)
        {
            head.r--;
        }
        else if (direction == DOWN)
        {
            head.r++;
        }

        if (board[head.r][head.c] == ' ')
        {
            snake_forward(&head, board[head.r][head.c]);
        }
        else if (board[head.r][head.c] == fruit || board[head.r][head.c] == big_fruit)
        {
            snake_forward(&head, board[head.r][head.c]);
            fruit_exist = !fruit_exist;
        }
        else if (board[head.r][head.c] == wall || board[head.r][head.c] == body)
        {
            Snake head_of_snake = snake_body[body_length - 2];

            if(head_of_snake.r != head.r || head_of_snake.c != head.c)
            {
                puts("Game Over");
                if(high_score < score)
                    data_save_to_file();
                break;
            }

        }
    }
}

void data_save_to_file()
{
    FILE *fp = fopen("high_score.txt", "w");
    if(fp != NULL)
    {
        fprintf(fp, "%d", score);
        fclose(fp);
    }
    else
    {
        puts("Error! Could not write data");
        exit(1);
    }
    
}

void data_read_from_file()
{
    FILE *fp = fopen("high_score.txt", "r");
    if(fp != NULL)
    {
        fscanf(fp, "%d", &high_score);
        fclose(fp);
    }
    else
    {
        puts("Error! Could not read data");
        printf("Enter to continue...");
        _getch();
    }
    
}


void fruit_taken(char input)
{
    int r, c;

    while (1)
    {
        r = (rand() % (row - 2)) + 1;
        c = (rand() % (column - 2)) + 1;

        if (board[r][c] == ' ')
        {
            if (input == fruit)
            {
                board[r][c] = fruit;
            }
            else if (input == big_fruit)
            {
                board[r][c] = big_fruit;
            }
            break;
        }
    }
}

void snake_forward(Snake *head, char input)
{
    int r, c;

    if (input == ' ')
    {
        r = snake_body[0].r;
        c = snake_body[0].c;

        board[r][c] = ' ';

        for (int i = 0; i < body_length - 1; i++)// 0 1 2 3 4
        {
            snake_body[i] = snake_body[i + 1];
        }

        snake_body[body_length - 1] = *head;
        r = snake_body[body_length - 2].r;
        c = snake_body[body_length - 2].c;

        board[r][c] = body;

        r = snake_body[body_length - 1].r;
        c = snake_body[body_length - 1].c;

        board[r][c] = head_sign;
    }
    else if (input == fruit || input == big_fruit)
    {
        snake_body[body_length++] = *head;

        r = snake_body[body_length - 2].r;
        c = snake_body[body_length - 2].c;
        board[r][c] = body;

        r = snake_body[body_length - 1].r;
        c = snake_body[body_length - 1].c;
        board[r][c] = head_sign;

        if(input == fruit)
        {
            score++;
        }
        else if(input == big_fruit)
        {
            score += 5;
        }
    }
}

void snake_run()
{
    int tail = 1;
    int head = body_length;

    head++;
    //tail++;

    while (board[10][head] != 'X')
    {
        system("cls");
        board[10][tail] = ' ';
        board[10][head] = body;
        display_board();

        Sleep(1000);

        head++;
        tail++;
    }
}

void wall_maker()
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            if (i == 0 || i == row - 1)
            {
                board[i][j] = wall;
                //printf("#");
            }
            else
            {
                if (j == 0 || j == column - 1)
                {
                    board[i][j] = wall;
                    //printf("#");
                }
                else
                {
                    board[i][j] = ' ';
                    //printf(" ");
                }
            }
        }

        board[i][column] = '\0';
        //printf("\n");
    }
}

void create_snake()
{
    int middle_point = row / 2;

    for (int i = 0; i < body_length; i++)
    {
        snake_body[i].r = middle_point;
        snake_body[i].c = (i + 1);

        board[snake_body[i].r][snake_body[i].c] = body;
    }
    
    board[snake_body[body_length - 1].r][snake_body[body_length - 1].c] = head_sign;
}

void display_board()
{
    printf("\n\n");
    printf("High Score = %d\n", high_score);
    printf("Your Score = %d\n", score);
    printf("\n\n");
    for (int i = 0; i < row; i++)
    {
        printf("%s\n", board[i]);
    }
}
