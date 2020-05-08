'''
A generic file-copy asset job.
'''
Version = 2

import subprocess
import shutil
import os
import platform
from mod import log

def file_is_dirty(src, dst) :
    if not os.path.exists(dst) :
        return True
    inputTime = os.path.getmtime(src)
    outputTime = os.path.getmtime(dst)
    if inputTime > outputTime :
        return True
    return False

#-------------------------------------------------------------------------------
def copy_files(src_dir, dst_dir):
    if not os.path.exists(dst_dir):
        os.makedirs(dst_dir)
    for o in os.listdir(src_dir) :
        f = os.path.join(src_dir, o)
        if os.path.isdir(f) :
            copy_files(f, dst_dir + f.split('/')[-1])
            continue
        ext = f.split('.')[-1].lower()        
        if ext == 'png' or ext == 'tga' or ext == 'h':
            continue
        if "\\" in f:
            dst = dst_dir + '/' + f.split('\\')[-1]
        else:
            dst = dst_dir + '/' + f.split('/')[-1]
        if file_is_dirty(f, dst) :
            shutil.copyfile(f, dst)
            log.info('copy from: ' + f)
            log.info('to: ' + dst)
            global gen_Header
            gen_Header = True
    # check for deleted files
    src_files = os.listdir(src_dir)
    dst_files = os.listdir(dst_dir)
    for o in [x for x in dst_files if x not in src_files] :
        f = os.path.join(dst_dir, o)
        if os.path.exists(f) :
            if os.path.isdir(f) :
                os.rmdir(f)
            else :
                os.remove(f)            
            gen_Header = True            

#-------------------------------------------------------------------------------
def gen_header(out_src, out_hdr):    
    # Extract the version number and plus it one to make a new apk
    if os.path.exists(out_hdr) :
        global Version
        with open(out_hdr, 'r') as f:
            for i in range(0,4) :
                line = f.readline()
                startIndex = line.find('#version:')
                if startIndex != -1 :
                    endIndex = line.find('#', startIndex + 9)
                    if endIndex != -1 :
                        versionNumber = line[startIndex + 9 : endIndex]
                        Version = int(versionNumber) + 1

    with open(out_hdr, 'w') as f:
        f.write('#pragma once\n')
        f.write('//------------------------------------------------------------------------------\n')
        f.write('// #version:{}#\n'.format(Version))
        f.write('// machine generated, do not edit!\n')
        f.write('//------------------------------------------------------------------------------\n')
        f.write('// JUST A DUMMY FILE, NOTHING TO SEE HERE\n')

    with open(out_src, 'w') as f:
        f.write('#include "{}"\n'.format(out_hdr.split('/')[-1]))
        f.write('//------------------------------------------------------------------------------\n')
        f.write('// #version:{}#\n'.format(Version))
        f.write('// machine generated, do not edit!\n')
        f.write('//------------------------------------------------------------------------------\n')
        f.write('// JUST A DUMMY FILE, NOTHING TO SEE HERE\n')
        f.write('int Version = {};\n'.format(Version))

#-------------------------------------------------------------------------------
def generate(input, out_src, out_hdr, args):
    s = 'fips-build' if 'fips-build' in args['deploy_dir'] else 'fips-deploy'
    p = args['deploy_dir'].split(s)[1].split('/')[1]
    src_root_path = args['cur_dir'] + '/assets/'
    if s == 'fips-build' :
        # making for android
        deploy_dir = args['deploy_dir'] + args['cur_dir'].split(p)[1] + '/android/' + args['app_name'] + '/assets/'
        if not os.path.exists(args['deploy_dir'] + args['cur_dir'].split(p)[1] + '/assets') :
            os.mkdir(args['deploy_dir'] + args['cur_dir'].split(p)[1] + '/assets')
    else :
        # for other platforms
        deploy_dir = args['deploy_dir'] + args['cur_dir'].split(p)[1] + '/assets/'
    if not os.path.exists(src_root_path) :
        log.info('No assets in ' + src_root_path)
        return        
    copy_files(src_root_path, deploy_dir)   
    if gen_Header :
        gen_header(out_src, out_hdr)

gen_Header = False
