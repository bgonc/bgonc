/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff_first.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:19:26 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:20:04 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		main(int argc, char *argv[])
{
	int		i;
	i = 0;
	if (argc > 1)
	{
		while(argv[1][i])
			write(1, &argv[1][i++], 1);
	}
	write(1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes strings as arguments, and displays its first
//argument followed by a \n.
//
//If the number of arguments is less than 1, the program displays \n.
//
//Example:
//
//$> ./aff_first_param vincent mit "l'ane" dans un pre et "s'en" vint | cat -e
//vincent$
//$> ./aff_first_param "j'aime le fromage de chevre" | cat -e
//j'aime le fromage de chevre$
//$> ./aff_first_param
//$