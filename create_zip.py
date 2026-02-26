#!/usr/bin/env python3
import zipfile
import os

def create_zip():
    zip_filename = 'FICHIER VOLCANO CORRIGER V2.zip'
    source_dir = 'MOD-VOLCANO'

    with zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED) as zf:
        for root, dirs, files in os.walk(source_dir):
            for file in files:
                file_path = os.path.join(root, file)
                arcname = os.path.relpath(file_path, os.path.dirname(source_dir))
                zf.write(file_path, arcname)
                print(f'Added: {arcname}')

    print(f'\nZIP file created successfully: {zip_filename}')
    # Count files in ZIP
    with zipfile.ZipFile(zip_filename, 'r') as zf:
        print(f'Total files in ZIP: {len(zf.namelist())}')

if __name__ == '__main__':
    create_zip()
