"""
pyxie game engine offline develop tools

convert 3d assets
convert images to platform dipend format
make pyxie package
upload package to devserver

"""
from pyxie.devtool._pyxietools import *
import os.path
import pyxie
import py_compile
import requests
import hashlib
import json
import shutil
import glob

TARGET_PLATFORM_PC = 0
TARGET_PLATFORM_IOS = 1
TARGET_PLATFORM_ANDROID = 2

URL = 'https://pyxieapp.appspot.com'

def makeDirectories(dir):
    path = os.path.normpath(dir)

    list = []
    while os.path.exists(path)==False:
        if path == '':
            break
        path,tail = os.path.split(path)
        if tail =='' and tail.find('.') != -1:
            list.append(path)
            break
        else:
            list.append(tail)

    i =len(list)-1
    while i >= 0:
        name, ext = os.path.splitext(list[i])
        if ext != '':
            path = os.path.join(path, list[i])
            path = os.path.normpath(path)
            break
        else:
            path = os.path.join(path, list[i])
            path = os.path.normpath(path)
            os.mkdir(path)
        i-=1

def findImages(path):
    list =[]
    for root, dirs, files in os.walk(path):
       if root.find('\.') != -1: continue
       for fname in files:
            name,ext = os.path.splitext(fname)
            if ext=='.png' or ext=='.dds' or ext=='.tga' or ext=='.psd':
                list.append(os.path.join(root, fname))
    return list

def findConvertSet(path):
    dict = {}
    for root, dirs, files in os.walk(path):
        if root.find('\.') != -1: continue
        folder = os.path.basename(root)
        baseFile = folder + '.dae'

        folderRule = False
        for f in files:
            if f == baseFile:
                folderRule = True
                break

        if folderRule:
            daes = [os.path.join(root, baseFile)]
            for f2 in files:
                f2_name, f2_ext = os.path.splitext(f2)
                if f2_ext == '.dae' and f2_name != folder:
                    daes.append(os.path.join(root, f2))
            dict[folder] = daes
        else:
            for f2 in files:
                f2_name, f2_ext = os.path.splitext(f2)
                if f2_ext == '.dae':
                    dict[folder] = [os.path.join(root, fname)]
    return dict

def replaceExt(file, ext):
    name, extold = os.path.splitext(file)
    return name+ext

def platformName(platform):
    if platform == TARGET_PLATFORM_PC:
        return 'pc'
    if platform == TARGET_PLATFORM_IOS:
        return 'ios'
    if platform == TARGET_PLATFORM_ANDROID:
        return 'android'

def getFiles(path):
    list =[]
    for root, dirs, files in os.walk(path):
        if root.find('\.') != -1: continue
        if root == '__pycache__': continue
        for fname in files:
            list.append(os.path.join(root, fname))
    return list

def checkFileType(file):
    name, ext = os.path.splitext(file)
    extTypes = (
        ( '.fbx','skp'),
        ( '.mb','skp'),
        ( '.ma','skp'),
        ( '.dae','mdl'),
        ( '.png','img'),
        ('.dds','img'),
        ('.tga','img'),
        ('.psd','img'),
        ('.py', 'src'),
        ('.pyxi','skp'),
        ('.pyxd', 'skp'))
    for t in extTypes:
        if t[0] == ext:
            return t[1]
    return 'cpy'

def findFile(imageDirectory, file, dir):
    files = imageDirectory[dir]
    if files is None: return None
    if file in files:
        return os.path.join(dir,file)
    for d in files:
        _,ext = os.path.splitext(d)
        if ext is '':
            rv = findFile(imageDirectory, file, os.path.join(dir, d))
            if rv: return rv
    return None

def createDiorectoryTree(filelist, root):
    imageDirectory = dict()
    for img in filelist:
        while True:
            img, file = os.path.split(img)
            if img not in imageDirectory:
                imageDirectory[img] = {file:0};
            elif file not in imageDirectory[img]:
                imageDirectory[img][file] = 0
            if img == root: break
    return imageDirectory

def removeRoot(path, root):
    newpath = path.replace(root, '', 1)
    if newpath[0] == '/' or newpath[0] == '\\':
        newpath = newpath[1:]
    return newpath

