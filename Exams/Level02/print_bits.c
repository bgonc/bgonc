/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:42:51 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:43:09 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	print_bits(unsigned char octet)
{
	int				i;
	unsigned char	bit;

	i = 7;
	while (i--)
	{
		bit = (octet >> i & 1) + '0';
		write(1, &bit, 1);
	}
	write(1, "\n", 1);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a function that takes a byte, and prints it in binary WITHOUT A NEWLINE
//AT THE END.
//
//Your function must be declared as follows:
//
//void	print_bits(unsigned char octet);
//
//Example, if you pass 2 to print_bits, it will print "00000010"