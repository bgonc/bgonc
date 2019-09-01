#include <unistd.h>

int		main(int argc, char *argv[])
{
	int index = 0;
	int rep;
	char c;

	if (argc == 2)
	{
		while (argv[1][index])
		{
			c = argv[1][index];
			if(( c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			{
				if (c >= 'a' && c <= 'z')
					rep = c - 'a';
				if (c >= 'A' && c <= 'Z')
					rep = c - 'A';
				while (rep > 0)
				{
					write (1, &c, 1);
					rep--;
				}
			}
			write (1, &c, 1);
			index++;
		}
	}
	write (1, "\n", 1);
	return(0);
}