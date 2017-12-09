#include <stdio.h>

void pyramid()
{
    int i, space, rows, k=0;
    printf("Enter number of rows: ");
    scanf("%d",&rows);
    printf("\nEntered rows: %d", rows);

    for(i=1; i<=rows; ++i, k=0)
    {
        for(space=1; space<=rows-i; ++space)
        {
            printf("  ");
        }

        while(k != 2*i-1)
        {
            printf("* ");
            ++k;
        }

        printf("\n");
    }
}

int main()
{
    pyramid();
    return 0;
}