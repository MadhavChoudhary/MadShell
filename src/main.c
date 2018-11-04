#include <sys/wait.h> //for waitpid()
#include <unistd.h>  //for chdir(), fork(), exec(), pid_t
#include <stdlib.h> //for malloc(), realloc(), free(), exit(), execvp(), EXIT_FAILURE, EXIT_SUCESSS
#include <stdio.h> //for stderr, getchar(), perror(), printf(), scanf()
#include <string.h> //for strcmp(), strtok()
#include <sys/types.h> //for pid, exec() stuff

#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h> //for mkdir
#include <errno.h> //for error handling
#include <dirent.h> //for dir things like ls -l
#include <signal.h> //for kill

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
  // sleep(1);
}

/*GLOBAL VARIABLES*/
int pipe_count=0, fd;
static char* args[512];
char *history_file;
char input_buffer[1024];
char *cmd_exec[100];
int flag, len;
char cwd[1024];
int flag_pipe=1;
int no_of_lines;
int environmment_flag;

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    fflush(stdout);
}

int flag_pipe, flag_without_pipe;
char history_data[1000][1000];
char current_directory[1000];
int output_redirection, input_redirection;
int bang_flag;
char ret_file[3000];
char his_var[2000];
char *input_redirection_file;
char *output_redirection_file;
extern char** environ;
int background;
  
// void fileprocess ()
// {
//   int fd;
//   history_file=(char *)malloc(100*sizeof(char));
//   strcpy(history_file,current_directory);
//   strcat(history_file, "/");
//   strcat(history_file, "history.txt"); 
//   fd=open(history_file, O_RDONLY|O_CREAT,S_IRUSR|S_IWUSR);
//   int bytes_read=0, i=0, x=0, index=0;
//   char buffer[1], temp_data[1000];
//     do 
//     {
//         bytes_read = read (fd, buffer, sizeof (buffer));
//         for (i=0; i<bytes_read; ++i) 
//                   {
//                     temp_data[index]=buffer[i];
//                     index++;
//                     if(buffer[i]=='\n')
//                         {
//                           temp_data[index-1]='\0';
//                           no_of_lines++; 
//                           index=0;
//                           strcpy(history_data[x], temp_data);
//                           x++;
//                           temp_data[0]='\0';
//                         }
//                   }
//     }
//     while (bytes_read == sizeof (buffer)); 
//     close (fd); 
// }
// void filewrite()
// {
  
//   int fd_out,ret_write,str_len=0;
//   char input_data[2000];
//   no_of_lines++;
//   char no[10];
//   sprintf (no, "%d", no_of_lines ); 
//   strcpy(input_data, " ");
//   strcat(input_data, no);
//   strcat(input_data, " ");
//   strcat(input_data, input_buffer);

//   str_len = strlen(input_data);
//   fd_out=open(history_file,O_WRONLY|O_APPEND|O_CREAT,S_IRUSR|S_IWUSR);
//   len=strlen(input_buffer);
//   ret_write=write(fd_out,input_data,str_len);
//   if(ret_write<0) 
//         {
//           printf("Error in writing file\n");
//           return;
//         }
//   close(fd_out);

// }

void clear_variables()
{
  fd =0;
  flag=0;
  len=0;
  no_of_lines=0;
  pipe_count=0;
  flag_pipe=0;
  flag_without_pipe=0;
  output_redirection=0;
  input_redirection=0;
  input_buffer[0]='\0';
  cwd[0] = '\0';
  environmment_flag=0;
  bang_flag=0;
  background=0;
}

// void bang_execute()
// {
//   char bang_val[1000];
//   char *tokenise_bang[100], *num_ch[10];
//   int i, n=1, num, index=0;
//   i=1;
//   if(input_buffer[i]=='!')
//         {
//            strcpy(bang_val, history_data[no_of_lines-1]);
//         }
//   else if(input_buffer[i]=='-')
//     {
//         n=1;
//         num_ch[0]=strtok(input_buffer,"-");
//         while ((num_ch[n]=strtok(NULL,"-"))!=NULL)
//               n++;
//         num_ch[n]=NULL;
//         num = atoi(num_ch[1]);

//         index = no_of_lines-num;
//         strcpy(bang_val, history_data[index]);
              
