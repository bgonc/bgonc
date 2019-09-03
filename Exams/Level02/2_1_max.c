/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   max.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:41:33 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:42:26 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		max(int *tab, unsigned int len)
{
	int		i;
	int		max_value;

	if (!tab)
		return (0);
	i = 0;
	while (len--)
	{
		if (tab[i] > tab[i + 1])
			max_value = tab[i];
		i += 1;
	}
	return (max_value);
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Write the following function:
//
//int		max(int* tab, unsigned int len);
//
//The first parameter is an array of int, the second is the number of elements in
//the array.
//
//The function returns the largest number found in the array.
//
//If the array is empty, the function returns 0