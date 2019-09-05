/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 09:23:18 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/09 09:23:20 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list	*begin_new;
	t_list	*new;
	t_list	*list;

	if (lst == NULL || f == NULL)
		return (NULL);
	list = f(lst);
	if (!(begin_new = ft_lstnew(list->content, list->content_size)))
	{
		free(begin_new);
		begin_new = NULL;
	}
	new = begin_new;
	while ((lst = lst->next))
	{
		list = f(lst);
		if (!(new->next = ft_lstnew(list->content, list->content_size)))
		{
			free(new->next);
			return (NULL);
		}
		new = new->next;
	}
	return (begin_new);
}
