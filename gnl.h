#ifndef GNL_H
# define GNL_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

typedef struct s_varstr
{
	char	*str;
	size_t	len;
}	t_varstr;

typedef struct s_readbuffer
{
	char	*linestart;
	char	buffer[BUFFER_SIZE];
}	t_readbuffer;

void	varstr_init(t_varstr *varstr);
char	*varstr_free(t_varstr *varstr);
char	*varstr_add(t_varstr *varstr, char *str2_start, char *str2_end);
char	*get_next_line(int fd);

#endif
