#include <gtk/gtk.h>
#include "tclogo-app.h"

#include <stdlib.h>
#include <time.h>

int
main(int    argc,
     char **argv)
{
    srand(time(NULL));
    return g_application_run(G_APPLICATION(tclogo_app_new()), argc, argv);
}
