#include <gtk/gtk.h>
#include "tclogo-app.h"

int
main(int    argc,
	 char **argv)
{	
	return g_application_run(G_APPLICATION(tclogo_app_new()), argc, argv);
}
