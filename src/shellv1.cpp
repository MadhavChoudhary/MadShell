#include <sys/wait.h> //for waitpid()
#include <unistd.h>  //for chdir(), fork(), exec(), pid_t
#include <stdlib.h> //for malloc(), realloc(), free(), exit(), execvp(), EXIT_FAILURE, EXIT_SUCESSS
#include <stdio.h> //for stderr, getchar(), perror(), printf(), scanf()
#include <string.h> //for strcmp(), strtok()
#include <sys/types.h> //for pid, exec() stuff

#include <string>
#include <iostream>
#include <vector>

#define MAD_RL_BUFSIZE 1024
#define MAD_CMD_BUFSIZE 1024
#define MAD_MAX_CMD 10
#define MAD_TOK_BUFSIZE 64
#define MAD_TOK_DELIM " " //change the delimiter if you want
#define MAD_CMD_DELIM " | "
#define clear() printf("\033[H\033[J")

using namespace std;

int num_commands;

int mad_cd(string *args);
int mad_help(string *args);
int mad_exit(string *args);

void init_shell()
{
	clear();
	printf("\n\n\n\n");
	printf("********************************************\n");
	printf("********************************************\n");
	printf("********************************************\n");
	printf("  ************   MAD SHELL   **************\n");
	// string username = getenv("USER");
	// printf("  ************  USER = %s  **************\n", username);
	printf("********************************************\n");
	printf("********************************************\n");
	printf("********************************************\n");
	printf("\n\n\n");
	sleep(1);
}

void mad_parse_command(string *commands, string **args)
{
	printf("Came to mad_parse_command\n");

	int position,idx=0;
	string delimiter = MAD_TOK_DELIM;
	string line;

	for(idx=0; idx<num_commands; idx++)
	{	
		position=0;
		line = commands[idx];
		size_t pos = 0;
		while ((pos = line.find(delimiter)) != string::npos) {
		    args[idx][position++] = line.substr(0, pos);
		    line.erase(0, pos + delimiter.length());
		}
		args[idx][position++] = line;
		
	}

	// for(idx=0; idx<num_commands; idx++)
	// {	for(position=0; position<2; position++){
	// 		if(args[idx][position].c_str()==NULL)
	// 			printf("empty ");
	// 		else
	// 			cout<<args[idx][position].c_str()<<" ";
	// 	}
	// 	cout<<endl;
	// }

	return;
}

void mad_parse_line(string line, string *commands)
{
	printf("Came to mad_parse_line\n");

	string delimiter = MAD_CMD_DELIM;
	int i=0;

	size_t pos = 0;
	while ((pos = line.find(delimiter)) != string::npos) {
	    commands[i++] = line.substr(0, pos);
	    line.erase(0, pos + delimiter.length());
	}
	commands[i++] = line;
	num_commands = i;

	return;
}

// string builtin_str[] = {
// 	"cd",
// 	"help",
// 	"exit"
// };

// int (*builtin_func[])(string *) = {
// 	&mad_cd,
// 	&mad_help,
// 	&mad_exit
// };

// int mad_num_builtins(){
// 	return sizeof(builtin_str)/sizeof(string );
// }

// //Implementation of builtin functions

// int mad_cd(string *args)
// {
// 	if(args[1] == NULL){
// 		fprintf(stderr, "mad_shell: expected an argument to cd\n");
// 	}else{
// 		if(chdir(args[1])!=0){
// 			perror("msh");
// 		}
// 	}			//change has to be made for no permission error

// 	return 1;
// }

// int mad_help(string *args)
// {
// 	int i;
// 	printf("********************************************\n");
// 	printf("********************************************\n");
// 	printf("********************************************\n");
// 	printf("Madhav's Shell\n");
// 	printf("	Type the commands with arguments required and press enter\n");
// 	printf("	The following are built in\n");

// 	for(i=0; i<mad_num_builtins(); i++){
// 		printf("	%s\n", builtin_str[i]);
// 	}


// 	printf("\nUse the man command for information on other programs\n\n");
// 	printf("********************************************\n");
// 	printf("********************************************\n");
// 	printf("********************************************\n");
// 	return 1;
// }

// int mad_exit()
// {
// 	return 0;
// }

// void closeAllPipes(pid_t *pipes)
// {
// 	int i;
// 	for(i=0; i<2*num_commands; i++)
// 		close(pipes[i]);
// }

// int mad_launch(string **args, int idx, pid_t *pipes)
// {
// 	if(idx>=num_commands)
// 		return 0;
// 	printf("At process %s\n",args[idx][0]);
// 	pid_t pid = fork();
// 	int status;
// 	if(pid==0)
// 	{
// 		if(idx==0)
// 			dup2(pipes[1], 1);
// 		else if(idx == num_commands-1)
// 			dup2(pipes[2*(idx-1)], 0);
// 		else
// 		{
// 			dup2(pipes[2*(idx-1)], 0);
// 			dup2(pipes[2*(idx)+1], 1);
// 		}

// 		closeAllPipes(pipes);
// 		execvp(args[idx][0], args[idx]);
// 	}
// 	else if(pid>0){
// 		mad_launch(args, idx+1, pipes);
// 		// do {
// 	 //      waitpid(pid, &status, WUNTRACED);
// 	 //    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
// 	}

// 	return 1;
// }

