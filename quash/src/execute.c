/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */

#include "execute.h"

#include <stdio.h>
#include <fcntl.h> //used for accessing constants associated with open(file id, ALL_IMPORTANT_PARAMETER_FLAGS);

#include "quash.h"
#include "pid_queue.h" //used to store Process IDs

// Remove this and all expansion calls to it
/**
 * @brief Note calls to any function that requires implementation
 */
#define IMPLEMENT_ME()                                                  \
	fprintf(stderr, "IMPLEMENT ME: %s(line %d): %s()\n", __FILE__, __LINE__, __FUNCTION__)

struct Job{
	int job_id; //used to identify current job. When added to background queue, assigned id of 1 + id of last job on queue
	int is_background_job; //int representation of boolean
	pid_queue process_queue; //the processes running as part of the job (either foreground, so wait to finish... or background, so monitor them)
};

//declare static variables
static int pipes_c_p[2][2]; //two pipes: pipes for input to and output from current (or last used) process. 0 end is output from pipe, 1 is input to pipe
static int write_to_pipe_index_c_p = 1; //indicates the index of the pipe the current (or next using) process will write to (if p_out is true), alternates between 0 and 1
static int read_from_pipe_index_c_p = 0; //indicates the index of the pipe the current (or next using) process will read from if p_in is true, alternates between 0 and 1
//static pid_queue foreground_process_id_queue; //holds process IDs for each foreground process
///static pid_queue background_process_id_queue; //^^^^^^^^^^^^^^^^^^^^^^^^^^ background process

/***************************************************************************
 * Interface Functions
 ***************************************************************************/

// Return a string containing the current working directory.
char* get_current_directory(bool* should_free) {

	//by default called by Quash on start, but not printed

	//debug output
	//char* current_dir = get_current_dir_name();
	//printf(current_dir);

	// Change this to true if necessary
	*should_free = true; //for memory leak issues

	//don't use system variable for current directory directly, but can grab system call to it
	return get_current_dir_name();
}

// Returns the value of an environment variable env_var
const char* lookup_env(const char* env_var) {
	// TODO: Lookup environment variables. This is required for parser to be able
	// to interpret variables from the command line and display the prompt
	// correctly
	// HINT: This should be pretty simple

	// TODO: Remove warning silencers
	//(void) env_var; // Silence unused variable warning

	return getenv(env_var); //gets environment variable as null-terimated string for given string provided
}

// Check the status of background jobs
void check_jobs_bg_status() {
	// TODO: Check on the statuses of all processes belonging to all background
	// jobs. This function should remove jobs from the jobs queue once all
	// processes belonging to a job have completed.
	IMPLEMENT_ME();

	// TODO: Once jobs are implemented, uncomment and fill the following line
	// print_job_bg_complete(job_id, pid, cmd);
}

// Prints the job id number, the process id of the first process belonging to
// the Job, and the command string associated with this job
void print_job(int job_id, pid_t pid, const char* cmd) {
	printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
	fflush(stdout);
}

// Prints a start up message for background processes
void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {
	printf("Background job started: ");
	print_job(job_id, pid, cmd);
}

// Prints a completion message followed by the print job
void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {
	printf("Completed: \t");
	print_job(job_id, pid, cmd);
}

/***************************************************************************
 * Functions to process commands
 ***************************************************************************/
// Run a program reachable by the path environment variable, relative path, or
// absolute path
void run_generic(GenericCommand cmd) {
	// Execute a program with a list of arguments. The `args` array is a NULL
	// terminated (last string is always NULL) list of strings. The first element
	// in the array is the executable
	char* exec = cmd.args[0];
	char** args = cmd.args;


	execvp(exec,args);

	// TODO: Remove warning silencers
	//(void) exec; // Silence unused variable warning
	//(void) args; // Silence unused variable warning

	// TODO: Implement run generic
	//IMPLEMENT_ME();

	perror("ERROR: Failed to execute program");
}

// Print strings
void run_echo(EchoCommand cmd) {
	// Print an array of strings. The args array is a NULL terminated (last
	// string is always NULL) list of strings.
	char** str = cmd.args;
	int i = 0;
	while(cmd.args[i]!=NULL){
		printf(cmd.args[i]);
		i++;
	}
	// TODO: Remove warning silencers
	//(void) str; // Silence unused variable warning

	// TODO: Implement echo
	//IMPLEMENT_ME();

	// Flush the buffer before returning
	fflush(stdout);
}

