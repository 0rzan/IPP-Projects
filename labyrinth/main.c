#include "vector.h"
#include "bitset.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

size_t k = 0;
size_t maze_size = 1;
size_t line_number = 1;
const size_t modulus = (size_t)UINT32_MAX + 1;
bool noway = false;
vector n, x, y;
bitset walls;

void read_error();
void input_free();
void coordinates_free();

void read(vector *v) {
	char c;
	c = getchar();

	if(c == EOF) {
		read_error();
	}

	if (!isspace(c) && ((c - '0') < 0 || (c - '0') > 9)) {
		read_error();
	}

	while (c != '\n') {
		if (isspace(c)) {
			c = getchar();
			continue;
		}

		size_t n = 0;

		while (!isspace(c)) {
			if ((c - '0') < 0 || (c - '0') > 9) {
				read_error();
			}

			else if (line_number < 4 && (n > (SIZE_MAX - (c - '0'))/10)) {
				read_error();
			}

			else if (line_number == 4 && (n > (UINT_MAX - (c - '0'))/10)) {
				read_error();
			}

			n = 10 * n + (c - '0');
			c = getchar();
		}
		vector_add(v, n);

		k++;
	}

	line_number++;
}

void walls_positions(bitset *walls) {
	char c;
	c = getchar();

	while (isspace(c)) {
		c = getchar();
	}

	if (c == 'R') {
		size_t a, b, m, r, s_0, s_prev, s_next, w;
		vector generator;
		vector_init(&generator);
		read(&generator);

		if (generator.length != 5) {
			vector_free(&generator);
			input_free();
			fprintf(stderr, "ERROR 4\n");
			exit(1);
		}

		a = generator.items[0];
		b = generator.items[1];
		m = generator.items[2];
		r = generator.items[3];
		s_0 = generator.items[4];

		if (a > UINT32_MAX || b > UINT32_MAX || m > UINT32_MAX || r > UINT32_MAX || s_0 > UINT32_MAX) {
			vector_free(&generator);
			input_free();
			fprintf(stderr, "ERROR 4\n");
			exit(1);
		}

		s_prev = s_0;

		for (size_t i = 1; i <= r; i++) {
			s_next = (a * s_prev + b) % m;
			w = s_next % maze_size;

			while (w <= maze_size) {
				bitset_change(walls, w, true);
				w += modulus;
			}

			s_prev = s_next;
		}

		vector_free(&generator);
	}

	else if (c == '0'){
		c = getchar();
		if (c != 'x') {
			input_free();
			fprintf(stderr, "ERROR 4\n");
			exit(1);
		}

		vector hex;
		vector_init(&hex);
		c = getchar();

		if (isspace(c)) {
			vector_free(&hex);
			input_free();
			fprintf(stderr, "ERROR 4\n");
			exit(1);
		}
		
		while (c == '0') {
			c = getchar();
		}
		
		vector_add(&hex, '0');

		bool space_in_middle = false;
		bool error_space = false;

		while (c != '\n') {
			if (isspace(c)) {
				space_in_middle = true;
				c = getchar();
			}

			else {
				if (space_in_middle) {
					error_space = true;
				}

				vector_add(&hex, c);
				c = getchar();
			}
		}

		if (error_space) {
			vector_free(&hex);
			input_free();
			fprintf(stderr, "ERROR 4\n");
			exit(1);
		}

		size_t iterator_bitset = 0;

		for (size_t i = hex.length - 1; i >= 0; i--) {
			if (hex.items[i] >= '0' && hex.items[i] <= '9') {
				size_t hex_bit = hex.items[i] - '0';

				for (size_t j = 0; j < 4; j++) {
					if (hex_bit % 2 == 1) {
						if (iterator_bitset >= maze_size) {
							vector_free(&hex);
							input_free();
							fprintf(stderr, "ERROR 4\n");
							exit(1);
						}

						bitset_change(walls, iterator_bitset, true);
					}

					iterator_bitset++;
					hex_bit /= 2;
				}
			}

			else if (hex.items[i] >= 'a' && hex.items[i] <= 'f') {
				size_t hex_bit = hex.items[i] - 'a' + 10;

				for (size_t j = 0; j < 4; j++) {
					if (hex_bit % 2 == 1) {
						if (iterator_bitset >= maze_size) {
							vector_free(&hex);
							input_free();
							fprintf(stderr, "ERROR 4\n");
							exit(1);
						}

						bitset_change(walls, iterator_bitset, true);
					}

					iterator_bitset++;
					hex_bit /= 2;
				}
			}

			else if (hex.items[i] >= 'A' && hex.items[i] <= 'F') {
				size_t hex_bit = hex.items[i] - 'A' + 10;

				for (size_t j = 0; j < 4; j++) {
					if (hex_bit % 2 == 1) {
						if (iterator_bitset >= maze_size) {
							vector_free(&hex);
							input_free();
							fprintf(stderr, "ERROR 4\n");
							exit(1);
						}

						bitset_change(walls, iterator_bitset, true);
					}

					iterator_bitset++;
					hex_bit /= 2;
				}
			}

			else {
				vector_free(&hex);
				input_free();
				fprintf(stderr, "ERROR 4\n");
				exit(1);
			}
			
			if (i == 0) 
				break;
		}

		vector_free(&hex);
	}

	else {
		input_free();
		fprintf(stderr, "ERROR 4\n");
		exit(1);	
	}
}

