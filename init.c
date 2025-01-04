#include "pipex.h"

void	app_init(t_app *app, int argc, char **argv, char **envp)
{
	static const char	*app_name = "pipex";
	static const char	*sc_sep = ": ";

	app->name = app_name;
	app->sc_sep = sc_sep;
	app->input_file = 0;
	app->output_file = 0;
	app->reader_limiter = NULL;
	app->reader_limiter_len = 0;
	app->in_argc = argc;
	app->in_argv = argv;
	app->in_envp = envp;
	app->reader_first_line_node = NULL;
	app->reader_last_line_node = NULL;
}

static void	app_init_file(t_app *app)
{
	app->command_count = app->in_argc - 3;
	app->input_file = open(app->in_argv[1], O_RDONLY);
	if (app->input_file < 0)
	{
		write_error_strs(4, app->name, app->sc_sep,
			app->in_argv[1], app->sc_sep);
		perror("");
	}
	app->output_file = open(app->in_argv[app->in_argc - 1],
			O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (app->output_file < 0)
	{
		write_error_strs(4, app->name, app->sc_sep,
			app->in_argv[app->in_argc - 1], app->sc_sep);
		perror("");
	}
}

static bool	app_init_heredoc(t_app *app)
{
	if (app->in_argc < 6)
	{
		write_error_strs(2, app->name, ": Invalid argument count\n");
		return (false);
	}
	app->command_count = app->in_argc - 4;
	app->reader_limiter = app->in_argv[2];
	app->reader_limiter_len = ft_strlen(app->in_argv[2]);
	if (!reader_store_input(app))
		return (reader_free_lines(app), false);
	app->output_file = open(app->in_argv[app->in_argc - 1],
			O_APPEND | O_CREAT | O_WRONLY);
	if (app->output_file <= -1)
	{
		write_error_strs(4, app->name, app->sc_sep,
			app->in_argv[app->in_argc - 1], app->sc_sep);
		perror("");
	}
	return (true);
}

bool	app_alloc_commands(t_app *app)
{
	size_t	memblock_size;

	if (ft_strncmp(app->in_argv[1], "here_doc", 9) == 0)
	{
		if (!app_init_heredoc(app))
			return (false);
	}
	else
		app_init_file(app);
	memblock_size = app->command_count * sizeof(t_command);
	app->commands = (t_command *) malloc(memblock_size);
	if (!app->commands)
		return (perror(app->name), false);
	ft_memset(app->commands, 0, memblock_size);
	if (app->input_file < 0)
		app->commands[0].return_code = 127;
	if (app->output_file < 0)
		app->commands[app->command_count - 1].return_code = 1;
	return (true);
}
