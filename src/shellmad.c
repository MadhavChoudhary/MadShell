#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#define clear() printf("\033[H\033[J")

void init_shell()
{
  clear();
  printf("\n\n\n\n");
  printf("********************************************\n");
  printf("********************************************\n");
  printf("********************************************\n");
  printf("  ************   MAD SHELL   **************\n");
  printf("********************************************\n");
  printf("********************************************\n");
  printf("********************************************\n");
  printf("\n\n\n");
}

void prompt()
{
   char cwd[100];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
          printf("MadShell::");
          printf("%s> ",cwd);
        }
   else
       perror("getcwd() error");
}

void mad_cd()
{
  // printf("Came here\n");
  char des[80];
  scanf("%s", des);
  char *h="/home";   
          chdir(h);
  // else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
  //         chdir(h);
  // else if(chdir(args[1])<0)
  //     printf("bash: cd: %s: No such file or directory\n", args[1]);

}

int main()
{
  
  char input_buffer[1024];
  int len;
  init_shell();

  while(1)
  {
    prompt();
    fgets(input_buffer, 1024, stdin);
    len = strlen(input_buffer);
    input_buffer[len-1]='\0';

    if(strcmp(input_buffer, "exit")==0)
      break;
  	if(strcmp(input_buffer, "cd")==0){
  		char des[80];
  		scanf("%s",des);
  		chdir(des);
  	}
  		
    system(input_buffer);     
  }
  	

}