//     }
//   else 
//     {
//       num_ch[0]=strtok(input_buffer,"!");
//       num = atoi(num_ch[0]);
//       strcpy(bang_val, history_data[num-1]);
//     }
//   tokenise_bang[0]=strtok(bang_val," ");
//   while ((tokenise_bang[n]=strtok(NULL,""))!=NULL)
//               n++;
//   tokenise_bang[n]=NULL;
//   strcpy(bang_val, tokenise_bang[1]);
//   printf("%s\n", bang_val );
//   strcpy(input_buffer, bang_val);

    
// }

// void environmment()
// {
//   int i =1, index=0;
//   char env_val[1000], *value;
//   while(args[1][i]!='\0')
//               {
//                    env_val[index]=args[1][i];
//                    index++;
//                     i++;
//               }
//   env_val[index]='\0';
//   value=getenv(env_val);

//   if(!value)
//       printf("\n");
//   else printf("%s\n", value);
// }

// void set_environment_variables()
// {  
// int n=1;
// char *left_right[100];
// if(args[1]==NULL)
//       {
//         char** env;
//           for (env = environ; *env != 0; env++)
//           {
//             char* value = *env;
//             printf("declare -x %s\n", value);    
//           }  
//          return; 
//       }
// left_right[0]=strtok(args[1],"=");
// while ((left_right[n]=strtok(NULL,"="))!=NULL)
//       n++;
// left_right[n]=NULL;

// setenv(left_right[0], left_right[1], 0);
// }

void parent_directory()
{
  if (getcwd(cwd, sizeof(cwd)) != NULL)
          {
           printf("%s\n", cwd );
          }
  else perror("getcwd() error");
}
// void echo_calling(char *echo_val)
// {
//   int i=0, index=0;
//   environmment_flag=0;
//   char new_args[1024],env_val[1000], *str[10];
//   str[0]=strtok(echo_val," ");
//   str[1]=strtok(NULL, "");
//   strcpy(env_val, args[1]);
//   if(str[1]==NULL)
//          {
//           printf("\n");
//           return;
//          } 
//   if (strchr(str[1], '$')) 
//                   {
//                   environmment_flag=1;
//                   }
  
//   memset(new_args, '\0', sizeof(new_args));
//   i=0; 
//   while(str[1][i]!='\0')
//     {
//       if(str[1][i]=='"')
//       {
//       index=0;     
//       while(str[1][i]!='\0')
//           {
//           if(str[1][i]!='"')
//                 {
//                 new_args[index]=str[1][i];
//                  flag=1;
//                 index++;
//                 }
//           i++;
//           }             
//       }
//       else if(str[1][i]==39)
//       {
//       index=0;
//       while(str[1][i]!='\0')
//           {
//           if(str[1][i]!=39)
//                 {
//                 new_args[index]=str[1][i];
//                  flag=1;
//                 index++;
//                 }
//           i++;
//           }               
//       }
//       else if(str[1][i]!='"')
//         {
//           new_args[index]=str[1][i];
//           index++;
//           i++;
//         }
//       else i++;    
//     }


// new_args[index]='\0';
// if((strcmp(args[1], new_args)==0)&&(environmment_flag==0))
//     printf("%s\n", new_args);
// else {
//      strcpy(args[1], new_args);
//       if(environmment_flag==1)
//                 {
//                 environmment();
//                 }
//       else if(environmment_flag==0)
//                 {
//                   printf("%s\n", new_args ); 
//                 }
//     }  
// }
// void history_execute_with_constants()
// {
//   int num, i, start_index;
//   if(bang_flag==1)
//         {
//          for(i=0; i<no_of_lines; i++)
//             printf("%s\n", history_data[i] ); 
//         }
//   else if(args[1]==NULL)
//       {
//         for(i=0; i<no_of_lines-1; i++)
//             printf("%s\n", history_data[i] );
//         printf(" %d %s\n", no_of_lines, his_var );
//       }
//   else
//       {
//         if(args[1]!=NULL)
//         num = atoi(args[1]);
//         if(num>no_of_lines) 
//         {
//           for(i=0; i<no_of_lines-1; i++)
//             printf("%s\n", history_data[i] );
//           printf(" %d %s\n", no_of_lines, his_var );
//         }
//         start_index=no_of_lines-num;
//         for(i=start_index; i<no_of_lines-1; i++)
//             printf("%s\n", history_data[i] );
//           printf(" %d %s\n", no_of_lines, his_var );
//       } 

