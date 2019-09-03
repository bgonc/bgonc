/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   union.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:46:21 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:47:49 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_comp(char *str, char *s1)
{
	int	a;

	a = 0;
	while (s1[a])
	{
		if (str[(unsigned int)s1[a]] == 0)
		{
			write(1, &s1[a], 1);
			str[(unsigned int)s1[a]] = 1;
		}
		a++;
	}
}

void	ft_union(char *s1, char *s2)
{
	char	str[256] = {0};

	ft_comp(str, s1);
	ft_comp(str, s2);
}

int		main(int ac, char **av)
{
	if (ac == 3)
		ft_union(av[1], av[2]);
	write(1, "\n", 1);
	return 0;
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes two strings and displays, without doubles, the
//characters that appear in either one of the strings.
//
//The display will be in the order characters appear in the command line, and
//will be followed by a \n.
//
//If the number of arguments is not 2, the program displays \n.
//
//Example:
//
//$>./union zpadinton "paqefwtdjetyiytjneytjoeyjnejeyj" | cat -e
//zpadintoqefwjy$
//$>./union ddf6vewg64f gtwthgdwthdwfteewhrtag6h4ffdhsd | cat -e
//df6vewg4thras$
//$>./union "rien" "cette phrase ne cache rien" | cat -e
//rienct phas$
//$>./union | cat -e
//$
//$>
//$>./union "rien" | cat -e
//$
//$>