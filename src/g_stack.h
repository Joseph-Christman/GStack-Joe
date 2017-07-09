#ifndef G_STACK_H
#define G_STACK_H

/* This is to trick the compilier into thinking
   this is part of Glib */
#define __GLIB_H_INSIDE__

#include <glib/gmessages.h>
#include <glib/garray.h>
#include <glib/gslice.h>
#include <glib/gatomic.h>
#include <glib/gmem.h>

typedef struct _GStack {
    GArray *pdata;
    gint ref_count;
} GStack;

typedef enum
{
    FREE_SEGMENT = 1 << 0,
    PRESERVE_WRAPPER = 1 << 1
} StackFreeFlags;


/* Not sure about this define
 * Check it later. */
#define g_stack_peek(s, t) (((t*) (void *) (s->pdata)->data) [((s->pdata)->len - 1)])
#define g_stack_pop(s, t) ((t*) (void *) (private_g_stack_pop(s)))

GStack* g_stack_new(guint element_size);

GStack* g_stack_sized_new(guint element_size,
                          guint reserved_size);

GStack* g_stack_push(GStack * fstack,
                     gconstpointer data);
                     

gpointer private_g_stack_pop(GStack * fstack);

GStack* g_stack_ref(GStack * stack);

void g_stack_unref (GStack * stack);

void g_stack_set_clear_func(GStack * stack,
                            GDestroyNotify clear_func);

gchar * g_stack_free (GStack *stack,
                      gboolean free_segment);

static gchar * stack_free(GStack *stack,
                         StackFreeFlags flag);

#endif
