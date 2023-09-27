/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtaib <mtaib@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/09 14:58:52 by mtaib             #+#    #+#             */
/*   Updated: 2023/09/25 10:59:48 by mtaib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cub3D.h"
#include <stdio.h>

int		check_file_name(char *str)
{
	int		i;

	i = ft_strlen(str) - 1;
	if (i < 4)
		return (1);
	if (!(str[i] == 'b' && str[i - 1] == 'u'
		&& str[i - 2] == 'c' && str[i - 3] == '.'))
		return (1);
	return (0);
}

int		max_len()
{
	int		max;
	int		i;
	char	**map;

	map = get_type()->map;
	max = ft_strlen(map[0]);
	i = 0;
	while (map[++i])
		if (max < ft_strlen(map[i]))
			max = ft_strlen(map[i]); 
	return (max);
}

int		check_borders(char **map, char c)
{
	int		i;
	int		j;
	
	i = -1;
	while (map[++i])
	{
		j = -1;
		while (map[i][++j])
		{
			if (map[i][j] == c)
			{
				if (i == 0 || !map[i + 1])
					return (print_error(5));
				if (map[i + 1][j] == '\n')
					break ;
			       	if ((i > 0 && j == 0)	
					  || (j > 0 && !map[i][j + 1])) 
					return (print_error(5));
				if ((i > 0 && map[i - 1][j] == ' ')
					   || (map[i + 1] && (map[i + 1][j] == ' ')) 
				           || (j > 0 && map[i][j - 1] == ' ')
					   || (j > 0 && map[i][j + 1] == ' ') 
				   )
					return (print_error(6));
			}
		}
	}
	return (0);
}

int		check_map_errors(char **map)
{
	char	c;

	c = check_chars(map);
	if ((int)c == 1)
		return (1);
	if (check_borders(map, c))
		return (1);
	if (check_borders(map, '0'))
		return (1);
	//if bonus
	if (check_borders(map, 'C'))
		return (1);
	////
	return (0);

}
int		is_all_value(char *str, char c)
{
	int		i;
	
	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] != c)
			return (0);
		i++;
	}
	return (1);
}

void	ft_square(char **map)
{
	int	i;
	int	j;
	int	max;

	max = max_len();
	i = -1;
	while (map[++i])
	{
		j = ft_strlen(map[i]);
		while (j++ < max)
			map[i] = ft_strjoin(map[i], " ");
	}
}

int		check_newline(int fd, char *str)
{
	char	*line;

	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] != '\n')
			str = ft_strjoin(str, line);
		if (line[0] != '\n' && !is_all_value(line, ' '))
			return (print_error(16));
	}
	return (0);	
}

int		handle_map(int	fd)
{
	char	*line;
	char	*str;

	str = NULL;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (line[0] != '\n')
			str = ft_strjoin(str, line);
		if (str && line[0] == '\n')
			if (check_newline(fd, str))
				return (1);
	}
	if (!str)
		return (print_error(9));
	get_type()->map = ft_split(str, '\n');
	ft_square(get_type()->map);
	if (check_map_errors(get_type()->map))
		return (1);
	/*int		i;
	char	**map;

	map = get_type()->map;
	i = -1;
	while (map[++i])
		printf("--%s--\n",map[i]);*/
	return (0);
}

int		check_file_content(int 	fd)
{
	char	*line;
	
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (get_type()->dirNbs < 6
				&& (line[0] == '0' || line[0] == '1')
				&& !ft_strchr(line, ','))
			return (print_error(12));
		if (check_elements(line))
			return (1);
		if (get_type()->dirNbs == 6)
		{
			if (handle_map(fd))
				return (1);
			return (0);
		}
	}
	return (0);
}

int		ft_parse(char *str)
{
	int		fd;
	
	if (check_file_name(str))
		return (print_error(10));
	fd = open(str, O_RDONLY);
	if (fd == -1)
		return (print_error(11));
	if (check_file_content(fd))
		return (1);
	return (0);
}
