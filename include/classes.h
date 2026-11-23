#include "aliases.h"
#include "heap.h"

struct Heap {
	heap_t *(*new)(int type);
	void *(*get)(const heap_t *H);
	dyn_table_t *(*get_dict)(const heap_t *H);
	// toutes les primitives
};

typedef struct Heap Heap;

Heap new_heap_class(void) {
	Heap instance;
	instance.new = new_heap;
	instance.get = get_heap;
	instance.get_dict = get_heap_dictionary;

	return instance;
}

static Heap HEAP;
