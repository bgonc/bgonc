/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_1_alpha_mirror.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 19:54:58 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/05 16:25:16 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		main(int argc, char *argv[])
{
	int index;
	index = 0;
	
	if (argc == 2)
	{
		while (argv[1][index])
		{
			if (argv[1][index] >= 'a' && argv[1][index] <= 'z')
				argv[1][index] = 'a' + 'z' - argv[1][index];
			else if (argv[1][index] >= 'A' && argv[1][index] <= 'Z')
				argv[1][index] = 'A' + 'Z' - argv[1][index];
			write (1, &argv[1][index], 1);
			index++;
		}
	}
	write (1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program called alpha_mirror that takes a string and displays this string
//after replacing each alphabetical character by the opposite alphabetical
//character, followed by a newline.
//
//'a' becomes 'z', 'Z' becomes 'A'
//'d' becomes 'w', 'M' becomes 'N'
//
//and so on.
//
//Case is not changed.
//
//If the number of arguments is not 1, display only a newline.
//
//Examples:
//
//$>./alpha_mirror "abc"
//zyx
//$>./alpha_mirror "My horse is Amazing." | cat -e
//Nb slihv rh Znzarmt.$
//$>./alpha_mirror | cat -e
//$
//$>