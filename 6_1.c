#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define SIZEOFBUF 1024
#define reset(bufsz) bufsz = SIZEOFBUF

// копирует n линий из файла in в файл out
void copy_n_lines(char **buf, size_t *bufsz, FILE *in, FILE *out, int n) {
	for (int i = 0; i < n; ++i) {
		getline(buf, bufsz, in);
		fputs(*buf, out);
	}
}

// комирует первые n столбцов из фала in в out
void copy_n_words(char **buf, size_t *bufsz, FILE *in, FILE *out, int n, char delim) {
	ssize_t cursz;
	for (int i = 0; i < n; ++i) {
		if ((cursz = getdelim(buf, bufsz, delim, in)) < 0) return;
		fputs(*buf, out);
	}
}

// пропускает первые n слов в файле in
void skip_n_words(char **buf, size_t *bufsz, FILE *in, int n, char delim) {
	for (int i = 0; i < n; ++i)
		getdelim(buf, bufsz, delim, in);
}

// вариант 1
void change_surnames(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsz = SIZEOFBUF;
	ssize_t cursz;
	// первые 2 строки такие же
	copy_n_lines(&buf, &bufsz, in, out, 2);
	while (!feof(in)) {
		// получаем первое слово
		if ((cursz = getdelim(&buf, &bufsz, '\t', in)) < 0) break;
		buf[cursz-1] = '\0'; // убираем '\t' в конце
		fprintf(out, "%s%c\t", buf, rand() % ('z'-'a'+1)+'a');
		// копируем оставшуюся линию
		if (getline(&buf, &bufsz, in) < 0) break;
		fputs(buf, out);
	}
	free(buf);
}

// вариант 2
void change_birth_dates(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsz = SIZEOFBUF;
	// первые 2 строки должны быть такими же
	copy_n_lines(&buf, &bufsz, in, out, 2);
	while (!feof(in)) {
		// и первые 3 слова тоже
		copy_n_words(&buf, &bufsz, in, out, 3, '\t');
		// меняем дату :devil:
		if (getdelim(&buf, &bufsz, '\t', in) < 0) break;
		int bd = atoi(buf); 
		if (!bd) break;
		fprintf(out, "%i\t", bd + rand()%11-5);	
		// выписываем оставшуюся строку
		if (getline(&buf, &bufsz, in) < 0) break;
		fputs(buf, out);
	}
	free(buf);
}

// вариант 3
void avg_score(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsz = SIZEOFBUF;
	size_t counter = 0;
	long marks[3] = {0};
	// первые 4 столбца
	skip_n_words(&buf, &bufsz, in, 4, '\t');
	// комируем оставшуюся линию
	copy_n_lines(&buf, &bufsz, in, out, 1);
	while (!feof(in)) {
		// первые 4 столбца не интересуют
		skip_n_words(&buf, &bufsz, in, 4, '\t');
		if (getline(&buf, &bufsz, in) < 0) break;
		// здесь и в цикле выделяем 3 числа и прибавляем их к
		// оценкам по правильному индексу
		// strotok, а не getdelim, потому что доходим до конца строки
		strtok(buf, "\t\n");
		int index = 0;
		do {
			int tmp = atoi(buf);
			if (!tmp) break;
			marks[index] += tmp;
			++index;
		} while ((buf = strtok(NULL, "\t\n")));
		++counter;
	}
	// выводим результат
	fprintf(out, "%lf\t%lf\t%lf\n", 
		(double)marks[0] / counter, 
		(double)marks[1] / counter, 
		(double)marks[2] / counter);
	free(buf);
}

// вариант 4
void switch_marks(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsz = SIZEOFBUF;
	// первые 2 линие такие же
	copy_n_lines(&buf, &bufsz, in, out, 2);
	while (!feof(in)) {
		copy_n_words(&buf, &bufsz, in, out, 4, '\t');
		if (getline(&buf, &bufsz, in) < 0) break;
		// здесь и в цикле выделяем 3 числа и изменяем их по заданию
		// strotok, а не getdelim, потому что доходим до конца строки
		strtok(buf, "\t\n");
		do {
			int tmp = atoi(buf);
			if (!tmp) break;
			fprintf(out, "%i\t", 5-tmp+2); // о неет, кто поменял все оценки...
		} while ((buf = strtok(NULL, "\t\n")));
		fputc('\n', out); // не забываем newline
	}
	free(buf);
}

// вариант 5
void excellents(FILE *in, FILE *out) {
	char *buf = malloc(SIZEOFBUF);
	size_t bufsz = SIZEOFBUF;
	ssize_t cursz;
	// возьмём столбцы из оригинального файла и скопируем нужные
	copy_n_lines(&buf, &bufsz, in, out, 1);
	copy_n_words(&buf, &bufsz, in, out, 1, '\t');
	skip_n_words(&buf, &bufsz, in, 3, '\t');
	copy_n_lines(&buf, &bufsz, in, out, 1);
	while (!feof(in)) {
		if ((cursz = getdelim(&buf, &bufsz, '\t', in)) < 0) break;
		// сохраняем фамилию на потом;
		char *lname = malloc(sizeof (char) * cursz);
		char *marks = NULL; // понадобится позже
		strncpy(lname, buf, cursz);
		// пропускаем 3 колонки (имя, отчество, дата рождения)
		skip_n_words(&buf, &bufsz, in, 3, '\t');
		// сохраняем строку оценок
		if ((cursz = getline(&buf, &bufsz, in)) < 0) goto free; // чтобы не было утечек памяти
		marks = malloc(sizeof (char) * cursz);
		strncpy(marks, buf, cursz);
		// цикл, в котором считаем кол-во пятёрок
		int fives_count = 0;
		strtok(buf, "\t\n");
		do {
			int tmp = atoi(buf);
			if (!tmp) break;
			if (tmp == 5) ++fives_count;
		} while ((buf = strtok(NULL, "\t\n")));
		if (fives_count >= 2)
			fprintf(out, "%s%s", lname, marks);
free:
		free(lname);
		free(marks);
	}
}

// массив функций для более простого выбора варианта
void (*variants[5])(FILE *, FILE *) = {
	change_surnames,
	change_birth_dates,
	avg_score,
	switch_marks,
	excellents
};

int main(int argc, char *argv[]) {
	srand(0xB19B00B5);
	
	// стандартный цикл обработки аргументов argv
	if (argc != 6) {
		fprintf(stderr, "usage: \n" "%s -i INPUT -o OUTPUT variant\n", *argv);
		return 1;
	}
	const char *in, *out;
	int var = 0;
	for (int i = 0; i < argc; ++i) {
		if (!strcmp(argv[i], "-i"))
			in = argv[++i];
		else if (!strcmp(argv[i], "-o")) 
			out = argv[++i];
		else 
			var = atoi(argv[i]);
	}
	if (var < 1 || var > 5) { 
		fprintf(stderr, "Неправильный вариант: %i\n", var);
		return 1;
	}
	// открытие файлов
	FILE *f_in = fopen(in, "r");
	FILE *f_out = fopen(out, "w+");
	if (!f_in || !f_out) {
		fprintf(stderr, "Не удалось открыть файл: %s\n", strerror(errno));
		return 1;
	}
	// сам вызов функции
	variants[--var](f_in, f_out);
	return 0;
}

