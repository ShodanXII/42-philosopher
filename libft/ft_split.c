/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achat <achat@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 09:44:03 by achat             #+#    #+#             */
/*   Updated: 2024/11/05 11:57:30 by achat            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_tokens(const char *s, char c);
static size_t	fller(char **tokens_v, const char *s, char c);
static size_t	ft_free(char **tokens_v, int position, size_t buffer);

char	**ft_split(const char *s, char c)
{
	size_t	tokens;
	char	**tokens_v;

	if (NULL == s)
		return (NULL);
	tokens = 0;
	tokens = count_tokens(s, c);
	tokens_v = ft_calloc(tokens + 1, sizeof(char *));
	if (!tokens_v)
		return (NULL);
	tokens_v[tokens] = NULL;
	if (fller(tokens_v, s, c))
		return (NULL);
	return (tokens_v);
}

static size_t	ft_free(char **tokens_v, int position, size_t buffer)
{
	int	i;

	i = 0;
	tokens_v[position] = malloc(buffer);
	if (NULL == tokens_v[position])
	{
		while (i < position)
		{
			free(tokens_v[i++]);
		}
		free(tokens_v);
		return (1);
	}
	return (0);
}

static size_t	fller(char **tokens_v, char const *s, char c)
{
	size_t	len;
	int		i;

	i = 0;
	while (*s)
	{
		len = 0;
		while (*s == c && *s)
			++s;
		while (*s != c && *s)
		{
			++len;
			++s;
		}
		if (len)
		{
			if (ft_free(tokens_v, i, len + 1))
				return (1);
			ft_strlcpy(tokens_v[i], s - len, len + 1);
		}
		++i;
	}
	return (0);
}

static size_t	count_tokens(char const *s, char delimeter)
{
	size_t	tokens;
	int		inside_token;

	tokens = 0;
	while (*s)
	{
		inside_token = 0;
		while (*s == delimeter && *s)
			++s;
		while (*s != delimeter && *s)
		{
			if (!inside_token)
			{
				++tokens;
				inside_token = 42;
			}
			++s;
		}
	}
	return (tokens);
}
