//init
PyDoc_STRVAR(soundInit_doc,
	"init the sound AL system \n"\
	"\n"\
	"sound.init()");

//release
PyDoc_STRVAR(soundRelease_doc,
	"release all sound source + buffer\n"\
	"\n"\
	"sound.release()");

//play
PyDoc_STRVAR(soundPlay_doc,
	"play the sound \n"\
	"\n"\
    "sound.play(filename, loop)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name\n"\
	"    loop : bool (optional)\n"\
	"        when it is True, the sound will play and allow it to loop infinitely");

//stop
PyDoc_STRVAR(soundStop_doc,
	"stop the sound \n"\
	"\n"\
	"sound.stop(filename)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name");

//stopAll
PyDoc_STRVAR(soundStopAll_doc,
	"stop all sound\n"\
	"\n"\
	"sound.stopAll()");

//load
PyDoc_STRVAR(soundLoad_doc,
	"load the sound to cache system\n"\
	"\n"\
	"sound.load(filename)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name");

//unload
PyDoc_STRVAR(soundUnload_doc,
	"unload the sound from cache system\n"\
	"\n"\
	"sound.unload(filename)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name");
    
//unloadUnused
PyDoc_STRVAR(soundUnloadUnused_doc,
	"unload the unused sound from cache system\n"\
	"\n"\
	"sound.unloadUnused()");

//setPositon
PyDoc_STRVAR(soundSetPositon_doc,
	"play the sound \n"\
	"\n"\
    "sound.setPositon(filename, x, y, z)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name\n"\
	"    x : float\n"\
	"    y : float\n"\
	"    z : float");
    
//setPitch
PyDoc_STRVAR(soundSetPitch_doc,
	"play the sound \n"\
	"\n"\
    "sound.setPitch(filename, value)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name\n"\
	"    value : float\n"\
	"        The pitch value");
    
//setGain
PyDoc_STRVAR(soundSetGain_doc,
	"play the sound \n"\
	"\n"\
    "sound.setGain(filename, value)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name\n"\
	"    value : float\n"\
	"        The Gain value");
    
//setRolloff
PyDoc_STRVAR(soundSetRolloff_doc,
	"play the sound \n"\
	"\n"\
    "sound.setRolloff(filename, value)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    filename : string\n"\
	"        The sound name\n"\
	"    value : float\n"\
	"        The Rolloff value");
    
//setListenerPosition
PyDoc_STRVAR(soundSetListenerPosition_doc,
	"play the sound \n"\
	"\n"\
    "sound.setListenerPosition(x, y, z)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    x : float\n"\
	"    y : float\n"\
	"    z : float");
    
//setListenerOrientation
PyDoc_STRVAR(soundSetListenerOrientation_doc,
	"play the sound \n"\
	"\n"\
    "sound.setListenerOrientation(xAt, yAt, zAt, xUp, yUp, zUp)\n"\
	"\n"\
	"Parameters\n"\
	"----------\n"\
	"    xAt : float\n"\
	"    yAt : float\n"\
	"    zAt : float\n"\
	"    xUp : float\n"\
	"    yUp : float\n"\
	"    zUp : float");