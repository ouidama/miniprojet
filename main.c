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

#ifdef _WIN32
	#define CLEAR "cls"
#else
	#define CLEAR "clear"
#endif

struct student {
	long long ID;
	char Fname[STRING_MAX];
	char Lname[STRING_MAX];
	int Year;
};

struct book {
	long long ID;
	char Title[STRING_MAX];
	char Author[STRING_MAX];
	int Date;

	int Borrowed;
	struct student *Borrower;
	struct tm BorrowDate;
	struct tm ReturnDate;
};

struct book books[BOOKS_MAX];
long long books_l = 0;

void keep_max(int *a, int b) {
	if (b > *a) {
		*a = b;
	}
}

void repeat_char(char c, int n) {
	for (int i = 0; i < n; i++) {
		putchar(c);
	}
}

void table_seperator(int field_widths[], int fields_l) {
	for (int i = 0; i < fields_l; i++) {
		putchar('+');
		repeat_char('-', field_widths[i] + 2);
	}
	putchar('+');
	putchar('\n');
}

void table_header(char fields[][STRING_MAX], int field_widths[], int fields_l) {
	table_seperator(field_widths, fields_l);

	for (int i = 0; i < fields_l; i++) {
		printf("| %s", fields[i]);
		repeat_char(' ',  field_widths[i] - strlen(fields[i]) + 1);
	}
	printf("|\n");

	table_seperator(field_widths, fields_l);
}

