#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pythonSound_doc_en.h"

namespace pyxie
{
	PyObject* sound_new(PyTypeObject* type, PyObject* args, PyObject* kw)
	{
		sound_obj* self = NULL;

		self = (sound_obj*)type->tp_alloc(type, 0);
		self->sound = new pyxieSound();

		return (PyObject*)self;
	}

	void sound_dealloc(sound_obj* self)
	{
		Py_TYPE(self)->tp_free(self);
	}

	PyObject* sound_str(sound_obj* self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "sound object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyObject* sound_Init(sound_obj* self)
	{
		self->sound->init();

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_Release(sound_obj* self)
	{
		self->sound->release();

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_Play(sound_obj* self, PyObject* args)
	{
		char* soundName;
		int loop = 0;
		if (!PyArg_ParseTuple(args, "s|i", &soundName, &loop))
			return NULL;

		self->sound->play(soundName, loop);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_Stop(sound_obj* self, PyObject* args)
	{
		char* soundName;
		if (!PyArg_ParseTuple(args, "s", &soundName))
			return NULL;

		self->sound->stop(soundName);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_StopAll(sound_obj* self)
	{
		self->sound->stopAllSound();

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_Load(sound_obj* self, PyObject* args)
	{
		char* soundName;
		if (!PyArg_ParseTuple(args, "s", &soundName))
			return NULL;

		self->sound->load(soundName);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_Unload(sound_obj* self, PyObject* args)
	{
		char* soundName;
		if (!PyArg_ParseTuple(args, "s", &soundName))
			return NULL;

		self->sound->unload(soundName);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_UnloadUnused(sound_obj* self)
	{
		self->sound->unloadUnused();

		Py_INCREF(Py_None);
		return Py_None;
	}	

	static PyObject* sound_SetPositon(sound_obj* self, PyObject* args)
	{
		char* soundName;
		float x, y, z;
		if (!PyArg_ParseTuple(args, "sfff", &soundName, &x, &y, &z))
			return NULL;

		self->sound->setPositon(soundName, x, y, z);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_SetPitch(sound_obj* self, PyObject* args)
	{
		char* soundName;
		float pitch;
		if (!PyArg_ParseTuple(args, "sf", &soundName, &pitch))
			return NULL;

		self->sound->setPitch(soundName, pitch);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_SetGain(sound_obj* self, PyObject* args)
	{
		char* soundName;
		float gain;
		if (!PyArg_ParseTuple(args, "sf", &soundName, &gain))
			return NULL;

		self->sound->setGain(soundName, gain);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_SetRolloff(sound_obj* self, PyObject* args)
	{
		char* soundName;
		float rolloff;
		if (!PyArg_ParseTuple(args, "sf", &soundName, &rolloff))
			return NULL;

		self->sound->setRolloff(soundName, rolloff);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_SetListenerPosition(sound_obj* self, PyObject* args)
	{
		float x, y, z;
		if (!PyArg_ParseTuple(args, "fff", &x, &y, &z))
			return NULL;

		self->sound->setListenerPosition(x, y, z);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* sound_SetListenerOrientation(sound_obj* self, PyObject* args)
	{
		float xAt, yAt, zAt, xUp, yUp, zUp;
		if (!PyArg_ParseTuple(args, "ffffff", &xAt, &yAt, &zAt, &xUp, &yUp, &zUp))
			return NULL;

		self->sound->setListenerOrientation(xAt, yAt, zAt, xUp, yUp, zUp);

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef sound_methods[] = {		
		{ "init", (PyCFunction)sound_Init, METH_NOARGS, soundInit_doc },
		{ "release", (PyCFunction)sound_Release, METH_NOARGS, soundRelease_doc },
		{ "play", (PyCFunction)sound_Play, METH_VARARGS, soundPlay_doc },
		{ "stop", (PyCFunction)sound_Stop, METH_VARARGS, soundStop_doc },
		{ "stopAll", (PyCFunction)sound_StopAll, METH_NOARGS, soundStopAll_doc },
		{ "load", (PyCFunction)sound_Load, METH_VARARGS, soundLoad_doc },
		{ "unload", (PyCFunction)sound_Unload, METH_VARARGS, soundUnload_doc },
		{ "unloadUnused", (PyCFunction)sound_UnloadUnused, METH_NOARGS, soundUnloadUnused_doc },
		{ "setPositon", (PyCFunction)sound_SetPositon, METH_VARARGS, soundSetPositon_doc },
		{ "setPitch", (PyCFunction)sound_SetPitch, METH_VARARGS, soundSetPitch_doc },
		{ "setGain", (PyCFunction)sound_SetGain, METH_VARARGS, soundSetGain_doc },
		{ "setRolloff", (PyCFunction)sound_SetRolloff, METH_VARARGS, soundSetRolloff_doc },
		{ "setListenerPosition", (PyCFunction)sound_SetListenerPosition, METH_VARARGS, soundSetListenerPosition_doc },
		{ "setListenerOrientation", (PyCFunction)sound_SetListenerOrientation, METH_VARARGS, soundSetListenerOrientation_doc },
		{ NULL,	NULL }
	};

	PyGetSetDef sound_getsets[] = {
		{ NULL, NULL }
	};

	PyTypeObject SoundType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.sound",						/* tp_name */
		sizeof(sound_obj),					/* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)sound_dealloc,			/* tp_dealloc */
		0,                                  /* tp_print */
		0,							        /* tp_getattr */
		0,                                  /* tp_setattr */
		0,                                  /* tp_reserved */
		0,                                  /* tp_repr */
		0,					                /* tp_as_number */
		0,                                  /* tp_as_sequence */
		0,                                  /* tp_as_mapping */
		0,                                  /* tp_hash */
		0,                                  /* tp_call */
		(reprfunc)sound_str,				/* tp_str */
		0,                                  /* tp_getattro */
		0,                                  /* tp_setattro */
		0,                                  /* tp_as_buffer */
		Py_TPFLAGS_DEFAULT,					/* tp_flags */
		0,									/* tp_doc */
		0,									/* tp_traverse */
		0,                                  /* tp_clear */
		0,                                  /* tp_richcompare */
		0,                                  /* tp_weaklistoffset */
		0,									/* tp_iter */
		0,									/* tp_iternext */
		sound_methods,						/* tp_methods */
		0,                                  /* tp_members */
		sound_getsets,						/* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		sound_new,							/* tp_new */
		0,									/* tp_free */
	};

}