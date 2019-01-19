#include "exec.h"

static COMMAND *root = 0;

// Here to set up to open a directory stream after having the path directory.

void dirStreaming (char *nameDir) {
    
    DIR *dir; // The DIR data type represents a directory stream.
    
    struct dirent *ptr; // struct dirent *readdir(DIR *dir);
    struct stat stripe;
    char P[BUFF]; // the path
    char *stream; // display the entry name
    
    
    dir =  opendir(nameDir); // opens and returns a directory stream for reading the directory
    if (dir == NULL) return;
    
    while ((ptr = readdir(dir))) { // returns a pointer to a structure representing the directory entry at the current position in the directory stream
        
    stream = ptr-> d_name; // now stream has the entry name
        
    sprintf (P, "%s/%s", nameDir, stream); // save the path line to P by sprintf()
        
    if (stat(P,&stripe) < 0) continue; // obtain information about the named file
        
    if (S_ISREG(stripe.st_mode) && (stripe.st_mode & 1)) // This macro should returns non-zero if the file is a regular file.
    save(stream,P);
    }
    closedir(dir);
}

// Here to get the path after ":"

int separate (char *array[], char *stripe, char target) {
    
    int count = 0;
    char *num, *copy;
    
    copy = stripe;
    
    while ((num = strchr(copy,target))) { // This step to match the path line with the target ":" to start count after ":"
        
        *num = 0;
        array[count++] = copy;
        copy = num+1;
    }
    array[count++] = copy;
    return count;
    
}

// Here to set up the directory path when the shell startup

char **pathDir() {
    
    char *ppath = getenv("PATH"); // get value of an It's environment with this pointer
    
    if (!ppath)
        
    ppath = PATH; // to make sure to get the right PATH
    
    char *stripe = strdup(ppath); // copy the string of the path to *buf
    
    int goal = 1;
    int count;
    
    for (count = 0; stripe[count]; count++)
    if (stripe[count] == TARGET)
    goal++;
    
    char **array = malloc((goal +1) * sizeof(char *));
    goal = separate(array, stripe, TARGET); // match the path line with the target ":"
    
    array[goal] = 0;
    
#ifdef DEBUG // enable the debugging mode with -DDEBUG option.
    
    for (count = 0; count < goal; count++)
        fprintf (stderr, "%s\n", array[count]);
#endif
    return array;
}


// searching from right to left on the tree and store the name and path into the tree
static COMMAND *lefting (COMMAND *T) {
    
    char *N = NULL, *P = NULL;
    
    if (T->R != NULL) {
        
	COMMAND *R = T->R;
        T->R = R->R;
        R->R = R->L;
        R->L = T->L;
        T->L = R;
        
        strcpy (N, T->N);
        strcpy (P, T->P);
        strcpy (T->N, R->N);
        strcpy (T->P, R->P);
        strcpy (R->N, N);
        strcpy (R->P, P);
        
    }
    return T;
}


// searching from left to right on the tree and store the name and path into the tree
static COMMAND *righting (COMMAND *T) {
    
    char *N = NULL, *P = NULL;
    
    if (T->L != NULL) {
        
        COMMAND *L = T->L;
        T->L = L->L;
        L->L = L->R;
        L->R = T->R;
        T->R = L;
        
        strcpy (N, T->N);
        strcpy (P, T->P);
        strcpy (T->N, L->N);
        strcpy (T->P, L->P);
        strcpy (L->N, N);
        strcpy (L->P, P);
    }
    return T;
}

// Here to set up the node for name and the path

static COMMAND *createNode (char *N, char *P) {
    
    COMMAND *pointer;
    
    pointer = malloc(sizeof(COMMAND));
    
    memset (pointer, 0, sizeof(COMMAND)); // fill a block of memory with a zero value
    
    pointer->N = strdup(N);
    pointer->P = strdup(P);
    
    return pointer;
}


// To set up the debugging message if the root did not succeed
int adding (COMMAND *T) {
    
    if (!T) return 0;
    return 1 + adding(T->L) + adding(T->R);
}

// To set up the debugging message if the root did not succeed
int bottom (COMMAND *T) {
    
    int Ltree;
    int Rtree;
    
    if (!T) return 0;
    
    Ltree = bottom(T->L);
    Rtree = bottom(T->R);
    
    return 1 + ((Ltree > Rtree) ? Ltree : Rtree);
    
}

// To set up the debugging message to stderr
void Tbug() {
    
    fprintf (stderr, "bottom = %d, added = %d\n", bottom(root), adding(root));
    
}


// here to store the path line and the name of entry

void save (char *N, char *P) {
    
    if (root == 0)
        
        root = createNode(N,P); // set the createNode function to set up the node
    
    else {
        
        layout(root,N,P); // or layout will make the node
    }
}

// Here to set up directions for the name and the path
static void layout (COMMAND *T, char *N, char *P) {
    
    int cmp;
    int right;
    
    for (;;) {
        
    cmp = strcmp(N, T->N);
    if (cmp == 0) return;
    if (cmp < 0) {
    if (T->L) {
                
    T = T->L;
    } else {
    T->L = createNode(N,P);
    return;
    }
    } else {
            
    if (T->R) {
                
    T = T->R;
    } else {
                
    T->R = createNode(N,P);
    return;
    }
    }
        
    }
    
    T = createState(T);
    right = countNode(T);
    T = state(T,right);
}

static COMMAND *createState (COMMAND *T) {
    
    while (T->L != NULL)
        T = righting(T);
    if (T->R != NULL)
        T->R = createState(T->R);
    return T;
    
}

// counting the right nodes

static int countNode (COMMAND *T) {
    
    int count = 1;
    
    if(T == NULL) return 0;
    
    while (T->R != NULL) {
        T = T->R;
        count++;
    }
    return count;
}


static COMMAND *state (COMMAND *T, int count) {
    
    int counted = count;
    COMMAND *Nroot = T;
    
    for (int a = 0; a < counted; a++){
        Nroot = lefting(Nroot);
        T = Nroot->R;
        
    for (int b = 0; b < counted/2-1; b++){
        T = lefting(T);
        T = T->R;
            
     }
        count>>=1;
     }
    
    return Nroot;
    
}

int find (char *word) {
    
    COMMAND *T;
    
    int cmp;
    
    T = root;
    
    while(T) {
        
    cmp = strcmp(word, T->N);
        
    if (cmp == 0) return 1;
        
    T = (cmp < 0) ? T->L : T->R; // Here to check if root less then 0 will go to left or will go to right
    }
    return 0;
}

