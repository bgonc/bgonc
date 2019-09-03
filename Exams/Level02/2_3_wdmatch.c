/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wdmatch.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:49:05 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:49:36 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr(char const *str)
{
	int		i;

	i = 0;
	while (str[i])
		write(1, &str[i++], 1);
}

int		main(int argc, char const *argv[])
{
	int		i;
	int		j;

	if (argc == 3)
	{
		i = 0;
		j = 0;
		while (argv[2][j])
			if (argv[2][j++] == argv[1][i])
				i += 1;
		if (!argv[1][i])
			ft_putstr(argv[1]);
	}
	write(1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes two strings and checks whether it's possible to
//write the first string with characters from the second string, while respecting
//the order in which these characters appear in the second string.
//
//If it's possible, the program displays the string, followed by a \n, otherwise
//it simply displays a \n.
//
//If the number of arguments is not 2, the program displays a \n.
//
//Examples:
//
//$>./wdmatch "faya" "fgvvfdxcacpolhyghbreda" | cat -e
//faya$
//$>./wdmatch "faya" "fgvvfdxcacpolhyghbred" | cat -e
//$
//$>./wdmatch "quarante deux" "qfqfsudf arzgsayns tsregfdgs sjytdekuoixq " | cat -e
//quarante deux$
//$>./wdmatch "error" rrerrrfiiljdfxjyuifrrvcoojh | cat -e
//$
//$>./wdmatch | cat -e
//$