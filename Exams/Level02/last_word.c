/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:39:39 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:40:09 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	fn_word(char *str, int *i)
{
	while (str[*i] != ' ' && str[*i] != '\t' && str[*i])
		(*i) += 1;
}

void	print_word(char *str, int start, int end)
{
	int		i;
	int		c;

	i = start;
	while (str[i] && str[i] != ' ' && str[i] != '\t' && end--)
		write(1, &str[i++], 1);
}

int		main(int argc, char **argv)
{
	int		i;
	int		start;
	int		end;

	if (argc == 2)
	{
		i = 0;
		start = 0;
		end = 0;
		while ((argv[1][i] == ' ' || argv[1][i] == '\t') && argv[1][i])
			i += 1;
		while (argv[1][i])
		{
			start = i;
			fn_word(argv[1], &i);
			end = i;
			while (argv[1][i] && (argv[1][i] == ' ' || argv[1][i] == '\t'))
				i += 1;
			if (!argv[1][i])
				print_word(argv[1], start, end);
		}
	}
	write(1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes a string and displays its last word followed by a \n.
//
//A word is a section of string delimited by spaces/tabs or by the start/end of
//the string.
//
//If the number of parameters is not 1, or there are no words, display a newline.
//
//Example:
//
//$> ./last_word "FOR PONY" | cat -e
//PONY$
//$> ./last_word "this        ...       is sparta, then again, maybe    not" | cat -e
//not$
//$> ./last_word "   " | cat -e
//$
//$> ./last_word "a" "b" | cat -e
//$
//$> ./last_word "  lorem,ipsum  " | cat -e
//lorem,ipsum$
//$>