#include "exec.h"


// Here to tokenize the command line
char **separation (char *stripe) {
    
    char **tokens;
    char *token;
    int BUFFER;
    int i = 0;
    
    BUFFER = BUFFS;
    
    tokens = malloc (BUFFER * sizeof(char * ));
    
    if(!tokens) {
        
        fprintf (stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok (stripe, BUFFTOK);
    
    while (token != NULL) {
        
        tokens[i] = token;
        i++;
        
    if (i >= BUFFER) {
            
       BUFFER += BUFFS;
            
       tokens = realloc (tokens, BUFFER * sizeof(char *));
            
     if(!tokens){
                
       fprintf(stderr,"lsh: allocation error\n");
       exit(EXIT_FAILURE);
                
      }
    }
      token = strtok(NULL, BUFFTOK);
        
    }
    
    tokens[i] = NULL;
    return tokens;
    
}

// create process with Its child until the process status changed
int processing (char **COMMAND_LINE, bool alive) {
    
    pid_t pid = fork();
    
    int stat;
    
    if (pid == 0) {
        
        execvp (COMMAND_LINE[0], COMMAND_LINE);
        perror ("Command not found !!\n");
        
    } else
        
        if (!alive) {
            wait(&stat);
            
        }
    return (WEXITSTATUS(stat));
    
    return 0;
}

// Here to set up the commands and the symbols
void COMMANDS (char **COMMAND_LINE) {
    
    int box = 0, add;
    
    if (strcmp(COMMAND_LINE[0], "cd") == 0) {
        
        COMMAND_CD(COMMAND_LINE[1]);
        
    } else if (strcmp(COMMAND_LINE[0], "echo") == 0) {
        
        assert (COMMAND_ECHO(COMMAND_LINE));
        
    } else if (strcmp(COMMAND_LINE[0], "history") == 0) {
        
        COMMAND_HISTORY();
        
    } else if (strcmp(COMMAND_LINE[0], "kill") == 0) {
        
        assert (COMMAND_KILL(COMMAND_LINE));
        
    } else if (strcmp(COMMAND_LINE[0], "wait") == 0) {
        
        assert (COMMAND_WAIT(COMMAND_LINE));
        
    } else if (strcmp(COMMAND_LINE[0], "times") == 0) {
        
        assert (COMMAND_TIMES(COMMAND_LINE));
        
    } else if (strcmp(COMMAND_LINE[0], "exit") == 0) {
        
        assert (COMMAND_EXIT());
        
    } else {
        
        int index = 0;
        
     while (COMMAND_LINE[box] != NULL) {
            
     if (strcmp(COMMAND_LINE[box], "|") == 0) {
                
         processing_PIPE(COMMAND_LINE);
         index = 1;
                
     } else if (strcmp(COMMAND_LINE[box], "&") == 0) {
                
         AND(COMMAND_LINE);
         index = 1;
                
     } else if (strcmp(COMMAND_LINE[box], "<") == 0) {
                
         index = 1;
                
         add = box+1;
                
        if (COMMAND_LINE[add] == NULL
           || COMMAND_LINE[add+1] == NULL
           || COMMAND_LINE[add+2] == NULL) {
                    
          printf ("Not entered enough arguments !!\n");
                    
                    
      } else {
                    
        if (strcmp(COMMAND_LINE[add+1], ">") != 0) {
                        
          printf ("%s is not > !! try agian\n", COMMAND_LINE[add+1]);
          }
                    
      }
                
         index = 1;
         insideDir(COMMAND_LINE, COMMAND_LINE[box+2]);
                
      } else if (strcmp(COMMAND_LINE[box], ">") == 0) {
                
         index = 1;
                
        if (COMMAND_LINE[box+1] == NULL) {
                    
        printf ("Not entered enough arguments !!\n");
        }
          index = 1;
          outOfDir(COMMAND_LINE, COMMAND_LINE[box+1]);
                
       }
            box++;
       }
        if(index != 1)
          processing (COMMAND_LINE, false);
       }
}


// Here is the history command

int COMMAND_HISTORY() {
    
    int storage_size = 1024;
    char storage[storage_size];
    
    FILE * file = fopen(".history","r+");
    int num = 1;


    while (fgets(storage, storage_size, file) != NULL) {

        
        printf ("%3d", num);
        printf (" %s", storage);
        num++;
    }
    fclose (file);
    return 1;
}

// Here to change the current directory
int COMMAND_CD (char *P) {
    
    chdir(P);
    return 1;
}

// Here is the echo command
int COMMAND_ECHO (char **COMMAND_LINE) {
    
    int echo = 0;
    
    for (int box = 0;COMMAND_LINE[box] != NULL;box++) {
        
        echo++;
    }
    
    for (int box = 1; box < echo; box++)
        
        fprintf (stdout, "%s ",COMMAND_LINE[box]);
    fprintf (stdout, "\n");
    return 1;
}

// Here is the kill command
int COMMAND_KILL (char **COMMAND_LINE) {
    
    char *signal;
    
    signal = COMMAND_LINE[1];
    
    if (signal[0] == '-')
        signal++;
    
    int s = atoi(signal);
    pid_t pid = (pid_t) atoi(COMMAND_LINE[2]);
    
    kill(pid,s);
    perror("kill");
    
    return 1;
    
}

// Here is the wait command
int COMMAND_WAIT (char **COMMAND_LINE) {
    
    int stat;
    
    pid_t pwait = atoi(COMMAND_LINE[1]);
    
    waitpid (pwait, &stat, 0);
    
    while (!WIFEXITED(stat)) {
        sleep(1);
    }
    return 1;
}

// Here is the times command
int COMMAND_TIMES (char **COMMAND_LINE) {
    
    struct timeval first, last;
    
    gettimeofday (&first, NULL);
    
    int stat;
    
    pid_t pid = fork();
    
    if (pid == 0) {
        
        execvp(COMMAND_LINE[1], COMMAND_LINE+1);
        perror("execvp");
        
    } else {
        
        wait(&stat);
        gettimeofday (&last, NULL);
        
    }
    
    double date = (last.tv_sec - first.tv_sec) +
    ((last.tv_usec - first.tv_usec) / 1000000.0);
    
    printf ("times : %f seconds\n", date);
    
    return 1;
}

// Here is the exit or logout command
int COMMAND_EXIT() {
    
    kill(getpid(), 9);
    perror("kill");
    return 1;
    
}

// Here to set up the pipe processing
void processing_PIPE (char **COMMAND_AREA) {
    
    int block = 0;
    int create = 0;
    int check = 0;
    int cal = 0;
    int buf = 0;
    int err = -1;
    int input1[2]; // input1[1] = Write , input1[0] = Read
    int input2[2]; // input2[1] = Write , input2[0] = Read
    int COMMAND_NUM = 0;
    char *COMMAND_LINE[BUFF];
    
    
    
    while (COMMAND_AREA[cal] != NULL) {
        
    // calculate the number of commands between the pipes
    if (strcmp(COMMAND_AREA[cal], "|") == 0) {
      COMMAND_NUM++;
    }
      cal++;
    }
    
    COMMAND_NUM++;
    
    // check each command between pipes
    while (COMMAND_AREA[check] != NULL && block != 1) { // block and break when = 1
        
     buf = 0;
        
    // save the commands in an array
    while (strcmp(COMMAND_AREA[check], "|") != 0) {
            
      COMMAND_LINE[buf] = COMMAND_AREA[check];
      check++;
            
    if (COMMAND_AREA[check] == NULL) {
                
      block = 1;
      buf++;
      break;
                
     }
      buf++;
     }
        
        
     COMMAND_LINE[buf] = NULL;
     check++;
        
    if (create % 2 != 0) {
      
      pipe(input1);
    } else {              // pipe is a connection between the two processes
      pipe(input2);
    }
        
     pid_t pid = fork(); // create process
        
    if (pid == -1) {
            
    if (create != COMMAND_NUM -1) {
                
    if (create % 2 != 0) {
                    
       close(input1[1]); // for odd
    } else {
       close(input2[1]); // for even
    }
                
    }
       printf ("process child not created !!\n");
       return;
            
    }
        
    if (pid == 0) {
            
    if (create == 0) {
       dup2(input2[0], STDIN_FILENO); //read
                
    } else if (create == COMMAND_NUM -1) {
                
    if (COMMAND_NUM % 2 != 0) {
       dup2(input1[0], STDIN_FILENO); //read
    } else {
       dup2(input2[0], STDIN_FILENO); //read
    }
                
    } else {
                
   if (create % 2 != 0) {
                    
        dup2(input2[0], STDIN_FILENO); //read
        dup2(input1[1], STDOUT_FILENO); //write
    } else {
        dup2(input1[0], STDIN_FILENO); //read
        dup2(input2[1], STDOUT_FILENO); //write
    }
                
    }
            
    if (execvp(COMMAND_LINE[0], COMMAND_LINE) == err) {
    kill(getpid(), SIGTERM);
                
    }
            
    }
        
        
    // now just close the pipe descriptors
    if (create == 0) {
    close (input2[1]);
            
    } else if (create == COMMAND_NUM -1) {
            
    if (COMMAND_NUM % 2 != 0) {
                
    close (input1[0]);
    } else {
    close (input2[0]);
    }
            
    } else {
            
            
    if (create % 2 != 0) {
                
    close (input2[0]);
    close (input1[1]);
    } else {
    close (input1[0]);
    close (input2[1]);
    }
            
    }
        
    waitpid(pid,NULL,0);
    create++;
    }
}
// command > file
void outOfDir (char **COMMAND_LINE, char *outOfFile) {
    
    int file = open (outOfFile, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR, 0600);
    perror("open");
    
    int stat;
    pid_t pid = fork();
    
    if (pid == 0) {
        dup2(file,1); // Here to duplicate a file descriptor with one
        close(file);
        perror("close");
        processing(COMMAND_LINE,false);
        
    } else {
        close (file);
        perror ("close");
        wait (&stat);
    }
    
}

// command < file
void insideDir (char **COMMAND_LINE, char *insideFile) {
    
    int file = open (insideFile, O_RDONLY, 0600);
    perror("open");
    
    int stat;
    
    pid_t pid = fork();
    
    if (pid == 0) {
        dup2(file,0); // Here to duplicate a file descriptor with zero
        processing(COMMAND_LINE,false);
        exit(0);
    } else {
        wait (&stat);
    }
}

void AND (char **COMMAND_AREA) {
    
    processing(COMMAND_AREA,true);
}
