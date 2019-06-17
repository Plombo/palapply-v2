/*
 * Copyright (c) 2018-2019 Bryan Cain
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include "palapply.h"

static bool file_exists(const char *path)
{
    FILE *fp = fopen(path, "rb");
    bool exists = !!fp;
    if (fp) fclose(fp);
    return exists;
}

static void choose_palette_file(GtkBuilder *builder, GtkEntry *entry)
{
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Select Palette File",
                                         parent_window,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    // Both filter
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "All palette sources (*.png, *.gif)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_filter_add_mime_type(filter, "image/gif");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // PNG filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Portable Network Graphics (*.png)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // GIF filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Graphics Interchange Format (*.gif)");
    gtk_file_filter_add_mime_type(filter, "image/gif");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(entry, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void choose_palette_file_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry"));
    choose_palette_file(builder, entry);
}

static void choose_palette_file_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "batchPaletteFileEntry"));
    choose_palette_file(builder, entry);
}

static void choose_input_file_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry"));
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Select Input File",
                                         parent_window,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    // All filter
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "All readable images (*.png, *.gif, *.pcx, *.bmp)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_filter_add_mime_type(filter, "image/gif");
    gtk_file_filter_add_pattern(filter, "*.pcx");
    gtk_file_filter_add_mime_type(filter, "image/bmp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // PNG filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Portable Network Graphics (*.png)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // GIF filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Graphics Interchange Format (*.gif)");
    gtk_file_filter_add_mime_type(filter, "image/gif");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // PCX filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "PC Paintbrush image (*.pcx)");
    gtk_file_filter_add_pattern(filter, "*.pcx");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // BMP filter
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Bitmap image (*.bmp)");
    gtk_file_filter_add_mime_type(filter, "image/bmp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(entry, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void choose_output_file_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry"));
    GtkWindow *parent_window = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Select Output Location",
                                         parent_window,
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    // PNG filter
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Portable Network Graphics (*.png)");
    gtk_file_filter_add_mime_type(filter, "image/png");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(entry, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void choose_directory(GtkBuilder *builder, GtkEntry *entry, const gchar *windowTitle)
{
    GtkWindow *parentWindow = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkWidget *dialog;
    gint res;

    dialog = gtk_file_chooser_dialog_new(windowTitle,
                                         parentWindow,
                                         GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(entry, filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

static void choose_input_directory_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "batchInputDirEntry"));
    choose_directory(builder, entry, "Select input directory");
}

static void choose_output_directory_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "batchOutputDirEntry"));
    choose_directory(builder, entry, "Select output directory");
}

static void convert_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow"));
    GtkWidget *progressDialog = GTK_WIDGET(gtk_builder_get_object(builder, "progressDialog"));
    GtkProgressBar *progressBar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progressBar"));
    GtkTextBuffer *progressLog = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "progressTextView")));
    const gchar *inputPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry")));
    const gchar *palettePath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry")));
    const gchar *rawOutputPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry")));

    gtk_widget_show_all(progressDialog);

    if (!readPalette(palettePath))
    {
        fprintf(stderr, "error: failed to load palette image '%s'\n", palettePath);
        return;
    }

    SDL_Surface *img = readSourceImage(inputPath);
    if (img)
    {
        printf("read image %s\n", inputPath);
        gtk_text_buffer_insert_at_cursor(progressLog, "Read image ", -1);
        gtk_text_buffer_insert_at_cursor(progressLog, inputPath, -1);
        gtk_text_buffer_insert_at_cursor(progressLog, "\n", -1);
    }
    else
    {
        fprintf(stderr, "error: failed to load image %s\n", inputPath);
        SDL_FreeSurface(img);
        return;
    }

    // add a ".png" suffix to the output path if it doesn't already have one
    gchar *outputPath;
    size_t outputPathLength;
    gchar *rawOutputPathExt = strrchr(rawOutputPath, '.');
    if (rawOutputPathExt == NULL || g_ascii_strcasecmp(rawOutputPathExt, ".png") != 0)
    {
        outputPathLength = strlen(rawOutputPath) + 4; // 4 bytes for ".png" suffix
        outputPath = malloc(outputPathLength + 1);
        snprintf(outputPath, outputPathLength + 1, "%s.png", rawOutputPath);
    }
    else
    {
        outputPath = strdup(rawOutputPath);
        outputPathLength = strlen(outputPath);
    }

    bool writeOutput;
    if (file_exists(outputPath))
    {
        fprintf(stderr, "file already exists!\n");
        GtkWidget *dialog = gtk_message_dialog_new(window,
                GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_QUESTION,
                GTK_BUTTONS_YES_NO,
                "The file %s already exists. Do you want to overwrite it?",
                outputPath);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        writeOutput = (response == GTK_RESPONSE_YES);
        gtk_widget_destroy(dialog);
    }
    else
    {
        writeOutput = true;
    }

    if (writeOutput)
    {
        if (!saveIndexedPNG(outputPath, img))
        {
            fprintf(stderr, "error: failed to save result '%s'\n", outputPath);
            gtk_text_buffer_insert_at_cursor(progressLog, "\nFailed to save image ", -1);
            gtk_text_buffer_insert_at_cursor(progressLog, outputPath, -1);
            gtk_text_buffer_insert_at_cursor(progressLog, "\n", -1);
            SDL_FreeSurface(img);
            free(outputPath);
            return;
        }
        else
        {
            gtk_text_buffer_insert_at_cursor(progressLog, "Saved image ", -1);
            gtk_text_buffer_insert_at_cursor(progressLog, outputPath, -1);
            gtk_text_buffer_insert_at_cursor(progressLog, "\n", -1);
            gtk_progress_bar_set_fraction(progressBar, 1.0);
        }
    }

    if (img->format->Amask)
    {
        if (needsMask(img))
        {
            // make a mask filename
            size_t maskPathLength = outputPathLength + 6; // 5 bytes for "-mask"
            gchar *maskPath = malloc(maskPathLength + 1);
            memcpy(maskPath, outputPath, outputPathLength - 4); // copy everything up to ".png" extension
            snprintf(maskPath + outputPathLength - 4, 10, "-mask.png");
            printf("mask path: %s\n", maskPath);

            bool writeMask;
            if (file_exists(maskPath))
            {
                fprintf(stderr, "mask file already exists!\n");
                GtkWidget *dialog = gtk_message_dialog_new(window,
                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_QUESTION,
                        GTK_BUTTONS_YES_NO,
                        "The file %s already exists. Do you want to overwrite it?",
                        maskPath);
                gint response = gtk_dialog_run(GTK_DIALOG(dialog));
                writeMask = (response == GTK_RESPONSE_YES);
                gtk_widget_destroy(dialog);
            }
            else
            {
                writeMask = true;
            }

            if (writeMask)
            {
                if (!saveMask(maskPath, img))
                {
                    fprintf(stderr, "error: failed to save alpha mask '%s'\n", maskPath);
                    gtk_text_buffer_insert_at_cursor(progressLog, "\nFailed to save alpha mask ", -1);
                    gtk_text_buffer_insert_at_cursor(progressLog, maskPath, -1);
                    gtk_text_buffer_insert_at_cursor(progressLog, "\n", -1);
                }
                else
                {
                    printf("saved alpha mask to '%s'\n", maskPath);
                    gtk_text_buffer_insert_at_cursor(progressLog, "Saved alpha mask ", -1);
                    gtk_text_buffer_insert_at_cursor(progressLog, maskPath, -1);
                    gtk_text_buffer_insert_at_cursor(progressLog, "\n", -1);
                }
            }

            free(maskPath);
        }
        else
        {
            printf("no alpha mask needed (simple alpha channel)\n");
        }
    }
    else
    {
        printf("no alpha mask needed (source has no alpha channel)\n");
    }

    SDL_FreeSurface(img);
    free(outputPath);
}

// Clear any previously written text from the log when the progress dialog is shown.
static void on_show_progress_dialog(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkTextBuffer *progressLog = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "progressTextView")));
    GtkTextIter start, end;

    gtk_text_buffer_get_start_iter(progressLog, &start);
    gtk_text_buffer_get_end_iter(progressLog, &end);
    gtk_text_buffer_delete(progressLog, &start, &end);
}

static void hide_progress_dialog(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *progressDialog = GTK_WIDGET(gtk_builder_get_object(builder, "progressDialog"));
    gtk_widget_hide(progressDialog);
}

int main(int argc, char **argv)
{
    GtkBuilder *builder;
    GObject *window;
    GObject *button;
    GError *error = NULL;

    gtk_init(&argc, &argv);

    /* Construct a GtkBuilder instance and load our UI description */
    builder = gtk_builder_new();
    if (gtk_builder_add_from_file(builder, "gui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "mainWindow");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    button = gtk_builder_get_object(builder, "singleInputFileBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_input_file_single), builder);

    button = gtk_builder_get_object(builder, "singlePaletteFileBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_palette_file_single), builder);

    button = gtk_builder_get_object(builder, "singleOutputFileBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_output_file_single), builder);

    button = gtk_builder_get_object(builder, "singleFileConvertButton");
    g_signal_connect(button, "clicked", G_CALLBACK(convert_single), builder);

    button = gtk_builder_get_object(builder, "batchInputDirBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_input_directory_batch), builder);

    button = gtk_builder_get_object(builder, "batchPaletteFileBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_palette_file_batch), builder);

    button = gtk_builder_get_object(builder, "batchOutputDirBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_output_directory_batch), builder);

    button = gtk_builder_get_object(builder, "progressDialogCloseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_progress_dialog), builder);

    GObject *dialog = gtk_builder_get_object(builder, "progressDialog");
    g_signal_connect(dialog, "show", G_CALLBACK(on_show_progress_dialog), builder);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    return 0;
}


