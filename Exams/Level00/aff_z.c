/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff_z.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:22:43 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:28:15 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int main (int argc, char **argv)
{
	if (argc > 0 && argv[0] > 0)
	{
		write (1, "z\n", 2);
	}
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes a string, and displays the first 'z'
//character it encounters in it, followed by a newline. If there are no
//'z' characters in the string, the program writes 'z' followed
//by a newline. If the number of parameters is not 1, the program displays
//'z' followed by a newline.
//
//Example:
//
//$> ./aff_z "abc" | cat -e
//z$
//$> ./aff_z "dubO a POIL" | cat -e
//z$
//$> ./aff_z "zaz sent le poney" | cat -e
//z$
//$> ./aff_z | cat -e
//z$