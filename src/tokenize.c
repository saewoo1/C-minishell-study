/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suhwpark <suhwpark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:32:13 by suhwpark          #+#    #+#             */
/*   Updated: 2023/03/07 17:57:06 by suhwpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_whitespace2(char line)
{
	if (line != 32 && !(line >= 9 && line <= 13))
		return (1);
	return (0);
}

static char	*quote_bulk(char *line, char c)
{
	int		i;
	char	*bulk;

	i = 1;
	while (line[i] != c)
		i++;
	bulk = ft_substr(line, 0, i + 1);
	return (bulk);
}

static char	*get_after_quote(char *line, char *bulk)
{
	char	*tmp;
	char	*real_bulk;
	int		i;

	i = 0;
	while (is_whitespace2(line[i]))
		i++;
	tmp = ft_substr(line, 0, i);
	real_bulk = ft_strjoin(bulk, tmp);
	free(tmp);
	return (real_bulk);
}

void	str_tokenize(t_info *info, char *line)
{
	int		i;
	char	*tmp;
	char	*bulk;
	char	*tmp2;

	i = 0;
	while (*line)
	{
		if (*line == '>')
		{
			if (*(line + 1) == '>')
			{
				insert_list(info, ">>", REDIR);
				line++;
			}
			else
				insert_list(info, ">", REDIR);
		}
		if (*line == '<')
		{
			if (*(line + 1) == '<')
			{
				insert_list(info, "<<", REDIR);
				line++;
			}
			else
				insert_list(info, "<", REDIR);
		}
		if (*line == '|')
			insert_list(info, "|", PIPE);
		if (*line == '\"')
		{
			bulk = quote_bulk(line, '\"');
			tmp = bulk;
			line += ft_strlen(bulk);
			if (*line != ' ')
			{
				bulk = get_after_quote(line, tmp);
				line += (ft_strlen(bulk) - ft_strlen(tmp));
			}
			insert_list(info, bulk, WORD);
			free(tmp);
			// free(bulk);
			if (*line == '\0')
				break ;
		}
		if (*line == '\'')
		{
			bulk = quote_bulk(line, '\'');
			tmp = bulk;
			line += ft_strlen(bulk);
			if (*line != ' ')
				bulk = get_after_quote(line, tmp);
			line += (ft_strlen(bulk) - ft_strlen(tmp));
			insert_list(info, bulk, WORD);
			free(tmp);
			// free(bulk);
			if (*line == '\0')
				break ;
		}
		if (*line != '>' && *line != '<' && *line != '|' && *line != ' ')
		{
			i = 0;
			while (line[i] && is_whitespace2(line[i]))
				i++;
			tmp = ft_substr(line, 0, i);
			insert_list(info, tmp, WORD);
			line += ft_strlen(tmp);
			free(tmp);
			if (*line == '\0')
				break ;
		}
		line++;
	}
}

int main()
{
	t_info *test;
	char *str = "<<< \"a ls | ca't' > \"b\" ";

	printf("line : %s\n", str);
	test = init_list();
	str_tokenize(test, str);
	while (test != NULL)
	{
		printf("cmd : %s, type : %d\n", test->cmd, test->type);
		test = test->next;
	}
	// system("leaks a.out");
}
