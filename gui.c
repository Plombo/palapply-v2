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
#include "helpfiles.h"

// disable SDL_main definition
#ifdef _WIN32
#undef main
#endif

static gchar *lastDirectory = NULL;

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

    if (lastDirectory != NULL)
    {
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), lastDirectory);
    }

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

        g_free(lastDirectory);
        lastDirectory = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
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

    if (lastDirectory != NULL)
    {
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), lastDirectory);
    }

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

        g_free(lastDirectory);
        lastDirectory = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
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

    if (lastDirectory != NULL)
    {
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), lastDirectory);
    }

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

        g_free(lastDirectory);
        lastDirectory = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
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

    if (lastDirectory != NULL)
    {
        gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), lastDirectory);
    }

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        gtk_entry_set_text(entry, filename);
        g_free(filename);

        g_free(lastDirectory);
        lastDirectory = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(dialog));
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

// enable the "Convert" button only if all required text fields have been filled
static void convert_button_enable_single(GtkBuilder *builder)
{
    gint inputPathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry"))));
    gint outputPathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry"))));
    gint palettePathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry"))));

    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "singleFileConvertButton")),
            inputPathCount > 0 && outputPathCount > 0 && palettePathCount > 0);
}

// enable the "Convert" button only if all required text fields have been filled
static void convert_button_enable_batch(GtkBuilder *builder)
{
    gint inputPathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchInputDirEntry"))));
    gint outputPathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchOutputDirEntry"))));
    gint palettePathCount = gtk_entry_buffer_get_length(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchPaletteFileEntry"))));

    gtk_widget_set_sensitive(GTK_WIDGET(gtk_builder_get_object(builder, "batchConvertButton")),
            inputPathCount > 0 && outputPathCount > 0 && palettePathCount > 0);
}

static void text_inserted_single(GtkEntryBuffer *buffer, guint position, gchar *chars, guint count, gpointer data)
{
    convert_button_enable_single((GtkBuilder*) data);
}

static void text_deleted_single(GtkEntryBuffer *buffer, guint position, guint count, gpointer data)
{
    convert_button_enable_single((GtkBuilder*) data);
}

static void text_inserted_batch(GtkEntryBuffer *buffer, guint position, gchar *chars, guint count, gpointer data)
{
    convert_button_enable_batch((GtkBuilder*) data);
}

static void text_deleted_batch(GtkEntryBuffer *buffer, guint position, guint count, gpointer data)
{
    convert_button_enable_batch((GtkBuilder*) data);
}

// inserts text at the end of a GtkTextBuffer
static void text_buffer_append(GtkTextBuffer *textBuffer, const gchar *text)
{
    GtkTextIter endIter;
    gtk_text_buffer_get_end_iter(textBuffer, &endIter);
    gtk_text_buffer_insert(textBuffer, &endIter, text, -1);
}

// sets the vertical scroll position of a GtkScrollable to the bottom
static void scroll_to_bottom(GtkScrollable *scrollable)
{
    GtkAdjustment *adj = gtk_scrollable_get_vadjustment(scrollable);
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));

    // run the main loop to actually apply the scrolling
    while (gtk_events_pending())
      gtk_main_iteration();
}

enum CustomOverwriteResponse {
    RESPONSE_NO_ALL,
    RESPONSE_YES_ALL,
};

