/**
 * @file a1.c
 * @author Emmanuel Ainoo & Ayomide Oduba
 * @brief 
 * @version 0.1
 * @date 2022-09-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/* Load Library Dependencies */
#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>


/**
 * @brief
 * Create Image Surface With Cairo
 */
struct {
    cairo_surface_t *image;
    gint img_width;
    gint img_height;
    int stride;  
} glob;

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

/* Static Function Definitions */
static void draw_image_on_window(cairo_t *, GtkWidget *widget);


/**
 * @brief 
 * Loads Image from file 
 * @param filename - name of file of image
 */
static void load_image(char *filename) {     
    glob.image = cairo_image_surface_create_from_png(filename);  

}

/**
 * @brief 
 * Handles Drawing Area Event
 * @param widget 
 * @param cr 
 * @param user_data 
 * @return gboolean 
 */
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {      
    draw_image_on_window(cr, widget);  // call draw image function on draw event to display image
    return FALSE;
}

/**
 * @brief 
 * Draws Image on Drawing Area Window
 * @param cr 
 * @param widget 
 */
static void draw_image_on_window(cairo_t *cr, GtkWidget *widget) {
    // cairo_scale (cr, 0.65, 0.65); // scale image to specific size
    cairo_set_source_surface(cr, glob.image, 0, 0);
    cairo_paint(cr);
}

/**
 * @brief 
 * Detects Color Pixels from Mouse Coordinates
 * @param widget 
 * @param data 
 * @param x_coordinate - x coordinate of mouse pointer
 * @param y_coordinate - y coordinate of mouse pointer
 */
static void pick_color (int x_coordinate, int y_coordinate) {
    

    float red, green, blue, alpha;
    unsigned char *image_data = cairo_image_surface_get_data (glob.image); // get image data from surface

    glob.img_width = cairo_image_surface_get_width(glob.image); // extract image height
    glob.img_height = cairo_image_surface_get_height(glob.image); // extract image width
    glob.stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, glob.img_width); // get image stride in 24Bits
    
    red = (float)image_data[x_coordinate * glob.stride + y_coordinate * 4 + 0] / 255; //extract blue pixel value from image data
    green = (float)image_data[x_coordinate * glob.stride + y_coordinate * 4 + 1] / 255; //extract green pixel value from image data
    blue = (float)image_data[x_coordinate * glob.stride + y_coordinate * 4 + 2] / 255; //extract red pixel value from image data
    alpha = (float)image_data[x_coordinate * glob.stride + y_coordinate * 4 + 3] / 255; 

    // print out pixel colors extracted
    printf ("pixel red: %f \n", (float) red);
    printf ("pixel green: %f \n", (float) green);
    printf ("pixel blue: %f \n", (float) blue);
    printf ("pixel alpha: %f \n", (float) alpha);
}

/**
 * @brief 
 * Event Handler for Mouse Left Click 
 * @param widget 
 * @param event 
 * @param data 
 * @return gboolean 
 */
static gboolean button_press_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == GDK_BUTTON_PRIMARY) {
        printf("Coordinates: (%u,%u)\n", (guint)event->x,(guint)event->y); // print selected coordinates
        pick_color(((event->x)-20),((event->y)-20)); // call pick color when left mouse is clicked
    }
    return TRUE;
}

/**
 * @brief 
 * Event Handler for Mouse Drag over Window
 * @param widget 
 * @param event 
 * @param data 
 * @return gboolean 
 */
static gboolean motion_notify_event_cb (GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->state & GDK_BUTTON1_MASK) {
        printf("Coordinates: (%u,%u)\n", (guint)event->x,(guint)event->y); // print selected coordinates
        pick_color(event->x,event->y); // call pick color when left mouse is clicked
    }
    return TRUE;
}


int main(int argc, char *argv[]) { 

    G_GNUC_BEGIN_IGNORE_DEPRECATIONS //suppress gtk depreciation warnings

    /* Initialize All GTK widgets */
    GtkWidget *window, *drawing_area, *table, *title, *wins, *h_align, *h_align2, *v_align, *pick_color_btn, *paint_color_btn, *quit_btn, *save_new_btn;
    GdkEventButton *event;


    /* Load Image From File */
    load_image("photos/color-wheel.png");

    
    /* Initialize GTK Window and Adjust Details */
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request (window, 650, 650);
    gtk_window_set_title(GTK_WINDOW(window), "Pick Color App");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    
    /* Create Table for Alignment of other Widgets */
    table = gtk_table_new(6, 4, FALSE);
    gtk_table_set_col_spacings(GTK_TABLE(table), 3);
    gtk_table_set_row_spacing(GTK_TABLE(table), 0, 3);

    
    /* Create Title for Picture View with Alignment */
    title = gtk_label_new("Picture Display"); // loaded image file name
    h_align = gtk_alignment_new(0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(h_align), title);
    gtk_table_attach(GTK_TABLE(table), h_align, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 0, 0);

    
    /* Create Drawing Area for Image Loading with Alignment */
    drawing_area = gtk_drawing_area_new();
    gtk_table_attach(GTK_TABLE(table), drawing_area, 0, 2, 1, 3, GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);

    
    /* Create Pick Color Button with Alignment */
    pick_color_btn = gtk_button_new_with_label("Pick Color");
    gtk_widget_set_size_request(pick_color_btn, 50, 30);
    gtk_table_attach(GTK_TABLE(table), pick_color_btn, 3, 4, 1, 2, GTK_FILL, GTK_SHRINK, 1, 1);

    
    /* Create Paint Color Button with Alignment */
    v_align = gtk_alignment_new(0, 0, 0, 0);
    paint_color_btn = gtk_button_new_with_label("Paint Color");
    gtk_widget_set_size_request(paint_color_btn, 50, 30);
    gtk_container_add(GTK_CONTAINER(v_align), paint_color_btn);
    gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
    gtk_table_attach(GTK_TABLE(table), v_align, 3, 4, 2, 3, GTK_FILL, GTK_FILL | GTK_EXPAND, 1, 1);


    /* Create Quit Button with Alignment */
    h_align2 = gtk_alignment_new(0, 1, 0, 0);
    quit_btn = gtk_button_new_with_label("Quit");
    gtk_container_add(GTK_CONTAINER(h_align2), quit_btn);
    gtk_widget_set_size_request(quit_btn, 70, 30);
    gtk_table_set_row_spacing(GTK_TABLE(table), 3, 5);
    gtk_table_attach(GTK_TABLE(table), h_align2, 0, 1, 4, 5, GTK_FILL, GTK_FILL, 0, 0);


    /* Create Save New Button with Alignment */
    save_new_btn = gtk_button_new_with_label("Save New");
    gtk_widget_set_size_request(save_new_btn, 70, 30);
    gtk_table_attach(GTK_TABLE(table), save_new_btn, 3, 4, 4, 5, GTK_FILL, GTK_FILL, 0, 0);
    gtk_container_add(GTK_CONTAINER(window), table);

    /* Register Event Handlers */
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL); 
    // g_signal_connect (drawing_area, "motion-notify-event", G_CALLBACK (motion_notify_event_cb), NULL);
    g_signal_connect (drawing_area, "button-press-event", G_CALLBACK (button_press_event_cb), NULL);

    /* Load Widgets on Screen */
    gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)| GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);
    gtk_widget_show_all(window);
    gtk_main();

        
    cairo_surface_destroy(glob.image);
    G_GNUC_END_IGNORE_DEPRECATIONS

    return 0;

}


