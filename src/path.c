#include "libraries.h"

str ROOT_PATH;
str PREV_PATH;

str long_path(str short_path)
{
    if (!prefix("~", short_path))
    {
        return short_path;
    }

    str longpath = calloc(strlen(short_path) + strlen(ROOT_PATH) + 3, sizeof(char));
    assert(longpath != NULL);

    strcat(longpath, ROOT_PATH);
    strcat(longpath, "/");
    strcat(longpath, short_path + 1);

    // free(short_path);
    short_path = longpath;

    return longpath;
}

str short_path(str long_path)
{
    if (!prefix(ROOT_PATH, long_path))
    {
        return long_path;
    }

    str shortpath = calloc(PATHLENGTH_MAX, sizeof(char));
    assert(shortpath != NULL);
    strcat(shortpath, "~");
    strcat(shortpath, long_path + strlen(ROOT_PATH));

    // free(long_path);
    long_path = shortpath;

    return shortpath;
}

str display_path(bool full)
{
    if (strlen(ROOT_PATH) == 0)
        set_root_path();

    str path = calloc(PATHLENGTH_MAX, sizeof(char));
    assert(path != NULL);

    if (!getcwd(path, PATHLENGTH_MAX))
        errors(false, false, "Couldn't get the Working Directory");

    if (full || !prefix(ROOT_PATH, path))
        return path;

    return short_path(path);
}

void set_root_path()
{
    str path = calloc(PATHLENGTH_MAX, sizeof(char));

    if (!getcwd(path, PATHLENGTH_MAX))
        errors(false, false, "Couldn't get the Working Directory");

    if (ROOT_PATH)
        free(ROOT_PATH);

    ROOT_PATH = path;
}

void set_prev_path(str prev_path)
{
    PREV_PATH = prev_path;
}

str get_prev_path()
{
    return PREV_PATH;
}