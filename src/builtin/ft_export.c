/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yongmipa <yongmipa@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 17:29:43 by yongmipa          #+#    #+#             */
/*   Updated: 2023/03/10 21:07:34 by yongmipa         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*
	# 예시
	export <변수명>=<값> : export abc=123
	-------------------------------
	# 짜증나는 예시
	export a                            #key값만 생성
	export b=                           #value에 ""
	export c+=world                     #환경변수 뒤에 덧붙이기 (아무것도 없어도 가능)
	export d="oh      my          god"  #echo출력과 export출력 다름. echo는 스페이스 하나로 처리 .. 미친새끼아녀 ?
	export e=elephant f=flower
	export a=b=c                        #a="b=c"
	export a=====b                      #a="====b"
	export a=b c d                      #a=b, c, d
	-------------------------------
	명령어가 export 하나면 (인자 X) -> 환경변수 오름차 순 출력 (형식 : declare -x [KEY=”VALUE”])

	명령어가 export z 와 같은 식으로 인자가 하나면 환경변수에 저장되지 않음.
	=> 저장 됩니다! env로 출력시 안 나오고, export로 출력시 나옴

	명령어가 export key=value 일 때

	이미 존재하는 환경변수면 free 후 새로 dup
	새로운 환경변수면 원래 크기 + 2 짜리 **new 생성
	기존 환경변수 dup 하고
	새 환경변수, NULL 추가
	기존 환경변수 free 해주고 **new로 대체
	파이프 뒤에 있다면 자식프로세스의 export이기 때문에 환경변수 생성 X

	key가 숫자로 시작하면 안 됨.
	스페이스, 특수문자 등은 key, value 모두 안되는데, .랑 -랑 /는 왜 됨 ;;
	작은, 큰 따옴표 제거하고 환경변수 목록에 추가되어야 함.
*/

char	**dup_envp(t_envp *head) //정렬하기 위한 배열 만들기
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
			ret[i] = ft_strjoin_free(ret[i], "=\"");
			ret[i] = ft_strjoin_free(ret[i], tmp->value);
			ret[i] = ft_strjoin_free(ret[i], "\"");
		}
		tmp = tmp->next;
		i++;
	}
	return (ret);
}

void	sort_arr(char **arr) //strcmp 사용해서 문자열 정렬
{
	char	*tmp;
	int		i;
	int		j;
	int		len;

	len = ft_arrlen(arr);
	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - 1)
		{
			if (ft_strcmp(*(arr + i), *(arr + j)) < 0)
			{
				tmp = *(arr + j);
				*(arr + j) = *(arr + i);
				*(arr + i) = tmp;
			}
			j++;
		}
		i++;
	}
}

int	check_argv(char *argv)
{
	char	*key;
	int		i;
	int		plus;
	int		ret;

	i = ft_strchr_int(argv, '=');
	if (i == -1)
		return (validate_key(argv));
	plus = ft_strchr_int(argv, '+');
	if (plus != -1 && plus < i && (plus != i - 1)) // + 기호가 = 앞에 있는데, 바로 앞에 있는 게 아닌 경우 -> key에 있다는 말
		return (0);
	if (plus == i - 1) // += 인 경우, + 앞까지가 key
		key = ft_substr(argv, 0, plus);
	else // 그냥 =인 경우, = 앞까지가 key
		key = ft_substr(argv, 0, i);
	ret = validate_key(key);
	free(key);
	return (ret);
}

void	add_envp(char *argv, t_envp *head) // =로 나눠진다는 보장 없음, 스페이스 및 숫자 예외 처리
{
	t_envp	*new;
	char	**arr;
	int		i;
	int		plus;

	if (!head || !argv)
		return ;
	if (!check_argv(argv))
	{
		printf("minishell: export: '%s' : not a valid identifier\n", argv);
		return ;
	}
	i = ft_strchr_int(argv, '=');
	plus = ft_strchr_int(argv, '+');
	if (i == -1) //없으면 key만
		insert_envp(head, argv, NULL);
	else
	{
		arr = (char **)ft_safe_malloc(2 * sizeof(char *));
		arr[1] = ft_substr(argv, i + 1, ft_strlen(argv) - i + 1);
		if (plus == i - 1)
		{
			arr[0] = ft_substr(argv, 0, i - 1);
			append_envp(head, arr[0], arr[1]);
		}
		else
		{
			arr[0] = ft_substr(argv, 0, i);
			insert_envp(head, arr[0], arr[1]);
		}
		free(arr[0]);
		free(arr[1]);
		free(arr);
	}
}

int	ft_export(t_info *arg, t_envp *head)
{
	t_info	*arg_tmp;
	t_envp	*env_tmp;
	char	**arr;
	int		i;

	arg_tmp = arg;
	env_tmp = head;
	if (arg_tmp->next == NULL)
	{
		arr = dup_envp(head);
		sort_arr(arr);
		i = 0;
		while (arr[i]) 
		{
			printf("declare -x %s\n", arr[i]);
			i++;
		}
		ft_free(arr);
		return (1);
	}
	arg_tmp = arg_tmp->next;
	while (arg_tmp)
	{
		add_envp(arg_tmp->cmd, head);
		arg_tmp = arg_tmp->next;
	}
	return (1);
}

// int main(int ac, char **av, char **env) //테스트 메인문
// {
// 	t_envp	*head = set_envp(env);
// 	t_info	*arg = init_list();
// 	t_info	*arg2 = init_list();

// 	insert_list(arg, "export", 2);
// 	insert_list(arg, "xxx=123", 2);
// 	insert_list(arg, "a====b", 2);
// 	insert_list(arg, "123a=123", 2);
// 	insert_list(arg, "xxx+=456", 2);
// 	insert_list(arg, "ooo+=456", 2);

// 	ft_export(arg, head);

// 	insert_list(arg2, "export", 2);
// 	ft_export(arg2, head);

// 	return (0);
// }
