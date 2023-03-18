/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yongmin_jakup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohyupar <sohyupar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/07 16:32:13 by suhwpark          #+#    #+#             */
/*   Updated: 2023/03/18 15:54:10 by sohyupar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_whitespace2(char line)
{
	if (line != 32 && !(line >= 9 && line <= 13))
		return (1);
	return (0);
}

char	*quote_bulk(char *line, char c)
{
	int		i;
	char	*bulk;

	i = 1;
	if (line[i] == c)
	{
		bulk = ft_strdup("");
		return (bulk);
	}
	while (line[i] != c)
		i++;
	bulk = ft_substr(line, 0, i + 1);
	return (bulk);
}

char	*get_after_quote(char *line, char *bulk)
{
	char	*tmp;
	char	*real_bulk;
	int		i;

	i = 0;
	while (line[i] && is_whitespace2(line[i]))
		i++;
	tmp = ft_substr(line, 0, i);
	real_bulk = ft_strjoin(bulk, tmp);
	free(tmp);
	return (real_bulk);
}
// 줄수 때매 뻇어요

static void	only_redir(t_info *info, char **line)
{
	if (**line == '>')
		insert_list(info, ">", REDIR_OUT);
	else if (**line == '<')
		insert_list(info, "<", REDIR_IN);
	else
		insert_list(info, "|", PIPE);
}

static void	redir_check(t_info *info, char **line)
{
	char	*temp;
	int		type;

	type = HEREDOC_IN;
	if (*(*line + 1) == **line)
	{
		if (*(*line + 1) == '>')
			type = HEREDOC_OUT;
		temp = ft_substr(*line, 0, 2);
		insert_list(info, temp, type);
		free(temp);
		// <<a 를 <<로만 판단해서 넣어놨습니다!
		if (*(*line + 2) != ' ' || *(*line + 2) != '\0' || *(*line + 2) != '<' || *(*line + 2) != '>')
			*line += 1;
		else
			*line += 2;
	}
	else
		only_redir(info, line);
}

void	str_tokenize(t_info *info, char *line)
{
	while (*line)
	{
		if (*line == '>' || *line == '<' || *line == '|')
			redir_check(info, &line);
		else if (*line == '\"' || *line == '\'')
			quote_process(info, &line);
		else if (*line != '>' && *line != '<' && *line != '|' && *line != ' ')
			ft_remainder(info, &line);
		if (*line == '\0')
			break ;
		line++;
	}
}

// int main()
// {
// 	t_info *test;
// 	t_info *head1;
// 	t_info *head2;

// 	char *str = "cat \"$USER\" | ls -al | cat '-e' | 'w'c -l > a";

// 	printf("line : %s\n", str);
// 	test = init_list();
// 	str_tokenize(test, str);
// 	head1 = test;
// 	while(head1 != NULL)
// 	{
// 		write(1, "1", 1);
// 		printf("cmd : %s, type : %d\n", head1->cmd, head1->type);
// 		head1 = head1->next;
// 	}
// 	clear_quote_in_token(test);
// 	head2 = test;
// 	while (head2 != NULL)
// 	{
// 		printf("a-fter cmd : %s, type : %d\n", head2->cmd, head2->type);
// 		head2 = head2->next;
// 	}
// 	// system("leaks a.out");
// }
