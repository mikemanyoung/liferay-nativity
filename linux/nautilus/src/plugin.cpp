#include "config.h"
#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-column-provider.h>
#include <glib/gi18n-lib.h>
#include "handlers.h"
#include "logger.h"
#include "requests.h"

extern "C" void nautilus_module_initialize (GTypeModule*module)
{
	RequestManager::instance();
	
	writeLog("nautilus_module_initialize\n");
	registerHandlers(module);
}


extern "C" void nautilus_module_shutdown (void)
{
	writeLog("nautilus_module_shutdown\n");
}

extern "C" void nautilus_module_list_types (const GType** types, int* num_types)
{
	writeLog("nautilus_module_list_types\n");

	static GType type_list[1];

	type_list[0] = NAUTILUS_TYPE_LIFERAY;
	*types = type_list;
	*num_types = 1;
}

