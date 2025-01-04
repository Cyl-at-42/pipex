
#include "pipex.h"

void	app_wait_commands(t_app *app)
{
	int	i;
	int	status;

	i = 0;
	while (i < app->command_count)
	{
		if (app->commands[i].pid > 0)
		{
			waitpid(app->commands[i].pid, &status, 0);
			app->commands[i].return_code = WEXITSTATUS(status);
		}
		i++;
	}
}

void	app_free_command(t_app *app, int cmd_index, int return_code)
{
	free_table(app->commands[cmd_index].command_argv);
	app->commands[cmd_index].command_argv = NULL;
	app->commands[cmd_index].return_code = return_code;
}

bool	app_init_command_ftn(t_app *app, int cmd_index)
{
	static const char	*cnf_str = {": Command not found\n"};
	char				*cmd_path;

	if (!app->path_env)
		return (write_error_strs(4, app->name, app->sc_sep,
				app->commands[cmd_index].command_argv[0], cnf_str),
			app_free_command(app, cmd_index, 127), true);
	cmd_path = find_cmd_in_path_env(app->commands[cmd_index].command_argv[0],
			app->path_env);
	if (!cmd_path)
		return (write_error_strs(4, app->name, app->sc_sep,
				app->commands[cmd_index].command_argv[0], cnf_str),
			app_free_command(app, cmd_index, 127), true);
	free(app->commands[cmd_index].command_argv[0]);
	app->commands[cmd_index].command_argv[0] = cmd_path;
	if (access(cmd_path, X_OK) == -1)
	{
		write_error_strs(2, app->name, app->sc_sep);
		perror(cmd_path);
		app_free_command(app, cmd_index, 126);
	}
	return (true);
}

bool	app_init_command(t_app *app, int cmd_index, char *cmdstr)
{
	static const char	*cnf_str = {": Command not found\n"};

	if (!*cmdstr)
		return (write_error_strs(3, app->name, ": ", cnf_str),
			app->commands[cmd_index].return_code = 127, true);
	app->commands[cmd_index].command_argv = split_args(cmdstr);
	if (!app->commands[cmd_index].command_argv)
		return (perror(app->name), false);
	if (!app->commands[cmd_index].command_argv[0])
		return (write_error_strs(3, app->name, ": ", cnf_str),
			app->commands[cmd_index].return_code = 127, true);
	if (ft_strrchr(app->commands[cmd_index].command_argv[0], '/'))
	{
		if (access(app->commands[cmd_index].command_argv[0], F_OK) == -1)
			return (write_error_strs(4, app->name, app->sc_sep,
					app->commands[cmd_index].command_argv[0], cnf_str),
				app_free_command(app, cmd_index, 127), true);
		if (access(app->commands[cmd_index].command_argv[0], X_OK) == -1)
			return (write_error_strs(2, app->name, app->sc_sep),
				perror(app->commands[cmd_index].command_argv[0]),
				app_free_command(app, cmd_index, 126), true);
		return (true);
	}
	return (app_init_command_ftn(app, cmd_index));
}

bool	app_init_commands(t_app *app)
{
	int	i;
	int	arg;

	arg = 2 + (app->reader_limiter != NULL);
	i = 0;
	while (i < app->command_count)
	{
		if (!app->commands[i].return_code)
			if (!app_init_command(app, i, app->in_argv[arg]))
				return (false);
		i++;
		arg++;
	}
	return (true);
}
