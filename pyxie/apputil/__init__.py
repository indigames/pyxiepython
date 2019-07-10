"""
pyxie game engine \n
apprication utilities
"""
import pyxie
import requests
import json
import hashlib
import os.path

PYXIE_DEV_URL = 'https://pyxieapp.appspot.com'

def createSprite(width:float=100, height:float=100, texture:str=None, uv_left_top:tuple=(0,0), uv_right_bottom:tuple=(1,1)):
    """
    Create Visible 2D Rectangle \n
    return : editableFigure
    """
    gen = pyxie.shaderGenerator()
    if texture != None:
        gen.setColorTexture(True)
    gen.setBoneCondition(1, 1)

    efig = pyxie.editableFigure("sprite")
    efig.addMaterial("mate", gen)
    efig.addMesh("mesh", "mate");

    w = width/2
    h = height/2
    points = (-w,h, w,h, -w,-h, w,-h)
    uvs = (0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0)
    tris = (0, 2, 1, 1, 2, 3)

    efig.setVertexElements("mesh", efig.VertexAttribure_POSITION, 0, 4, points, 2)
    efig.setVertexElements("mesh", efig.VertexAttribure_UV0, 0, 4, uvs, 2)
    efig.setTriangles("mesh", 0, 2, tris);
    efig.addJoint("joint");
    efig.setMaterialParam("mate", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));

    if texture != None:
        efig.setMaterialParamTexture("mate", "ColorSampler", texture,
                                     wrap_s=pyxie.SAMPLERSTATE_BORDER,wrap_t=pyxie.SAMPLERSTATE_BORDER,
                                     minfilter=pyxie.SAMPLERSTATE_NEAREST, magfilter=pyxie.SAMPLERSTATE_NEAREST)
        efig.setMaterialRenderState("mate", "blend_enable", True)
    return efig

def createBox(points):
    """
    Create Visible 2D Rectangle \n
    return : editableFigure
    """

    gen = pyxie.shaderGenerator()
    gen.setBoneCondition(1, 1)
    gen.setCalcBinormalInShader(True)

    efig = pyxie.editableFigure("box")
    efig.addMaterial("mate01", gen)
    efig.addMesh("mesh01", "mate01");

    efig.setVertexElements("mesh01",efig.VertexAttribure_POSITION, 0, 4, points, 1)

    tris = (0,1,3,  1,2,3)
    efig.setTriangles("mesh01", 0, 2, tris);
    efig.addJoint("joint01");
    efig.setMaterialParam("mate01", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));

    return efig

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

def devserver_ls(dir):
    url = PYXIE_DEV_URL + '/ls'
    param = {'directory': dir}
    response = requests.post(url, param)
    list = None
    if response.status_code == 200:
        list = json.loads(response.text)
    return list

def devserver_download(src,dst):
    list = devserver_ls(src)
    if list is None: return
    url = PYXIE_DEV_URL+'/download'

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
            response = requests.post(url, param)
            if response.status_code == 200:
                with open(path, 'wb') as f:
                    f.write(response.content)




