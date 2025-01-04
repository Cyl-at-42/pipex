#include "pipex.h"

static unsigned int	count_args(char const *str)
{
	unsigned int	argcount;
	int				isarg;
	int				isargprev;
	char			chr;

	argcount = 0;
	isargprev = 0;
	while (*str)
	{
		chr = *str;
		isarg = (chr && chr != ' ');
		if (isarg && !isargprev)
			argcount++;
		if ((chr == '\"') || (chr == '\''))
			str = ft_strrchr(str, chr);
		str++;
		isargprev = isarg;
	}
	return (argcount);
}

static char	*alloc_arg(char *argstart, char *argend)
{
	unsigned int	arglen;
	char			*newarg;
	char			*argptr;

	if ((argend - argstart) != 1)
	{
		if (((*argstart == '\"') && (*(argend - 1) == '\"'))
			|| ((*argstart == '\'') && (*(argend - 1) == '\'')))
		{
			argstart++;
			argend--;
		}
	}
	arglen = argend - argstart;
	newarg = (char *)malloc((arglen + 1) * sizeof(char));
	if (!newarg)
		return (NULL);
	argptr = newarg;
	while (argstart < argend)
		*argptr++ = *argstart++;
	*argptr = 0;
	return (newarg);
}

static unsigned int	alloc_args(char **table, char *str, unsigned int argindex)
{
	int		isarg;
	int		isargprev;
	char	*argstart;

	argstart = str;
	isargprev = 0;
	while (argstart)
	{
		isarg = (*str && *str != ' ');
		if (isarg && !isargprev)
			argstart = str;
		if (!isarg && isargprev)
		{
			table[argindex++] = alloc_arg(argstart, str);
			if (!table[argindex - 1])
				return (argindex - 1);
		}
		if (!*str)
			argstart = NULL;
		if (*str == '\"' || *str == '\'')
			str = ft_strrchr(str, *str);
		str++;
		isargprev = isarg;
	}
	return (argindex);
}

static char	**free_all(char **table, unsigned int argcount)
{
	unsigned int	i;

	i = 0;
	while (i < argcount)
		free(table[i++]);
	free(table);
	return (NULL);
}

char	**split_args(char const *s)
{
	unsigned int	argcount;
	unsigned int	allocated_argcount;
	char			**ret_table;

	argcount = count_args(s);
	ret_table = (char **)malloc((argcount + 1) * sizeof(char *));
	if (!ret_table)
		return (NULL);
	if (argcount)
	{
		allocated_argcount = alloc_args(ret_table, (char *)s, 0);
		if (allocated_argcount != argcount)
			return (free_all(ret_table, allocated_argcount));
	}
	ret_table[argcount] = 0;
	return (ret_table);
}
