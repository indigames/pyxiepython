#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "Backyard.h"
#include "pyVectorMath.h"
#include "pythonCamera_doc_en.h"

namespace pyxie
{
	PyObject *camera_new(PyTypeObject *type, PyObject *args, PyObject *kw) {
		char* name = "camera";
		camera_obj * self = NULL;
		if (PyArg_ParseTuple(args, "|s", &name)) {
			self = (camera_obj*)type->tp_alloc(type, 0);
			self->camera = pyxieResourceCreator::Instance().NewCamera(name,nullptr);
		}
		return (PyObject *)self;
	}

	void  camera_dealloc(camera_obj *self)
	{
		self->camera->DecReference();
		Py_TYPE(self)->tp_free(self);
	}

	PyObject *camera_str(camera_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "camera object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}

	static PyObject *camera_Render(camera_obj *self, PyObject *args, PyObject* kwargs)
	{
		static char* kwlist[] = { "showcase","texture","clearColor","clearDepth","clearColorValue", NULL };
		showcase_obj* sco;
		texture_obj* tex = nullptr;
		int clearColor = true;
		int clearDepth = true;
		PyObject* clearColorValue = nullptr;
		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|OiiO", kwlist, &sco, &tex, &clearColor, &clearDepth, &clearColorValue)) return NULL;

		if (sco && sco->ob_base.ob_type != &ShowcaseType) {
			PyErr_SetString(PyExc_TypeError, "Parameter error.");
			return NULL;
		}
		if (tex && (tex->ob_base.ob_type != &TextureType)) tex = nullptr;

		float buff[4] = { 0.2f, 0.6f, 0.8f , 0.0f};
		float* v= nullptr;
		int d;
		if (clearColorValue) {
			v = pyObjToFloat(clearColorValue, buff, d);
		}
		if (!v) v = buff;


		if (tex && (!tex->renderTarget))
			tex->renderTarget = pyxieResourceCreator::Instance().NewRenderTarget(tex->colortexture, tex->depth, tex->stencil);

		Backyard::Instance().RenderRequest(self->camera, sco->showcase, tex?tex->renderTarget:nullptr, clearColor, clearDepth, v);

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyObject* camera_getPosition(camera_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->camera->GetPosition().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}

	int camera_setPosition(camera_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetCameraPosition(*((Vec3*)v1));
		self->camera->Step(0.0f);
		return 0;
	}

	PyObject* camera_getRotation(camera_obj* self)
	{
		vec_obj* quatobj = PyObject_New(vec_obj, _QuatType);
		if (!quatobj) return NULL;
		vmath_cpy(self->camera->GetRotation().P(), 4, quatobj->v);
		quatobj->d = 4;
		return (PyObject*)quatobj;
	}
	int camera_setRotation(camera_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetCameraRotation(*((Quat*)v1));
		return 0;
	}

	PyObject* camera_getScale(camera_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->camera->GetScale().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}
	int camera_setScale(camera_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetScale(*((Vec3*)v1));
		return 0;
	}

	PyObject* camera_getFieldOfView(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetFieldOfView());
	}
	int camera_setFieldOfView(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to fieldOfView.");
			return -1;
		}
		self->camera->SetFieldOfView((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* camera_getAspectRate(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetAspectRate());
	}
	int camera_setAspectRate(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to aspectRate.");
			return -1;
		}
		self->camera->SetAspectRate((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* camera_getOrthoWidth(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetOrthoWidth());
	}
	int camera_setOrthoWidth(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value)|| PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to orthoWidth.");
			return -1;
		}
		self->camera->SetOrthoWidth((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* camera_getNearPlane(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetNearPlane());
	}
	int camera_setNearPlane(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to nearPlane.");
			return -1;
		}
		self->camera->SetNearPlane((float)PyFloat_AsDouble(value));
		return 0;
	}
	PyObject* camera_getFarPlane(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetFarPlane());
	}
	int camera_setFarPlane(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to farPlane.");
			return -1;
		}
		self->camera->SetFarPlane((float)PyFloat_AsDouble(value));
		return 0;
	}
	PyObject* camera_getLockon(camera_obj* self) {
		return PyLong_FromLong(self->camera->GetLockon());
	}
	int camera_setLockon(camera_obj* self, PyObject* value) {

		if (!PyLong_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "Only FlyThru(0) or LookAt(1) can be set to mode.");
			return -1;
		}
		bool lockon = (bool)PyLong_AsLong(value);
		self->camera->LockonTarget(lockon);

