
//shaderGeneragtor
PyDoc_STRVAR(shaderGeneragtor_doc,
	"shaderGenerator is abstracted to be compatible\n"\
	"with multiple platforms by replacing \n"\
	"shader code with simple procedure\n"\
	"You can see the current shader code with print (shaderGeneragtor)");

//setBoneCondition
PyDoc_STRVAR(setBoneCondition_doc,
	"Specifys the number of  bone palette and number of bone influencen\n"\
	"\n"\
	"shaderGenerater.setBoneCondition(numBoneInfluence, numBonePalette)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    numBoneInfluence : int\n"\
	"        Number of bones that one vertex refers to simultaneously\n"\
	"    numBonePalette : int\n"\
	"        Number of bones to put in the bones palette\n"\
	"        What is meaningful at present is only whether it is zero or not");

//setSpecular
PyDoc_STRVAR(setSpecular_doc,
	"Enable or disable specular reflection\n"\
	"\n"\
	"shaderGenerater.setSpecular(enableSpecular, textureMapChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableSpecular : bool\n"\
	"        Enable or disable specular reflection\n"\
	"    textureMapChannel : int\n"\
	"        Specify Specular strength map\n"\
	"        The following values can be selected\n"\
	"        pyxie.MAPCHANNEL_NONE\n"\
	"        pyxie.MAPCHANNEL_COLOR_RED\n"\
	"        pyxie.MAPCHANNEL_COLOR_ALPHA\n"\
	"        pyxie.MAPCHANNEL_NORMAL_RED\n"\
	"        pyxie.MAPCHANNEL_NORMAL_ALPHA\n"\
	"        pyxie.MAPCHANNEL_VERTEX_COLOR_RED\n"\
	"        pyxie.MAPCHANNEL_VERTEX_COLOR_ALPHA");


//setAmbientOcclusion
PyDoc_STRVAR(setAmbientOcclusion_doc,
	"Enable or disable ambient occlusion mapping.\n"\
	"\n"\
	"shaderGenerater.setAmbientOcclusion(textureMapChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    textureMapChannel : int\n");


//setColorTextureUVChannel
PyDoc_STRVAR(setColorTextureUVChannel_doc,
	"Specifies the UV channel of the color map texture\n"\
	"RGB and A can be specified separately\n"\
	"Supports 3 channels of UV0, UV1and UV2\n"\
	"\n"\
	"shaderGenerater.setColorTextureUVChannel(rgbChannel, aChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    rgbChannel : int\n"\
	"        uv channel no (0, 1, 2)\n"\
	"    aChannel : int\n"\
	"        uv channel no (0, 1, 2)\n");



//setNormalTextureUVChannel
PyDoc_STRVAR(setNormalTextureUVChannel_doc,
	"Specifies the UV channel of the normal map texture\n"\
	"RGBand A can be specified separately\n"\
	"Supports 3 channels of UV0, UV1and UV2\n"\
	"\n"\
	"shaderGenerater.setNormalTextureUVChannel(rgbChannel, aChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    rgbChannel : int\n"\
	"        uv channel no (0, 1, 2)\n"\
	"    aChannel : int\n"\
	"        uv channel no (0, 1, 2)\n");

//setLightTextureUVChannel
PyDoc_STRVAR(setLightTextureUVChannel_doc,

	"Specifies the UV channel of the light map texture\n"\
	"RGBand A can be specified separately\n"\
	"Supports 3 channels of UV0, UV1and UV2\n"\
	"\n"\
	"shaderGenerater.setLightTextureUVChannel(rgbChannel, aChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    rgbChannel : int\n"\
	"        uv channel no (0, 1, 2)\n"\
	"    aChannel : int\n"\
	"        uv channel no (0, 1, 2)\n");


//setOverlayColorTextureUVChannel
PyDoc_STRVAR(setOverlayColorTextureUVChannel_doc,

	"Specifies the UV channel of the overlay color map texture\n"\
	"RGBand A can be specified separately\n"\
	"Supports 3 channels of UV0, UV1and UV2\n"\
	"\n"\
	"shaderGenerater.setOverlayColorTextureUVChannel(rgbChannel, aChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    rgbChannel : int\n"\
	"        uv channel no (0, 1, 2)\n"\
	"    aChannel : int\n"\
	"        uv channel no (0, 1, 2)\n");

