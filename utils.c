#include "pipex.h"

void	write_error_strs(int str_count, ...)
{
	va_list	arglist;
	char	*error_str;

	va_start(arglist, str_count);
	while (str_count)
	{
		error_str = va_arg(arglist, char *);
		write(2, error_str, ft_strlen(error_str));
		str_count--;
	}
	va_end(arglist);
}

void	free_table(char **table)
{
	char	**table_ptr;

	if (!table)
		return ;
	table_ptr = table;
	while (*table_ptr)
		free(*table_ptr++);
	free(table);
}

char	*find_path_env(char **env_table)
{
	while (*env_table)
	{
		if (!ft_strncmp("PATH=", *env_table, 5))
			return (*env_table + 5);
		env_table++;
	}
	return (NULL);
}

static void	path_add(char *dest, char *src)
{
	while (*dest++)
		;
	*(dest - 1) = '/';
	while (*src)
		*dest++ = *src++;
	*dest = 0;
}

char	*find_cmd_in_path_env(char *cmd, char *paths_str)
{
	char	*res;
	char	**paths_table;
	int		i;

	paths_table = ft_split_ex(paths_str, ':', ft_strlen(cmd) + 1);
	if (!paths_table)
		return (NULL);
	res = NULL;
	i = 0;
	while (paths_table[i])
	{
		path_add(paths_table[i], cmd);
		if (!res && (access(paths_table[i], F_OK) == 0))
			res = paths_table[i];
		if (res != paths_table[i])
			free(paths_table[i]);
		i++;
	}
	free(paths_table);
	return (res);
}