// Sets an environment variable
void run_export(ExportCommand cmd) {
	// Write an environment variable
	const char* env_var = cmd.env_var;
	const char* val = cmd.val;

	// TODO: Remove warning silencers
	//(void) env_var; // Silence unused variable warning
	//(void) val;     // Silence unused variable warning
	setenv(env_var, val, 1);
	// TODO: Implement export.
	// HINT: This should be quite simple.
	//IMPLEMENT_ME();

	//
}

// Changes the current working directory
void run_cd(CDCommand cmd) {
	// Get the directory name
	const char* dir = cmd.dir;

	// Check if the directory is valid
	if (dir == NULL) {
		perror("ERROR: Failed to resolve path");
		return;
	}

	int chdir_err_no = chdir(dir);

	if(chdir_err_no != 0) //something went wrong
		printf("ERROR! Failed to change directory");

	// TODO: Update the PWD environment variable to be the new current working
	// directory and optionally update OLD_PWD environment variable to be the old
	// working directory.
	char* old_pwd; //read old directory
	old_pwd = get_current_dir_name();

	setenv("OLD_PWD",old_pwd,1); //forces old PWD value to be current PWD value (before we change current PWD)
	free(old_pwd); //free as required by get_current_dir_name
	setenv("PWD",dir,1); //forces PWD value to be new directory (PWD now points to the just-changed-to directory)

	//IMPLEMENT_ME();
}

// Sends a signal to all processes contained in a job
void run_kill(KillCommand cmd) {
	int signal = cmd.sig;
	int job_id = cmd.job;

	// TODO: Remove warning silencers
	(void) signal; // Silence unused variable warning
	(void) job_id; // Silence unused variable warning

	// TODO: Kill all processes associated with a background job
	IMPLEMENT_ME();
}


// Prints the current working directory to stdout
void run_pwd() {
	// TODO: Print the current working directory
	char* fetched_cur_dir;
	fetched_cur_dir = get_current_dir_name(); //get current directory and print to console
	printf(fetched_cur_dir);

	free(fetched_cur_dir); //do we need to allocate/free string? - Yes, it says so in get_current_dir_name docs

	// Flush the buffer before returning
	fflush(stdout);
}

// Prints all background jobs currently in the job list to stdout
void run_jobs() {
	// TODO: Print background jobs
	IMPLEMENT_ME();

	// Flush the buffer before returning
	fflush(stdout);
}

/***************************************************************************
 * Functions for command resolution and process setup
 ***************************************************************************/

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for child processes.
 *
 * This version of the function is tailored to commands that should be run in
 * the child process of a fork.
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void child_run_command(Command cmd) {
	CommandType type = get_command_type(cmd);

	switch (type) {
	case GENERIC:
		run_generic(cmd.generic);
		break;

	case ECHO:
		run_echo(cmd.echo);
		break;

	case PWD:
		run_pwd();
		break;

	case JOBS:
		run_jobs();
		break;

	case EXPORT:
	case CD:
	case KILL:
	case EXIT:
	case EOC:
		break; //on non-child-run command, just return

	default:
		fprintf(stderr, "Unknown command type: %d\n", type);
	}
}

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for the quash process.
 *
 * This version of the function is tailored to commands that should be run in
 * the parent process (quash).
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void parent_run_command(Command cmd) {
	CommandType type = get_command_type(cmd);

	switch (type) {
	case EXPORT:
		run_export(cmd.export);
		break;

	case CD:
		run_cd(cmd.cd);
		break;

	case KILL:
		run_kill(cmd.kill);
		break;

	case GENERIC:
	case ECHO:
	case PWD:
	case JOBS:
	case EXIT:
	case EOC:
		break;

	default:
		fprintf(stderr, "Unknown command type: %d\n", type);
	}
}

/**
 * @brief Creates one new process centered around the @a Command in the @a
 * CommandHolder setting up redirects and pipes where needed
 *
 * @note Processes are not the same as jobs. A single job can have multiple
 * processes running under it. This function creates a process that is part of a
 * larger job.
 *
 * @note Not all commands should be run in the child process. A few need to
 * change the quash process in some way
 *
 * @param holder The CommandHolder to try to run
 *
 * @sa Command CommandHolder
 */
