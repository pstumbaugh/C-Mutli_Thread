//create using:
// gcc --std=gnu99 -o main main.c  
//then run using:
//

#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>


struct movie *processFile(char *filePath);
struct movie *createMovie(char *currLine);
void redirectInput(char* inputFile);
void redirectOutput(char* outputFile);

int main(int argc, char *argv[])
{
    //variables
    bool inputFlag = false;
    bool outputFlag = false;
    bool runBothInputOutputFlag = false;
    char *inputFile = NULL;
    char *outputFile = NULL;
    
    //save stdin and stdout
    int stdin_save = dup(STDIN_FILENO); // save stdin position
    int stdout_save = dup(STDOUT_FILENO); // save stdout position



    for (int argvCounter = 0; argv[argvCounter] != NULL; argvCounter++)
    {
        if (strcmp(argv[argvCounter], "<") == 0) // input 
        {
            inputFlag = true;
            inputFile = argv[argvCounter+1];
            if (inputFile == NULL) //no file to read input from
            {
                perror("Error - No file to read input from");
                return 1;
            }
        }
        
        if (strcmp(argv[argvCounter], ">") == 0) // output 
        {
            outputFlag = true;
            outputFile = argv[argvCounter+1];
            if (outputFile == NULL) //no file to read input from
            {
                perror("Error - No file to print output to");
                return 1;
            }
        }
    }
    

    //redirect input and/or output here:
    if (inputFlag == true)
        redirectInput(inputFile);
    
    
    if (outputFlag == true && inputFlag == true) //both input and output needed
    {
        runBothInputOutputFlag = true;
    }

    dup2(stdout_save, STDOUT_FILENO); // restore standard out
    dup2(stdin_save, STDIN_FILENO); // restore standard in 

    return EXIT_SUCCESS;
}



/*

    struct movie *list = processFile(myFile);                                   ******need to change to arg
    int numOfMovies = movieCount(list);
    printf("Processed file %s and parsed data for %i movies\n\n",
        myFile, numOfMovies);

    




//creator for movie struct. Will parse through current line to gather info
//INPUT: currLine cstring pointer
//OUTPUT: movie struct node
struct movie *createMovie(char *currLine)
{
    //Variables
    //allocate memory for new movie struct (will free at end of function)
    struct movie *currMovie = malloc(sizeof(struct movie));
    // For use with strtok_r
    char *saveptr;
    char *langSaveptr;
    //token placeholders for info
    char *token;
    char *langToken;

    // Get movie title
    token = strtok_r(currLine, ",", &saveptr); //from first token
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);
    
    //get movie year
    token = strtok_r(NULL, ",", &saveptr); //from second token
    currMovie->tempYear = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempYear, token);
    //convert from string to int
    currMovie->year = atoi(currMovie->tempYear);

    //get movie language(s)
    //token includes all languages
    token = strtok_r(NULL, ",", &saveptr); //from third token
    langToken = strtok_r(token, ";", &langSaveptr); //get first language in token
    int langCounter = 0;
    while (langCounter < 5)
    {
        //nothing in token to save, break
        if (langToken == NULL)
        {
            break;
        }
        //first item in token list, remove leading "["
        else if (langCounter == 0)
        {
            if (langToken[strlen(langToken)-1] == ']') //only one language
            {
                int tempSize = sizeof(langToken);
                memcpy(langToken, langToken+1,tempSize);//remove leading "["
                //above adds last char on to word again ("]")
                //below is strlen(...)-2 to take off both trailing "]" characters
                langToken[strlen(langToken)-2] = '\0'; //replace last char with null
                int size = sizeof(langToken);
                strncpy(currMovie->languages[langCounter], langToken, size+1); //save language into array position
                break;
            }
            else
            {
                int tempSize = sizeof(langToken);
                memcpy(langToken, langToken+1,tempSize);
            }
        }

        //if last langauge, remove trailing "]" and break
        if (langToken[strlen(langToken)-1] == ']')
        {
            langToken[strlen(langToken)-1] = '\0'; //replace last char with null
            int size = sizeof(langToken);
            strncpy(currMovie->languages[langCounter], langToken, size+1); //save language into array position
            break;
        }
        //else copy item into languages array at desired array position
        else
        {
            int size = sizeof(langToken);
            strncpy(currMovie->languages[langCounter], langToken, size+1);
            //get next word, (or '\0' if nothing left)
            langToken = strtok_r(NULL, ";", &langSaveptr);
            langCounter++;
        }
    }

    //get movie rating
    //delimted by new line \n
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->tempRating = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->tempRating, token);
    //convert from string to double
    char* end;
    currMovie->rating = strtod(currMovie->tempRating, &end);
    
    //assume this node will be at end of list, set next pointer to NULL
    currMovie->next = NULL;

    return currMovie;
}


//read through input file and parse information into a linked list of movies
//INPUT: cstring pointer filePath
//OUTPUT: head pointer to linked list
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading
    FILE *moviesFile = fopen(filePath, "r");

    //variables to use while making new linked list of movies
    char *currLine = NULL; //assume starting with no data, null
    size_t len = 0;
    ssize_t nread;
    //setup head/tail pointers for linked list to NULL (empty list)
    //head will point to head of linked list.
    //tail will point to last item in linked list
    struct movie *head = NULL;
    struct movie *tail = NULL;

    //get first line and do nothing with it
    //first line is just column titles
    getline(&currLine, &len, moviesFile);

    // Read the file line by line (starting at line 2)
    //loop until out of data
    while ((nread = getline(&currLine, &len, moviesFile)) != -1)
    {
        //if in loop, assume there is a new movie to get
        //create a new movie node, call createMovie function to get info
        // from line
        struct movie *newNode = createMovie(currLine);

        //check if it's the first line. If yes, set head and tail pointers
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else //there are movies already in data, add to end of linked list
        {
            tail->next = newNode;
            tail = newNode; //reset tail pointer to current node
        }
    }
    
    free(currLine); //free up memory
    fclose(moviesFile); //close file
    return head;
}


*/



void redirectInput(char* inputFile)
{
    int fd0 = open(inputFile, O_RDONLY); //open input file read only
    if (fd0 == -1) //if error opening input file, display error
    {
        printf("cannot open %s for input\n", inputFile);
        fflush(stdout);
        exit(1);
    } 
    else 
    {
        if (dup2(fd0, STDIN_FILENO) == -1)  //redirect stdin to input file descriptor
        {
        	perror("dup2");
        	fflush(stdout);
        }
        close(fd0);	
    }
    return;
}




void redirectOutput(char* outputFile)
{
    //open output file
    int fd1 = open(outputFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd1 == -1) //if error creating output file, display error
    {
        printf("cannot create %s for output\n", outputFile);
        fflush(stdout);
        exit(1);			
    } 
    else
    {
        if (dup2(fd1, STDOUT_FILENO) == -1) //redirect stout to output file descriptor
        {
            perror("dup2");
            fflush(stdout);
        }
        close(fd1);	
    }
    return;
}




