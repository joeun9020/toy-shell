#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    100
#define MAX_DIR    100    
#define LEN_HOSTNAME	30


int main(void)
{
    char command[MAX_LEN_LINE];
    char *arg;
    char *args[] = {command,arg, NULL};

    int ret, status;
    pid_t pid, cpid;

///// hostname

    char hostname[LEN_HOSTNAME + 1];
    memset(hostname, 0x00, sizeof(hostname));
    gethostname(hostname, LEN_HOSTNAME);
 

///// cwd
    char cwd[MAX_DIR];
    getcwd(cwd,sizeof(cwd));




    while (true) {
        char *s;
        int len;


	///// print&color
        printf("\e[36;1m%s@\e[36;1m%s:", getpwuid(getuid())->pw_name,hostname);    //green
	printf("\e[33m%s$ \e[0m",cwd);      //yellow and reset



	///// command
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
 	}
	

	len = strlen(command);
	
	if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }	

	arg=strtok(command, " ");
        if(strcmp(arg,"exit")==0){   // 'exit' command 
            printf("Exit.\n");
	    exit(0);
       	}
	else if (strcmp(arg, "clear")==0){ // 'clear' command  	
    	    system("clear");		
	    arg=strtok(NULL, " ");
	    args[0]="/bin/clear";
	    args[1]=arg;	        
	}
	

	
        ///// make child process    
        pid = fork(); 
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            //printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                //printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            if (strcmp(command,"ls")==0){  // 'ls' command
		arg=strtok(NULL, " "); //if no more seperation, return NULL
		args[0]="/bin/ls";
		args[1]=arg;
		}
	    else if (strcmp(arg,"pwd")==0){  // 'pwd' command
		arg=strtok(NULL, " ");
		args[0]="/bin/pwd";
		args[1]=arg;
		}
	    
	    

	    
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
