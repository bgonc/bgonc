/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_4rotone.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 19:30:36 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/05 14:25:45 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		main(int argc, char *argv[])
{
	int		index;
	char	letter;
	index = 0;
	if (argc == 2)
	{
		while (argv[1][index])
		{
			letter = argv[1][index];
			if (argv[1][index] >= 'A' && argv[1][index] <= 'Y')
				letter += 1;
			if (argv[1][index] >= 'a' && argv[1][index] <= 'y')
				letter += 1;
			if (argv[1][index] == 'z' || argv[1][index] == 'Z')
				letter -= 25;
			write(1, &letter, 1);
			index += 1;
		}
	}
	write(1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes a string and displays it, replacing each of its
//letters by the next one in alphabetical order.
//
//'z' becomes 'a' and 'Z' becomes 'A'. Case remains unaffected.
//
//The output will be followed by a \n.
//
//If the number of arguments is not 1, the program displays \n.
//
//Example:
//
//$>./rotone "abc"
//bcd
//$>./rotone "Les stagiaires du staff ne sentent pas toujours tres bon." | cat -e
//Mft tubhjbjsft ev tubgg of tfoufou qbt upvkpvst usft cpo.$
//$>./rotone "AkjhZ zLKIJz , 23y " | cat -e
//BlkiA aMLJKa , 23z $
//$>./rotone | cat -e
//$
//$>
//$>./rotone "" | cat -e
//$
//$>