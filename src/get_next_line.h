/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/13 16:24:08 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/13 16:24:10 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "../libft/libft.h"

# define BUFF_SIZE 8
# define OK 1
# define END 0
# define ERROR -1
# define NL mem->nl
# define RET mem->ret
# define BUF mem->buf
# define SEP '\n'
# define RETURN_IF(x, y) if (y) return(x);

typedef struct		s_mlist
{
	char			buf[BUFF_SIZE + 1];
	int				fd;
	size_t			nl;
	ssize_t			ret;
	struct s_mlist	*next;
}					t_mlist;

int					get_next_line(const int fd, char **line);

#endif
