#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int map[8][8] = { 0 };
    int i, j;
    int row, col;
    int count;

    srand((unsigned int)time(NULL));

    // 지뢰 10개 배치
    count = 0;

    while (count < 10)
    {
        i = rand() % 8;
        j = rand() % 8;

        if (map[i][j] != -1)
        {
            map[i][j] = -1;
            count++;
        }
    }

    // 주변 지뢰 개수 계산
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (map[i][j] == -1)
            {
                continue;
            }

            count = 0;

            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    int nr = i + r;
                    int nc = j + c;

                    if (nr >= 0 && nr < 8 &&
                        nc >= 0 && nc < 8)
                    {
                        if (map[nr][nc] == -1)
                        {
                            count++;
                        }
                    }
                }
            }

            map[i][j] = count;
        }
    }

    // 게임판 출력
    printf("===== MINESWEEPER =====\n");

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (map[i][j] == -1)
                printf(" * ");
            else
                printf(" %d ", map[i][j]);
        }
        printf("\n");
    }

    // 사용자 좌표 입력
    printf("\nEnter row and column (0~7): ");
    scanf_s("%d %d", &row, &col);

    if (row >= 0 && row < 8 && col >= 0 && col < 8)
    {
        if (map[row][col] == -1)
            printf("BOOM! Mine!\n");
        else
            printf("Number: %d\n", map[row][col]);
    }
    else
    {
        printf("Invalid input!\n");
    }

    return 0;
}