//setOverlayNormalColorTextureUVChannel
PyDoc_STRVAR(setOverlayNormalColorTextureUVChannel_doc,

	"Specifies the UV channel of the overlay normal map texture\n"\
	"RGBand A can be specified separately\n"\
	"Supports 3 channels of UV0, UV1and UV2\n"\
	"\n"\
	"shaderGenerater.setOverlayNormalColorTextureUVChannel(rgbChannel, aChannel)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    rgbChannel : int\n"\
	"        uv channel no (0, 1, 2)\n"\
	"    aChannel : int\n"\
	"        uv channel no (0, 1, 2)\n");

//setCalcBinormalInShader
PyDoc_STRVAR(setCalcBinormalInShader_doc,
	"Whether to calculate Binormal values used in \n"\
	"normal mapping calculations in the shader\n"\
	"\n"\
	"shaderGenerater.setCalcBinormalInShader(calcBinormal)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    calcBinormal : bool\n");



//setNumDirLamp
PyDoc_STRVAR(setNumDirLamp_doc,
	"Specifies the number of directional light sources affected.\n"\
	"You can specify up to 3\n"\
	"\n"\
	"shaderGenerater.setNumDirLamp(numLamp)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    numLamp : int\n"\
	"        Number of directional light (0, 1, 2, 3)\n");

	
//setNumPointLamp
PyDoc_STRVAR(setNumPointLamp_doc,
	"Specifies the number of point light sources affected.\n"\
	"You can specify up to 7\n"\
	"\n"\
	"shaderGenerater.setNumPointLamp(numLamp)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    numLamp : int\n"\
	"        Number of point light (0 - 7)\n");


//setAmbientType
PyDoc_STRVAR(setAmbientType_doc,
	"Specifies the ambient light type\n"\
	"\n"\
	"shaderGenerater.setAmbientType(type)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    type : int \n"\
	"        Ambient light type\n"\
	"        pyxie.AMBIENT_TYPE_NONE\n"\
	"        pyxie.AMBIENT_TYPE_AMBIENT\n"\
	"        pyxie.AMBIENT_TYPE_HEMISPHERE\n");



//setClutLamp
PyDoc_STRVAR(setClutLamp_doc,
	"Enable or disable color look up table lighting\n"\
	"Useful for expressing cartoon lighting\n"\
	"\n"\
	"shaderGenerater.setClutLamp(enableCLUT)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableCLUT : bool\n");

//setVertexColor
PyDoc_STRVAR(setVertexColor_doc,
	"Enable or disable vertex color\n"\
	"\n"\
	"shaderGenerater.setVertexColor(enableVertexColor)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableVertexColor : bool\n");


//setVertexAlpha
PyDoc_STRVAR(setVertexAlpha_doc,
	"Whether to references the vertex color alpha channel as a transparent source\n"\
	"\n"\
	"shaderGenerater.setVertexAlpha(enableVertexAlpha)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableVertexAlpha : bool\n");


//setColorTexture
PyDoc_STRVAR(setColorTexture_doc,
	"Whether to enable color texture\n"\
	"\n"\
	"shaderGenerater.setColorTexture(enableTexture)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableTexture : bool\n");


//setNormalTexture
PyDoc_STRVAR(setNormalTexture_doc,
	"Whether to enable normal texture\n"\
	"\n"\
	"shaderGenerater.setNormalTexture(enableTexture)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableTexture : bool\n");



//setLightTexture
PyDoc_STRVAR(setLightTexture_doc,
	"Whether to enable light texture\n"\
	"\n"\
	"shaderGenerater.setLightTexture(enableTexture)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableTexture : bool\n");


//setOverlayColorTexture
PyDoc_STRVAR(setOverlayColorTexture_doc,
	"Whether to enable overlay color texture\n"\
	"\n"\
	"shaderGenerater.setOverlayColorTexture(enableTexture)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableTexture : bool\n");

//setOverlayNormalTexture
PyDoc_STRVAR(setOverlayNormalTexture_doc,
	"Whether to enable overlay normal texture\n"\
	"\n"\
	"shaderGenerater.setOverlayNormalTexture(enableTexture)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableTexture : bool\n");

	
//setOverlaySpecularTexture
PyDoc_STRVAR(setOverlaySpecularTexture_doc,
	"Whether to enable overlay specular texture\n"\
	"\n"\
	"shaderGenerater.setOverlaySpecularTexture(enableSpecular)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableSpecular : bool\n");

//setOverlayVertexAlpha
PyDoc_STRVAR(setOverlayVertexAlpha_doc,
	"Whether to refer to the alpha channel of the vertex color \n"\
	"when calculating overlay map transparency\n"\
	"\n"\
	"shaderGenerater.setOverlayVertexAlpha(enableVertexAlpha)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    enableVertexAlpha : bool");