// }

static char* skipwhite(char* s)
{
  while (isspace(*s)) ++s;
  return s;
}
void mad_parse_commands(char *com_exec)
{
  int m=1;
  args[0]=strtok(com_exec," ");       
  while((args[m]=strtok(NULL," "))!=NULL)
          m++;
}
void mad_parse_i_o(char *cmd_exec)
{
  char *io_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  io_token[0]=strtok(new_cmd_exec1,"<");       
  while((io_token[m]=strtok(NULL,">"))!=NULL)
        m++;
  io_token[1]=skipwhite(io_token[1]);
  io_token[2]=skipwhite(io_token[2]);
  input_redirection_file=strdup(io_token[1]);
  output_redirection_file=strdup(io_token[2]);
  mad_parse_commands(io_token[0]);
  
}
void mad_parse_i(char *cmd_exec)
{
  char *i_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  i_token[0]=strtok(new_cmd_exec1,"<");       
  while((i_token[m]=strtok(NULL,"<"))!=NULL)
        m++;
  i_token[1]=skipwhite(i_token[1]);
  input_redirection_file=strdup(i_token[1]);
  mad_parse_commands(i_token[0]);
}
void mad_parse_o(char *cmd_exec)
{
  char *o_token[100];
  char *new_cmd_exec1;  
  new_cmd_exec1=strdup(cmd_exec);
  int m=1;
  o_token[0]=strtok(new_cmd_exec1,">");       
  while((o_token[m]=strtok(NULL,">"))!=NULL)
          m++;
  o_token[1]=skipwhite(o_token[1]);
  output_redirection_file=strdup(o_token[1]); 
  mad_parse_commands(o_token[0]);   
  
}
char* skipcomma(char* str)
{
  int i=0, j=0;
  char temp[1000];
  while(str[i++]!='\0')
            {
              if(str[i-1]!='"')
                    temp[j++]=str[i-1];
            }
        temp[j]='\0';
        str = strdup(temp);
  
  return str;
}

static int mad_execute(int input, int first, int last, char *cmd_exec)
{

  int mypipefd[2], ret, input_fd, output_fd;
  ret = pipe(mypipefd);
  if(ret == -1){
        perror("pipe");
        return 1;
  }

  pid_t pid = fork();
 
  if (pid == 0) 
  {
        if (first==1 && last==0 && input==0) {
          dup2( mypipefd[1], 1 );
        } 
        else if (first==0 && last==0 && input!=0) 
        {
          dup2(input, 0);
          dup2(mypipefd[1], 1);
        } 
        else{
          dup2(input, 0);
        }
        if (strchr(cmd_exec, '<') && strchr(cmd_exec, '>')) 
        {
          input_redirection=1;
          output_redirection=1;
          mad_parse_i_o(cmd_exec);
        }
       else if (strchr(cmd_exec, '<')) 
            {
              input_redirection=1;
              mad_parse_i(cmd_exec);
            }
       else if (strchr(cmd_exec, '>')) 
            {
              output_redirection=1;
              mad_parse_o(cmd_exec);
            }
        if(output_redirection == 1){                
            output_fd= creat(output_redirection_file, 0644);
            if (output_fd < 0){
                fprintf(stderr, "Failed to open %s for writing\n", output_redirection_file);
                return(EXIT_FAILURE);
            }
            dup2(output_fd, 1);
            close(output_fd);
            output_redirection=0;
        }
          if(input_redirection  == 1){
              input_fd=open(input_redirection_file,O_RDONLY, 0);
              if (input_fd < 0){
                  fprintf(stderr, "Failed to open %s for reading\n", input_redirection_file);
                  return(EXIT_FAILURE);
               }
               dup2(input_fd, 0);
               close(input_fd);
               input_redirection=0;
          }
         // if (strcmp(args[0], "export") == 0){
         //              set_environment_variables();
         //              return 1;
         //  }
        // if (strcmp(args[0], "echo") == 0)
        //           {
        //           echo_calling(cmd_exec);
        //           } 
        // else if (strcmp(args[0], "history") == 0)
        //          {
        //           history_execute_with_constants();
        //           } 
     
        else if(execvp(args[0], args)<0){
            //General category for exe files
            printf("Executing file: %s\n",args[0]);
            char str[80];
            strcpy(str, "./");
            strcat(str,args[0]);
            args[0]=str;
            // printf("Executing file: %s\n",args[0]);
            if(execvp(args[0], args)<0){
              printf("Executable not found\n");
              exit(0);
            }
        }
  }
  else if(background==0){
     waitpid(pid, 0, 0);  
  }

  if (last == 1)
    close(mypipefd[0]);
  if (input != 0) 
    close(input);
  close(mypipefd[1]);

  return mypipefd[0];
}

