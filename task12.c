/*
This is assignment 1
Done bt Timofei Podkorytov
for Operating systems course
in fall 2024
*/
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
/*
Here I tried to make a function call and print the errno value.

Initially the value was 0 when no call was made. If attempt a open call 
for a non existing file we get errno 2
*/

int main(){
    //all the values are printed to standard output
    printf("Error value initally is %d\n", errno);
    char* name="file.txt";//the name of the file we want to open
    int returnValue = open(name, O_RDWR);//open call
    printf("Error value after open is %d\n", errno);
    printf("Returned value is %d\n", returnValue);
    int closeValue = close(returnValue);//close call
    printf("Error value is %d\n", errno);
    printf("Close file value after close is %d\n", closeValue);
    return 0;
}