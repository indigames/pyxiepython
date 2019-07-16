"""
graphicsHelper.py

Functions to help build graphics data
"""

import pyxie
import pyvmath as vmath

def createSprite(width:float=100, height:float=100, texture:str=None, uv_left_top:tuple=(0,0), uv_right_bottom:tuple=(1,1), normal=None):
    """
    Create Visible Rectangle

    :param width: width of sprite
    :param height: height of sprite
    :param texture: texture file name
    :param uv_left_top: texture uv value of left top cornar
    :param uv_right_bottom: texture uv value of right bottom cornar
    :param normal: specify rectangle's nomal vector
    :return: editableFigure
    """
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

    return createMesh(points, tris, texture, uvs)


def createMesh(points, tris, texture:str=None, uvs = None):
    """
    Create a polygon mesh by specifying vertex coordinates and triangle index

    :param points: list or tuple of points
    :param tris: list or tuple of triangle indices
    :param texture: file path of texture
    :param uvs: list or tuple of  texture uv
    :return: editableFigure
   
    """
    gen = pyxie.shaderGenerator()
    if texture != None:
        gen.setColorTexture(True)
    gen.setBoneCondition(1, 1)

    efig = pyxie.editableFigure("sprite")
    efig.addMaterial("mate", gen)
    efig.addMesh("mesh", "mate");

    efig.setVertexElements("mesh", efig.VertexAttribure_POSITION, points)
    if uvs:
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
