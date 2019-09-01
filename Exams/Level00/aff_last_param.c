/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff_last_param.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:21:19 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 13:09:17 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		main(int argc, char *argv[])
{
	int		i;
	i = 0;
	if (argc > 1)
	{
		while(argv[argc - 1][i])
			write(1, &argv[argc - 1][i++], 1);
	}
	write(1, "\n", 1);
	return (0);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program that takes strings as arguments, and displays its last
//argument followed by a newline.
//
//If the number of arguments is less than 1, the program displays a newline.
//
//Examples:
//
//$> ./aff_last_param "zaz" "mange" "des" "chats" | cat -e
//chats$
//$> ./aff_last_param "j'aime le savon" | cat -e
//j'aime le savon$
//$> ./aff_last_param
//$