int mad_execute(string **args)
{
	printf("Came to mad_execute\n");
  int status;
  int i,j=0;

  // arguments for commands; your parser would be responsible for
  // setting up arrays like these

  vector<char *> commands[num_commands];

  for(i=0; i<num_commands; i++)
  {
  	j=0;
  	while(args[i][j]!=""){
  		commands[i].push_back(const_cast<char *>(args[i][j].c_str()));
  		j++;
  	}
  	commands[i].push_back(nullptr);

  }


// make 2 pipes (cat to grep and grep to cut); each has 2 fds

  int pipes[4];
  pipe(pipes); // sets up 1st pipe
  pipe(pipes + 2); // sets up 2nd pipe

  // we now have 4 fds:
  // pipes[0] = read end of cat->grep pipe (read by grep)
  // pipes[1] = write end of cat->grep pipe (written by cat)
  // pipes[2] = read end of grep->cut pipe (read by cut)
  // pipes[3] = write end of grep->cut pipe (written by grep)

  // Note that the code in each if is basically identical, so you
  // could set up a loop to handle it.  The differences are in the
  // indicies into pipes used for the dup2 system call
  // and that the 1st and last only deal with the end of one pipe.

  // fork the first child (to execute cat)
  sleep(1);
  if (fork() == 0)
    {

    	printf("Fork command 1\n");
      // replace cat's stdout with write part of 1st pipe

      dup2(pipes[1], 1);

      // close all pipes (very important!); end we're using was safely copied

      close(pipes[0]);
      close(pipes[1]);
      close(pipes[2]);
      close(pipes[3]);

      printf("Exec 1 command\n");
      execvp(args[0][0].c_str(), commands[0].data());
    }
  else
    {
    	printf("Fork 1 fail\n");
      // fork second child (to execute grep)

      if (fork() == 0)
	{
		printf("Fork command 2\n");
	  // replace grep's stdin with read end of 1st pipe
	  
	  dup2(pipes[0], 0);

	  // replace grep's stdout with write end of 2nd pipe

	  dup2(pipes[3], 1);

	  // close all ends of pipes

	  close(pipes[0]);
	  close(pipes[1]);
	  close(pipes[2]);
	  close(pipes[3]);

	  printf("Exec 2 command\n");
	  execvp(args[1][0].c_str(), commands[1].data());
	}
      else
	{

		printf("fork 2 fail\n");
	  // fork third child (to execute cut)


	  if (fork() == 0)
	    {
	    	printf("fork command 3\n");
	      // replace cut's stdin with input read of 2nd pipe

	      dup2(pipes[2], 0);

	      // close all ends of pipes

	      close(pipes[0]);
	      close(pipes[1]);
	      close(pipes[2]);
	      close(pipes[3]);

	      printf("Exec 3 command\n");
	      execvp(args[2][0].c_str(), commands[2].data());
	    }

	}
    }
      
  // only the parent gets here and waits for 3 children to finish
  
  close(pipes[0]);
  close(pipes[1]);
  close(pipes[2]);
  close(pipes[3]);
  printf("Out?\n");
  for (i = 0; i < 3; i++)
    wait(&status);
	printf("Really Out?\n");
}

// // int mad_execute(string **args)
// // {
// // 	printf("Came to mad_execute\n");

// // 	if(num_commands==1){
// // 		if(args[0][0]==NULL) return 1; //doesnt work

// // 		if(execvp(args[0][0], args[0])<0)
// // 		{
// // 			printf("Completed Process\n");
// // 			printf("Error executing the command\n");
// // 			return 1;
// // 		}
// // 		printf("Completed Process\n");
// // 		exit(EXIT_FAILURE);
// // 	}

// // 	int i;
// // 	int status;

// // 	pid_t pipes[2*num_commands];

// // 	for(i=0; i<2*num_commands; i+=2){
// // 		pipe(pipes+i);
// // 	}

// // 	for(i=0;i<num_commands;i++)
// // 		printf("%s\n",args[i][0]);

// // 	mad_launch(args, 0, pipes);

// // 	closeAllPipes(pipes);

// // 	for(i=0; i<num_commands-1; i++)
// // 		wait(&status);	

// // 	return 1;
// // }

// int lsh_launch(string **args)
// {
//   pid_t pid;
//   int status;

//   pid = fork();
//   if (pid == 0) {
//     // Child process
//     if (execvp(args[0][0], args[0]) == -1) {
//       perror("lsh");
//     }
//     exit(EXIT_FAILURE);
//   } else if (pid < 0) {
//     // Error forking
//     perror("lsh");
//   } else {
//     // Parent process
//     do {
//       waitpid(pid, &status, WUNTRACED);
//     } while (!WIFEXITED(status) && !WIFSIGNALED(status));
//   }

//   return 1;
// }

// int lsh_execute(string **args)
// {
//   int i;

//   if (args[0][0] == NULL) {
//     // An empty command was entered.
//     return 1;
//   }

//   return lsh_launch(args);
// }

void mad_prompt()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s>", cwd);
}

void mad_shell(void)
{
	string line;
	string *commands = new string[MAD_CMD_BUFSIZE];
	string **args = new string *[MAD_MAX_CMD];

	int status,i,idx,position;

	for(i=0;i<MAD_MAX_CMD;i++)
		args[i] = new string[MAD_CMD_BUFSIZE];

	init_shell();

	do
	{
		mad_prompt();
		getline(cin, line);
		mad_parse_line(line,commands);
		mad_parse_command(commands, args);

		status = mad_execute(args);

		delete[] commands;
		delete[] args;

	}while(status);
}

int main(int argc, char **argv)
{
	//load config files to set prompt style 

	//Run the mad shell
	mad_shell();

	//Cleanup process

	//Exit
	return EXIT_SUCCESS;
}