int mad_cd(char **args);
int mad_help(char **args);
int mad_exit(char **args);
int mad_pwd(char **args);
int mad_env(char **args);
int mad_mkdir(char **args);
int mad_rmdir(char **args);
int mad_ls(char **args);
int mad_cp(char **args);

extern int errno; //to handle errors
/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
  "cd",
  "help",
  "env",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &mad_cd,
  &mad_help,
  &mad_env,
  &mad_exit
};

int mad_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/
int mad_cd(char **args)
{
  // printf("Came here\n");
  char *h="/home";   
  if(args[1]==NULL)
          chdir(h);
  else if ((strcmp(args[1], "~")==0) || (strcmp(args[1], "~/")==0))
          chdir(h);
  else if(chdir(args[1])<0)
      printf("bash: cd: %s: No such file or directory\n", args[1]);

  return 1;
}

int mad_pwd(char **args)
{
  if (getcwd(cwd, sizeof(cwd)) != NULL)
          {
           printf("%s\n", cwd );
          }
  else perror("getcwd() error");

  return 1;
}

int mad_mkdir(char **args)
{
  if(args[1]==NULL){
    printf("Enter location\n");
    return 0;
  }
  char *str = cwd;
  strcat(str, args[1]);
  if(mkdir(str, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==-1)
    perror("Mad_shell: ");

  return 1;
}

int mad_rmdir(char **args)
{
  if(args[1]==NULL){
    printf("Enter a Path to remove folder\n");
    return 0;
  }
  char *str = cwd;
  strcat(str, args[1]);
  if(rmdir(str)==-1)
    perror("Mad_shell: ");

  return 1;
}

int mad_ls(char **args)
{
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(cwd)) == NULL)
        printf("can’t open %s", cwd);

    while ((dirp = readdir(dp)) != NULL){
        struct stat fileStat;
        stat(dirp->d_name,&fileStat);
        printf("%s\n", dirp->d_name);   
      if(args[1]!=NULL){

        printf("————————————————————————\n");
        printf("File Size: \t\t%llu bytes\n",fileStat.st_size);
        printf("Number of Links: \t%d\n",fileStat.st_nlink);
        printf("File inode: \t\t%llu\n",fileStat.st_ino);
 
        printf("File Permissions: \t");
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");
 
        printf("The file %s a symbolic link\n\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not"); 
      } 
    }
    printf("\n");
    return 1;
}

