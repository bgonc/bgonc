/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgoncalv <bgoncalv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/31 17:56:20 by bgoncalv          #+#    #+#             */
/*   Updated: 2019/09/01 14:29:38 by bgoncalv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stdio.h>
//
//char	*ft_strcpy(char *s1, char *s2);
//
//int		main(void)
//{
//	char src[] = "Sad but true";
//	char dest[15];
//
//	ft_strcpy(dest, src);
//	printf("%s <-- source, %s < --- destination", src, dest);
//	return (0);
//}

char	*ft_strcpy(char *s1, char *s2)
{
	int		index;

	index = 0;

	while (s2[index] != '\0')
		{
			s1[index] = s2[index];
			index++;
		}
		s1[index] = '\0';
		return (s1);
}

//Allowed functions: 
//--------------------------------------------------------------------------------
//
//Reproduce the behavior of the function strcpy (man strcpy).
//
//Your function must be declared as follows:
//
//char    *ft_strcpy(char *s1, char *s2);