static bool convert_file(GtkBuilder *builder, const gchar *inputPath, const gchar *outputPath,
                         const gchar *palettePath, bool batchMode, gint *response)
{
    GtkWindow *progressDialog = GTK_WINDOW(gtk_builder_get_object(builder, "progressDialog"));
    GtkScrollable *progressTextView = GTK_SCROLLABLE(gtk_builder_get_object(builder, "progressTextView"));
    GtkTextBuffer *progressLog = gtk_text_view_get_buffer(GTK_TEXT_VIEW(progressTextView));

    if (!readPalette(palettePath))
    {
        text_buffer_append(progressLog, "Failed to load palette from ");
        text_buffer_append(progressLog, palettePath);
        text_buffer_append(progressLog, "\n");
        scroll_to_bottom(progressTextView);
        return false;
    }

    SDL_Surface *img = readSourceImage(inputPath);
    if (!img)
    {
        text_buffer_append(progressLog, "\nFailed to read image ");
        text_buffer_append(progressLog, inputPath);
        text_buffer_append(progressLog, "\n");
        scroll_to_bottom(progressTextView);
        SDL_FreeSurface(img);
        return false;
    }

    bool writeOutput = true;
    if (file_exists(outputPath))
    {
        if (*response == RESPONSE_YES_ALL)
        {
        }
        else if (*response == RESPONSE_NO_ALL)
        {
            writeOutput = false;
        }
        else
        {
            GtkWidget *dialog = gtk_message_dialog_new(progressDialog,
                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_QUESTION,
                    batchMode ? GTK_BUTTONS_NONE : GTK_BUTTONS_YES_NO,
                    "The file %s already exists. Do you want to overwrite it?",
                    outputPath);

            if (batchMode)
            {
                gtk_dialog_add_buttons(GTK_DIALOG(dialog),
                        "_Cancel", GTK_RESPONSE_CANCEL,
                        "N_o to all", RESPONSE_NO_ALL,
                        "_No", GTK_RESPONSE_NO,
                        "Y_es to all", RESPONSE_YES_ALL,
                        "_Yes", GTK_RESPONSE_YES,
                        NULL);
            }

            *response = gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);

            writeOutput = (*response == GTK_RESPONSE_YES || *response == RESPONSE_YES_ALL);
            if (*response == GTK_RESPONSE_CANCEL)
            {
                SDL_FreeSurface(img);
                return false;
            }
        }
    }

    if (writeOutput)
    {
        if (!saveIndexedPNG(outputPath, img))
        {
            text_buffer_append(progressLog, "\nFailed to save image ");
            text_buffer_append(progressLog, outputPath);
            text_buffer_append(progressLog, "\n");
            scroll_to_bottom(progressTextView);
            SDL_FreeSurface(img);
            return false;
        }
        else
        {
            text_buffer_append(progressLog, "Saved image ");
            text_buffer_append(progressLog, outputPath);
            text_buffer_append(progressLog, "\n");
            scroll_to_bottom(progressTextView);
        }
    }
    else
    {
        text_buffer_append(progressLog, "Not overwriting ");
        text_buffer_append(progressLog, outputPath);
        text_buffer_append(progressLog, "\n");
        scroll_to_bottom(progressTextView);
    }

    if (img->format->Amask)
    {
        if (alphaType(img) == ALPHA_MASK_NEEDED)
        {
            // make a mask filename
            size_t outputPathLength = strlen(outputPath);
            size_t maskPathLength = outputPathLength + 6; // 5 bytes for "-mask"
            gchar *maskPath = malloc(maskPathLength + 1);
            memcpy(maskPath, outputPath, outputPathLength - 4); // copy everything up to ".png" extension
            snprintf(maskPath + outputPathLength - 4, 10, "-mask.png");
            printf("mask path: %s\n", maskPath);

            bool writeMask = true;
            if (file_exists(maskPath))
            {
                if (*response == RESPONSE_YES_ALL)
                {
                }
                else if (*response == RESPONSE_NO_ALL)
                {
                    writeMask = false;
                }
                else
                {
                    GtkWidget *dialog = gtk_message_dialog_new(progressDialog,
                        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                        GTK_MESSAGE_QUESTION,
                        batchMode ? GTK_BUTTONS_NONE : GTK_BUTTONS_YES_NO,
                        "The file %s already exists. Do you want to overwrite it?",
                        maskPath);

                    if (batchMode)
                    {
                        gtk_dialog_add_buttons(GTK_DIALOG(dialog),
                                "_Cancel", GTK_RESPONSE_CANCEL,
                                "N_o to all", RESPONSE_NO_ALL,
                                "_No", GTK_RESPONSE_NO,
                                "Y_es to all", RESPONSE_YES_ALL,
                                "_Yes", GTK_RESPONSE_YES,
                                NULL);
                    }

                    *response = gtk_dialog_run(GTK_DIALOG(dialog));
                    gtk_widget_destroy(dialog);

                    writeMask = (*response == GTK_RESPONSE_YES || *response == RESPONSE_YES_ALL);
                    if (*response == GTK_RESPONSE_CANCEL)
                    {
                        SDL_FreeSurface(img);
                        free(maskPath);
                        return false;
                    }
                }
            }

            if (writeMask)
            {
                if (!saveMask(maskPath, img))
                {
                    text_buffer_append(progressLog, "\nFailed to save alpha mask ");
                    text_buffer_append(progressLog, maskPath);
                    text_buffer_append(progressLog, "\n");
                    scroll_to_bottom(progressTextView);
                    free(maskPath);
                    SDL_FreeSurface(img);
                    return false;
                }
                else
                {
                    text_buffer_append(progressLog, "Saved alpha mask ");
                    text_buffer_append(progressLog, maskPath);
                    text_buffer_append(progressLog, "\n");
                    scroll_to_bottom(progressTextView);
                }
            }
            else
            {
                text_buffer_append(progressLog, "Not overwriting ");
                text_buffer_append(progressLog, maskPath);
                text_buffer_append(progressLog, "\n");
                scroll_to_bottom(progressTextView);
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
    return true;
}

static void convert_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *progressDialog = GTK_WIDGET(gtk_builder_get_object(builder, "progressDialog"));
    GtkProgressBar *progressBar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progressBar"));
    GtkTextBuffer *progressLog = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "progressTextView")));
    const gchar *inputPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry")));
    const gchar *palettePath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry")));
    const gchar *rawOutputPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry")));

    gtk_progress_bar_set_fraction(progressBar, 0.0);
    gtk_widget_show_all(progressDialog);

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

    gint response = GTK_RESPONSE_NONE;
    if (convert_file(builder, inputPath, outputPath, palettePath, false, &response))
    {
        gtk_progress_bar_set_fraction(progressBar, 1.0);
        text_buffer_append(progressLog, "\nDone");
    }
    else
    {
        text_buffer_append(progressLog, "\nAn error occurred");
    }

    free(outputPath);
}

