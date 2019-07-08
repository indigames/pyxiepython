"""
pyxie game engine \n
apprication utilities
"""
import pyxie

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

