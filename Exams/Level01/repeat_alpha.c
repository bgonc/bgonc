#include <unistd.h>

int main (int argc, char *argv[])
{
	int index = 0;
	int rep; // counter to count the alpha position in ascii table
	char c;
	if (argc == 2)
	{
		while (argv[1][index])
		{
			c = argv[1][index]; 
			if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) // if it's a alpha
			{
				if (c >= 'a' && c <= 'z')
					rep = c - 'a';
				if (c >= 'A' && c <= 'Z')
					rep = c - 'A';
				while (rep > 0) // when the counter is greater then 0 then write it
				{
					write (1, &c, 1);
					rep--;
				}
			}
			write (1, &c, 1); // anyways write each word for one time.
			index++;
		}
	}
	write (1, "\n", 1);
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