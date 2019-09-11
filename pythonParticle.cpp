#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieTime.h"
#include "pyVectorMath.h"
#include "pythonFigure_doc_en.h"
#include "pyxieParticle.h"

#include <Python.h>

typedef void *(*swig_converter_func)(void *, int *);
typedef struct swig_type_info *(*swig_dycast_func)(void **);

typedef struct swig_type_info
{
	const char *name;			 /* mangled name of this type */
	const char *str;			 /* human readable name of this type */
	swig_dycast_func dcast;		 /* dynamic cast function down a hierarchy */
	struct swig_cast_info *cast; /* linked list of types that can cast into this type */
	void *clientdata;			 /* language specific type data */
	int owndata;				 /* flag if the structure owns the clientdata */
} swig_type_info;

/* Structure to store a type and conversion function used for casting */
typedef struct swig_cast_info
{
	swig_type_info *type;		   /* pointer to type that is equivalent to this type */
	swig_converter_func converter; /* function to cast the void pointers */
	struct swig_cast_info *next;   /* pointer to next cast in linked list */
	struct swig_cast_info *prev;   /* pointer to the previous cast */
} swig_cast_info;

typedef struct
{
	PyObject_HEAD
	void *ptr;
	swig_type_info *ty;
	int own;
	PyObject *next;
} SwigPyObject;

typedef struct
{
	PyObject *klass;
	PyObject *newraw;
	PyObject *newargs;
	PyObject *destroy;
	int delargs;
	int implicitconv;
	PyTypeObject *pytype;
} SwigPyClientData;

