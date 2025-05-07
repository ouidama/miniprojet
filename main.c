#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>

#define STRING_MAX 32
#define BOOKS_MAX 255

#define numlen(n) (floor(log10l(n)) + 1)
#define table_seperator() {\
	putchar('+');\
	repeat_char('-', id_w + 2);\
	putchar('+');\
	repeat_char('-', title_w + 2);\
	putchar('+');\
	repeat_char('-', author_w + 2);\
	putchar('+');\
	repeat_char('-', 6);\
	putchar('+');\
	putchar('\n');\
}

struct book {
	long long ID;
	char Title[STRING_MAX];
	char Author[STRING_MAX];
	int Date;
};

struct book books[BOOKS_MAX];
long long books_l = 0;

void repeat_char(char c, int n) {
	for (int i = 0; i < n; i++) {
		putchar(c);
	}
}

void print_books(struct book books_list[BOOKS_MAX], long long books_list_l) {
	if (books_list_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int id_w = 2;
	int title_w = 5;
	int author_w = 6;
	for (int i = 0; i < books_list_l; i++) {
		struct book *b = &books_list[i];
		if (numlen(b->ID) > id_w) {
			id_w = numlen(b->ID);
		}
		if (strlen(b->Title) > title_w)  {
			title_w = strlen(b->Title);
		}
		if (strlen(b->Author) > author_w)  {
			author_w = strlen(b->Author);
		}
	}

	putchar('+');
	repeat_char('-', id_w+2);
	putchar('+');
	repeat_char('-', title_w+2);
	putchar('+');
	repeat_char('-', author_w+2);
	putchar('+');
	repeat_char('-', 6);
	putchar('+');
	putchar('\n');

	printf("| ID");
	repeat_char(' ', id_w - 2 + 1);
	printf("| Titre");
	repeat_char(' ', title_w - 5 + 1);
	printf("| Auteur");
	repeat_char(' ', author_w - 6 + 1);
	printf("| Date");
	printf(" |\n");

	table_seperator();

	for (int i = 0; i < books_list_l; i++) {
		struct book *b = &books_list[i];
		int b_id_w = numlen(b->ID);
		int b_title_w = strlen(b->Title);
		int b_author_w = strlen(b->Author);
		int b_date_w = numlen(b->Date);

		printf("| %lld", b->ID);
		repeat_char(' ', id_w - b_id_w + 1);
		printf("| %s", b->Title);
		repeat_char(' ', title_w - b_title_w + 1);
		printf("| %s", b->Author);
		repeat_char(' ', author_w - b_author_w + 1);
		printf("| %d", b->Date);
		repeat_char(' ', 4 - b_date_w + 1);
		printf("|\n");

		table_seperator();
	}
}

int read_ll(long long *ll) {
	char *string = NULL;
	size_t size = 0;
	ssize_t chars_read;

	chars_read = getline(&string, &size, stdin);
	char *endptr;
	errno = 0;
	long long t_ll = strtoll(string, &endptr, 10);

	if (errno || endptr == string) {
		free(string);
		return 1;
	}

	*ll = t_ll;

	free(string);
	return 0;
}

int read_i(int *i) {
	char *string = NULL;
	size_t size = 0;
	ssize_t chars_read;

	chars_read = getline(&string, &size, stdin);
	char *endptr;
	errno = 0;
	int t_i= strtol(string, &endptr, 10);

	if (errno || endptr == string) {
		free(string);
		return 1;
	}

	*i = t_i;

	free(string);
	return 0;
}

int read_str(char str[]) {
	char *string = NULL;
	size_t size = 0;
	ssize_t chars_read;

	chars_read = getline(&string, &size, stdin);

	if (chars_read - 1> 32 || chars_read - 1 <= 0) {
		free(string);
		return 1;
	}

	strncpy(str, string, chars_read-1);
	return 0;
}

int is_duplicate_id(long long id) {
	for (int i = 0; i < books_l; i++) {
		if (id == books[i].ID) {
			return 1;
		}
	}
	return 0;
}

void add_book() {
	if (books_l == BOOKS_MAX) {
		printf("Vous avez atteint le nombre maximum de livres possible.\n");
		return;
	}

	struct book *b = &books[books_l];

	printf("Identifiant\n> ");
	while (1) { 
		if (read_ll(&b->ID)) { 
			printf("L'identifiant doit etre un nombre!\n> ");
			continue;
		}
		if (b->ID < 0) {
			printf("L'identifiant doit etre un positif!\n> ");
			continue;
		}
		if (is_duplicate_id(b->ID)) {
			printf("L'identifiant doit etre unique!\n> ");
			continue;
		}
		break;
	}

	printf("Titre\n> ");
	while (read_str(b->Title)) { 
		printf("Le titre doit avoir entre 1 and 32 caracteres!\n> ");
	}

	printf("Auteur\n> ");
	while (read_str(b->Author)) { 
		printf("Le nom d'auteur doit avoir entre 1 and 32 caracteres!\n> ");
	}

	printf("Date\n> ");
	while (1) { 
		if (read_i(&b->Date)) { 
			printf("La date doit etre un nombre!\n> ");
			continue;
		}
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		int current_year = tm.tm_year + 1900;
		if (b->Date > current_year || b->Date < 0) {
			printf("La date doit etre entre 0 et %d!\n> ", current_year);
			continue;
		}
		break;
	}

	books_l++;
}

void delete_book_id() {
	long long id;
	printf("Identifiant\n> ");
	while (1) { 
		if (read_ll(&id)) { 
			printf("L'identifiant doit etre un nombre!\n> ");
			continue;
		}
		if (id < 0) {
			printf("L'identifiant doit etre un positif!\n> ");
			continue;
		}
		break;
	}

	int found = 0;
	for (int i = 0; i < books_l; i++) {
		if (books[i].ID == id) {
			fount = 1;
			for (int j = i; j < books_l - 1; j++) {
				books[j] = books[j+1];
			}
		}
	}

	if (!found) {
		printf("Aucun livre avec cet identifiant existe.\n");
	}
}

void delete_book_title() {
}

void delete_book() {
	if (books_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int input;
	while (1) {
		printf(
			"1: Par titre?\n"
			"2: Par identifiant?\n"
			"> "
		);
		while (1) { 
			if (read_i(&input) || input <= 0 || input > 2) { 
				printf("Votre choix doit etre un nombre entre 1 et 2!\n> ");
				continue;
			}
			break;
		}
		switch (input) {
			case 1: delete_book_title(); break;
			case 2: delete_book_id(); break;
		}
	}
}

int main() {
	int input;
	while (1) {
		printf(
			"0: Quitter\n"
			"1: Ajouter un livre\n"
			"2: Afficher les livres\n"
			"3: Supprimer un livre\n"
			"> "
		);
		while (1) { 
			if (read_i(&input) || input < 0 || input > 3) { 
				printf("Votre choix doit etre un nombre entre 0 et 3!\n> ");
				continue;
			}
			break;
		}
		switch (input) {
			case 0: exit(0);
			case 1: add_book(); break;
			case 2: print_books(books, books_l); break;
			case 3: delete_book(); break;
		}
		printf("\n");
		repeat_char('-', 80);
		printf("\n\n\n");
	}
}
