
#include <ply-boot-splash-plugin.h>

#include <stdlib.h>

/*
    Plugin structure
*/
struct _ply_boot_splash_plugin 
{
    ply_pixel_display_t* display;
};

static void on_draw (void* user_data,
    ply_pixel_buffer_t *pixel_buffer,
    int x,
    int y,
    int width,
    int height,
    ply_pixel_display_t *pixel_display)
{

}

/*
    Plugin hooks
*/

static ply_boot_splash_plugin_t*
create_plugin (ply_key_file_t *key_file)
{
    ply_boot_splash_plugin_t* plugin;
    
    plugin = calloc(1,sizeof(ply_boot_splash_plugin_t));
    
    return plugin;
}

static void
destroy_plugin (ply_boot_splash_plugin_t* plugin)
{
}

static void
set_keyboard (ply_boot_splash_plugin_t* plugin,
              ply_keyboard_t* keyboard)
{
}

static void
unset_keyboard (ply_boot_splash_plugin_t* plugin,
                ply_keyboard_t* keyboard)
{
}

static void
add_pixel_display (ply_boot_splash_plugin_t* plugin,
                   ply_pixel_display_t* display)
{
    if (plugin->display==NULL) {
        plugin->display=display;
        
        ply_pixel_display_set_draw_handler (display,
                                            (ply_pixel_display_draw_handler_t)on_draw,
                                            NULL);
    }
}

static void
remove_pixel_display (ply_boot_splash_plugin_t *plugin,
                      ply_pixel_display_t      *display)
{
}

static bool
show_splash_screen (ply_boot_splash_plugin_t *plugin,
                    ply_event_loop_t         *loop,
                    ply_buffer_t             *boot_buffer,
                    ply_boot_splash_mode_t    mode)
{
}

static void
update_status (ply_boot_splash_plugin_t *plugin,
               const char               *status)
{
}

static void
on_boot_progress (ply_boot_splash_plugin_t *plugin,
                  double                    duration,
                  double                    percent_done)
{
}

static void
hide_splash_screen (ply_boot_splash_plugin_t *plugin,
                    ply_event_loop_t         *loop)
{
}

static void
on_root_mounted (ply_boot_splash_plugin_t *plugin)
{
}

static void
become_idle (ply_boot_splash_plugin_t *plugin,
             ply_trigger_t            *idle_trigger)
{
}

static void
display_normal (ply_boot_splash_plugin_t *plugin)
{
}

static void
display_password (ply_boot_splash_plugin_t *plugin,
                  const char               *prompt,
                  int                       bullets)
{
}

static void
display_question (ply_boot_splash_plugin_t *plugin,
                  const char               *prompt,
                  const char               *entry_text)
{
}

static void
display_message (ply_boot_splash_plugin_t *plugin,
                 const char               *message)
{
}

static void
hide_message (ply_boot_splash_plugin_t *plugin,
              const char               *message)
{
}

ply_boot_splash_plugin_interface_t *
ply_boot_splash_plugin_get_interface (void)
{
        static ply_boot_splash_plugin_interface_t plugin_interface =
        {
                .create_plugin        = create_plugin,
                .destroy_plugin       = destroy_plugin,
                .set_keyboard         = set_keyboard,
                .unset_keyboard       = unset_keyboard,
                .add_pixel_display    = add_pixel_display,
                .remove_pixel_display = remove_pixel_display,
                .show_splash_screen   = show_splash_screen,
                .update_status        = update_status,
                .on_boot_progress     = on_boot_progress,
                .hide_splash_screen   = hide_splash_screen,
                .on_root_mounted      = on_root_mounted,
                .become_idle          = become_idle,
                .display_normal       = display_normal,
                .display_password     = display_password,
                .display_question     = display_question,
                .display_message      = display_message,
                .hide_message         = hide_message,
        };

        return &plugin_interface;
}