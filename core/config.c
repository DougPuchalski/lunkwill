#include "lunkwill.h"

/** \brief Starting libconfig to parse config*/
int load_config(char *config_file_name)
{
    config_init(&session.config);

    if (!config_read_file(&session.config, config_file_name))
    {
        config_destroy(&session.config);
        return 1;
    }
    return 0;
}