int mad_cp(char **args){

  int BUFF_SIZE=1024;
  int srcFD,destFD,nbread,nbwrite;
  char *buff[BUFF_SIZE];
 
  if(strcmp(args[1], "--help")==0)
  {
    printf("\nUsage: cp source_file destination_file\n");
    return 0;
  }
 
  srcFD = open(args[1],O_RDONLY);
 
  if(srcFD == -1)
  {
    printf("\nError opening file %s errno = %d\n",args[1],errno);
    return 0; 
  }

  destFD = open(args[2],O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
 
  if(destFD == -1)
  {
    printf("\nError opening file %s errno = %d\n",args[2],errno);
    return 0;
  }
 
  /*Start data transfer from src file to dest file till it reaches EOF*/
  while((nbread = read(srcFD,buff,BUFF_SIZE)) > 0)
  {
    if(write(destFD,buff,nbread) != nbread)
      printf("\nError in writing data to %s\n",args[2]);
  }
  
  if(nbread == -1)
    printf("\nError in reading data from %s\n",args[1]);
  
  if(close(srcFD) == -1)
    printf("\nError in closing file %s\n",args[1]);
 
  if(close(destFD) == -1)
    printf("\nError in closing file %s\n",args[2]);
 
  return 1;
}

int mad_help(char **args)
{
   int i;
   printf("********************************************\n");
   printf("********************************************\n");
   printf("********************************************\n");
   printf("  Madhav's Shell\n");
   printf("  Type the commands with arguments required and press enter\n");
   printf("  The following are built in\n");

   for(i=0; i<mad_num_builtins(); i++){
     printf("  %s\n", builtin_str[i]);
   }
   
   printf("\nUse the man command for information on other programs\n\n");
   printf("********************************************\n");
   printf("********************************************\n");
   printf("********************************************\n");
   return 1;
}

int mad_env(char **args)
{
  if(args[1]==NULL){
    printf("Required Env Variable\n");
    return 0;
  }
  if(args[2]==NULL){
    printf("What to add to this variable?\n");
    return 0;
  }
    char *str = getenv(args[1]);
    strcat(str,":");
    strcat(str,args[2]);
    setenv("PATH",str, 1);
    system("echo $PATH");

    return 1;
}

int mad_exit(char **args)
{
  return 0;
}

static int mad_parse_launch(char *cmd_exec, int input, int first, int last)
{
    char *new_cmd_exec1;  
    new_cmd_exec1=strdup(cmd_exec);

        int m=1;
        int i;
        args[0]=strtok(cmd_exec," ");       
        while((args[m]=strtok(NULL," "))!=NULL)
              m++;
        args[m]=NULL;
        if (args[0] != NULL) 
            {

            // if (strcmp(args[0], "exit") == 0) 
            //         exit(0);
            if (strcmp(args[0], "echo") != 0) 
                    {
                      cmd_exec = skipcomma(new_cmd_exec1);
                      int m=1;
                      args[0]=strtok(cmd_exec," ");       
                      while((args[m]=strtok(NULL," "))!=NULL)
                                m++;
                      args[m]=NULL;

                    }

            //checking for builtin commands        
            for (i = 0; i < mad_num_builtins(); i++) {
              // printf("%s %s\n",args[0],builtin_str[i]);
              if (strcmp(args[0], builtin_str[i]) == 0) {
                return (*builtin_func[i])(args);
              }
            }
              int arg_len = strlen(args[0]);
              if(args[0][arg_len-1]== '&')
              {
                args[0][arg_len-1] = '\0';
                // printf("Pushing to background\n");
                background=1;
              }      
            }
    // if(background==1) return mad_execute_background(args);        
    return mad_execute(input, first, last, new_cmd_exec1);
}


void mad_parse_line()
{

    int i, n=1, input, first;

    input=0;
    first= 1;

    cmd_exec[0]=strtok(input_buffer,"|");

    while ((cmd_exec[n]=strtok(NULL,"|"))!=NULL)
          n++;
    cmd_exec[n]=NULL;
    pipe_count=n-1;
    for(i=0; i<n-1; i++){
      input = mad_parse_launch(cmd_exec[i], input, first, 0);
      first=0;
    }
    input=mad_parse_launch(cmd_exec[i], input, first, 1);
    input=0;

    return;

}

void prompt()
{
  // char shell[1000];
   // if (getcwd(cwd, sizeof(cwd)) != NULL)
   //      {
   //        strcpy(shell, "Mad_shell:");
   //        strcat(shell, cwd);
   //        strcat(shell, "> ");

   //        printf("%s", shell);
   //      }
   // else
   //     perror("getcwd() error");

   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
          printf("MadShell::");
          printf("%s> ",cwd);
        }
   else
       perror("getcwd() error");

}

int main()
{   
    char ch[2]={"\n"};
    getcwd(current_directory, sizeof(current_directory));
    signal(SIGINT, sigintHandler);
    init_shell();

    while (1)
    {
      clear_variables();
      prompt();
      fgets(input_buffer, 1024, stdin);
      if(strcmp(input_buffer, ch)==0)
            {
              continue;
            }
      // if(input_buffer[0]!='!')
      //       {
      //           fileprocess();
      //           filewrite(); 
      //       }         
      len = strlen(input_buffer);
      input_buffer[len-1]='\0';
      strcpy(his_var, input_buffer);
      if(strcmp(input_buffer, "exit") == 0) 
            {
              flag = 1;
              break;
            }
      // if(input_buffer[0]=='!')  
      //         {
      //           fileprocess();
      //           bang_flag=1;
      //           bang_execute();
      //         }
      mad_parse_line();
      // waitpid(pid,&status,0);
         
    }  
    if(flag==1)
      {
        printf("Closing...\n");
        exit(0);       
        return 0;
      }
return 0;
}
