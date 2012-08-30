#include <demo/game.h>
#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
    Demo::Game g;
    g_type_init();
    return g.Main(argc, argv);
}
