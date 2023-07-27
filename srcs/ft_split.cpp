/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano </var/mail/hsano>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 12:58:17 by hsano             #+#    #+#             */
/*   Updated: 2023/07/04 14:47:10 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <stdio.h>
#include <stdbool.h>

static size_t	count_c(char const *s, char c)
{
	size_t	i;
	size_t	cnt;
	int		match_flag;

	i = 0;
	cnt = 0;
	match_flag = true;
	if (c == '\0')
		return (1);
	while (s[i])
	{
		if (s[i] == c)
			match_flag = true;
		else if (match_flag == true)
		{
			match_flag = false;
			cnt++;
		}
		i++;
	}
	return (cnt);
}

static char	*write_array(char const *s, size_t diff, size_t i, int *error)
{
	char	*p;
    std::vector<std::string>

	//p = malloc(diff + 1);
    p = new size_t[diff+1]
	if (!p)
	{
		*error = true;
		return (NULL);
	}
	p[diff] = '\0';
	while (diff--)
		p[diff] = s[i + diff];
	return (p);
}

static char	**last_check(char **p, int error, size_t j)
{
	size_t	i;

	if (error)
	{
		i = 0;
		while (i < j)
		{
			//free(p[i]);
            delete(p[i]);
			i++;
		}
		//free(p);
        delete(p);
	}
	return (p);
}

static char	**make_array(char **p, char const *s, int c, size_t cnt)
{
	char	*tmp_p;
	int		error;
	size_t	i;
	size_t	j;
	size_t	diff;

	j = 0;
	i = 0;
	error = false;
	while (j < cnt && !error)
	{
		tmp_p = ft_strchr(&(s[i]), c);
		if (tmp_p == NULL)
			break ;
		diff = tmp_p - &(s[i]);
		if (diff > 0)
			p[j++] = write_array(s, diff, i, &error);
		i += diff + 1;
	}
	diff = ft_strlen(s) - i;
	if (j < cnt && !error)
		p[j] = write_array(s, diff, i, &error);
	return (last_check(p, error, j));
}

char	**ft_split(char const *s, char c)
{
	char	**p;
	size_t	cnt;

	if (s == NULL)
		return (NULL);
	if (s[0] == '\0')
	{
		//p = (char **)malloc(sizeof(char *) * 1);
		p = new char*[1];
		if (p == NULL)
			return (NULL);
		p[0] = NULL;
		return (p);
	}
	cnt = count_c(s, c);
	//p = (char **)malloc(sizeof(char *) * (cnt + 1));
    p = new char*[cnt + 1];

	if (p == NULL)
		return (NULL);
	p[cnt] = NULL;
	p = make_array(p, s, (int)c, cnt);
	return (p);
}
//int main(int argc, char **argv)
//{
//
//	char **p;
//	char c;
//	size_t	size;
//	size_t i;
//
//	i = 0;
//	size = 2;
//	if (argc == 4)
//	{
//		c = argv[2][0];
//		p = ft_split(argv[1],c);
//		size = atoi(argv[3]);
//		while(i < size)
//		{
//			printf("p[%zu]=%s\n",i,p[i]);
//			i++;
//		}
//
//	}
//	char *tmp = "\0aa\0bbb";
//	printf("test No.2:%s:end\n",tmp);
//	char	*s = "      split       this for   me  !       ";
//	char	**expected = ft_split(s , ' ');
//	printf("test No.3:%s:end\n",tmp);
//
//	i = 0;
//	size = 5;
//	while(i < size)
//	{
//		if (expected[i] == NULL)
//			break;
//		printf("e[%zu]=%s\n",i,expected[i]);
//		i++;
//	}
//	return (0);
//}