namespace pyxie
{
PyObject *particle_new(PyTypeObject *type, PyObject *args, PyObject *kw)
{
	particle_obj *self = NULL;
	PyObject *arg1;
	PyObject *arg2;
	int count;
	float radius;

	if (!PyArg_ParseTuple(args, "OOif", &arg1, &arg2, &count, &radius))
	{
		printf("ERROR: figure_update_particles, parse fail!\n");
		return (PyObject *)self;
	}

	b2Vec2 *posBuff = NULL;
	b2Color *colBuff = NULL;

	SwigPyObject *obj1 = (SwigPyObject *)arg1;
	if (obj1 && obj1->ptr)
	{
		if (obj1->ty)
		{
			posBuff = reinterpret_cast<b2Vec2 *>(obj1->ptr);
		}
	}

	SwigPyObject *obj2 = (SwigPyObject *)arg2;
	if (obj2 && obj2->ptr)
	{
		if (obj2->ty)
		{
			colBuff = reinterpret_cast<b2Color *>(obj2->ptr);
		}
	}

	self = (particle_obj *)type->tp_alloc(type, 0);
	self->figure = pyxieParticle::new_particle(posBuff, colBuff, count, radius);
	return (PyObject *)self;
}

void particle_dealloc(particle_obj *self)
{
	self->figure->DecReference();
	Py_TYPE(self)->tp_free(self);
}

PyObject *particle_str(particle_obj *self)
{
	char buf[64];
	pyxie_snprintf(buf, 64, "particle object");
	return _PyUnicode_FromASCII(buf, strlen(buf));
}

PyObject *figure_getPosition(particle_obj *self)
{
	vec_obj *v3robj = PyObject_New(vec_obj, _Vec3Type);
	if (!v3robj)
		return NULL;
	vmath_cpy(self->figure->GetPosition().P(), 3, v3robj->v);
	v3robj->d = 3;
	return (PyObject *)v3robj;
}
int figure_setPosition(particle_obj *self, PyObject *value)
{
	int d1;
	float buff[4];
	float *v1 = pyObjToFloat((PyObject *)value, buff, d1);
	if (!v1)
		return NULL;
	self->figure->SetPosition(*((Vec3 *)v1));
	return 0;
}

PyObject *figure_getRotation(particle_obj *self)
{
	vec_obj *quatobj = PyObject_New(vec_obj, _QuatType);
	if (!quatobj)
		return NULL;
	vmath_cpy(self->figure->GetRotation().P(), 4, quatobj->v);
	quatobj->d = 4;
	return (PyObject *)quatobj;
}
int figure_setRotation(particle_obj *self, PyObject *value)
{
	int d1;
	float buff[4];
	float *v1 = pyObjToFloat((PyObject *)value, buff, d1);
	if (!v1)
		return NULL;
	self->figure->SetRotation(*((Quat *)v1));
	return 0;
}

PyObject *figure_getScale(particle_obj *self)
{
	vec_obj *v3robj = PyObject_New(vec_obj, _Vec3Type);
	if (!v3robj)
		return NULL;
	vmath_cpy(self->figure->GetScale().P(), 3, v3robj->v);
	v3robj->d = 3;
	return (PyObject *)v3robj;
}
int figure_setScale(particle_obj *self, PyObject *value)
{
	int d1;
	float buff[4];
	float *v1 = pyObjToFloat((PyObject *)value, buff, d1);
	if (!v1)
		return NULL;
	self->figure->SetScale(*((Vec3 *)v1));
	return 0;
}

static PyObject *figure_BindAnimator(particle_obj *self, PyObject *args)
{
	int slot = 0;
	PyObject *arg2 = nullptr;

	if (PyArg_ParseTuple(args, "i|O", &slot, &arg2))
	{
		if (arg2)
		{
			if (PyUnicode_Check(arg2))
			{
				const char *motionName = PyUnicode_AsUTF8(arg2);
				self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, motionName);
			}
			else if (arg2->ob_type == &AnimatorType)
			{
				animator_obj *anime = (animator_obj *)arg2;
				self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, anime->anime);
			}
			else
			{
				PyErr_SetString(PyExc_TypeError, "Argument of connectAnimator must be (integer, animator) or (integer) if unbind.");
				return NULL;
			}
		}
		else
		{
			self->figure->BindAnimator((pyxieFigure::AnimatorSlot)slot, (pyxieAnimator *)nullptr);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_GetCamera(particle_obj *self, PyObject *args)
{
	char *name = NULL;
	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	camera_obj *obj = PyObject_New(camera_obj, &CameraType);
	obj->camera = pyxieResourceCreator::Instance().NewCamera(name, self->figure);
	return (PyObject *)obj;
}

static PyObject *figure_GetEnvironment(particle_obj *self)
{
	environment_obj *obj = PyObject_New(environment_obj, &EnvironmentType);
	obj->envSet = pyxieResourceCreator::Instance().NewEnvironmentSet(self->figure);
	return (PyObject *)obj;
}

static PyObject *figure_Step(particle_obj *self, PyObject *args)
{
	float s = FLT_MAX;
	if (!PyArg_ParseTuple(args, "|f", &s))
		return NULL;
	if (s == FLT_MAX)
		s = (float)pyxieTime::Instance().GetElapsedTime();
	self->figure->Step(s);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_SetTime(particle_obj *self, PyObject *args)
{
	float s;
	if (!PyArg_ParseTuple(args, "f", &s))
		return NULL;
	self->figure->SetEvalTime(s);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_Dump(particle_obj *self, PyObject *args)
{
	char *file;
	if (!PyArg_ParseTuple(args, "s", &file))
		return NULL;
	self->figure->Dump(file);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_setBlendingWeight(particle_obj *self, PyObject *args)
{
	int slot;
	float value;
	if (!PyArg_ParseTuple(args, "if", &slot, &value))
		return NULL;
	self->figure->SetBlendingWeight(slot, value);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_getBlendingWeight(particle_obj *self, PyObject *args)
{
	int slot;
	if (!PyArg_ParseTuple(args, "i", &slot))
		return NULL;
	float value = self->figure->GetBlendingWeight(slot);
	return PyFloat_FromDouble(value);
}

static PyObject *figure_getJoint(particle_obj *self, PyObject *args)
{

	char *jointName;
	if (!PyArg_ParseTuple(args, "s", &jointName))
		return NULL;

	int idx = self->figure->GetJointIndex(GenerateNameHash(jointName));

	auto joint = self->figure->GetJoint(idx);

	PyObject *joint_obj = PyTuple_New(3);

	vec_obj *pos = (vec_obj *)PyObject_New(vec_obj, _Vec3Type);
	vec_obj *rot = (vec_obj *)PyObject_New(vec_obj, _QuatType);
	vec_obj *scale = (vec_obj *)PyObject_New(vec_obj, _Vec3Type);
	for (int i = 0; i < 4; i++)
	{
		pos->v[i] = joint.translation[i];
		rot->v[i] = joint.rotation[i];
		scale->v[i] = joint.scale[i];
	}
	PyTuple_SetItem(joint_obj, 0, (PyObject *)pos);
	PyTuple_SetItem(joint_obj, 1, (PyObject *)rot);
	PyTuple_SetItem(joint_obj, 2, (PyObject *)scale);
	return joint_obj;
}

static PyObject *figure_setJoint(particle_obj *self, PyObject *args, PyObject *kwargs)
{

	static char *kwlist[] = {"jointName", "position", "rotation", "scale", NULL};

	char *jointName;
	PyObject *arg1 = nullptr;
	PyObject *arg2 = nullptr;
	PyObject *arg3 = nullptr;
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|OOO", kwlist, &jointName, arg1, arg2, arg3))
		return NULL;

	Joint joint;
	float *v;
	int d;
	float buff[4];
	if (arg1)
	{
		v = pyObjToFloat(arg1, buff, d);
		for (int i = 0; i < d; i++)
			joint.translation[i] = v[i];
	}
	if (arg2)
	{
		v = pyObjToFloat(arg2, buff, d);
		for (int i = 0; i < d; i++)
			joint.rotation[i] = v[i];
	}
	if (arg3)
	{
		v = pyObjToFloat(arg3, buff, d);
		for (int i = 0; i < d; i++)
			joint.scale[i] = v[i];
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_update_particles(particle_obj *self, PyObject *args)
{
	PyObject *arg1;
	PyObject *arg2;
	int count;
	float radius;

	if (!PyArg_ParseTuple(args, "OOif", &arg1, &arg2, &count, &radius))
	{
		printf("ERROR: figure_update_particles, parse fail!\n");
		return NULL;
	}

	b2Vec2 *posBuff = NULL;
	b2Color *colBuff = NULL;

	SwigPyObject *obj1 = (SwigPyObject *)arg1;
	if (obj1 && obj1->ptr)
	{
		if (obj1->ty)
		{
			posBuff = reinterpret_cast<b2Vec2 *>(obj1->ptr);
		}
	}

	SwigPyObject *obj2 = (SwigPyObject *)arg2;
	if (obj2 && obj2->ptr)
	{
		if (obj2->ty)
		{
			colBuff = reinterpret_cast<b2Color *>(obj2->ptr);
		}
	}

	if (self->figure)
	{
		self->figure->UpdateParticles(posBuff, colBuff, count, radius);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_set_camera(particle_obj *self, PyObject *args)
{	
	PyObject *arg;
	if (!PyArg_ParseTuple(args, "O", &arg))
	{
		printf("ERROR: figure_set_camera, parse fail!\n");
		return NULL;
	}
	
	if (arg->ob_type == &CameraType) {
		camera_obj* cam = (camera_obj*)arg;
		self->figure->SetCamera(cam->camera);
	}	

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *figure_set_ppm(particle_obj *self, PyObject *args)
{	
	float ppm;
	if (!PyArg_ParseTuple(args, "f", &ppm))
	{
		printf("ERROR: figure_set_ppm, parse fail!\n");
		return NULL;
	}
	
	self->figure->SetPPM(ppm);
	
	Py_INCREF(Py_None);
	return Py_None;
}

PyMethodDef particle_methods[] = {
	{"connectAnimator", (PyCFunction)figure_BindAnimator, METH_VARARGS, connectAnimator_doc},
	{"getCamera", (PyCFunction)figure_GetCamera, METH_VARARGS, getCamera_doc},
	{"getEnvironment", (PyCFunction)figure_GetEnvironment, METH_NOARGS, getEnvironment_doc},
	{"step", (PyCFunction)figure_Step, METH_VARARGS, step_doc},
	{"setTime", (PyCFunction)figure_SetTime, METH_VARARGS, setTime_doc},
	{"setBlendingWeight", (PyCFunction)figure_setBlendingWeight, METH_VARARGS, setBlendingWeight_doc},
	{"getBlendingWeight", (PyCFunction)figure_getBlendingWeight, METH_VARARGS, getBlendingWeight_doc},
	{"getJoint", (PyCFunction)figure_getJoint, METH_VARARGS, getJoint_doc},
	{"setJoint", (PyCFunction)figure_setJoint, METH_VARARGS | METH_KEYWORDS, setJoint_doc},
	{"UpdateParticles", (PyCFunction)figure_update_particles, METH_VARARGS, setJoint_doc},
	{"SetCamera", (PyCFunction)figure_set_camera, METH_VARARGS, setJoint_doc},
	{"SetPPM", (PyCFunction)figure_set_ppm, METH_VARARGS, setJoint_doc},

	//{ "dump", (PyCFunction)figure_Dump, METH_VARARGS },

	{NULL, NULL}};

PyGetSetDef particle_getsets[] = {
	{const_cast<char *>("position"), (getter)figure_getPosition, (setter)figure_setPosition, position_doc, NULL},
	{const_cast<char *>("rotation"), (getter)figure_getRotation, (setter)figure_setRotation, rotation_doc, NULL},
	{const_cast<char *>("scale"), (getter)figure_getScale, (setter)figure_setScale, scale_doc, NULL},
	{NULL, NULL}};

PyTypeObject ParticleType = {
	PyVarObject_HEAD_INIT(NULL, 0) "pyxie.particle", /* tp_name */
	sizeof(particle_obj),							 /* tp_basicsize */
	0,												 /* tp_itemsize */
	(destructor)particle_dealloc,					 /* tp_dealloc */
	0,												 /* tp_print */
	0,												 /* tp_getattr */
	0,												 /* tp_setattr */
	0,												 /* tp_reserved */
	0,												 /* tp_repr */
	0,												 /* tp_as_number */
	0,												 /* tp_as_sequence */
	0,												 /* tp_as_mapping */
	0,												 /* tp_hash */
	0,												 /* tp_call */
	(reprfunc)particle_str,							 /* tp_str */
	0,												 /* tp_getattro */
	0,												 /* tp_setattro */
	0,												 /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,								 /* tp_flags */
	0,												 /* tp_doc */
	0,												 /* tp_traverse */
	0,												 /* tp_clear */
	0,												 /* tp_richcompare */
	0,												 /* tp_weaklistoffset */
	0,												 /* tp_iter */
	0,												 /* tp_iternext */
	particle_methods,								 /* tp_methods */
	0,												 /* tp_members */
	particle_getsets,								 /* tp_getset */
	0,												 /* tp_base */
	0,												 /* tp_dict */
	0,												 /* tp_descr_get */
	0,												 /* tp_descr_set */
	0,												 /* tp_dictoffset */
	0,												 /* tp_init */
	0,												 /* tp_alloc */
	particle_new,									 /* tp_new */
	0,												 /* tp_free */
};

} // namespace pyxie