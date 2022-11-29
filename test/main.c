//
// Created by Ян Дербиков  on 29.11.2022.
//

#include <stdio.h>
#ifndef LAB5_COPY_MAIN_C
#define LAB5_COPY_MAIN_C


struct student {
    int id;
    float percentage;
    char name;
};


int main(){

    char array[5] = {'H', 'e', 'l', 'l', 'o'};
    char arr[] = "Hello";
    int nums [3] = {23, 56,13};
    printf("element is %d\n", nums[2]);
    printf("element is %d\n", arr[2]);
    printf("element is %d\n", array[4]);


    struct student oleg;
    oleg.id = 3;
    oleg.percentage = 12.323233;

    struct student alex;
    alex.percentage = 3.2323;
    alex.name = 'O';

    char flex = (char) "d32";

    printf("hello %.2f and %.2f",
           oleg.percentage, alex.percentage);
    printf("%s", flex);



    return 0;
}


#endif //LAB5_COPY_MAIN_C
