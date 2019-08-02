//animator
PyDoc_STRVAR(animator_doc,
	"animator is a key frame animation data object \n"\
	"\n"\
	"Constructor\n"\
	"----------\n"\
	"    anim = pyxie.animator(path)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    path : string"
	"        animation file path");



//loop
PyDoc_STRVAR(loop_doc,
	"Whether to play loop\n"\
	"\n"\
	"type : bool\n"\
	"(read/write)");

//evalTime
PyDoc_STRVAR(evalTime_doc,
	"Current animation frame second\n"\
	"\n"\
	"type : float\n"\
	"(read / write)");


	//totalEvalTime
PyDoc_STRVAR(totalEvalTime_doc,
	"Cumulative animation frame second\n"\
	"\n"\
	"type : float\n"\
	"(read / write)");


//startTime
PyDoc_STRVAR(startTime_doc,
	"Animation start time second(Loop back point)\n"\
	"\n"\
	"type : float\n"\
	"(read / write)");

	
//endTime
PyDoc_STRVAR(endTime_doc,
	"Animation end time second\n"\
	"\n"\
	"type : float	\n"\
	"(read / write)");

//speed
PyDoc_STRVAR(speed_doc,
	"Animation play speed\n"\
	"\n"\
	"type : float	\n"\
	"(read / write)\n"\
	"(default 1.0)");

//defaultEndTime
PyDoc_STRVAR(defaultEndTime_doc,
	"Default animation end time second\n"\
	"\n"\
	"type : float\n"\
	"(read only)");


//elapsedTime
PyDoc_STRVAR(elapsedTime_doc,
	"Time from previous frame to current frame\n"\
	"type : float\n"\
	"(read only)");

//rewind
PyDoc_STRVAR(rewind_doc,
	"Return evalTime to start time\n"\
	"\n"\
	"animator.rewind()");

