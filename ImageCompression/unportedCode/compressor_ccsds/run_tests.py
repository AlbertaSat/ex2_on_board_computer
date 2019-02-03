#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

# Luca Fossati (Luca.Fossati@esa.int), European Space Agency

# Software distributed under the "European Space Agency Public License – v2.0".

# All Distribution of the Software and/or Modifications, as Source Code or Object Code,
# must be, as a whole, under the terms of the European Space Agency Public License – v2.0.
# If You Distribute the Software and/or Modifications as Object Code, You must:
# (a)	provide in addition a copy of the Source Code of the Software and/or
# Modifications to each recipient; or
# (b)	make the Source Code of the Software and/or Modifications freely accessible by reasonable
# means for anyone who possesses the Object Code or received the Software and/or Modifications
# from You, and inform recipients how to obtain a copy of the Source Code.

# The Software is provided to You on an “as is” basis and without warranties of any
# kind, including without limitation merchantability, fitness for a particular purpose,
# absence of defects or errors, accuracy or non-infringement of intellectual property
# rights.
# Except as expressly set forth in the "European Space Agency Public License – v2.0",
# neither Licensor nor any Contributor shall be liable, including, without limitation, for direct, indirect,
# incidental, or consequential damages (including without limitation loss of profit),
# however caused and on any theory of liability, arising in any way out of the use or
# Distribution of the Software or the exercise of any rights under this License, even
# if You have been advised of the possibility of such damages.
#

import sys, os, csv, glob

