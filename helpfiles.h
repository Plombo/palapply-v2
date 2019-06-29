/*
 * Copyright (c) 2010-2019 Bryan Cain
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

#ifndef HELPFILES_H
#define HELPFILES_H

static const char *singleFileHelpText =
    "<big><big><b>Single File Mode</b></big></big>\n"
    "\n"
    "<b>How to Use</b>\n"
    "\n"
    "The path to the input image file goes in the \"input image\" field. The input file is an image in any color mode. The input image must be in PNG, GIF, PCX, or BMP format.\n"
    "\n"
    "The path to the output image file goes in the \"output image\" field. The output image is the file that the finished product will be saved to. The output image must be in PNG format. It will be an image in indexed format that uses the palette specified in the \"palette image\" field.\n"
    "\n"
    "The palette file is specified in the \"palette\" field. If it is an image in PNG or GIF format, it must be in <i>indexed color mode</i>, meaning the image must already have a palette. The image's internal palette will be applied to the input image. Alternatively, you can use a palette in ACT format instead of an image. Either way, note that this field contains the path to a single file, not a directory."
    "\n"
    "Once you have specified all three files, click on the \"Convert\" button to apply the palette to the image.";

static const char *batchHelpText =
    "<big><big><b>Batch Mode</b></big></big>\n"
    "\n"
    "<b>Purpose</b>\n"
    "\n"
    "The purpose of the batch mode is to convert multiple files at once. The files to be converted are selected from a directory of your choice and are selected based on their extensions. For example, you can choose to convert all PNG files in the directory of your choice.\n"
    "\n"
    "The output file paths are chosen automatically based on the selected output directory and the name of the corresponding input file. Alpha mask images are the original filename suffixed with \"-mask\". For example, if the filename of one input file was circle.bmp, the output file would be named circle.png and saved to the selected output directory. If it needed an alpha mask, the mask would be named circle-mask.png and saved to the output directory.\n"
    "\n"
    "<b>How to Use</b>\n"
    "\n"
    "The \"input directory\" field holds the directory containing all of the files you wish to convert to indexed mode. In the \"extension of input files\" box, select from the list the file extension of the images you wish to convert.\n"
    "\n"
    "The \"output directory\" field holds the directory that the converted files will be saved to. Output files are always in PNG format, regardless of the input file format.\n"
    "\n"
    "<i><b>Note</b>: If you want to edit the existing image files instead of saving the result to another file, make the input and output directories the same.</i>\n"
    "\n"
    "The palette file is specified in the \"palette\" field. If it is an image in PNG or GIF format, it must be in <i>indexed color mode</i>, meaning the image must already have a palette. The image's internal palette will be applied to all input images. Alternatively, you can use a palette in ACT format instead of an image. Either way, note that this field contains the path to a single file, not a directory."
    "\n"
    "Once you have filled in all three fields, click on the \"Convert\" button to apply the palette to the images.";

#endif

