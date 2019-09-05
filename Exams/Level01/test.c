#include <unistd.h>

int		main(int argc, char *argv[])
{
	int index;
	int rep;

	index = 0;

	if(argc == 2)
	{
		while(argv[1][index])
			{
				if(argv[1][index] >= 'a' && argv[1][index] <= 'z')
				{	
					rep = argv[1][index] - 95;
					while(--rep)
					write(1, &argv[1][index], 1);
				}
				else if(argv[1][index] >= 'a' && argv[1][index] <= 'z')
				{
					rep = argv[1][index] - 95;
					write(1, &argv[1][index], 1);
					rep++;
				}
				
			else
				write(1, &argv[1][index], 1);
				index++;
			}
	}
	write(1, "\n", 1);
	return (0);
}