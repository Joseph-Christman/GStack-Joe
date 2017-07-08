#include "g_stack.h"

/**
 * g_stack_new:
 * @element_size: The size of each element in bytes
 *
 * Creates a new stack with a reference count of 1.
 *
 * Returns: the new stack
 */
GStack * g_stack_new(guint element_size) 
{
    GStack *the_stack;

    the_stack = g_slice_new(GStack);

    the_stack->ref_count = 1; 
    the_stack->pdata = g_array_new(FALSE, 
                                   TRUE,
                                   element_size);

    return the_stack;
}

/**
 * g_stack_sized_new:
 * @element_size: size of each element in the stack
 * @reserved_size: number of elements preallocated.
 *
 * Creates a new stack with @reserved_size elements preallocated 
 * and has a reference count of 1 
 *
 * Returns: The new stack
 */
GStack * g_stack_sized_new(guint element_size,
                           guint reserved_size)
{
    GStack *the_stack;

    the_stack = g_slice_new(GStack);

    the_stack->ref_count = 1;
    the_stack->pdata = g_array_sized_new(FALSE,
                                         TRUE,
                                         element_size,
                                         reserved_size);

    return the_stack;
}

/**
 * g_stack_push:
 * @fstack: a Gstack
 * @data: A pointer to the data to be pushed onto the stack
 *
 * Pushes @data onto the end of @fstack
 *
 * Returns: The updated stack.
 */
GStack* g_stack_push(GStack* fstack,
                     gconstpointer data)
{
    g_array_append_val(fstack->pdata, data);

    return fstack; 
}

/* This function is not meant to be called directly */
gpointer private_g_stack_pop(GStack* fstack) 
{
    if(fstack->pdata->len == 0) {
        return NULL;
    }

    gpointer * temp = g_stack_peek(fstack, gpointer *);
    gpointer ret_value = temp;

    g_array_remove_index_fast(fstack->pdata, fstack->pdata->len - 1);
    return ret_value;
}

/**
 * g_stack_ref:
 * @stack: a GStack 
 *
 * Atomically increments the reference count 
 *
 * Returns: The updated stack.
 */
GStack * g_stack_ref(GStack * stack) 
{
    g_return_val_if_fail(stack, NULL);

    g_atomic_int_inc(&stack->ref_count);

    return stack;
}

/**
 * g_stack_unref:
 * @stack: A Gstack
 *
 * Atomically decrements the reference count of @stack 
 *
 * Returns: The updated stack.
 */
void g_stack_unref(GStack * stack) 
{
    g_return_if_fail(stack);

    if(g_atomic_int_dec_and_test(&stack->ref_count)) {
        stack_free(stack, FREE_SEGMENT);
    }
}
/**
 * g_stack_set_clear_func
 * @stack: A Gstack
 * @clear_func: a function to clear an element of @stack
 *
 * Sets a function to clear an element of @stack.
 */
void g_stack_set_clear_func(GStack * stack,
                            GDestroyNotify clear_func)
{
    g_array_set_clear_func(stack->pdata, clear_func);
}

/**
 * g_array_free:
 * @stack: A Gstack
 * @free_segment: if true, the actual element data will be freed
 * as well.
 *
 * Frees the memory allocated for @stack. If @free_segment is true,
 * it frees the memory block holding the elements as well.
 *
 * Returns: The element data if @free_segment is false, otherwise NULL. 
 */
gchar * g_stack_free(GStack *stack,
                     gboolean free_segment)
{
    StackFreeFlags flags;

    g_return_val_if_fail(stack, NULL);

    flags = (free_segment ? FREE_SEGMENT : 0);

    if(!g_atomic_int_dec_and_test(&stack->ref_count)) {
        flags |= PRESERVE_WRAPPER;
    }

    return stack_free(stack, flags);
}

/* This function is not meant to be called directly. */
static gchar * stack_free(GStack *stack,
                          StackFreeFlags flags)
{
    gchar *segment;
    g_array_free(stack->pdata, flags);

    if(flags & FREE_SEGMENT) {
        g_free(stack->pdata);
        segment = NULL;
    }
    else {
        segment = (gchar*) stack->pdata->data;
    }

    if(flags & PRESERVE_WRAPPER) {
        stack->pdata->data = NULL;
    }
    else {
        g_slice_free1(sizeof(GStack), stack);
    }

    return segment;
}