def build(src,dst,target, unit):
    list = getFiles(src)
    for infile in list:
        type = checkFileType(infile)
        if type == 'cpy':
            outfile = os.path.normpath(infile.replace(src, dst, 1))
            makeDirectories(outfile)
            shutil.copy(infile, outfile)
        elif type == 'src':
            outfile = os.path.normpath(infile.replace(src, dst, 1))
            outfile = replaceExt(outfile, '.pyc')
            makeDirectories(outfile)
            py_compile.compile(infile, outfile)

    allimages = findImages(src)
    imageDirectory = createDiorectoryTree(allimages, src)

    daes = findConvertSet(src)
    allTextures = dict()

    tmpfig = pyxie.editableFigure("tmpfigure")
    for key, data in daes.items():
        tmpfig.clear()
        loadCollada(data[0], tmpfig, unit)
        for i in range(1, len(data)):
            loadColladaAnimation(data[i], tmpfig, unit)
        path, file = os.path.split(data[0])

        texs = tmpfig.getTextureSource()
        for tex in texs:
            filename = os.path.basename(tex['path'])
            current = path
            inputimage = None
            while True:
                inputimage = findFile(imageDirectory,filename, current)
                if inputimage is not None: break
                if current == src: break
                current,_ = os.path.split(current)
            if inputimage is None:
                print('file not found '+ tex['path'])
            else:
                allTextures[inputimage] = tex;
                inputimage = inputimage.replace('\\', '/')
                inputimage = removeRoot(inputimage,src);
                newTex = {'path':inputimage, 'normal': tex['normal'], 'wrap': tex['wrap']}
                tmpfig.replaceTextureSource(tex, newTex)

        outfile = os.path.normpath(data[0].replace(src, dst, 1))
        outfile = replaceExt(outfile, '.pyxf')
        makeDirectories(outfile)
        tmpfig.saveFigure(outfile)

    for img in allimages:
        if img not in allTextures:
            newTex = {'path': removeRoot(img,src), 'normal': False, 'wrap': False}
            allTextures[img] = newTex

    for key, val in allTextures.items():
        outfile = os.path.normpath(key.replace(src, dst, 1))
        outfile = replaceExt(outfile, '.pyxi')
        makeDirectories(outfile)
        convertTextureToPlatform(key, outfile, target, val['normal'], val['wrap'])

def pack(root):
    list =glob.glob(os.path.join(root,'*' ))
    for dir in list:
        if os.path.isdir(dir):
            compressFolder(dir)

def mkdir(dir, name):
    url = URL + '/mkdir'
    param = {'directory':dir,'name':name}
    response = requests.post(url, param)
    if response.status_code == 200:
        return True
    return False

def makeProjectDir(user,app,version, platform):
    root = '/'
    if mkdir(root,user):
        root += user
        if mkdir(root,app):
            root += '/'
            root += app
            if mkdir(root,version):
                root += '/'
                root += version
                if mkdir(root,platformName(platform)):
                    return True
    return False

def upload(src, user,app,version, target):

    if makeProjectDir(user, app, version, target) != True:
        print('failed to connect server.')
        return

    addr = '/'+user+'/'+app+'/'+str(version)+'/'+platformName(target)

    url = URL+'/ls'
    param = {'directory':addr}
    response = requests.post(url, param)


    serverFiles = {}
    if response.status_code == 200:
        serverFiles = json.loads(response.text)

    localFiles = []
    list =glob.glob(os.path.join(src,'*' ))
    for f in list:
        if os.path.isfile(f):
            localFiles.append(f)
    for path in localFiles:
        fileDataBinary = open(path, 'rb').read()
        if len(fileDataBinary) == 0: continue
        hash = hashlib.md5()
        hash.update(fileDataBinary)
        hashcode = hash.hexdigest()

        f = os.path.basename(path)
        if f not in serverFiles or serverFiles[f]['hash'] != hashcode:
            url = URL+'/upload'
            XLSX_MIMETYPE = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'
            files = {'file': (f, fileDataBinary, XLSX_MIMETYPE)}
            param = {'directory':addr, 'hash':hashcode}
            response = requests.post(url, param, files=files)
            if response.status_code == 200:
                print(path + " uploaded.")
        else:
            print(path + " is not changed.")

def deploy(projectFolder, userID, appName, appVersion, unit = 1.0):

    '''
    Convert all assets to platform dipend format and upload to the pyxie develop server
    All intermediate files are stored in the '.tmp' folder

    projectFolder : root path of project to convert.
    userID : Your ID
    appName : apprication name
    appVersion : apprication version (don't use period)
    unit : 3d model's base scale unit. you can adjust 3d model scale.
    '''
    unit = 1.0

    target = TARGET_PLATFORM_PC
    dst = '.tmp/stage/'+appName+'/'+platformName(target)
    build(projectFolder,dst,target,unit);
    pack(dst)
    upload(dst, userID,appName,appVersion, target)
