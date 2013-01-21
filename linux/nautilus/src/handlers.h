#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_FR  (nautilus_fr_get_type ())
#define NAUTILUS_FR(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_FR, NautilusFr))
#define NAUTILUS_IS_FR(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_FR))

typedef struct _NautilusFr      NautilusFr;
typedef struct _NautilusFrClass NautilusFrClass;

struct _NautilusFr {
	GObject __parent;
};

struct _NautilusFrClass {
	GObjectClass __parent;
};

GType nautilus_fr_get_type      (void);
void  registerHandlers(GTypeModule *module);

G_END_DECLS

#endif
