/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_bits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 11:44:09 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 11:44:51 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned char   reverse_bits(unsigned char octet)
{
	return  (((octet >> 0) & 1) << 7) | \
			(((octet >> 1) & 1) << 6) | \
			(((octet >> 2) & 1) << 5) | \
			(((octet >> 3) & 1) << 4) | \
			(((octet >> 4) & 1) << 3) | \
			(((octet >> 5) & 1) << 2) | \
			(((octet >> 6) & 1) << 1) | \
			(((octet >> 7) & 1) << 0);
}

//Allowed functions:
//--------------------------------------------------------------------------------
//
//Write a function that takes a byte, reverses it, bit by bit (like the
//example) and returns the result.
//
//Your function must be declared as follows:
//
//unsigned char	reverse_bits(unsigned char octet);
//
//Example:
//
//  1 byte
//_____________
//0100  0001
//     ||
//     \/
 //1000  0010