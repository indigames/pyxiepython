"""
launch_server.py

access pyxieLauncher apprication server
"""

import requests
import json
import hashlib
import os.path
import glob

LAUNCH_SERVER_URL = 'https://pyxieapp.appspot.com'


def mkdir(dir,name):
    """
    Make a directory on the server

    :param dir:  parent directory name
    :param name: directory to create
    :return: 0 if success , not 0 if error
    """
    url = LAUNCH_SERVER_URL + '/mkdir'
    param = {'directory':dir,'name':name}
    try:
        response = requests.post(url, param)
    except:
        print('mkdir ; network error')
        return -1

    if response.status_code != 200:
        print('mkdir ; network error')
        return -1
    return 0


def ls(dir):
    """
    Lists directory contents of files and directories on the server

    :param dir: directory path
    :return: files, err
              files : {filename : {type:filetype, hash:hashvalue, sizse:filesize}}
               err: not 0 if error occurred
    """
    url = LAUNCH_SERVER_URL + '/ls'
    param = {'directory': dir}
    try:
        response = requests.post(url, param)
    except:
        return None, -1
    if response.status_code == 200:
        list = json.loads(response.text)
        return list,0
    return None, -1

def download_files(src,dst):
    """
    Download all files in the specified directory on the server

    :param src: server directory path
    :param dst: local direcgtory path to save files
    :return: not 0 if error occurred
    """
    list,err = ls(src)
    if list is None: return err

    url = LAUNCH_SERVER_URL+'/download'

    for key, val in list.items():
        localHash = ""
        path = dst+'/'+key
        if os.path.exists(path):
            with open(path, 'rb') as f:
                fileDataBinary = open(path, 'rb').read()
                if len(fileDataBinary) != 0:
                    hash = hashlib.md5()
                    hash.update(fileDataBinary)
                    localHash = hash.hexdigest()

        if localHash != val['hash']:
            param = {'directory':src,'file':key}

            try:
                response = requests.post(url, param)
            except:
                print('download {} was failed', key)
                return -1

            if response.status_code == 200:
                with open(path, 'wb') as f:
                    f.write(response.content)
        else:
            print('download {} was skipped because no change', key)
    return 0


def upload_files(src,dst):
    url = LAUNCH_SERVER_URL + '/ls'
    param = {'directory': dst}
    response = requests.post(url, param)

    serverFiles = {}
    if response.status_code == 200:
        serverFiles = json.loads(response.text)

    localFiles = []
    list = glob.glob(os.path.join(src, '*'))
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
            url = LAUNCH_SERVER_URL + '/upload'
            XLSX_MIMETYPE = 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'
            files = {'file': (f, fileDataBinary, XLSX_MIMETYPE)}
            param = {'directory': dst, 'hash': hashcode}

            try:
                response = requests.post(url, param, files=files)
            except:
                print('upload {} was failed', f)
                return -1

            if response.status_code == 200:
                print(path + " uploaded.")
        else:
            print(path + " is not changed.")

    return 0