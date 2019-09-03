/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_bits.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:45:21 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 13:22:34 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned char	swap_bits(unsigned char octet)
{
	return ((octet << 4) | (octet >> 4));
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Write a function that takes a byte, swaps its halves (like the example) and
//returns the result.
//
//Your function must be declared as follows:
//
//unsigned char	swap_bits(unsigned char octet);
//
//Example:
//
//  1 byte
//_____________
// 0100 | 0001
//     \ /
//     / \
// 0001 | 0100