#include <stdlib.h>
#include <stdio.h>

#define vowsz (sizeof vows)
#define consz (sizeof cons)

char vows[] = {'a', 'e', 'i', 'o', 'u'};
char cons[] = {
	'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',
	'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'
};

int main(int argc, char *argv[]) {
	srand(69420);
	if (argc != 3) {
		fputs("Error: usage: provide number of arguments and output filename\n"
			"./a.out 69 amogus\n", stderr);
		return 1;
	}
	int amount = atoi(argv[1]);
	FILE *f = fopen(argv[2], "w+");
	fprintf(f, "Группа N%u\n", rand());
	fputs("Ф\tИ\tО\tг.р.\tФизика\tМатан\tИнф-ка\n", f);
	for (int i = 0; i < amount; ++i) {
		char lname[8] = {0}, 
		     name[8] = {0}, 
		     mname[8] = {0};
		int byear = rand()%125+1900, 
		    scphy = rand() % 4 + 2, 
		    scmat = rand() % 4 + 2, 
		    scinf = rand() % 4 + 2;
		lname[0] = rand() % ('Z'-'A'+1)+'A';
		name[0] = rand() % ('Z'-'A'+1)+'A';
		mname[0] = rand() % ('Z'-'A'+1)+'A';
		char llastvow = 0;
		char lastvow = 0;
		char mlastvow = 0;
		for (int j = 1; j < 6; ++j) {
			if(llastvow) lname[j] = cons[rand() % consz];
			else lname[j] = vows[rand() % vowsz];
			if(lastvow) name[j] = cons[rand() % consz];
			else name[j] = vows[rand() % vowsz];
			if(mlastvow) mname[j] = cons[rand() % consz];
			else mname[j] = vows[rand() % vowsz];
			llastvow = !llastvow;
			lastvow = !lastvow;
			mlastvow = !mlastvow;
		}
		fprintf(f, "%s\t%s\t%s\t%i\t%i\t%i\t%i\n", 
			lname, name, mname,
			byear, scphy, scmat, scinf);
	}
	fclose(f);
}