		return 0;
	}
	PyObject* camera_getPan(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetPan());
	}
	int camera_setPan(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to pan.");
			return -1;
		}
		self->camera->SetPan((float)PyFloat_AsDouble(value));
		return 0;
	}
	PyObject* camera_getTilt(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetTilt());
	}
	int camera_setTilt(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to tilt.");
			return -1;
		}
		self->camera->SetTilt((float)PyFloat_AsDouble(value));
		return 0;
	}
	PyObject* camera_getRoll(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetRoll());
	}
	int camera_setRoll(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to tilt.");
			return -1;
		}
		self->camera->SetRoll((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* camera_getTarget(camera_obj* self) {
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->camera->GetTarget().P(), 3, v3robj->v);
		v3robj->d= 3;
		return (PyObject*)v3robj;
	}

	int camera_setTarget(camera_obj* self, PyObject* value) {
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetTarget(*((Vec3*)v1));
		self->camera->Step(0.0f);

		return 0;
	}

	PyObject* camera_getOrthographicProjection(camera_obj* self) {
		return PyBool_FromLong(self->camera->IsOrthographicProjection());
	}

	int camera_setOrthographicProjection(camera_obj* self, PyObject* value) {

		if (!PyLong_Check(value)) {
			PyErr_SetString(PyExc_TypeError, "Only bool value can be set to mode.");
			return -1;
		}
		uint32_t orth = (uint32_t)PyLong_AsLong(value);
		if (orth > 1) {
			PyErr_SetString(PyExc_TypeError, "Only bool value can be set to mode.");
			return -1;
		}
		self->camera->SetOrthographicProjection(orth);

		return 0;
	}
	PyObject* camera_getScreenScale(camera_obj* self) {
		vec_obj* v2robj = PyObject_New(vec_obj, _Vec2Type);
		if (!v2robj) return NULL;
		vmath_cpy(self->camera->GetScreenScale().P(), 2, v2robj->v);
		v2robj->d = 2;
		return (PyObject*)v2robj;
	}
	int camera_setScreenScale(camera_obj* self, PyObject* value) {
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetScreenScale(*((Vec2*)v1));

		return 0;
	}
	PyObject* camera_getScreenOffset(camera_obj* self) {
		vec_obj* v2robj = PyObject_New(vec_obj, _Vec2Type);
		if (v2robj) return NULL;
		vmath_cpy(self->camera->GetScreenOffset().P(), 2, v2robj->v);
		v2robj->d = 2;
		return (PyObject*)v2robj;
	}

	int camera_setScreenOffset(camera_obj* self, PyObject* value) {
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->camera->SetScreenOffset(*((Vec2*)v1));

		return 0;
	}

	PyObject* camera_getScreenRadian(camera_obj* self) {
		return PyFloat_FromDouble(self->camera->GetScreenRadian());
	}
	int camera_setScreenRadian(camera_obj* self, PyObject* value) {
		if (!(PyFloat_Check(value) || PyLong_Check(value))) {
			PyErr_SetString(PyExc_TypeError, "Only float value can be set to distance.");
			return -1;
		}
		self->camera->SetScreenRadian((float)PyFloat_AsDouble(value));
		return 0;
	}

	PyObject* camera_getProjectionMatrix(camera_obj* self) {
		mat_obj* m4obj = PyObject_New(mat_obj, _Mat44Type);
		if (!m4obj) return NULL;
		Mat4 mat;
		self->camera->GetProjectionMatrix(mat);
		vmath_cpy(mat.P(), 16, m4obj->m);
		m4obj->d = 4;
		return (PyObject*)m4obj;
	}

	PyObject* camera_getViewInverseMatrix(camera_obj* self) {
		mat_obj* m4obj = PyObject_New(mat_obj, _Mat44Type);
		if (!m4obj) return NULL;
		Mat4 mat;
		self->camera->GetViewInverseMatrix(mat);
		vmath_cpy(mat.P(), 16, m4obj->m);
		m4obj->d = 4;
		return (PyObject*)m4obj;
	}

	PyObject* camera_getScreenMatrix(camera_obj* self) {
		mat_obj* m4obj = PyObject_New(mat_obj, _Mat44Type);
		if (!m4obj) return NULL;
		Mat4 mat;
		self->camera->GetScreenMatrix(mat);
		vmath_cpy(mat.P(), 16, m4obj->m);
		m4obj->d = 4;
		return (PyObject*)m4obj;
	}

	PyMethodDef camera_methods[] = {
		{ "shoot", (PyCFunction)camera_Render, METH_VARARGS | METH_KEYWORDS, shoot_doc },
		{ NULL,	NULL }
	};

	PyGetSetDef camera_getsets[] = {
		{ const_cast<char*>("position"), (getter)camera_getPosition, (setter)camera_setPosition,position_doc, NULL },
		{ const_cast<char*>("rotation"), (getter)camera_getRotation, (setter)camera_setRotation,rotation_doc, NULL },
		{ const_cast<char*>("scale"),    (getter)camera_getScale,    (setter)camera_setScale,scale_doc, NULL },
		{ const_cast<char*>("fieldOfView"),    (getter)camera_getFieldOfView,    (setter)camera_setFieldOfView,fieldOfView_doc, NULL },
		{ const_cast<char*>("aspectRate"),    (getter)camera_getAspectRate,    (setter)camera_setAspectRate,aspectRate_doc, NULL },
		{ const_cast<char*>("orthoWidth"),    (getter)camera_getOrthoWidth,    (setter)camera_setOrthoWidth,orthoWidth_doc, NULL },
		{ const_cast<char*>("nearPlane"),    (getter)camera_getNearPlane,    (setter)camera_setNearPlane,nearPlane_doc, NULL },
		{ const_cast<char*>("farPlane"),    (getter)camera_getFarPlane,    (setter)camera_setFarPlane,farPlane_doc, NULL },
		{ const_cast<char*>("lockon"),    (getter)camera_getLockon,    (setter)camera_setLockon,lockon_doc, NULL },
		{ const_cast<char*>("pan"),    (getter)camera_getPan,    (setter)camera_setPan,pan_doc, NULL },
		{ const_cast<char*>("tilt"),    (getter)camera_getTilt,    (setter)camera_setTilt,tilt_doc, NULL },
		{ const_cast<char*>("roll"),    (getter)camera_getRoll,    (setter)camera_setRoll,tilt_doc, NULL },
		{ const_cast<char*>("target"),    (getter)camera_getTarget,    (setter)camera_setTarget,target_doc, NULL },
		{ const_cast<char*>("orthographicProjection"),    (getter)camera_getOrthographicProjection,    (setter)camera_setOrthographicProjection,orthographicProjection_doc, NULL },
		{ const_cast<char*>("screenScale"),    (getter)camera_getScreenScale,    (setter)camera_setScreenScale,screenScale_doc, NULL },
		{ const_cast<char*>("screenOffset"),    (getter)camera_getScreenOffset,    (setter)camera_setScreenOffset,screenOffset_doc, NULL },
		{ const_cast<char*>("screenRadian"),    (getter)camera_getScreenRadian,    (setter)camera_setScreenRadian,screenRadian_doc, NULL },
		{ const_cast<char*>("projectionMatrix"),(getter)camera_getProjectionMatrix, NULL, projectionMatrix_doc,NULL},
		{ const_cast<char*>("viewInverseMatrix"),(getter)camera_getViewInverseMatrix, NULL, viewInverseMatrix_doc,NULL},
		{ const_cast<char*>("screenMatrix"),(getter)camera_getScreenMatrix, NULL, screenMatrix_doc,NULL},
		{ NULL, NULL }
	};

	PyTypeObject CameraType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.camera",						/* tp_name */
		sizeof(camera_obj),                 /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)camera_dealloc,			/* tp_dealloc */
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
		(reprfunc)camera_str,               /* tp_str */
		0,                                  /* tp_getattro */
		0,                                  /* tp_setattro */
		0,                                  /* tp_as_buffer */
		Py_TPFLAGS_DEFAULT,					/* tp_flags */
		camera_doc,							/* tp_doc */
		0,									/* tp_traverse */
		0,                                  /* tp_clear */
		0,                                  /* tp_richcompare */
		0,                                  /* tp_weaklistoffset */
		0,									/* tp_iter */
		0,									/* tp_iternext */
		camera_methods,						/* tp_methods */
		0,                                  /* tp_members */
		camera_getsets,                     /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		camera_new,							/* tp_new */
		0,									/* tp_free */
	};

}