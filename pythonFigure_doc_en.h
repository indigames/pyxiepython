
//position
PyDoc_STRVAR(position_doc,
	"3D position of object \n"\
	"\n"\
	"type :  pyvmath.vec3 : (x,y,z)");

//rotation
PyDoc_STRVAR(rotation_doc,
	"3D rotation of object \n"\
	"\n"\
	"type :  pyvmath.quat : (x,y,z,w)");

//scale
PyDoc_STRVAR(scale_doc,
	"3D scale of object \n"\
	"\n"\
	"type :  pyvmath.vec3 : (x,y,z)");

//connectAnimator
PyDoc_STRVAR(connectAnimator_doc,
	"Apply animation data to the model.\n"\
	"\n"\
	"figure.connectAnimator(slot, anime)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    slot : int\n"\
	"        There are 6 slots, and the final motion is output by the following calculation.\n"\
	"\n"\
	"        A0* A1 + B0 * B1 + C0 * C1\n"\
	"       (* :weighted average , + : Additive synthesis)\n"\
	"\n"\
	"       Slots specify the following values \n"\
	"       pyxie.ANIMETION_SLOT_A0\n"\
	"       pyxie.ANIMETION_SLOT_A1\n"\
	"       pyxie.ANIMETION_SLOT_B0\n"\
	"       pyxie.ANIMETION_SLOT_B1\n"\
	"       pyxie.ANIMETION_SLOT_C0\n"\
	"       pyxie.ANIMETION_SLOT_C1\n"\
	"\n"\
	"    anime : string or pyxie.animator\n"\
	"        Animation name or pyxie.animator object\n"\
	"\n"\
	"        File name of the underlying motion data\n"\
	"\n"\
	"Examples\n"\
	"--------\n"\
	"figure.connectAnimator(pyxie.ANIMETION_SLOT_A0, 'animationA')\n"\
	"figure.connectAnimator(pyxie.ANIMETION_SLOT_A1, 'animationB')\n"\
	"figure.setBlendingWeight(pyxie.ANIMETION_PART_A), 0.5)\n"\
	"\n"\
	"It becomes the middle posture of animationA and animationB");

//getCamera
PyDoc_STRVAR(getCamera_doc,
	"Get camera information included in the scene\n"\
	"\n"\
	"camera = figure.getCamera(cameraName)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    cameraName : string\n"\
	"        Name of camea node\n"\
	"        If omitted, the first camera found is returned.\n"\
	"        It is not necessary to specify a name if the scene contains only one camera\n"\
	"Returns\n"\
	"-------\n"\
	"    camera : pyxie.camera\n"\
	"        camera object");
	
	
//getEnvironment
PyDoc_STRVAR(getEnvironment_doc,
	"Get environmental information included in the scene\n"\
	"Environmental information is information such as lightingand fog.\n"\
	"\n"\
	"env = figure.getEnvironment()\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"    env : pyxie.environment\n"\
	"        enbironment object\n"\
	"\n"\
	" environment object\n"\
	"\n"\
	"Specification of environment is still under construction");

//step
PyDoc_STRVAR(step_doc,
	"Advances the animation applied with connectAnimator() by elapsedTime\n"\
	"\n"\
	"figure.step(elapsedTime)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    elapsedTime : float\n"\
	"        Specifies the delta animation time in seconds\n"\
	"        If omitted, it will be the value of pyxie.elapsedTime()");

//setTime
PyDoc_STRVAR(setTime_doc,
	"Specifies the animation time applied by connectAnimator\n"\
	"\n"\
	"figure.setTime(time)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    time : float\n"\
	"        Specifies the animation time in seconds");
	
//setBlendingWeight
PyDoc_STRVAR(setBlendingWeight_doc,
	"Specifies the weight of the weighted average of animation parts\n"\
	"\n"\
	"figure.setBlendingWeight(part, weight)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    part : int\n"\
	"        Animation part for which you want to specify weights\n"\
	"\n"\
	"		 Specify the following value\n"\
	"        pyxie.ANIMETION_PART_A\n"\
	"        pyxie.ANIMETION_PART_B\n"\
	"        pyxie.ANIMETION_PART_C\n"\
	"\n"\
	"    weight : float\n"\
	"        Specify the weight of weighted average in the range of 0.0 to 1.0");


//getBlendingWeight
PyDoc_STRVAR(getBlendingWeight_doc,
	"Get weight of weighted average of animation part\n"\
	"\n"\
	"weight = figure.getBlendingWeight(part)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    part : int\n"\
	"        Animation part for which you want to obtain a weighted average\n"\
	"\n"\
	"		 Specify the following value\n"\
	"        pyxie.ANIMETION_PART_A\n"\
	"        pyxie.ANIMETION_PART_B\n"\
	"        pyxie.ANIMETION_PART_C\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"    weight\n"\
	"        Weighted average value specified for each part");
