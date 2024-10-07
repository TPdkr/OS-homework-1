/*
This is assignment 1
Done bt Timofei Podkorytov
for Operating systems course
in fall 2024
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
GLOBAL VERIABLES---------------------------------------------------------------
*/
//here we get the environment variable
extern char **environ;
//This variables stores whether we need to print to stderr
int writeTrace=0;

/*
FUNCTIONS----------------------------------------------------------------------
I created some functions in order to abstract and make code more readable.

All of them can print what we are doing if -v is used
*/

/*
PRINTING THE TEXT
this function prints an array of strings and is used for outputing environ

It takes an array of strings and prints the contents until
NULL is encountered
*/
void printenv(char** env){
    writeTrace ? fprintf(stderr,"we are printing the environment variable\n"):1;
    for(char** envLine = env; *envLine!=NULL; envLine++){
        printf("%s\n",*envLine);
    }
}

/*
IS ASSIGNMENT?
This function checks if a givenn expression is an assignment

It takes a string and returns whether it is a variable declaration
*/
int isAssignment(char* testString){
    writeTrace ? fprintf(stderr,"check if an arguement is an assignment\n"):1; 
    //does it have '='?   
    int hasequals=strstr(testString,"=")!=NULL;
    //there must only be one '=' sign!
    return ((hasequals)&&(strrchr(testString,'=')==strstr(testString,"=")));
 
}

/*
TEXT SIZE
This function finds the length of the multiline string and returns an integer

we give it the multiline string array
*/
int textSize(char** text){
    writeTrace ? fprintf(stderr,"environment size counted\n"):1; 
    int size=0;
    for(char** line=text; *line; line++){
        size++;
    }
    return size;
}

/*
COPY TEXT
This function copies the text from one poiter to the other

it takes the source and the destination pointer as well as the size
of the data being copied
*/
void textCopy(char*** dest, char** source, int size){
    writeTrace ? fprintf(stderr,"environment is copied for editing\n"):1; 
    //we allocate memory in a new variable and check the correctness
    *dest=(char**) malloc((size+1)*sizeof(char*));
    if (dest==NULL){
        fprintf(stderr,"error malloc");
    }
    writeTrace ? fprintf(stderr,"memory for copy allocated\n"):1; 
    //we move data to a new environment variable
    int lineNum=0;
    for(char** line = source; *line!=0; line++){
        (*dest)[lineNum]=strdup(*line);
        //check for correct copying
        if ((*dest)[lineNum]==NULL){
            fprintf(stderr,"error allocating memory\n");
            exit(1);
        }
        lineNum++;
    }
    writeTrace ? fprintf(stderr,"copy successfullt completed\n"):1; 
}

/*
EXECUTE COMMAND
It checks that a command is correctly executed and prints error otherwise.
It takes a pointer to an array of strings and the current arguement number.
*/
int executeCom(char** argv, int argPos){
    writeTrace ? fprintf(stderr, "trying to execute a command\n"):1;
    if(execvp(argv[argPos], &argv[argPos])==-1){
        //there was an issue executing the given command
        fprintf(stderr,"Error executing command\n");
        exit(1);
        return -1;
    }
    return 0;
}

/*
DELETE VARIABLE FROM TEXT
this is a function that deletes a variable from a text of assignments

It takes an array of strinngs we call text, a string containing variable name
and the pointer to text size
*/
int deleteName(char** text, char* name, int* size){
    //we print what we do to error
    writeTrace ? fprintf(stderr, "we are deleting an environment variable\n"):1;
    int found=0;
    //we search for the correct varibale and mover evrything forward 1 place
    //in order to destroy it
    for(int i=0; i<*size;i++){
        //we search for the variable
        //the name is at the beginning of the line and has = after it
        if ((strstr(text[i], name)==text[i])&& text[i][strlen(name)] == '='){
            found=1;
            free(text[i]);
        }
        //we copy elements forward
        if(found && (i <(*size-1))){
            text[i]=text[i+1];
        }
    }
    if(found){
        //we put null at the end and decrease size
        text[*size]=NULL;
        (*size)--;
        writeTrace ? fprintf(stderr,"successfully deleted the variable\n"):1;
        return 0;
    }
    //error!
    fprintf(stderr,"no such variable found!\n");
    return -1;
}
/*
MAIN---------------------------------------------------------------------------
*/