static void convert_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *progressDialog = GTK_WIDGET(gtk_builder_get_object(builder, "progressDialog"));
    GtkScrollable *progressTextView = GTK_SCROLLABLE(gtk_builder_get_object(builder, "progressTextView"));
    GtkTextBuffer *progressLog = gtk_text_view_get_buffer(GTK_TEXT_VIEW(progressTextView));
    GtkProgressBar *progressBar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "progressBar"));
    GtkComboBoxText *extensionBox = GTK_COMBO_BOX_TEXT(gtk_builder_get_object(builder, "batchInputFileExtensionBox"));

    gchar *inputExtension = gtk_combo_box_text_get_active_text(extensionBox);
    const gchar *inputDirPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "batchInputDirEntry")));
    GDir *inputDir = g_dir_open(inputDirPath, 0, NULL);
    const gchar *outputDirPath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "batchOutputDirEntry")));
    const gchar *palettePath = gtk_entry_get_text(GTK_ENTRY(gtk_builder_get_object(builder, "batchPaletteFileEntry")));

    gtk_progress_bar_set_fraction(progressBar, 0.0);
    gtk_widget_show_all(progressDialog);

    struct InputName {
        gchar *name;
        struct InputName *next;
    };
    struct InputName *nameList = NULL, *cur = NULL;
    unsigned int numInputFiles = 0;
    const gchar *name;
    while ((name = g_dir_read_name(inputDir)))
    {
        if (g_str_has_suffix(name, inputExtension))
        {
            struct InputName *nameNode = malloc(sizeof(struct InputName));
            nameNode->name = strdup(name);
            nameNode->next = NULL;

            if (nameList == NULL)
            {
                nameList = nameNode;
            }

            if (cur)
            {
                cur->next = nameNode;
            }
            cur = nameNode;
            ++numInputFiles;
        }
    }
    g_dir_close(inputDir);
    g_free(inputExtension);

    bool ok = true;
    gint response = GTK_RESPONSE_NONE;
    unsigned int convertedCount = 0;
    cur = nameList;
    while (cur != NULL)
    {
        name = cur->name;

        // make full input path
        size_t inputPathLength = strlen(inputDirPath) + strlen(name) + 1;
        gchar *inputPath = malloc(inputPathLength + 1);
        snprintf(inputPath, inputPathLength + 1, "%s/%s", inputDirPath, name);
        printf("input file: %s\n", inputPath);

        // make full output path
        size_t outputPathLength = strlen(outputDirPath) + strlen(name) + 1;
        gchar *outputPath = malloc(outputPathLength + 1);
        snprintf(outputPath, outputPathLength + 1, "%s/%s", outputDirPath, name);

        // replace output extension with ".png"
        // this method works because the input filename is guaranteed to end with a 3-letter extension
        memcpy(&outputPath[outputPathLength - 4], ".png", 4);
        printf("output file: %s\n", outputPath);

        // do the actual conversion
        ok = convert_file(builder, inputPath, outputPath, palettePath, true, &response);
        free(inputPath);
        free(outputPath);

        if (response == GTK_RESPONSE_CANCEL || !ok)
        {
            break;
        }

        // update progress bar
        ++convertedCount;
        gtk_progress_bar_set_fraction(progressBar, (gdouble) convertedCount / numInputFiles);

        cur = cur->next;
    }

    // Free the input name list
    while (nameList != NULL)
    {
        struct InputName *next = nameList->next;
        free(nameList->name);
        free(nameList);
        nameList = next;
    }

    if (response == GTK_RESPONSE_CANCEL)
    {
        text_buffer_append(progressLog, "\nCanceled");
        scroll_to_bottom(progressTextView);
    }
    else if (!ok)
    {
        text_buffer_append(progressLog, "\nAn error occurred");
        scroll_to_bottom(progressTextView);
    }
    else if (numInputFiles == 0)
    {
        text_buffer_append(progressLog, "No input files\n");
        scroll_to_bottom(progressTextView);
    }
    else
    {
        gtk_progress_bar_set_fraction(progressBar, 1.0);
        text_buffer_append(progressLog, "\nDone");
        scroll_to_bottom(progressTextView);
    }

    scroll_to_bottom(progressTextView);
}

