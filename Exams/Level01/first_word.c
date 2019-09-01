/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 17:40:15 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/08/31 19:29:57 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		ft_spaces(int i)
{
	if (i == '\t' || i == '\n' || i == '\v' || i == '\f' || i == ' ')
		return(1);
	return (0);
}

int		main(int argc, char *argv[])
{
	int index;

	index = 0;

	if(argc == 2)
	{
		while(ft_spaces(argv[1][index]))
			index ++;
		while(!(ft_spaces(argv[1][index]))&& argv[1][index])
			write(1, &argv[1][index++], 1);
	}
	write(1, "\n", 1);
	return(0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes a string and displays its first word, followed by a
//newline.
//
//A word is a section of string delimited by spaces/tabs or by the start/end of
//the string.
//
//If the number of parameters is not 1, or if there are no words, simply display
//a newline.
//
//Examples:
//
//$> ./first_word "FOR PONY" | cat -e
//FOR$
//$> ./first_word "this        ...       is sparta, then again, maybe    not" | cat -e
//this$
//$> ./first_word "   " | cat -e
//$
//$> ./first_word "a" "b" | cat -e
//$
//$> ./first_word "  lorem,ipsum  " | cat -e
//lorem,ipsum$
//$>