void create_process(CommandHolder holder) {
	bool p_in  = holder.flags & PIPE_IN; //tells us if this process reads from a pipe
	bool p_out = holder.flags & PIPE_OUT; //tells us if this process writes to a pipe
	bool r_in  = holder.flags & REDIRECT_IN; //tells us if output from another process redirects into this process's input
	bool r_out = holder.flags & REDIRECT_OUT;
	bool r_app = holder.flags & REDIRECT_APPEND; // This can only be true if r_out
	int input_file_fd = 0; //the file descriptor for the input file, will be always 0 unless r_in is true
	int output_file_fd = 0; //the file desciptor for the output file, ^^^^^^^^^^^^^^^^^^^^^^ r_out is true
	// is true
	// TODO: Setup new process

	if (p_out) { //we need to create a new pipe to write info to (in child process)
		pipe (pipes_c_p[write_to_pipe_index_c_p]);
	}
	else if(r_out != 0) //if we will redirect output to a file (instead of a pipe or terminal)
	{
		int flags = O_WRONLY | O_CREAT;
		if(r_app) //if we also need to append to the file, add to argument list
		{
			flags = flags | O_APPEND;
		}

		output_file_fd = open(holder.redirect_out, flags);
		if(output_file_fd == -1) //if we failed to open output file
		{
			printf("ERROR: Unable to open file %s",holder.redirect_out);
			return;
		}
	}
	if(r_in != 0 && p_in == 0) //if we are reading input from a file, not pipe (which takes higher precedence than file)
	{
		input_file_fd = open(holder.redirect_in, O_RDONLY);
		if(input_file_fd == -1) //we failed to open input file
		{
			printf("ERROR: Unable to open file %s",holder.redirect_in);
			return;
		}
	}
	pid_t current_id = fork();
	
	push_front_pid_queue(&foreground_process_id_queue, current_id);
	printf("pushing process %ld to the queue",(long)current_id);

	//the actual RUN-COMMAND part
	if(current_id == 0) //this is child
	{
		//child closes/uses pipes
		if(p_in != 0) //if we need to read old pipe
		{
			dup2(pipes_c_p[read_from_pipe_index_c_p][0],STDIN_FILENO);
			//for the current read pipe: forward terminal input requests to read from the output of pipe
			close(pipes_c_p[read_from_pipe_index_c_p][1]); //we won't be writing to old read-pipe
		}
		else if(r_in != 0) //if we want to read in from file
		{
			dup2(input_file_fd,STDIN_FILENO); //input to process redirected to file
		}


		if(p_out != 0) //if we need to write to new pipe
		{
			dup2(pipes_c_p[write_to_pipe_index_c_p][1],STDOUT_FILENO); //redirect process output to write to input of pipe
		}
		else if(r_out != 0) //writing to file, not pipe or terminal
		{
			dup2(output_file_fd,STDOUT_FILENO);//redirect process output to write to file
		}
		//if is EXPORT, CD, KILL then
		child_run_command(holder.cmd); // if possible, run the child component of command
	}
	else
	{
		//we created pipes we won't be using, time to close them
		if(p_out != 0) //if we created an output pipe earlier, close it so it doesn't stall
			close(pipes_c_p[write_to_pipe_index_c_p][1]); //we won't be writing to this pipe in parent
		//IF is GENERIC, ECHO, PWD, JOBS, then
		else if(r_out != 0) //we opened file, but won't use in parent
		{
			close(output_file_fd);
		}
		if(r_in != 0)
		{
			close(input_file_fd);
		}

		//swap the pipes: THE FIRST IS THE LAST AND THE LAST THE FIRST
		read_from_pipe_index_c_p = (read_from_pipe_index_c_p + 1) % 2; //basically, alternate between 0 and 1
		write_to_pipe_index_c_p = (write_to_pipe_index_c_p + 1) % 2; //^^^^^^^^

		parent_run_command(holder.cmd); // if possible, run the parent component of command
	}
}

// Run a list of commands
void run_script(CommandHolder* holders) {
	if (holders == NULL)
		return;

	//TODO: create the Job associated with the current script execution
	
	//foreground_process_id_queue = new_pid_queue (1); //define forground process queue to read from

	check_jobs_bg_status();

	if (get_command_holder_type(holders[0]) == EXIT &&
			get_command_holder_type(holders[1]) == EOC) {
		end_main_loop();
		return;
	}

	CommandType type;

	// Run all commands in the `holder` array
	for (int i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i)
		create_process(holders[i]);

	if (!(holders[0].flags & BACKGROUND)) {
		// Not a background Job
		// TODO: Wait for all processes under the job to complete
		IMPLEMENT_ME();
	}
	else {
		// A background job.
		// TODO: Push the new job to the job queue
		IMPLEMENT_ME();

		// TODO: Once jobs are implemented, uncomment and fill the following line
		// print_job_bg_start(job_id, pid, cmd);
	}
}
