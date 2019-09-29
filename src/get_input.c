/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 17:04:19 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 17:04:21 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

char					*del_char(char *input)
{
	size_t				i;
	char				*new;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
		i++;
	if (!i || !(new = ft_strnew(sizeof(char) * (i - 1))))
		return (NULL);
	new = ft_strncpy(new, input, sizeof(char) * (i - 1));
	free(input);
	return (new);
}

char					*get_input(char *init, char change)
{
	static char			*input = NULL;
	char				*tmp;

	if (init)
	{
		if (input)
			free(input);
		return ((input = ft_strdup(init)));
	}
	if (ft_isalnum(change))
	{
		if ((tmp = ft_strdup(input)))
		{
			if (input)
				free(input);
			input = ft_strjoin(tmp, &change);
			free(tmp);
		}
	}
	else if (input && change == 8)
		input = del_char(input);
	if (!input || !ft_strlen(input))
		return (NULL);
	return (input);
}

t_vec					get_arrow_input(SDL_Keycode key, t_vec old)
{
	if (key == SDLK_UP)
		old.y -= 7;
	else if (key == SDLK_DOWN)
		old.y += 7;
	else if (key == SDLK_LEFT)
		old.x -= 7;
	else if (key == SDLK_RIGHT)
		old.x += 7;
	return (old);
}
