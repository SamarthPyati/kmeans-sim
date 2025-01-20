#define NOB_IMPLEMENTATION
#include "./nob.h"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "cc");     // compiler 
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-std=c17", "-g"); // flags 
    nob_cmd_append(&cmd, "-I/opt/homebrew/Cellar/raylib/5.5/include", 
        "-L/opt/homebrew/Cellar/raylib/5.5/lib", "-lraylib"); // libraries
    nob_cmd_append(&cmd, "-o", "kmeans"); // output 
    nob_cmd_append(&cmd, "main.c"); // input 
    if (!nob_cmd_run_sync(cmd)) return 1;
    return 0;
}