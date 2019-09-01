/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:30:35 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:31:21 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_strcmp(char *s1, char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1 += 1;
		s2 += 2;
	}
	return (*s1 - *s2);
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Reproduce the behavior of the function strcmp (man strcmp).
//
//Your function must be declared as follows:
//
//int    ft_strcmp(char *s1, char *s2);