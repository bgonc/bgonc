/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 19:58:46 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 13:24:38 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>

int	ft_atoi(const char *str)
{
	int i = 0;
	int sign = 1;
	int nb = 0;

	while ((str[i] != 0) && (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f' || str[i] == '\r' || str[i] == ' '))
		i++;
	if ((str[i] != 0) && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while ( str[i] != 0 && str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	return (nb * sign);
}

//int main (void)
//{
//	printf("The integer value from atoi function is %d\n", ft_atoi("123456789"));
//	return (0);
//}
/* ***************************************************************************** */

//Allowed functions: None
//--------------------------------------------------------------------------------
//
//Write a function that converts the string argument str to an integer (type int)
//and returns it.
//
//It works much like the standard atoi(const char *str) function, see the man.
//
//Your function must be declared as follows:
//
//int	ft_atoi(const char *str);