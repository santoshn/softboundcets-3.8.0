#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){

  int arr[100];
  int num, i, j;

  size_t result = 0;

  for(i = 0; i < 100; i++){
    arr[i] = (i * 100 +2) % 102;
  }

  printf("enter a number\n");
  scanf("%d", &num);

  
  for(j= 0; j< 10000000; j++){
    for(i = 0; i< num; i++){
      result = result + arr[i];
    }
  }
 
  printf("the result is %zd\n", result);

  return 0;
}
