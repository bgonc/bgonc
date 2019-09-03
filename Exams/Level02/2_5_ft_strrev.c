/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:35:56 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:36:20 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char		*ft_strrev(char *str)
{
	int		i;
	int		len;
	char	tmp;

	len = 0;
	while (str[len])
		len += 1;
	i = -1;
	while (++i < --len)
	{
		tmp = str[i];
		str[i] = str[len];
		str[len] = tmp;
	}
	str[i] = '\0';
	return (str);
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Write a function that reverses (in-place) a string.
//
//It must return its parameter.
//
//Your function must be declared as follows:
//
//char    *ft_strrev(char *str);