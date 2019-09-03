/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 17:52:52 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 15:08:46 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr(char *str)
{
	int index;

	index = 0;

	while (str[index] != '\0')
		write(1, &str[index++], 1);
}

//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a function that displays a string on the standard output.
//
//The pointer passed to the function contains the address of the string's first
//character.
//
//Your function must be declared as follows:
//
//void	ft_putstr(char *str);