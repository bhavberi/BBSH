#include<stdio.h>
void bubble(int [],int);
int main()
{
int array[50], n; 
printf("Enter number of elements:\n"); 
scanf("%d", &n);
printf("Enter %d integers:\n",n);
for (int c = 0; c < n; c++)
scanf("%d", &array[c]);
bubble(array,n); 
printf("Sorted list in ascending order:\n");
for (int c = 0; c < n; c++)
printf("%d\n", array[c]);
return 0;
}
void bubble(int x[],int b)
{
    int swap;
    for (int c = 0 ; c < b - 1; c++)
    {
       for (int d = 0 ; d < b - c - 1; d++)
       {
           if (x[d] > x[d+1]) 
           {
                swap = x[d]; 
                x[d] = x[d+1];
                x[d+1] = swap;
            }
        }
    }

}
