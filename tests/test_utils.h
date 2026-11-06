#pragma once

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Petite infrastructure de tests, sans dépendance externe.
 *
 * Chaque fichier de test possède son propre main et inclut cet en-tête.
 * Les assertions n'interrompent pas le programme : les échecs sont comptés
 * puis résumés à la fin, ce qui permet de tester toutes les primitives même
 * lorsqu'une d'entre elles échoue.
 */

static int test_assertions = 0;
static int test_failures = 0;

#define TEST_ASSERT(condition)                                                 \
	do {                                                                      \
		test_assertions++;                                                   \
		if (!(condition)) {                                                  \
			test_failures++;                                                \
			fprintf(stderr, "  [ÉCHEC] %s:%d : %s\n", __FILE__, __LINE__,   \
			        #condition);                                            \
		}                                                                    \
	} while (0)

#define TEST_ASSERT_EQ_INT(expected, actual)                                   \
	do {                                                                      \
		long test_expected_ = (long)(expected);                              \
		long test_actual_ = (long)(actual);                                  \
		test_assertions++;                                                   \
		if (test_expected_ != test_actual_) {                                \
			test_failures++;                                                \
			fprintf(                                                        \
			    stderr, "  [ÉCHEC] %s:%d : attendu %ld, obtenu %ld (%s)\n", \
			    __FILE__, __LINE__, test_expected_, test_actual_, #actual); \
		}                                                                    \
	} while (0)

#define TEST_ASSERT_EQ_PTR(expected, actual)                                   \
	do {                                                                      \
		const void *test_expected_ = (const void *)(expected);               \
		const void *test_actual_ = (const void *)(actual);                   \
		test_assertions++;                                                   \
		if (test_expected_ != test_actual_) {                                \
			test_failures++;                                                \
			fprintf(                                                        \
			    stderr, "  [ÉCHEC] %s:%d : attendu %p, obtenu %p (%s)\n",   \
			    __FILE__, __LINE__, test_expected_, test_actual_, #actual); \
		}                                                                    \
	} while (0)

#define TEST_ASSERT_STR_EQ(expected, actual)                                   \
	do {                                                                      \
		const char *test_expected_ = (expected);                             \
		const char *test_actual_ = (actual);                                 \
		test_assertions++;                                                   \
		if (test_actual_ == NULL ||                                          \
		    strcmp(test_expected_, test_actual_) != 0) {                     \
			test_failures++;                                                \
			fprintf(stderr,                                                 \
			        "  [ÉCHEC] %s:%d : chaîne attendue \"%s\", obtenue "    \
			        "\"%s\"\n",                                             \
			        __FILE__, __LINE__, test_expected_,                     \
			        test_actual_ != NULL ? test_actual_ : "(null)");        \
		}                                                                    \
	} while (0)

/* Alloue un entier sur le tas (possession transférée à la structure testée). */
static inline int *test_new_int(int value) {
	int *integer = malloc(sizeof(*integer));
	if (integer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	*integer = value;
	return integer;
}

/*
 * Capture de stdout.
 *
 * Utilisation :
 *   test_capture_t capture = test_capture_begin();
 *   vue_a_tester(...);
 *   char *sortie = test_capture_end(&capture);
 *   ... vérifications sur sortie ...
 *   free(sortie);
 */
typedef struct test_capture_t {
	int saved_stdout_fd;
	FILE *temporary_file;
} test_capture_t;

static inline test_capture_t test_capture_begin(void) {
	test_capture_t capture = {.saved_stdout_fd = -1, .temporary_file = NULL};

	fflush(stdout);

	capture.saved_stdout_fd = dup(fileno(stdout));
	if (capture.saved_stdout_fd < 0) {
		perror("dup");
		exit(EXIT_FAILURE);
	}

	capture.temporary_file = tmpfile();
	if (capture.temporary_file == NULL) {
		perror("tmpfile");
		exit(EXIT_FAILURE);
	}

	if (dup2(fileno(capture.temporary_file), fileno(stdout)) < 0) {
		perror("dup2");
		exit(EXIT_FAILURE);
	}

	return capture;
}

static inline char *test_capture_end(test_capture_t *capture) {
	if (fflush(stdout) != 0) {
		perror("fflush");
		exit(EXIT_FAILURE);
	}

	if (dup2(capture->saved_stdout_fd, fileno(stdout)) < 0) {
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(capture->saved_stdout_fd);

	if (fseek(capture->temporary_file, 0, SEEK_END) != 0) {
		perror("fseek");
		exit(EXIT_FAILURE);
	}

	long size = ftell(capture->temporary_file);
	if (size < 0) {
		perror("ftell");
		exit(EXIT_FAILURE);
	}
	rewind(capture->temporary_file);

	char *buffer = malloc((size_t)size + 1);
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	size_t read_count =
	    fread(buffer, 1, (size_t)size, capture->temporary_file);
	buffer[read_count] = '\0';

	fclose(capture->temporary_file);
	capture->temporary_file = NULL;

	return buffer;
}

static inline int test_summary(const char *suite_name) {
	if (test_failures == 0) {
		printf("%s : OK (%d assertions)\n", suite_name, test_assertions);
		return EXIT_SUCCESS;
	}

	fprintf(stderr, "%s : ÉCHEC (%d échec(s) sur %d assertions)\n", suite_name,
	        test_failures, test_assertions);
	return EXIT_FAILURE;
}
