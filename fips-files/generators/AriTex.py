#-------------------------------------------------------------------------------
#   AriTex.py
#
#   Fips code-generator script for invoking texturec during the build.
#
#   Use the cmake macro 'ari_tex([texture-file] [texture-format])' inside a
#   fips target (fips_begin_* / fips_end_*) to hook the code-generation
#   build job into the build process.
#-------------------------------------------------------------------------------

Version = 1

import os, platform, subprocess
import genutil as util
from mod import log

#-------------------------------------------------------------------------------
def find_texc():
    texc_path = os.path.dirname(os.path.abspath(__file__))
    texc_path += '/../../bin/'
    if platform.system() == 'Windows':
        texc_path += 'win32/'
    elif platform.system() == 'Darwin':
        texc_path += 'osx/'
    elif platform.system() == 'Linux':
        if os.uname()[1] == 'raspberrypi':
            texc_path += 'raspi/'
        else:
            texc_path += 'linux/'
    else:
        log.error('Unknown host system {}'.format(platform.system()))
    return texc_path + 'texturec'

#-------------------------------------------------------------------------------
def fileFormatDirty(filePath, fmt) :
    '''
    Reads the first 4 lines of a file, checks whether there's an 
    $$format:X statemenet in it, returns False if the format
    number in the file is equal to the arg format.
    '''
    f = open(filePath, 'r')
    for i in range(0,4) :
        line = f.readline()
        startIndex = line.find('#format:')
        if startIndex != -1 :
            endIndex = line.find('#', startIndex + 8)
            if endIndex != -1 :
                lastfmt = line[startIndex + 8 : endIndex]
                if lastfmt == fmt :
                    return False

    # fallthrough: no format or format doesn't match
    return True

#-------------------------------------------------------------------------------
def generateHeader(hdrPath, fmt) :
    with open(hdrPath, 'w') as f :
        f.write("// #version:{}#\n".format(Version))
        f.write("// #format:{}#\n".format(fmt))

#-------------------------------------------------------------------------------
def generate(input, out_src, out_hdr, args):
    out_tex = input + '.ktx'
    inputs = [input]
    if (os.path.exists(out_tex)) :
        inputs.append(out_tex)
    if util.isDirty(Version, inputs, [out_hdr]) or not os.path.exists(out_tex) or fileFormatDirty(out_hdr, args['texfmt']):
        print('## texurec: {} {}'.format(input, args['texfmt']))
        cmd = [find_texc(), 
                '-f', input,
                '-o', out_tex,
                '-t', args['texfmt'],
                '-m'
                ]
        res = subprocess.call(cmd)
        if res != 0:
            log.error('texturec returned with error code {}'.format(res))
        else:
            generateHeader(out_hdr, args['texfmt'])

