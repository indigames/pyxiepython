"""
pyxie game engine \n
apprication utilities
"""
import pyxie
import os.path
import pyvmath as vmath


def createSprite(width:float=100, height:float=100, texture:str=None, uv_left_top:tuple=(0,0), uv_right_bottom:tuple=(1,1), normal=None):
    """
    Create Visible 2D Rectangle

    :param width: width of sprite
    :param height: height of sprite
    :param texture: texture file name
    :param uv_left_top: texture uv value of left top cornar
    :param uv_right_bottom: texture uv value of right bottom cornar
    :return: editableFigure
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
    points = ((-w,h,0.0), (w,h,0.0), (-w,-h,0.0), (w,-h,0.0))
    uvs = (0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0)
    tris = (0, 2, 1, 1, 2, 3)

    if normal is not None:
        newpoints = []
        nom0 = (0, 0, 1)
        mat = vmath.mat33(vmath.quat_rotation(nom0, normal))
        for p in points:
            newpoints.append(mat * p)
        points = newpoints

    efig.setVertexElements("mesh", efig.VertexAttribure_POSITION, points)
    efig.setVertexElements("mesh", efig.VertexAttribure_UV0,uvs)
    efig.setTriangles("mesh", tris);
    efig.addJoint("joint");
    efig.setMaterialParam("mate", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));
    #efig.setMaterialRenderState("mate", "cull_face_enable", False)

    if texture != None:
        efig.setMaterialParamTexture("mate", "ColorSampler", texture,
                                     wrap_s=pyxie.SAMPLERSTATE_BORDER,wrap_t=pyxie.SAMPLERSTATE_BORDER,
                                     minfilter=pyxie.SAMPLERSTATE_NEAREST, magfilter=pyxie.SAMPLERSTATE_NEAREST)
        efig.setMaterialRenderState("mate", "blend_enable", True)
    return efig

def createBox(points):
    """
    Create Visible 2D Rectangle \n
    :param points:
    :return: editableFigure
    """

    gen = pyxie.shaderGenerator()
    gen.setBoneCondition(1, 1)
    gen.setCalcBinormalInShader(True)

    efig = pyxie.editableFigure("box")
    efig.addMaterial("mate01", gen)
    efig.addMesh("mesh01", "mate01");

    efig.setVertexElements("mesh01",efig.VertexAttribure_POSITION, points)

    tris = (0,1,3,  1,2,3)
    efig.setTriangles("mesh01", 0, 2, tris);
    efig.addJoint("joint01");
    efig.setMaterialParam("mate01", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));

    return efig

def makeDirectories(dir):
    """
    Make local directory recursive
    :param dir: file path to make directory
    :return: None
    """
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

def platformName(platform):
    """
    Return the name corresponding to the platform type

    :param platform: pyxie.TARGET_PLATFORM_XX
    :return: string
    """
    if platform == pyxie.TARGET_PLATFORM_PC:
        return 'pc'
    if platform == pyxieTARGET_PLATFORM_IOS:
        return 'ios'
    if platform == pyxieTARGET_PLATFORM_ANDROID:
        return 'android'
    return 'unknown'

