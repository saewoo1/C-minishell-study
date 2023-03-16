/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_quote_util.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suhwpark <suhwpark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:02:24 by yongmipa          #+#    #+#             */
/*   Updated: 2023/03/16 17:34:46 by suhwpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_next_quote(char *line, char quote, int quote_idx)
{
	int	next_idx;

	next_idx = quote_idx + 1;
	while (line[next_idx])
	{
		if (line[next_idx] == quote)
			return (next_idx);
		next_idx++;
	}
	return (-1); // 에러 출력으로 바꾸는게 나을듯염 -> main문에서 validate 실패 시 출력 ! ! 
}

int	*count_q(char *munja)
{
	int	*q;
	int	i;

	q = (int *)malloc(sizeof(int) * 2);
	i = -1;
	q[0] = 0;
	q[1] = 0;
	while (munja[++i])
	{
		if (munja[i] == '\"')
			q[1]++;
		else if (munja[i] == '\'')
			q[0]++;
	}
	return (q);
}

int	here_quote(char *line)
{
	int	len;

	len = 0;
	while (line[len])
	{
		if (line[len] == '\'' || line[len] == '\"')
			break ;
		len++;
	}
	return (len); // -1 이면 오류 처리 해보자
}

char	*get_full_token(t_info *cmd)
{
	t_info	*head;
	char	*tmp;
	char	*full_token;

	head = cmd;
	full_token = ft_strjoin("", head->cmd);
	head = head->next;
	while (head != NULL)
	{
		tmp = full_token;
		full_token = ft_strjoin(full_token, head->cmd);
		free(tmp);
		head = head->next;
	}
	list_delete(&cmd);
	return (full_token);
}

int	is_quote(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\"' || s[i] == '\'')
			return (i);
		i++;
	}
	return (-1);
}