if __name__ == "__main__":
    if len(sys.argv) != 7:
        raise Exception('Error, the command line for running the tests is: run_tests.py csv_parameters_file compressor_exec original_img_folder compressed_img_folder residuals_img_folder decompressor_exec')
    try:
        csv_file_handle = open(sys.argv[1], 'rb')
    except IOError:
        raise Exception('Error in opening file ' + sys.argv[1])
    if not os.path.exists(sys.argv[2]):
        csv_file_handle.close()
        raise Exception('Compressor executable ' + sys.argv[2] + ' not existing')
    if not os.path.exists(sys.argv[3]):
        csv_file_handle.close()
        raise Exception('Folder ' + sys.argv[3] + ' not existing')
    if not os.path.exists(sys.argv[4]):
        csv_file_handle.close()
        raise Exception('Folder ' + sys.argv[4] + ' not existing')
    if not os.path.exists(sys.argv[5]):
        csv_file_handle.close()
        raise Exception('Folder ' + sys.argv[5] + ' not existing')
    if not os.path.exists(sys.argv[6]):
        csv_file_handle.close()
        raise Exception('Decompressor executable ' + sys.argv[6] + ' not existing')    
    compressor_exec = os.path.abspath(sys.argv[2])
    orig_images = os.path.abspath(sys.argv[3])
    compressed_images = os.path.abspath(sys.argv[4])
    residual_images = os.path.abspath(sys.argv[5])
    decompressor_exec = os.path.abspath(sys.argv[6])
    csv_reader = csv.reader(csv_file_handle, delimiter=',')
    
    # Now we can start the processing; as the case of the files and
    # their extension can be different from the one in the CSV file
    # I first process the files in the folders
    orig_folder = {}
    compressed_folder = {}
    residuals_folder = {}
    for i in os.listdir(orig_images):
        orig_folder[os.path.splitext(i)[0].lower()] = i
    for i in os.listdir(compressed_images):
        compressed_folder[os.path.splitext(i)[0].lower()] = i
    for i in os.listdir(residual_images):
        residuals_folder[os.path.splitext(i)[0].lower()] = i
    csv_reader.next()
    csv_reader.next()
    for row in csv_reader:
        img_name = row[0] + '_' + row[1]
        img_name = img_name.lower()
        if not orig_folder.has_key(img_name):
            print(img_name + ' not among original images')
            continue
        if not compressed_folder.has_key(img_name):
            print compressed_folder[img_name]
            print(img_name + ' not among compressed images')
        if not residuals_folder.has_key(img_name):
            print(img_name + ' not among residuals images')
        if not compressed_folder.has_key(img_name) and not residuals_folder.has_key(img_name):
            continue
        # Now I build the command line, by parsing the CSV file with the parameters
        command_line = '--input ' + os.path.join(orig_images, orig_folder[img_name]) + ' --output compressed.fl --rows ' + row[3] + ' --columns ' + row[2] + ' --bands ' + row[4]
        if row[5] == 'Y':
            command_line = command_line + ' --signed_sample'
        command_line = command_line + ' --dyn_range ' + row[6] + ' --pred_bands ' + row[7]
        if row[8] == 'F':
            command_line = command_line + ' --full'
        if row[9] == 'N':
            command_line = command_line + ' --neighbour_sum'
        command_line = command_line + ' --reg_size ' + row[10] + ' --w_resolution ' + row[11] + ' --w_interval ' + row[12] + ' --w_initial ' + row[13] + ' --w_final ' + row[14]
        if row[15] == 'Y':
            command_line = command_line + ' --weight_init_file ' + os.path.join(compressed_images, img_name) + '_weights --w_init_resolution 5'
        command_line = command_line + ' --out_format ' + row[16]
        litteral = False
        try:
            int(row[17])
        except ValueError:
            litteral = True
        if not litteral:
            command_line = command_line + ' --out_depth ' + row[17]
        command_line = command_line + ' --word_len ' + row[18]
        if row[19] == 'S':
            command_line = command_line + ' --sample_adaptive --u_max ' + row[20] + ' --y_star ' + row[21] + ' --y_0 ' + row[22]
            if row[23] == 'Table':
                command_line = command_line + ' --k_init_file ' + os.path.join(compressed_images, img_name) + '_accinit'
            else:
                command_line = command_line + ' --k ' + row[23]
        else:
            command_line = command_line + ' --block_size ' + row[24] + ' --ref_interval ' + row[25]
        command_line = command_line + ' --dump_residuals'
        if row[29] == 'BIP':
            command_line = command_line + ' --in_format BI --in_depth ' + row[4]
        else:
            command_line = command_line + ' --in_format BSQ'
        if not row[0] in ['LANDSAT', 'SPOT5']:
            command_line = command_line + ' --reg_input'
            if row[28] == 'L':
                command_line = command_line + ' --in_byte_ordering LITTLE'
            else:
                command_line = command_line + ' --in_byte_ordering BIG'
        
        # Let's run the compressor
        result = os.popen(compressor_exec + ' ' + command_line + ' 2>&1').readlines()
        # Check is any error was signaled
        error = False
        for i in result:
            if 'error' in i.lower():
                print('\n***ERROR: ' + img_name)
                print('\n'.join(result))
                error = True
                break
        if not result:
            print('\n***ERROR: ' + img_name)
            print('\nNo output has been produced')
            error = True
        if error:
            print('command_line: ' + command_line + '\n\n')
            continue
        else:
            print('RESULT: ' + img_name)
            print('\n'.join(result))
        # Now I check the residuals: by default they are output by the compressor as BIP; in case
        # the ordering of the output compressed image is different, it is necessary to
        # change it to match it.
        command_line_conv = ''
        if row[16] != 'BI' or (row[16] == 'BI' and row[17] != row[4]):
            command_line_conv = '--input residuals_compressed.fl.bip --output residuals_compressed.fl.bip --rows ' + row[3] + ' --columns ' + row[2] + ' --bands ' + row[4]
            command_line_conv = command_line_conv + ' --in_format BI --in_depth ' + row[4] + ' --in_byte_ordering LITTLE --dyn_range 16 --out_format ' + row[16]
            litteral = False
            try:
                int(row[17])
            except ValueError:
                litteral = True
            if not litteral:
                command_line_conv = command_line_conv + ' --out_depth ' + row[17]
            os.popen(os.path.join(os.path.split(compressor_exec)[0], 'converter') + ' ' + command_line_conv + ' 2>&1')
        # Make a diff with the residuals and compressed files and point out any error
        if compressed_folder.has_key(img_name):
            result = os.popen('diff -q compressed.fl ' + os.path.join(compressed_images, compressed_folder[img_name])).readlines()
            if result:
                error = True
                print('\n***ERROR: ' + img_name)
                print('\ncompressed version differ')
                print('command_line: ' + command_line + '\n')
        if residuals_folder.has_key(img_name):
            result = os.popen('diff -q residuals_compressed.fl.bip ' + os.path.join(residual_images, residuals_folder[img_name])).readlines()
            if result:
                print('\n***ERROR: ' + img_name)
                print('\nresidual version differ')
                if not error:
                    print('command_line: ' + command_line + '\n')
                else:
                    print('\n')
                if command_line_conv:
                    print('command_line_conv: ' + command_line_conv + '\n')
                error = True

        if not error:
            print('Compression of ' + img_name + ' correctly executed\n')
        
        # Now let's move to test the decompressor
        command_line_dec = '--input ' + os.path.join(compressed_images, compressed_folder[img_name]) + ' --output decompressed.raw --dump_residuals --out_format '
        if row[29] == 'BIP':
            command_line_dec = command_line_dec + 'BI --out_depth ' + row[4]
        else:
            command_line_dec = command_line_dec + 'BSQ'
        if row[28] == 'L':
            command_line_dec = command_line_dec + ' --out_byte_ordering LITTLE'
        else:
            command_line_dec = command_line_dec + ' --out_byte_ordering BIG'

        result = os.popen(decompressor_exec + ' ' + command_line_dec + ' 2>&1').readlines()
        error = False
        for i in result:
            if 'error' in i.lower():
                print('\n***ERROR - Decompressor: ' + img_name)
                print('\n'.join(result))
                error = True
                break
        if not result:
            print('\n***ERROR - Decompressor: ' + img_name)
            print('\nNo output has been produced')
            error = True
        if error:
            print('command_line: ' + command_line_dec + '\nencoder: ' + command_line + '\n')
            continue
        else:
            print('RESULT - Decompressor: ' + img_name)
            print('\n'.join(result))
        # Let's now check that decompression was correctly executed
        result = os.popen('diff -q decompressed.raw ' + os.path.join(orig_images, orig_folder[img_name])).readlines()
        if result:
            error = True
            print('\n***ERROR - Decompressor: ' + img_name)
            print('\ndecompressed version differ')
            print('command_line: ' + command_line_dec + '\nencoder: ' + command_line + '\n')
        command_line_conv = ''
        if row[16] != 'BI' or (row[16] == 'BI' and row[17] != row[4]):
            command_line_conv = '--input residuals_decompressed.raw.bip --output residuals_decompressed.raw.bip --rows ' + row[3] + ' --columns ' + row[2] + ' --bands ' + row[4]
            command_line_conv = command_line_conv + ' --in_format BI --in_depth ' + row[4] + ' --in_byte_ordering LITTLE --dyn_range 16 --out_format ' + row[16]
            litteral = False
            try:
                int(row[17])
            except ValueError:
                litteral = True
            if not litteral:
                command_line_conv = command_line_conv + ' --out_depth ' + row[17]
            os.popen(os.path.join(os.path.split(compressor_exec)[0], 'converter') + ' ' + command_line_conv + ' 2>&1')
        result = os.popen('diff -q residuals_decompressed.raw.bip ' + os.path.join(residual_images, residuals_folder[img_name])).readlines()
        if result:
            error = True
            print('\n***ERROR - Decompressor: ' + img_name)
            print('\ndecompressed residuals differ')
            print('command_line: ' + command_line_dec + '\nencoder: ' + command_line + '\n')
            if command_line_conv:
                print('command_line converter: ' + command_line_conv + '\n')
            
        if not error:
            print('Decompression of ' + img_name + ' correctly executed\n')

        sys.stdout.flush()
