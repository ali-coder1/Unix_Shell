#include "exec.h"

/*
 Here some of the sources that I learned many things from them to do in this project and I walked through instructions on many websites and the man page as well.
 1)
 https://brennan.io/2015/01/16/write-a-shell-in-c/
 2)
 https://www.geeksforgeeks.org/making-linux-shell-c/
 3)
 https://gist.github.com/parse/966049
 4)
 https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf
 */




int main (int argc, char *argv[]) {
    
    
    char directory[BUFF], buff[BUFF], **arguments, **array, *stripe;
    
    int count = 0;
    
    array = pathDir(); // to have the directory path into an array
    
    
    printf("\n\t-------------------------------------------------------------------\n");
    printf("\t\t\t               SHELL\n");
    printf("\t-------------------------------------------------------------------\n");
    printf("\t   This applicaiton represents a shell that can perform \n");
    printf("\t   some of the most important functionalities of a regular shell.\n");
    printf("\t   Pipes, Input/Output, Background , signals\n");
    printf("\t   kill, cd, time, times, history, echo, exit, wait\n");
    printf("\t-------------------------------------------------------------------\n");
    printf("\n\n");
    
    
    
    for (count = 0; array[count] != 0; count++)
        dirStreaming(array[count]);
    
    for (;;) {
        
     printf ("%s> ", getcwd(directory,BUFF)); // get the pathname of the current working directory
        
    if ((stripe = fgets(buff, BUFF, stdin)) == NULL) {
            
     printf("wrong access !!\n");
    break;
    }
        
    while (isspace(*stripe)) // to make sure for white space for the command line
     stripe++;
        
     stripe[strlen(stripe) - 1] = 0;
        
     arguments = separation(stripe); // have it after tokenize it

    if ((arguments[0] = strtok(stripe," \n\t")) == NULL) continue;
    if (find(arguments[0]) && arguments[1] == NULL) {
            
     processing(arguments, false); // Here processing will take care to run most of regular commands
     printf ("command found");
    
    } else
            
     COMMANDS(arguments); // or COMMANDS will do run that commands I specified
        
    }
    
}
