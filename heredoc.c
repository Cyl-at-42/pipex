#include "pipex.h"

bool	app_create_heredoc_process(t_app *app)
{
	pid_t		new_pid;
	t_listnode	*node;
	t_listnode	*prev_node;

	if (pipe(app->commands[0].pipe_fds) == -1)
		return (perror(app->name), false);
	new_pid = fork();
	if (new_pid == -1)
		return (perror(app->name), false);
	if (new_pid)
		return (reader_free_lines(app),
			close(app->commands[0].pipe_fds[PIPE_WRITE_END_FD]), true);
	close(app->commands[0].pipe_fds[PIPE_READ_END_FD]);
	node = app->reader_first_line_node;
	while (node)
	{
		write(app->commands[0].pipe_fds[PIPE_WRITE_END_FD], node->content,
			ft_strlen(node->content));
		prev_node = node;
		node = node->next;
		free(prev_node->content);
		free(prev_node);
	}
	app_clean_exit(app);
	exit(EXIT_SUCCESS);
}

bool	reader_add_line(t_app *app, char *line)
{
	t_listnode	*newnode;

	newnode = (t_listnode *)malloc(sizeof(t_listnode));
	if (!newnode)
		return (false);
	newnode->content = line;
	newnode->next = NULL;
	if (!app->reader_first_line_node)
	{
		app->reader_first_line_node = newnode;
		app->reader_last_line_node = newnode;
	}
	else
	{
		app->reader_last_line_node->next = newnode;
		app->reader_last_line_node = newnode;
	}
	return (true);
}

void	reader_free_lines(t_app *app)
{
	t_listnode	*node;
	t_listnode	*prev_node;

	node = app->reader_first_line_node;
	while (node)
	{
		prev_node = node;
		node = node->next;
		free(prev_node->content);
		free(prev_node);
	}
}

bool	reader_store_input(t_app *app)
{
	char	*newline;

	newline = (char *) 1;
	while (newline)
	{
		write(STDOUT_FILENO, "\033[1mheredoc> \033[0m", 17);
		newline = get_next_line(0);
		if (newline)
		{
			if (ft_strncmp(newline, app->reader_limiter,
					app->reader_limiter_len) == 0)
			{
				if (newline[app->reader_limiter_len] == '\n')
				{
					free(newline);
					return (true);
				}
			}
			if (!reader_add_line(app, newline))
				return (false);
		}
	}
	write(STDOUT_FILENO, "\n", 1);
	return (true);
}
