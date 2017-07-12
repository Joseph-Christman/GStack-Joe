This is a wrapper around the glib data structure gArray to provide a stack
style interface. 

This is my first time trying something like this. I've only done a little
testing, so this probably shouldn't be used in production. I was mostly
trying to sharpen my C skills, and increase my knowledge of the glib library.

Functions 

==

GStack * g_stack_new(guint element_size) - This function returns a pointer
to a new stack with a reference count of 1 with elements of element_size.

GStack * g_stack_sized_new(guint element_size, guint reserved_size) - This
function returns a pointer  to a new stack with reserved_size elements of
element_size.

GStack * g_stack_push(GStack * fstack, gconstpointer data) - This function
pushes the pointer data onto the stack, and then returns a pointer to
the updated stack.

\#define g_stack_pop(GStack * stack, Type) - This macro returns the 
element on top of the stack casted to Type, and removes it from the stack.

\#define g_stack_peek(GStack * stack, Type) - This macro returns the element
on top of the stack casted to Type.

GStack * g_stack_ref(GStack * stack) - This function atomically increments
the reference count and returns a pointer to the updated stack.

void g_stack_unref(GStack * stack) - This function atomically decrements
the reference count. The stack is deleted if there are no more references.

void g_stack_set_clear_func(GStack * stack, GDestroyNotify) - Sets a 
function to clear elements of the stack.

gchar * g_stack_free(GStack *stack, gboolean free_segment) - This function
frees the memory allocated to the stack.

