/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohyupar <sohyupar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 15:00:37 by yongmipa          #+#    #+#             */
/*   Updated: 2023/03/18 21:16:15 by sohyupar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_envp	*init_envp(void)
{
	t_envp	*envp;

	envp = (t_envp *)ft_safe_malloc(sizeof(t_envp));
	envp->next = NULL;
	envp->key = NULL;
	envp->value = NULL;
	return (envp);
}

int	check_dupkey(t_envp *envp, char *key)
{
	t_envp	*head;

	head = envp;
	while (head != NULL)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (1);
		head = head->next;
	}
	return (0);
}

void	insert_envp(t_envp *envp, char *key, char *value)
{
	t_envp	*head;

	head = envp;
	if (head->key == NULL)
	{
		head->key = ft_strdup(key);
		head->value = ft_strdup(value);
		return ;
	}
	if (check_dupkey(envp, key))
	{
		while (ft_strcmp(head->key, key) != 0)
			head = head->next;
		free(head->value);
		head->value = ft_strdup(value);
		return ;
	}
	while (head->next != NULL)
		head = head->next;
	head->next = (t_envp *)ft_safe_malloc(sizeof(t_envp));
	head->next->key = ft_strdup(key);
	head->next->value = ft_strdup(value);
	head->next->next = NULL;
}

void	append_envp(t_envp *envp, char *key, char *value)
{
	t_envp	*head;

	head = envp;
	while (head->next) //envp 탐색하면서 마지막 도달하거나, 동일 key 찾으면 나옴
	{
		if (ft_strcmp(head->key, key) == 0)
			break ;
		head = head->next;
	}
	if (head->next == NULL) // 마지막까지 동일 key 못 찾은 경우
	{
		if (ft_strcmp(head->key, key) == 0) // 마지막이 동일 key인 경우
			head->value = ft_strjoin_free(head->value, value);
		else // 끝까지 없는 경우, 그냥 insert
			insert_envp(envp, key, value);
	}
	else //동일 key 있는 경우 현재 value 뒤에 append
		head->value = ft_strjoin_free(head->value, value);
}

void	delete_envp_all(t_envp **envp)
{
	t_envp	*tmp;

	while (*envp)
	{
		tmp = (*envp)->next;
		free((*envp)->key);
		free((*envp)->value);
		free(*envp);
		*envp = tmp;
	}
}

t_envp	*set_envp(char **envp)
{
	t_envp	*head;
	char	**arr;
	int		i;
	int		j;

	i = 0;
	head = init_envp();
	while (envp[i])
	{
		j = ft_strchr_int(envp[i], '=');
		arr = (char **)ft_safe_malloc(2 * sizeof(char *));
		arr[0] = ft_substr(envp[i], 0, j);
		arr[1] = ft_substr(envp[i], j + 1, ft_strlen(envp[i]) - j + 1);
		insert_envp(head, arr[0], arr[1]);
		free(arr[0]);
		free(arr[1]);
		free(arr);
		i++;
	}
	return (head);
}

char	**envp_to_arr(t_envp *head) //execve에 넣을 envp 2차원 배열 생성
{
	t_envp	*tmp;
	char	**ret;
	int		i;

	tmp = head;
	ret = (char **)ft_safe_malloc(sizeof(char *) * (size_envp(head) + 1));
	i = 0;
	while (tmp)
	{
		ret[i] = ft_strdup(tmp->key);
		if (tmp->value)
		{
			ret[i] = ft_strjoin_free(ret[i], "=");
			ret[i] = ft_strjoin_free(ret[i], tmp->value);
		}
		tmp = tmp->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

// int	main(int ac, char **av, char **envp) //test 메인문
// {
// 	int i = 0;
// 	t_envp	*head;

// 	head = set_envp(envp);
// 	printf("////////////////////////\n");
// 	printf("original output\n");
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		i++;
// 	}
// 	printf("////////////////////////\n");
// 	printf("ft_env output\n");
// 	ft_env(head);
// 	return 0;
// }
