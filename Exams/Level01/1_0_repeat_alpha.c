#include <unistd.h>

int		main(int argc, char *argv[])
{
	int index;
	int rep;
	index = 0;

	if (argc == 2)
	{
		while (argv[1][index])
		{
			if(argv[1][index] >= 'A' && 'Z' >= argv[1][index])
			{
				rep = argv[1][index] - 63;
				while(--rep)
				write(1, &argv[1][index], 1);
			}
			else if(argv[1][index] >= 'a' && 'z' >= argv[1][index])
			{	
				rep = argv[1][index] - 95;
				while(--rep)
				write(1, &argv[1][index], 1);
			}
		else
			write(1, &argv[1][index], 1);
		index++;
		}
	}
	write(1, "\n", 1);
	return (0);
}


//Allowed functions: write
//--------------------------------------------------------------------------------
//
//Write a program called repeat_alpha that takes a string and display it
//repeating each alphabetical character as many times as its alphabetical index,
//followed by a newline.
//
//'a' becomes 'a', 'b' becomes 'bb', 'e' becomes 'eeeee', etc...
//
//Case remains unchanged.
//
//If the number of arguments is not 1, just display a newline.
//
//Examples:
//
//$>./repeat_alpha "abc"
//abbccc
//$>./repeat_alpha "Alex." | cat -e
//Alllllllllllleeeeexxxxxxxxxxxxxxxxxxxxxxxx.$
//$>./repeat_alpha 'abacadaba 42!' | cat -e
//abbacccaddddabba 42!$
//$>./repeat_alpha | cat -e
//$
//$>
//$>./repeat_alpha "" | cat -e
//$
//$>