int main(int arg, char *argv[]){
    //some variables we need inside the main function
    int opt;//needed for choosing a flag
    int argPos=1;//the current arguement in array
    int envSize=textSize(environ);//the size of the environment variable
    char** envcpy;//we do all the operations with the copy of the environment
    //this gives us more safety and robustness

    //we copy the environ to envcpy
    textCopy(&envcpy, environ, envSize+arg);
    //here we determine the flags and do the corresponding actions
    while((opt = getopt(arg, argv, ":vu:"))!=-1){
        switch(opt){
            case 'v':
                //we need to trace of everything to stderr
                writeTrace=1;
                argPos++;
                fprintf(stderr,"writing to standard error enabled\n");
                break;
            case 'u':
                //we need to delete an environment variable
                argPos+=2;
                if (optarg == NULL) {
                    fprintf(stderr,"Missing argument for -u\n");
                    exit(1);
                }
                //we call the function that deletes
                deleteName(envcpy,optarg, &envSize);
                break;
            case ':':
                //nothing to be done but call an error
            case '?':
                //unknown flag meaning invalid input
                fprintf(stderr,"unknown flag detected!\n");
                exit(1);
                break;
        }
    }
    //we look through the given arguements in order to do coresponding actions
    for (; argPos<arg; argPos++){
        if(isAssignment(argv[argPos])){
            //we detected an assignment and need to add it to the envcpy
            writeTrace ? fprintf(stderr,"detected assignment\n"):1;
            envcpy[envSize]=strdup(argv[argPos]);
            //this is needed to check for errors
            if(envcpy[envSize]==NULL){
                fprintf(stderr,"error dublicating string\n");
                exit(1);
            }
            envSize++;
            writeTrace ? fprintf(stderr,"assignment added to the environment\n"):1;
        } else{
            /*we detected spmething that is not an assignment and thus must be 
            a command to execute*/
            executeCom(argv, argPos);
            break;
        }
    }
    //we copy the edited variable back
    textCopy(&environ, envcpy, envSize);
    //here we actually print the environment
    printenv(environ);
    //clearing memory in order to ensure no memory leaks
    writeTrace ? fprintf(stderr,"Execution completed. Clearing memory\n"):1;
    for (int i = 0; i < envSize; i++) {
        free(envcpy[i]);
    }
    free(envcpy);
    //done!
    return 0;
}

/*
raccoon                                             
                                                 .''.                 
              .;codl;.            ..          'lxxlcoc                
              c,',;cdkd;.  ...............  'dxd:'..,l                
              .'.. .;c:;...''',,''..'.','...':l:. ..;.                
               ..    ..',,'.',;::''';;,,..',::;. ..;'                 
                .'.  .,....',:cl:,.':oc:;,,,;:c:,co'                  
                  ';;:,,'';;:cckk:.'x0doollclcoooxc.                  
                .':cllododdOOO,oX;..dW:oKKK0000Okkkx;.                
               .:oddxkOOkxc.... c...,o....ck00KKKKKK0kdc'.            
             .:oxdoc::,..    .  ....'.. ......,:lx0KKKkl,.            
             .;cll:;.          .'....,'..........,lkOkl,.             
              ..,,..         .lkk:''c00d'  .   ...,loc'               
                ....        :kKKO;'';ONWKc.......':lc,.               
                 ..........'oxoo...;'.x0KK:;,;,'':loc,..              
                  ....   . .'cc,     .cOxc,.....;:lodl:'              
                ......         ......'..     ..',:clxxo:'.            
               ..''....                     ...';coodkko:'.           
              .'','......               .. ......',::lxko;'.          
              .',;,,,'..                  . .....',;;coxkdc,.         
             ..;::;....        .. ... ...   ....''.,:ccccddl:'.       
            ..';;'',,'...     ..   .'. ...  . .....',;:lclddo:.       
  '        .';:;;::;'......  .. .''.,........   ......';;:oddc'       
            .';;,,'..'.......... .,...' .....  ......',,,:lddl,       
          ................ ................... .......'..',,'..       
*/