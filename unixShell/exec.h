// All needed Headers ..
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <dirent.h>
#include <assert.h>
#include <ctype.h>

// All needed define variables ..
#define BUFF 1024
#define BUFFS 64
#define TARGET      ":"
#define PATH "/bin:/usr/bin:/usr/local/bin"
#define BUFFTOK " \a\t\n\r"

typedef struct tree {
    
    char *N;  // The Name
    char *P;  // The Path
    struct tree *L, *R; // Left and Right
} COMMAND;

void dirStreaming (char *nameDir);
int find (char *word);
void save (char *N, char *P);

static COMMAND *createNode (char *N, char *P);
static void layout (COMMAND *T, char *N, char *P);

static COMMAND *lefting (COMMAND *T);
static COMMAND *righting (COMMAND *T);
static COMMAND *state (COMMAND *T, int count);
static int countNode (COMMAND *T);
static COMMAND *createState (COMMAND *T);

char **pathDir (void);
char **separation (char *stripe);

int processing (char **COMMAND_LINE, bool alive);
int COMMAND_ECHO (char **COMMAND_LINE);
int COMMAND_HISTORY (void);
int COMMAND_CD (char *P);
int COMMAND_KILL (char **COMMAND_LINE);
int COMMAND_TIMES (char **COMMAND_LINE);
int COMMAND_WAIT (char **COMMAND_LINE);
int COMMAND_EXIT (void);

void COMMANDS (char **COMMAND_LINE);
void processing_PIPE (char **COMMAND_AREA);
void outOfDir (char **COMMAND_LINE, char *outOfFile);
void insideDir (char **COMMAND_LINE, char *insideFile);
void AND (char **COMMAND_AREA);
void Tbug();
int bottom (COMMAND *T);
int adding (COMMAND *T);


