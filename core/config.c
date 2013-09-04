#include "config.h"

int load_config(config_t *config, char *config_file_name)
{
	config_init(config);

	if (!config_read_file(config, config_file_name))
	{
		config_destroy(config);
		return 1;
	}
	return 0;
}

int create_config(config_t *config, char *config_file_name)
{
	config_setting_t *config_prop, *prop_port, *prop_listen, *prop_timeout;

    config_init(config);
    
    if((config_prop=config_setting_add(config_root_setting(config), "SOCKET",CONFIG_TYPE_GROUP))==NULL)goto fail_exit;	
	if((prop_port=config_setting_add(config_prop, "PORT", CONFIG_TYPE_INT))==NULL) goto fail_exit;
	if((prop_listen=config_setting_add(config_prop, "LISTEN_QUEUE", CONFIG_TYPE_INT))==NULL) goto fail_exit;
	if((prop_timeout=config_setting_add(config_prop, "RCV_TIMEOUT", CONFIG_TYPE_INT))==NULL) goto fail_exit;
    if(!config_setting_set_int(prop_port, 3000))goto fail_exit;
    if(!config_setting_set_int(prop_listen, 20))goto fail_exit;
    if(!config_setting_set_int(prop_timeout, 1))goto fail_exit;

    if((config_prop=config_setting_add(config_root_setting(config), "WORKER",CONFIG_TYPE_GROUP))==NULL)goto fail_exit;	
	if((config_prop=config_setting_add(config_prop, "MAX_NUM_THREADS", CONFIG_TYPE_INT))==NULL) goto fail_exit;
    if(!config_setting_set_int(config_prop, 15))goto fail_exit;
    
    //Include default options from modules
    if((config_prop=config_setting_add(config_root_setting(config), "MODULES",CONFIG_TYPE_GROUP))==NULL)goto fail_exit;	
    //module_default_config(config_root_setting(config_prop));
    
    config_write_file(config, config_file_name); 
    
    return 0;
    
    fail_exit:
		config_destroy(config);
		return 1;
}
