#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <errno.h>

#define STRING_MAX 32
#define BOOKS_MAX 255
#define STUDENTS_MAX 255

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

	if (chars_read - 1> STRING_MAX || chars_read - 1 <= 0) {
		free(string);
		return 0;
	}

	strncpy(str, string, chars_read-1);
	return chars_read-1;
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
	while (!read_str(b->Title)) { 
		printf("Le titre doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	printf("Auteur\n> ");
	while (!read_str(b->Author)) { 
		printf("Le nom d'auteur doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
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

	for (int i = 0; i < books_l; i++) {
		if (books[i].ID == id) {
			for (int j = i; j < books_l - 1; j++) {
				books[j] = books[j+1];
			}
			books_l--;
			return;
		}
	}
	printf("Aucun livre avec cet identifiant existe.\n");
}

void delete_book_title() {
	char title[STRING_MAX];
	int title_l;
	while (!(title_l = read_str(title))) { 
		printf("Le titre doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	struct book books_dup_title[BOOKS_MAX];
	long long books_dup_title_l = 0;
	for (int i = 0; i < books_l; i++) {
		if (!strncmp(books[i].Title, title, title_l)) {
			books_dup_title[books_dup_title_l] = books[i];
			books_dup_title_l++;
		}
	}

	if (!books_dup_title_l) {
		printf("Aucun livre avec ce titre existe.\n");
		return;
	}
	
	if (books_dup_title_l == 1) {
		for (int i = 0; i < books_l; i++) {
			if (books[i].ID == books_dup_title[0].ID) {
				for (int j = i; j < books_l - 1; j++) {
					books[j] = books[j+1];
				}
				books_l--;
				return;
			}
		}
	}

	print_books(books_dup_title, books_dup_title_l);
	printf(
		"1: Supprimer tous?\n"
		"2: Par identifiant?\n"
		"> "
	);
	int input;
	while (1) { 
		if (read_i(&input) || input <= 0 || input > 2) { 
			printf("Votre choix doit etre un nombre entre 1 et 2!\n> ");
			continue;
		}
		break;
	}
	switch (input) {
		case 1:
			for (int i = 0; i < books_dup_title_l; i++) {
				for (int j = 0; j < books_l; j++) {
					if (books[j].ID == books_dup_title[i].ID) {
						for (int k = j; k < books_l - 1; k++) {
							books[k] = books[k+1];
						}
						books_l--;
						break;
					}
				}
			}
			break;

		case 2:
			delete_book_id();
			break;
	}
}

void delete_book() {
	if (books_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int input;
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

void search_book_title() {
	char title[STRING_MAX];
	int title_l;
	while (!(title_l = read_str(title))) { 
		printf("Le titre doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	struct book books_dup_title[BOOKS_MAX];
	long long books_dup_title_l = 0;
	for (int i = 0; i < books_l; i++) {
		if (!strncmp(books[i].Title, title, title_l)) {
			books_dup_title[books_dup_title_l] = books[i];
			books_dup_title_l++;
		}
	}

	if (!books_dup_title_l) {
		printf("Aucun livre avec ce titre existe.\n");
		return;
	}

	print_books(books_dup_title, books_dup_title_l);
}

void search_book_id() {
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

	struct book book_with_id[BOOKS_MAX];
	long long book_with_id_l = 0;
	for (int i = 0; i < books_l; i++) {
		if (books[i].ID == id) {
			book_with_id[book_with_id_l] = books[i];
			book_with_id_l++;
		}
	}

	if (!book_with_id_l) {
		printf("Aucun livre avec cet identifiant existe.\n");
		return;
	}

	print_books(book_with_id, 1);
}

void search_book() {
	if (books_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int input;
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
		case 1: search_book_title(); break;
		case 2: search_book_id(); break;
	}
}

struct student {
	long long ID;
	char Fname[STRING_MAX];
	char Lname[STRING_MAX];
	int Year;
};

struct student students[STUDENTS_MAX];
long long students_l = 0;

void add_student() {
	if (students_l == STUDENTS_MAX) {
		printf("Vous avez atteint le nombre maximum des etudiants possible.\n");
		return;
	}

	struct student *s = &students[students_l];

	printf("Identifiant\n> ");
	while (1) { 
		if (read_ll(&s->ID)) { 
			printf("L'identifiant doit etre un nombre!\n> ");
			continue;
		}
		if (s->ID < 0) {
			printf("L'identifiant doit etre un positif!\n> ");
			continue;
		}
		if (is_duplicate_id(s->ID)) {
			printf("L'identifiant doit etre unique!\n> ");
			continue;
		}
		break;
	}

	printf("Prenom\n> ");
	while (!read_str(s->Fname)) { 
		printf("Le prenom doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	printf("Nom\n> ");
	while (!read_str(s->Lname)) { 
		printf("Le nom doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	printf("Niveau d'etude\n> ");
	while (1) { 
		if (read_i(&s->Year)) { 
			printf("Le niveau doit etre un nombre!\n> ");
			continue;
		}
		if (s->Year > 5 || s->Year < 0) {
			printf("La niveau d'etude doit etre entre 1 et 5!\n> ");
			continue;
		}
		break;
	}

	students_l++;
}

void print_students() {
	if (students_l == 0) {
		printf("Il y'a aucun etudiant.\n");
		return;
	}

	int id_w = 2;
	int fname_w = 6;
	int lname_w = 3;
	for (int i = 0; i < students_l; i++) {
		struct book *s = &students[i];
		if (numlen(s->ID) > id_w) {
			id_w = numlen(s->ID);
		}
		if (strlen(s->Fname) > fname_w)  {
			fname_w = strlen(s->Fname);
		}
		if (strlen(s->Lname) > author_w)  {
			lname_w = strlen(s->Lname);
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

int main() {
	int input;
	while (1) {
		printf(
			"0: Quitter\n"
			"1: Ajouter un livre\n"
			"2: Afficher les livres\n"
			"3: Supprimer un livre\n"
			"4: Chercher un livre\n"
			"5: Ajouter un etudiant\n"
			"6: Afficher les etudiants\n"
			"> "
		);
		while (1) { 
			if (read_i(&input) || input < 0 || input > 6) { 
				printf("Votre choix doit etre un nombre entre 0 et 4!\n> ");
				continue;
			}
			break;
		}
		switch (input) {
			case 0: exit(0);
			case 1: add_book(); break;
			case 2: print_books(books, books_l); break;
			case 3: delete_book(); break;
			case 4: search_book(); break;
			case 5: add_student(); break;
			case 6: print_students(); break;
		}
		printf("\n");
		repeat_char('-', 80);
		printf("\n\n\n");
	}
}