static void show_help_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *helpDialog = GTK_WIDGET(gtk_builder_get_object(builder, "singleFileHelpDialog"));
    gtk_widget_show_all(helpDialog);
}

static void close_help_single(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *helpDialog = GTK_WIDGET(gtk_builder_get_object(builder, "singleFileHelpDialog"));
    gtk_widget_hide(helpDialog);
}

static void show_help_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *helpDialog = GTK_WIDGET(gtk_builder_get_object(builder, "batchHelpDialog"));
    gtk_widget_show_all(helpDialog);
}

static void close_help_batch(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkWidget *helpDialog = GTK_WIDGET(gtk_builder_get_object(builder, "batchHelpDialog"));
    gtk_widget_hide(helpDialog);
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

// Glade's support for "About" dialogs is lacking, so do this manually.
static void show_about_dialog(GtkWidget *widget, gpointer data)
{
    GtkBuilder *builder = (GtkBuilder*) data;
    GtkAboutDialog *aboutDialog = GTK_ABOUT_DIALOG(gtk_about_dialog_new());

    gtk_about_dialog_set_program_name(aboutDialog, "PalApply v2");
    gtk_about_dialog_set_version(aboutDialog, "Pre-release");
    gtk_about_dialog_set_copyright(aboutDialog, "Copyright (c) 2010-2019 Bryan Cain");
    gtk_about_dialog_set_license_type(aboutDialog, GTK_LICENSE_GPL_3_0);
    gtk_about_dialog_set_website(aboutDialog, "https://github.com/Plombo/palapply-v2");
    gtk_about_dialog_set_website_label(aboutDialog, "Website");
    const gchar *authors[] = {"Bryan Cain", NULL};
    gtk_about_dialog_set_authors(aboutDialog, authors);

    gtk_window_set_transient_for(GTK_WINDOW(aboutDialog), GTK_WINDOW(gtk_builder_get_object(builder, "mainWindow")));
    gtk_dialog_run(GTK_DIALOG(aboutDialog));
    gtk_widget_destroy(GTK_WIDGET(aboutDialog));
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

    button = gtk_builder_get_object(builder, "singleFileHelpButton");
    g_signal_connect(button, "clicked", G_CALLBACK(show_help_single), builder);

    button = gtk_builder_get_object(builder, "singleFileHelpCloseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(close_help_single), builder);

    button = gtk_builder_get_object(builder, "singleFileAboutButton");
    g_signal_connect(button, "clicked", G_CALLBACK(show_about_dialog), builder);

    button = gtk_builder_get_object(builder, "batchInputDirBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_input_directory_batch), builder);

    button = gtk_builder_get_object(builder, "batchPaletteFileBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_palette_file_batch), builder);

    button = gtk_builder_get_object(builder, "batchOutputDirBrowseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(choose_output_directory_batch), builder);

    button = gtk_builder_get_object(builder, "batchConvertButton");
    g_signal_connect(button, "clicked", G_CALLBACK(convert_batch), builder);

    button = gtk_builder_get_object(builder, "batchHelpButton");
    g_signal_connect(button, "clicked", G_CALLBACK(show_help_batch), builder);

    button = gtk_builder_get_object(builder, "batchHelpCloseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(close_help_batch), builder);

    button = gtk_builder_get_object(builder, "batchAboutButton");
    g_signal_connect(button, "clicked", G_CALLBACK(show_about_dialog), builder);

    button = gtk_builder_get_object(builder, "progressDialogCloseButton");
    g_signal_connect(button, "clicked", G_CALLBACK(hide_progress_dialog), builder);

    GObject *dialog = gtk_builder_get_object(builder, "progressDialog");
    g_signal_connect(dialog, "show", G_CALLBACK(on_show_progress_dialog), builder);

    // Only enable the "convert" button when all of the requisite text fields have text in them.
    convert_button_enable_single(builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_single), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleInputFileEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_single), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_single), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singleOutputFileEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_single), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_single), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "singlePaletteFileEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_single), builder);

    // Do the same thing as the last block, but for batch mode.
    convert_button_enable_batch(builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchInputDirEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_batch), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchInputDirEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_batch), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchOutputDirEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_batch), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchOutputDirEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_batch), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchPaletteFileEntry")))),
            "inserted-text", G_CALLBACK(text_inserted_batch), builder);
    g_signal_connect(G_OBJECT(gtk_entry_get_buffer(GTK_ENTRY(gtk_builder_get_object(builder, "batchPaletteFileEntry")))),
            "deleted-text", G_CALLBACK(text_deleted_batch), builder);

    // Populate the help dialogs with the help text. Ideally, this would be done in Glade, but it doesn't seem to
    // support populating GtkTextBuffers with markup.
    GtkTextIter iter;
    GtkTextBuffer *helpTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "singleFileHelpTextView")));
    gtk_text_buffer_get_start_iter(helpTextBuffer, &iter);
    gtk_text_buffer_insert_markup(helpTextBuffer, &iter, singleFileHelpText, -1);

    helpTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gtk_builder_get_object(builder, "batchHelpTextView")));
    gtk_text_buffer_get_start_iter(helpTextBuffer, &iter);
    gtk_text_buffer_insert_markup(helpTextBuffer, &iter, batchHelpText, -1);

    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_main();

    g_free(lastDirectory);
    lastDirectory = NULL;

    return 0;
}


