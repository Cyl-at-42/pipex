
#include "pipex.h"

void	app_exec_command_redirect_stdio(t_app *app, int cmd_index)
{
	if (!cmd_index)
	{
		if (app->input_file)
			dup2(app->input_file, STDIN_FILENO);
		else
		{
			dup2(app->commands[0].pipe_fds[PIPE_READ_END_FD], STDIN_FILENO);
			close(app->commands[0].pipe_fds[PIPE_READ_END_FD]);
		}
		dup2(app->commands[cmd_index + 1].pipe_fds[PIPE_WRITE_END_FD],
			STDOUT_FILENO);
	}
	else if (cmd_index == (app->command_count - 1))
	{
		dup2(app->commands[cmd_index].pipe_fds[PIPE_READ_END_FD], STDIN_FILENO);
		dup2(app->output_file, STDOUT_FILENO);
	}
	else
	{
		dup2(app->commands[cmd_index].pipe_fds[PIPE_READ_END_FD], STDIN_FILENO);
		dup2(app->commands[cmd_index + 1].pipe_fds[PIPE_WRITE_END_FD],
			STDOUT_FILENO);
	}
}

int	app_exec_command(t_app *app, int cmd_index)
{
	int	ret_code;

	app_exec_command_redirect_stdio(app, cmd_index);
	app_close_pipes_fds(app);
	ret_code = app->commands[cmd_index].return_code;
	if (!app->commands[cmd_index].command_argv)
		ret_code = EXIT_FAILURE;
	if (ret_code)
		exit(ret_code);
	execve(app->commands[cmd_index].command_argv[0],
		app->commands[cmd_index].command_argv, app->in_envp);
	return (perror("execve"), EXIT_FAILURE);
}

bool	app_exec_commands(t_app *app)
{
	pid_t	new_pid;
	int		i;

	i = 0;
	while (i < app->command_count)
	{
		if (!app->commands[i].return_code)
		{
			new_pid = fork();
			if (app->commands[i].pid == -1)
				perror(app->name);
			if (new_pid == 0)
				app_exec_command(app, i);
			app->commands[i].pid = new_pid;
		}
		i++;
	}
	return (true);
}
