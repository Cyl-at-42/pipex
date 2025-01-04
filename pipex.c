#include "pipex.h"

static void	close_pipe_fds(int *pipe_fds)
{
	if (!pipe_fds[PIPE_READ_END_FD])
		return ;
	close(pipe_fds[PIPE_READ_END_FD]);
	close(pipe_fds[PIPE_WRITE_END_FD]);
}

void	app_close_pipes_fds(t_app *app)
{
	int	i;

	i = 1;
	while (i < app->command_count)
		close_pipe_fds(app->commands[i++].pipe_fds);
}

bool	app_create_pipes(t_app *app)
{
	int	i;

	i = 1;
	while (i < app->command_count)
	{
		if (pipe(app->commands[i].pipe_fds) == -1)
			return (perror(app->name), false);
		i++;
	}
	return (true);
}

void	app_clean_exit(t_app *app)
{
	int	i;

	i = 0;
	while (i < app->command_count)
	{
		free_table(app->commands[i].command_argv);
		i++;
	}
	free(app->commands);
}

int	main(int argc, char **argv, char **envp)
{
	t_app	app;
	int		ret_code;

	app_init(&app, argc, argv, envp);
	if (argc < 5)
		return (write_error_strs(3, app.name, app.sc_sep,
				"Invalid argument count\n"), 2);
	if (!app_alloc_commands(&app))
		return (EXIT_FAILURE);
	if (!app_create_pipes(&app))
		return (app_clean_exit(&app), EXIT_FAILURE);
	app.path_env = find_path_env(envp);
	if (!app_init_commands(&app))
		return (app_clean_exit(&app), EXIT_FAILURE);
	if (!app.input_file)
		if (!app_create_heredoc_process(&app))
			return (false);
	if (!app_exec_commands(&app))
		return (app_clean_exit(&app), EXIT_FAILURE);
	app_close_pipes_fds(&app);
	app_wait_commands(&app);
	ret_code = app.commands[app.command_count - 1].return_code;
	return (app_clean_exit(&app), ret_code);
}