vector convert_to_coordinates(size_t v) {
	vector coordinates;
	vector_init(&coordinates);

	for(size_t i = 0; i < n.length; i++) {
		if (i == n.length - 1) {
			vector_add(&coordinates, (size_t)(v + 1));
		}

		else {
			vector_add(&coordinates, (size_t)((v % n.items[i]) + 1));
			v -= (v % n.items[i]);
			v /= n.items[i];
		}
	}

	return coordinates;
}

size_t convert_to_number(vector *coordinates) {
	size_t v = 0;
	size_t mod = 1;

	for(size_t i = 0; i < n.length; i++) {
		v += (coordinates->items[i] - 1) * mod;
		mod *= n.items[i];
	}

	return v;
}

void get_neighbours(size_t v, vector *neighbours) {
	vector coordinates = convert_to_coordinates(v);
	size_t mod = 1;

	for (size_t i = 0; i < n.length; i++) {
		if (coordinates.items[i] != 1) {
			if (!bitset_check(&walls, (size_t)(v - mod))) {
				vector_add(neighbours, (size_t)(v - mod));
				bitset_change(&walls, (size_t)(v - mod), true);
			}
		}

		if (coordinates.items[i] != n.items[i]) {
			if (!bitset_check(&walls, (size_t)(v + mod))) {
				vector_add(neighbours, (size_t)(v + mod));
				bitset_change(&walls, (size_t)(v + mod), true);
			}
		}

		mod *= n.items[i];
	}

	vector_free(&coordinates);
}

size_t bfs() {
	vector s, s_neighbours;
	vector_init(&s);
	vector_init(&s_neighbours);

	vector_add(&s, convert_to_number(&x));
	bitset_change(&walls, convert_to_number(&x), true);

	size_t distance = 0;

	while (!vector_is_empty(&s) || !vector_is_empty(&s_neighbours)) {
		if (vector_is_empty(&s)) {
			vector_free(&s);
			vector_init(&s);

			for (size_t i = 0; i < s_neighbours.length; i++) {
				vector_add(&s, s_neighbours.items[i]);
			}

			vector_free(&s_neighbours);
			vector_init(&s_neighbours);
			distance++;
		}

		else {
			size_t v = vector_pop(&s);

			if (v == convert_to_number(&y)) {
				vector_free(&s);
				vector_free(&s_neighbours);
				return distance;
			}

			get_neighbours(v, &s_neighbours);
		}
	}

	vector_free(&s);
	vector_free(&s_neighbours);
	noway = true;
	
	return (0);
}

void input_free() {
	vector_free(&n);
	vector_free(&x);
	vector_free(&y);
	bitset_free(&walls);
}

void coordinates_free() {
	vector_free(&n);
	vector_free(&x);
	vector_free(&y);
}

void read_error() {
	if (line_number == 4) {
		input_free();
		fprintf(stderr, "ERROR %zu\n", line_number);
		exit(1);
	}

	else {
		coordinates_free();
		fprintf(stderr, "ERROR %zu\n", line_number);
		exit(1);
	}
}

int main() {
	vector_init(&n);
	vector_init(&x);
	vector_init(&y);
	read(&n);

	if (n.length == 0) {
		coordinates_free();
		fprintf(stderr, "ERROR 1\n");
		exit(1);
	}

	read(&x);

	if (x.length != n.length) {
		coordinates_free();
		fprintf(stderr, "ERROR 2\n");
		exit(1);
	}

	read(&y);

	if (y.length != n.length) {
		coordinates_free();
		fprintf(stderr, "ERROR 3\n");
		exit(1);
	}

	for (size_t i = 0; i < n.length; i++) {
		if (n.items[i] > SIZE_MAX / maze_size) {
			coordinates_free();
			fprintf(stderr, "ERROR 0\n");
			exit(1);
		}
		maze_size *= n.items[i];
	}

	if (maze_size == 0) {
		coordinates_free();
		fprintf(stderr, "ERROR 1\n");
		exit(1);
	}

	bitset_init(&walls, (size_t)(maze_size));
	walls_positions(&walls);

	for (size_t i = 0; i < x.length; i++) {
		if (x.items[i] > n.items[i] || x.items[i] < 1) {
			input_free();
			fprintf(stderr, "ERROR 2\n");
			exit(1);
		}
	}

	for (size_t i = 0; i < y.length; i++) {
		if (y.items[i] > n.items[i] || y.items[i] < 1) {
			input_free();
			fprintf(stderr, "ERROR 3\n");
			exit(1);
		}
	}

	if (bitset_check(&walls, convert_to_number(&x))) {
		input_free();
		fprintf(stderr, "ERROR 2\n");
		exit(1);
	}

	if (getchar() != EOF) {
		input_free();
		fprintf(stderr, "ERROR 5\n");
		exit(1);
	}

	size_t ans = bfs();

	if (!noway) {
		input_free();
		printf("%zu\n", ans);
	}
	else {
		input_free();
		printf("NO WAY\n");
	}

	return (0);
}
