
#ifndef FT_H
# define FT_H

# include <unistd.h>

char	**ft_split_ex(char const *s, char c, size_t extrachars);
void	*ft_memset(void *s, int c, size_t n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strrchr(const char *s, int c);

#endif
