#ifndef __HANDLERS_H__
#define __HANDLERS_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_LIFERAY  (nautilus_liferay_get_type ())
#define NAUTILUS_LIFERAY(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_LIFERAY, NautilusLiferay))
#define NAUTILUS_IS_LIFERAY(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_LIFERAY))

typedef struct _NautilusLiferay      NautilusLiferay;
typedef struct _NautilusLiferayClass NautilusLiferayClass;

struct _NautilusLiferay {
	GObject __parent;
};

struct _NautilusLiferayClass {
	GObjectClass __parent;
};

GType nautilus_liferay_get_type      (void);
void  registerHandlers(GTypeModule *module);

G_END_DECLS

#endif
