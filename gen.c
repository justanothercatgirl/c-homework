#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	srand(69420);
	if (argc != 3) {
		fputs("Error: usage: provide number of arguments and outpit filename\n"
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
		    scphy = rand() % 5 + 1, 
		    scmat = rand() % 5 + 1, 
		    scinf = rand() % 5 + 1;
		lname[0] = rand() % ('Z'-'A'+1)+'A';
		name[0] = rand() % ('Z'-'A'+1)+'A';
		mname[0] = rand() % ('Z'-'A'+1)+'A';
		for (int j = 1; j < 7; ++j) {
			lname[j] = rand() % ('z'-'a'+1)+'a';
			name[j] = rand() % ('z'-'a'+1)+'a';
			mname[j] = rand() % ('z'-'a'+1)+'a';
		}
		fprintf(f, "%s\t%s\t%s\t%i\t%i\t%i\t%i\n", 
			lname, name, mname,
			byear, scphy, scmat, scinf);
	}
	fclose(f);
}
