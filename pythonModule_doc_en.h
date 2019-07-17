

//getElapsedTime
PyDoc_STRVAR(getElapsedTime_doc,
	"Get elapsed time from former frame\n"\
	"\n"\
	"pyxie.getElapsedTime()\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"elapsedTime : float");

//swap
PyDoc_STRVAR(swap_doc,
	"Swap GDI frame buffers and disolay new frame image.");


//window
PyDoc_STRVAR(window_doc,
	"Open a new frame window or resize existing window\n"\
	"on window system OS(Windows, osx, linux)\n"\
	"\n"\
	"This function is ignored on the smartphone\n"\
	"\n"\
	"pyxie.window(show, width, hight)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"show : bool\n"\
	"    show the window if true, hide it if false\n"\
	"width : int\n"\
	"    The physical width of the window\n"\
	"height : int\n"\
	"    The physical height of the window");


//singleTouch
PyDoc_STRVAR(singleTouch_doc,
	"Get screen touch information\n"\
	"\n"\
	"touchInfo = pyxie.singleTouch(fingerNo)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"fingerNo : int\n"\
	"    What number finger touched the screen\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"touchInfo : dict\n"\
	"    Dictionary containing touch information\n"\
	"    'id' : int\n"\
	"        Finger id\n"\
	"    'org_x' : int\n"\
	"        Pressed x position\n"\
	"    'org_y' : int\n"\
	"        Pressed y position\n"\
	"    'cur_x' : int\n"\
	"        Current x position\n"\
	"    'cur_y' : int\n"\
	"        Current y position\n"\
	"    'delta_x' : int\n"\
	"        Delta x position\n"\
	"    'delta_y' : int\n"\
	"        Delta y position\n"\
	"    'delta_t' : int\n"\
	"        Delta time\n"\
	"    'elapsed_t' : int\n"\
	"        Elapsed time\n"\
	"    'num_tap' : int\n"\
	"        Number of consecutive taps\n"\
	"    'is_pressed' : bool\n"\
	"        is pressd or not\n"\
	"    'is_holded' : bool\n"\
	"        is holded or not\n"\
	"    'is_released' : bool\n"\
	"        is released or not\n"\
	"    'is_tapped' : bool\n"\
	"        is tapped or not\n"\
	"    'is_flicked' : bool\n"\
	"        is flicked or not\n"\
	"    'is_moved' : bool\n"\
	"        is moved or not\n"\
	"    'is_longpressed' : bool\n"\
	"        is longpressed or not\n");


//viewSize
PyDoc_STRVAR(viewSize_doc,
	"Returns the virtual size of the screen\n"\
	"\n"\
	"The virtual size does not depend on the physical resolution.\n"\
	"The default value  of virtual size is 500.0 for \n"\
	"the longer side in the verticaland horizontal directions, \n"\
	"and the other side is determined by the aspect ratio.\n"\
	"\n"\
	"(w,h) = pyxie.viewSize()\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"w : float\n"\
	"    Width of virtual screen size\n"\
	"h : float\n"\
	"    Height of virtual screen size");

//setViewLength
PyDoc_STRVAR(setViewLength_doc,
	"Set virtual screen size\n"\
	"\n"\
	"Set the value of the longer side of vertical or horizontal\n"\
	"\n"\
	"pyxie.setViewLength(length)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"length : float\n"\
	"    length of virtual screen\n");

//setRoot
PyDoc_STRVAR(setRoot_doc,
	"Set the system root directory\n"\
	"\n"\
	"you should not change it basically\n"\
	"\n"\
	"pyxie.setRoot(path)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"path : string\n"\
	"    Full path of system root directory.");

//getRoot
PyDoc_STRVAR(getRoot_doc,
	"Get the system root directory\n"\
	"\n"\
	"path = pyxie.getRoot()\n"\
	"\n"\
	"Returns\n"\
	"-------\n"\
	"path : string\n"\
	"    Full path of system root directory.");

