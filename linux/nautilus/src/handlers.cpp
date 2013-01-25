#include "config.h"
#include <string.h>
#include <glib/gi18n-lib.h>
#include <gio/gio.h>
#include <libnautilus-extension/nautilus-extension-types.h>
#include <libnautilus-extension/nautilus-file-info.h>
#include <libnautilus-extension/nautilus-menu-provider.h>
#include "handlers.h"
#include "logger.h"
#include "requests.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

static GObjectClass *parent_class;

extern "C" void commandExecuted (NautilusMenuItem *item,
		       gpointer          user_data)
{
	std::string cmdId("menuExec:");
	cmdId += boost::lexical_cast<std::string>((int)user_data);

	RequestManager::instance().menuExecuted(cmdId); 

/*	GList            *files, *scan;
	NautilusFileInfo *file;
	char             *dir;
	GString          *cmd;

	files = (GList*) g_object_get_data (G_OBJECT (item), "files");
	file = (NautilusFileInfo*) files->data;

	for (scan = files; scan; scan = scan->next)
        {
		NautilusFileInfo* file = (NautilusFileInfo*)scan->data;
		char             *uri;

		uri = nautilus_file_info_get_uri (file);
		g_string_append_printf (cmd, " %s", g_shell_quote (uri));
		g_free (uri);
	}

	g_string_free (cmd, TRUE);*/
}


extern "C" GList* nautilus_fr_get_file_items (NautilusMenuProvider* provider, GtkWidget* window, GList* files)
{
	GList    *items = NULL;
	GList    *scan;

	if (files == NULL)
		return NULL;

	std::string cmd("menuQuery");
	for (scan = files; scan; scan = scan->next) {
		NautilusFileInfo* file = (NautilusFileInfo*)scan->data;
		char             *uri;

		uri = nautilus_file_info_get_uri (file);
		cmd += ":";
		cmd += g_filename_from_uri(uri,NULL,NULL);
		g_free (uri);
	}

	std::string answer(RequestManager::instance().queryMenuItems(cmd));
	//answer = "item1:item2,false:item3,true";
	if (answer.empty())
		return NULL;

	std::vector<std::string> itemsArray;
	boost::split(itemsArray, answer, boost::is_any_of(":"));

	if (itemsArray.empty())
		return NULL;

	writeLog("Items count: %d\n", itemsArray.size());

	NautilusMenuItem *item;
	item = nautilus_menu_item_new ("LiferayMenu",_("Liferay"),_(""),"drive-harddisk");

	items = g_list_append(items, item);

	NautilusMenu* menu = nautilus_menu_new();
	nautilus_menu_item_set_submenu(item, menu);

	for (int i=0;i<itemsArray.size();++i)
	{
		std::string itemTitle(itemsArray[i]);
		if (itemTitle == "_SEPARATOR_")
			continue;

		bool enabled(true);

		int pos = itemTitle.find(",");
		if (pos != itemTitle.npos)
		{
			enabled = itemTitle.substr(pos + 1,itemTitle.npos) == "true";
			itemTitle.erase(pos,itemTitle.npos);
		}

		NautilusMenuItem *childItem = nautilus_menu_item_new (itemTitle.c_str(),itemTitle.c_str(),_(""),"drive-harddisk");

		if (!enabled)
		{
			GValue sensitive = G_VALUE_INIT;
		    	g_value_init (&sensitive, G_TYPE_BOOLEAN);
			g_value_set_boolean (&sensitive, FALSE);
		  
		  	g_object_set_property (G_OBJECT(childItem), "sensitive", &sensitive);
		}
		else
		{
			g_signal_connect(childItem, "activate", G_CALLBACK(commandExecuted), (gpointer)i);
			//g_object_set_data_full(G_OBJECT(childItem), "files", nautilus_file_info_list_copy(files), (GDestroyNotify) 				nautilus_file_info_list_free);
		}

		nautilus_menu_append_item(menu, childItem);
	}


	return items;
}


extern "C" void
nautilus_fr_menu_provider_iface_init (NautilusMenuProviderIface *iface)
{
	iface->get_file_items = nautilus_fr_get_file_items;
}


extern "C" void
nautilus_fr_instance_init (NautilusFr *fr)
{
}


extern "C" void
nautilus_fr_class_init (NautilusFrClass *clazz)
{
	parent_class = (GObjectClass*) g_type_class_peek_parent (clazz);
}


static GType fr_type = 0;


extern "C" GType 
nautilus_fr_get_type (void)
{
	return fr_type;
}


extern "C" void registerHandlers(GTypeModule *module)
{
	writeLog("registerHandlers entered\n");

	static const GTypeInfo info = {
		sizeof (NautilusFrClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) nautilus_fr_class_init,
		NULL,
		NULL,
		sizeof (NautilusFr),
		0,
		(GInstanceInitFunc) nautilus_fr_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) nautilus_fr_menu_provider_iface_init,
		NULL,
		NULL
	};

	fr_type = g_type_module_register_type (module, G_TYPE_OBJECT,"LiferayPlugin", &info,(GTypeFlags) 0);

	writeLog("g_type_module_register_type returned %d\n", fr_type);

	g_type_module_add_interface (module,
				     fr_type,
				     NAUTILUS_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}

