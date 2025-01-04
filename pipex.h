#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/wait.h>
# include "ft.h"
# include "gnl.h"

# define PIPE_READ_END_FD 0
# define PIPE_WRITE_END_FD 1

typedef struct s_command
{
	char	**command_argv;
	int		pipe_fds[2];
	pid_t	pid;
	int		return_code;
}	t_command;

typedef struct s_listnode
{
	char				*content;
	struct s_listnode	*next;
}	t_listnode;

typedef struct s_app
{
	const char	*name;
	const char	*sc_sep;
	int			input_file;
	int			output_file;
	int			in_argc;
	char		**in_argv;
	char		**in_envp;
	char		*path_env;
	int			command_count;
	t_command	*commands;
	char		*reader_limiter;
	int			reader_limiter_len;
	t_listnode	*reader_first_line_node;
	t_listnode	*reader_last_line_node;
}	t_app;

char	**split_args(char const *s);
void	write_error_strs(int str_count, ...);
void	free_table(char **table);
char	*find_path_env(char **env_table);
char	*find_cmd_in_path_env(char *cmd, char *paths_str);
void	app_close_pipes_fds(t_app *app);
bool	app_create_pipes(t_app *app);
void	app_clean_exit(t_app *app);
void	app_init(t_app *app, int argc, char **argv, char **envp);
bool	app_alloc_commands(t_app *app);
bool	app_exec_commands(t_app *app);
void	app_wait_commands(t_app *app);
bool	app_init_commands(t_app *app);
void	app_free_command(t_app *app, int cmd_index, int return_code);
bool	app_create_heredoc_process(t_app *app);
bool	reader_store_input(t_app *app);
void	reader_free_lines(t_app *app);

#endif
