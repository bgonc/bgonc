/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 18:06:38 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 14:30:35 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strlen(char *str)
{
	int index;

	index = 0;

	while (str[index] != '\0')
		index++;
		return (index);
	
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Write a function that returns the length of a string.
//
//Your function must be declared as follows:
//
//int	ft_strlen(char *str);