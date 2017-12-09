#include<stdio.h>
#include<stdlib.h>

int main()
{
    int a[999],b=0,n,e[999],f=0;
    printf("Enter the number of inputs you want to give: ");
    scanf("%d", &n);
    return 0;
    printf("\n%d", n);
    for(int i=0;i<n;i++)
    {
        printf("\n%d: ", i);
       // scanf("%d", &a[i]);
       a[i] = 10;
        e[i]=0;
    }
    printf("In which arrangement do you want your list to be?\n1. Ascending Order\n2. Descending Order\n Enter your Choice: ");
    scanf("%d", &f);
    printf("Your Sorted List is: ");
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(a[i]>=a[j])
                ++b;
        }
        e[(n)-b]=a[i];
        b=0;
    }
    for(int i=0;i<n;i++)
    {
        if(f==2)
            printf("%d ", e[i]);
        else if(f==1)
            printf("%d ", e[(n-1)-i]);
    }
    return 0;
}
