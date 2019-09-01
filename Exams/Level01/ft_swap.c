/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 19:26:04 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/08/31 19:26:09 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

//Allowed functions: 
//--------------------------------------------------------------------------------
//
//Write a function that swaps the contents of two integers the adresses of which
//are passed as parameters.
//
//Your function must be declared as follows:
//
//void	ft_swap(int *a, int *b);