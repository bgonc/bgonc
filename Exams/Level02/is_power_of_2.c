/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_power_of_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:38:47 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:39:12 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	    is_power_of_2(unsigned int n)
{
	return ((n & (-n)) == n ? 1 : 0);
}

//Allowed functions: None
//--------------------------------------------------------------------------------
//
//Write a function that determines if a given number is a power of 2.
//
//This function returns 1 if the given number is a power of 2, otherwise it returns 0.
//
//Your function must be declared as follows:
//
//int	    is_power_of_2(unsigned int n);