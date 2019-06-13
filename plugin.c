
#define PLY_ENABLE_TRACING

#include <ply-boot-splash-plugin.h>
#include <ply-logger.h>
#include <ply-image.h>
#include <ply-label.h>

#include <systemd/sd-journal.h>

#include <stdlib.h>
#include <stdio.h>

void console(char* msg)
{
    sd_journal_print(LOG_DEBUG,msg);
}

/*
    Plugin structure
*/
struct _ply_boot_splash_plugin 
{
    ply_pixel_display_t* display;
    ply_event_loop_t* loop;
    bool quit_request;
    
    ply_image_t* logo;
    
    int px;
    int dx;
    
    double percent_done;
};

static void on_draw (void* user_data,
    ply_pixel_buffer_t *pixel_buffer,
    int x,
    int y,
    int width,
    int height,
    ply_pixel_display_t *pixel_display)
{
    ply_boot_splash_plugin_t* plugin = (ply_boot_splash_plugin_t*)user_data;
    
    ply_rectangle_t rect;
    
    rect.x=x;
    rect.y=y;
    rect.width=width;
    rect.height=height;
    
    
    
    ply_pixel_buffer_fill_with_hex_color(pixel_buffer,&rect,0xeff0f1ff);
    
    //logo
    
    int lx,ly;
    int lw,lh;
    
    lw = ply_image_get_width(plugin->logo);
    lh = ply_image_get_height(plugin->logo);
    
    lx = (width/2) - (lw/2);
    ly = (height/2) - (lh/2);
    
    ply_pixel_buffer_t* lpx = ply_image_get_buffer(plugin->logo);
    
    ply_pixel_buffer_fill_with_buffer(pixel_buffer,lpx,lx,ly);
    
    //progress bar
    
    if (plugin->px>width-32) {
        plugin->dx=-1;
    }
    if (plugin->px<=0) {
        plugin->dx=1;
    }

    plugin->px+=(plugin->dx*8);
    
    uint32_t* data = ply_pixel_buffer_get_argb32_data(pixel_buffer);
    
    int pw = plugin->percent_done*width;
    
    if (pw>=width) {
        pw=width-1;
    }
    
    for (int j=0;j<4;j++) {
        for (int i=0;i<pw;i++) {
            int px = i;
            int py = height - 160 - j;
            
            data[px+py*width] = 0xff3daee9;
        }
    }
}

static void
detach_from_event_loop (ply_boot_splash_plugin_t *plugin)
{
    console("detach from event loop\n");
    plugin->quit_request=true;
}

static void
on_timeout (ply_boot_splash_plugin_t *plugin)
{
    if (plugin->quit_request==false) {

        ply_pixel_display_draw_area(plugin->display,
            0,0,
            ply_pixel_display_get_width(plugin->display),
            ply_pixel_display_get_height(plugin->display));
        
        ply_event_loop_watch_for_timeout (plugin->loop,
                                              0.016,
                                              (ply_event_loop_timeout_handler_t)
                                              on_timeout, plugin);
    }
}

/*
    Plugin hooks
*/

static ply_boot_splash_plugin_t*
create_plugin (ply_key_file_t *key_file)
{
    ply_boot_splash_plugin_t* plugin;
    
    console("create_plugin\n");
    
    plugin = calloc(1,sizeof(ply_boot_splash_plugin_t));
    
    plugin->quit_request=false;
    plugin->px=0;
    plugin->dx=1;
    plugin->logo=ply_image_new("/usr/share/plymouth/themes/messier/logo.png");
    
    plugin->percent_done=0.0;
    
    return plugin;
}

static void
destroy_plugin (ply_boot_splash_plugin_t* plugin)
{
    console("destroy_plugin");
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
    console("add_pixel_display\n");
    
    if (plugin->display==NULL) {
        plugin->display=display;
        console("adding draw handler...\n");
        
        
        ply_pixel_display_set_draw_handler (display,
                                            (ply_pixel_display_draw_handler_t)on_draw,
                                            plugin);
    }
}

static void
remove_pixel_display (ply_boot_splash_plugin_t *plugin,
                      ply_pixel_display_t      *display)
{
    console("remove_pixel_display\n");
}

static bool
show_splash_screen (ply_boot_splash_plugin_t *plugin,
                    ply_event_loop_t         *loop,
                    ply_buffer_t             *boot_buffer,
                    ply_boot_splash_mode_t    mode)
{
    console("show_splash_screen\n");
    
    plugin->loop=loop;
    
    ply_image_load(plugin->logo);
    
    ply_event_loop_watch_for_exit (loop, (ply_event_loop_exit_handler_t)
                                       detach_from_event_loop,
                                       plugin);
    
    ply_event_loop_watch_for_timeout (loop,
                                          1.0 / 30.0,
                                          (ply_event_loop_timeout_handler_t)
                                          on_timeout, plugin);
    
    return true;
}

static void
update_status (ply_boot_splash_plugin_t *plugin,
               const char               *status)
{
    char buff[128];
    
    sprintf(buff,"* %s",status);
    console(buff);
    
}

static void
on_boot_progress (ply_boot_splash_plugin_t *plugin,
                  double                    duration,
                  double                    percent_done)
{
    plugin->percent_done=percent_done;
}

static void
hide_splash_screen (ply_boot_splash_plugin_t *plugin,
                    ply_event_loop_t         *loop)
{
    console("hide_splash_screen");
    ply_event_loop_stop_watching_for_exit (plugin->loop, (ply_event_loop_exit_handler_t)
                                                       detach_from_event_loop,
                                                       plugin);
}

static void
on_root_mounted (ply_boot_splash_plugin_t *plugin)
{
    console("root mounted");
    
}

static void
become_idle (ply_boot_splash_plugin_t *plugin,
             ply_trigger_t            *idle_trigger)
{
    console("become idle");
    ply_trigger_pull (idle_trigger, NULL);
}

static void
display_normal (ply_boot_splash_plugin_t *plugin)
{
    console("display_normal\n");
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
    console("ply_boot_splash_plugin_get_interface\n");
    
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