void print_books(struct book books_list[BOOKS_MAX], long long books_list_l) {
	if (books_list_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	char fields[][STRING_MAX] = { "ID", "Titre", "Auteur", "Date", "Emprunte par", "Date d'emprunt", "La date de retour" };
	int fields_l = 7;
	int field_widths[fields_l];
	for (int i = 0; i < fields_l; i++) {
		field_widths[i] = strlen(fields[i]);
	}

	for (int i = 0; i < books_list_l; i++) {
		struct book *b = &books_list[i];
		keep_max(&field_widths[0], numlen(b->ID));
		keep_max(&field_widths[1], strlen(b->Title));
		keep_max(&field_widths[2], strlen(b->Author));

		if (b->Borrowed) {
			keep_max(&field_widths[5], numlen(b->Borrower->ID));
		}
	}

	table_header(fields, field_widths, fields_l);

	for (int i = 0; i < books_list_l; i++) {
		struct book *b = &books_list[i];
		char b_fields[fields_l][STRING_MAX];

		sprintf(b_fields[0], "%lld", b->ID);
		strcpy(b_fields[1], b->Title);
		strcpy(b_fields[2], b->Title);
		sprintf(b_fields[3], "%d", b->Date);
		if (b->Borrowed) {
			sprintf(b_fields[4], "%lld", b->Borrower->ID);

			char borrow_date_s[STRING_MAX];
			sprintf(borrow_date_s, "%d/%d/%d", b->BorrowDate.tm_mday, b->BorrowDate.tm_mon + 1, b->BorrowDate.tm_year + 1900);
			strcpy(b_fields[5], borrow_date_s);

			char return_date_s[STRING_MAX];
			sprintf(return_date_s, "%d/%d/%d", b->ReturnDate.tm_mday, b->ReturnDate.tm_mon + 1, b->ReturnDate.tm_year + 1900);
			strcpy(b_fields[6], return_date_s);
		} else {
			strcpy(b_fields[4], "-");
			strcpy(b_fields[5], "-");
			strcpy(b_fields[6], "-");
		}

		int b_field_widths[fields_l];
		for (int i = 0; i < fields_l; i++) {
			b_field_widths[i] = strlen(b_fields[i]);
		}

		for (int i = 0; i < fields_l; i++) {
			printf("| %s", b_fields[i]);
			repeat_char(' ', field_widths[i] - b_field_widths[i] + 1);
		}
		printf("|\n");

		table_seperator(field_widths, fields_l);
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

int is_duplicate_id_book(long long id) {
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
		if (is_duplicate_id_book(b->ID)) {
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
		if (b->Date > current_year || b->Date < 1) {
			printf("La date doit etre entre 1 et %d!\n> ", current_year);
			continue;
		}
		break;
	}

	b->Borrowed = 0;

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

struct student students[STUDENTS_MAX];
long long students_l = 0;

int is_duplicate_id_student(long long id) {
	for (int i = 0; i < students_l; i++) {
		if (id == students[i].ID) {
			return 1;
		}
	}
	return 0;
}

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
	if (is_duplicate_id_student(s->ID)) {
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
	if (s->Year > 5 || s->Year <= 0) {
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
		struct student *s = &students[i];
		if (numlen(s->ID) > id_w) {
			id_w = numlen(s->ID);
		}
		if (strlen(s->Fname) > fname_w)  {
			fname_w = strlen(s->Fname);
		}
		if (strlen(s->Lname) > lname_w)  {
			lname_w = strlen(s->Lname);
		}
	}

	char fields[][STRING_MAX] = { "ID", "Prenom", "Nom", "Niveau d'etudes" };
	int field_widths[] = { id_w, fname_w, lname_w, 15 };
	table_header(fields, field_widths, 4);

	for (int i = 0; i < students_l; i++) {
		struct student *s = &students[i];
		int s_id_w = numlen(s->ID);
		int s_fname_w = strlen(s->Fname);
		int s_lname_w = strlen(s->Lname);
		int s_year_w = numlen(s->Year);

		printf("| %lld", s->ID);
		repeat_char(' ', id_w - s_id_w + 1);
		printf("| %s", s->Fname);
		repeat_char(' ', fname_w - s_fname_w + 1);
		printf("| %s", s->Lname);
		repeat_char(' ', lname_w - s_lname_w + 1);
		switch (s->Year) {
			case 1: printf("| 1ere annee"); break;
			case 2: printf("| 2eme annee"); break;
			case 3: printf("| 3eme annee"); break;
			case 4: printf("| 4eme annee"); break;
			case 5: printf("| 5eme annee"); break;
		}
		repeat_char(' ', 6);
		printf("|\n");

		table_seperator(field_widths, 4);
	}
}

void borrow_book() {
	if (books_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}
	if (students_l == 0) {
		printf("Il y'a aucun etudiant.\n");
		return;
	}

	long long int book_id;
	char title[STRING_MAX];

	printf("Identifiant du livre\n> ");
	while (1) { 
		if (read_ll(&book_id)) { 
			printf("L'identifiant doit etre un nombre!\n> ");
			continue;
		}
		if (book_id < 0) {
			printf("L'identifiant doit etre un positif!\n> ");
			continue;
		}
		break;
	}

	printf("Titre\n> ");
	int title_l;
	while (!(title_l = read_str(title))) { 
		printf("Le titre doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	struct book *b = NULL;
	for (int i = 0; i < books_l; i++) {
		if (books[i].ID == book_id && !strncmp(title, books[i].Title, title_l)) {
			b = &books[i];
		}
	}

	if (!b) {
		printf("Aucun livre avec cet identifiant et titre existe!");
		return;
	}

	long long int student_id;
	char lname[STRING_MAX];

	printf("Identifiant d'etudiant\n> ");
	while (1) { 
		if (read_ll(&student_id)) { 
			printf("L'identifiant doit etre un nombre!\n> ");
			continue;
		}
		if (student_id < 0) {
			printf("L'identifiant doit etre un positif!\n> ");
			continue;
		}
		break;
	}

	printf("Nom\n> ");
	int lname_l;
	while (!(lname_l = read_str(lname))) { 
		printf("Le nom doit avoir entre 1 and %d caracteres!\n> ", STRING_MAX);
	}

	b->Borrower = NULL;
	for (int i = 0; i < students_l; i++) {
		if (students[i].ID == student_id && !strncmp(lname, students[i].Lname, lname_l)) {
			b->Borrower = &students[i];
		}
	}

	if (!(b->Borrower)) {
		printf("Aucun etudiant avec cet identifiant et nom existe!");
		return;
	}

	b->Borrowed++;

	printf("Date d'emprunt (Sous la forme JJ/MM/AAAA)\n> ");
	char date_s[STRING_MAX];
	int year, month, day;
	while (1) {
		if (!read_str(date_s) || sscanf(date_s, "%d/%d/%d/%d", &day, &month, &year, &year) != 3) {
			printf("La date d'emprunt doit etre ecrit sous la form JJ/MM/AAAA!\n> ");
			continue;
		}

		if (year < 1900) {
			printf("L'annne ne peut pas etre inferieur a 1900!\n> ");
			continue;
		}

		if (month < 1 || month > 12) {
			printf("Le mois doit etre entre 1 et 12!\n> ");
			continue;
		}


		if (day < 1 || day > 31) {
			printf("Le jour doit etre entre 1 et 31!\n> ");
			continue;
		}

		b->BorrowDate.tm_year = year - 1900;
		b->BorrowDate.tm_mon = month - 1;
		b->BorrowDate.tm_mday = day;

		time_t t = time(NULL);
		if (mktime(&b->BorrowDate) > mktime(localtime(&t))) {
			printf("La date d'emprunt ne peut pas etre dans le futur!\n> ");
			continue;
		}

		break;
	}

	printf("Date de retour (Sous la forme JJ/MM/AAAA)\n> ");
	while (1) {
		if (!read_str(date_s) || sscanf(date_s, "%d/%d/%d/%d", &day, &month, &year, &year) != 3) {
			printf("La date de retour doit etre ecrit sous la form JJ/MM/AAAA!\n> ");
			continue;
		}

		if (year < 1900) {
			printf("L'annne ne peut pas etre inferieur a 1900!\n> ");
			continue;
		}

		if (month < 1 || month > 12) {
			printf("Le mois doit etre entre 1 et 12!\n> ");
			continue;
		}


		if (day < 1 || day > 31) {
			printf("Le jour doit etre entre 1 et 31!\n> ");
			continue;
		}

		b->ReturnDate.tm_year = year - 1900;
		b->ReturnDate.tm_mon = month - 1;
		b->ReturnDate.tm_mday = day;

		time_t t = time(NULL);
		if (mktime(&b->ReturnDate) < mktime(&b->BorrowDate)) {
			printf("La date de retour ne peut pas etre avant la date d'emprunt!\n> ");
			continue;
		}

		break;
	}
}

void return_book() {
	if (books_l == 0) {
		printf("Il y'a aucun livre.\n");
		return;
	}

	int borrowed_books_l = 0;
	for (int i = 0; i < books_l; i++) {
		if (books[i].Borrowed) {
			borrowed_books_l++;
		}
	}
	if (!borrowed_books_l) {
		printf("Il y'a aucun livre emprunter!\n");
		return;
	}

	long long int id;
	printf("Identifiant du livre\n> ");
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
		if (id == books[i].ID) {
			if (!books[i].Borrowed) {
				printf("Ce livre n'est pas emprunter!\n");
				return;
			}
			books[i].Borrowed = 0;
			return;
		}
	}

	printf("Aucun livre avec cet identifiant existe!\n");
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
			"7: Emprunter un livre\n"
			"8: Retourner un livre\n"
			"9: effacer l'affichage\n"
			"> "
		);
		while (1) { 
			if (read_i(&input) || input < 0 || input > 9) { 
				printf("Votre choix doit etre un nombre entre 0 et 9!\n> ");
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
			case 7: borrow_book(); break;
			case 8: return_book(); break;
			case 9: system(CLEAR); continue;
		}
		printf("\n");
		repeat_char('-', 80);
		printf("\n\n\n");
	}
}
