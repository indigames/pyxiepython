#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieTime.h"
#include "structmember.h"
#include "pyVectorMath.h"

namespace pyxie
{
	PyObject *editablefigure_new(PyTypeObject *type, PyObject *args, PyObject *kw) {

		auto t = &EditableFigureType;

		char* path;
		editablefigure_obj * self = NULL;
		if (PyArg_ParseTuple(args, "s", &path)) {
			self = (editablefigure_obj*)type->tp_alloc(type, 0);
			//self = PyObject_New(editablefigure_obj, &EditableFigureType);
			self->editablefigure = pyxieResourceCreator::Instance().NewEditableFigure(path);
			self->VertexAttribure_POSITION = ATTRIBUTE_ID_POSITION;
			self->VertexAttribure_NORMAL = ATTRIBUTE_ID_NORMAL;
			self->VertexAttribure_TANGENT = ATTRIBUTE_ID_TANGENT;
			self->VertexAttribure_BINORMAL = ATTRIBUTE_ID_BINORMAL;
			self->VertexAttribure_UV0 = ATTRIBUTE_ID_UV0;
			self->VertexAttribure_UV1 = ATTRIBUTE_ID_UV1;
			self->VertexAttribure_UV2 = ATTRIBUTE_ID_UV2;
			self->VertexAttribure_UV3 = ATTRIBUTE_ID_UV3;
			self->VertexAttribure_COLOR = ATTRIBUTE_ID_COLOR;
			self->VertexAttribure_BLENDINDICES = ATTRIBUTE_ID_BLENDINDICES;
			self->VertexAttribure_BLENDWEIGHT = ATTRIBUTE_ID_BLENDWEIGHT;
			self->VertexAttribure_PSIZE = ATTRIBUTE_ID_PSIZE;
		}
		return (PyObject *)self;
	}

	void  editablefigure_dealloc(editablefigure_obj *self)
	{
		self->editablefigure->DecReference();
		Py_TYPE(self)->tp_free(self);
		//PyObject_Del(self);
	}

	PyObject * editablefigure_str(editablefigure_obj *self)
	{
		char buf[64];
		pyxie_snprintf(buf, 64, "editable figure object");
		return _PyUnicode_FromASCII(buf, strlen(buf));
	}


	PyObject* editablefigure_getPosition(editablefigure_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->editablefigure->GetPosition().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}
	int editablefigure_setPosition(editablefigure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->editablefigure->SetPosition(*((Vec3*)v1));
		return 0;
	}

	PyObject* editablefigure_getRotation(editablefigure_obj* self)
	{
		vec_obj* quatobj = PyObject_New(vec_obj, _QuatType);
		if (!quatobj) return NULL;
		vmath_cpy(self->editablefigure->GetRotation().P(), 4, quatobj->v);
		quatobj->d = 4;
		return (PyObject*)quatobj;
	}
	int editablefigure_setRotation(editablefigure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->editablefigure->SetRotation(*((Quat*)v1));
		return 0;
	}

	PyObject* editablefigure_getScale(editablefigure_obj* self)
	{
		vec_obj* v3robj = PyObject_New(vec_obj, _Vec3Type);
		if (!v3robj) return NULL;
		vmath_cpy(self->editablefigure->GetScale().P(), 3, v3robj->v);
		v3robj->d = 3;
		return (PyObject*)v3robj;
	}
	int editablefigure_setScale(editablefigure_obj* self, PyObject* value)
	{
		int d1;
		float buff[4];
		float* v1 = pyObjToFloat((PyObject*)value, buff, d1);
		if (!v1) return NULL;
		self->editablefigure->SetScale(*((Vec3*)v1));
		return 0;
	}

	static PyObject* editablefigure_AddMaterial(editablefigure_obj* self, PyObject* args)
	{
		char* name;
		shaderGen_obj* gen;

		if (PyArg_ParseTuple(args, "sO", &name, &gen)) {
			if (gen->ob_base.ob_type != &ShaderGeneratorType) {
				PyErr_SetString(PyExc_TypeError, "Argument of sddMaterial must be (string, shaderGenerator).");
				return NULL;
			}
			self->editablefigure->AddMaterial(name, *gen->shaderDesc);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_AddMesh(editablefigure_obj* self, PyObject* args)
	{
		char* meshName;
		char* materialName;
		if (PyArg_ParseTuple(args, "ss", &meshName, &materialName)) {
			self->editablefigure->AddMesh(meshName, materialName);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	int GetVertexAttribureSize(AttributeID id) {
		static int attrsize[] = { 0,3,3,3,3,2,2,2,2,4,4,4,1 };
		return attrsize[id];
	}

	static PyObject* editablefigure_SetVertexElements(editablefigure_obj* self, PyObject* args)
	{
		//efig.setVertexElemetns("mesh01", efig.ElementType_POSITION, 0, 4, points)
		char* meshName;
		int vertAttr;
		int offset = 0;
		PyObject* points;

		if (PyArg_ParseTuple(args, "siO|i", &meshName, &vertAttr, &points, &offset)) {
			if (vertAttr < 1 || vertAttr >= NUM_ATTRIBUTE_ID) {
				PyErr_SetString(PyExc_TypeError, "The value of vertexAttribure is incorrect.");
				return NULL;
			}
			const VertexAttribute& attr = pyxieEditableFigure::GetVertexAttribute((AttributeID)vertAttr);
			int inputVertexSize = attr.size;
			int bufferSize = pyObjToFloatArray(points, nullptr, inputVertexSize);
			int outputVertxSize = GetVertexAttribureSize((AttributeID)vertAttr);
			if (outputVertxSize == 0) {
				PyErr_SetString(PyExc_TypeError, "Parameter error.");
				return NULL;
			}

			float* buffer = (float*)PYXIE_MALLOC(bufferSize * sizeof(float));
			MemoryCleaner creaner(buffer);
			pyObjToFloatArray(points, buffer, inputVertexSize);
			self->editablefigure->SetVertexValues(meshName, (AttributeID)vertAttr, offset, buffer, bufferSize / inputVertexSize);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SetTriangles(editablefigure_obj* self, PyObject* args)
	{
		//efig.setTriangles("mesh01", 0, 2, tris);
		char* meshName;
		int offset;
		int numTris;
		PyObject* tris;

		if (PyArg_ParseTuple(args, "siiO", &meshName, &offset, &numTris, &tris)) {

			if (numTris <= 0) {
				PyErr_SetString(PyExc_TypeError, "numTris must have a value of 1 or more .");
				return NULL;
			}
			uint32_t* buffer = (uint32_t*)PYXIE_MALLOC(numTris * sizeof(int) * 3);
			MemoryCleaner creaner(buffer);
			uint32_t* bufferPtr = buffer;

			int type = 0;
			if (PyTuple_Check(tris)) type = 1;
			else if (PyList_Check(tris)) type = 2;
			if (type == 0) {
				PyErr_SetString(PyExc_TypeError, "points must be tuple or list of int value.");
				return NULL;
			}

			int inSize = (int)(type == 1 ? PyTuple_Size(tris) : PyList_Size(tris)) / 3;

			for (int i = 0; i < numTris; i++) {
				int idx = i % inSize;
				for (int j = 0; j < 3; j++) {
					PyObject* value = type == 1 ? PyTuple_GET_ITEM(tris, idx * 3 + j)
						: PyList_GET_ITEM(tris, idx * 3 + j);
					if (!PyLong_Check(value)) {
						PyErr_SetString(PyExc_TypeError, "points must be tuple or list of int value.");
						return NULL;
					}
					*bufferPtr = (uint32_t)PyLong_AsLong(value);
					bufferPtr++;
				}
			}
			self->editablefigure->SetIndexValues(meshName, offset, buffer, numTris);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_addJoint(editablefigure_obj* self, PyObject* args, PyObject* kwargs)
	{
		static char* kwlist[] = { "jointName", "scaleCompensate","parentJointName", NULL };
		char* jointName;
		int scaleCompensate = 0;
		char* parentJointName = nullptr;

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ps", kwlist,
			&jointName, &scaleCompensate, &parentJointName))
			return NULL;

		Joint joint;
		int parentIndex = -1;
		if (parentJointName) parentIndex = self->editablefigure->FindSkeketonIndex(parentJointName);
		self->editablefigure->AddJoint(parentIndex, joint, (bool)scaleCompensate, jointName);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SetMaterialParam(editablefigure_obj* self, PyObject* args)
	{
		//efig.setMaterialParam("mate01", "DiffuseColor", (1.0, 1.0, 1.0, 1.0));

		char* materialName;
		char* paramName;
		PyObject* param;
		if (!PyArg_ParseTuple(args, "ssO", &materialName, &paramName, &param)) return NULL;

		float buff[4];

		Py_ssize_t size = 0;
		ShaderParameterDataType type = ParamTypeUnknown;
		if (PyTuple_Check(param)) {
			size = PyTuple_Size(param);
			if (size <= 0 || size > 4) {
				PyErr_SetString(PyExc_TypeError, "parameter error.");
				return NULL;
			}
			for (Py_ssize_t i = 0; i < size; i++) {
				auto item = PyTuple_GET_ITEM(param, i);
				if (!PyFloat_Check(item)) {
					PyErr_SetString(PyExc_TypeError, "parameter error.");
					return NULL;
				}
				buff[i] = (float)PyFloat_AsDouble(item);
			}
		}
		else if (PyList_Check(param)) {
			size = PyList_Size(param);
			if (size <= 0 || size > 4) {
				PyErr_SetString(PyExc_TypeError, "parameter error.");
				return NULL;
			}
			for (Py_ssize_t i = 0; i < size; i++) {
				auto item = PyList_GET_ITEM(param, i);
				if (!PyFloat_Check(item)) {
					PyErr_SetString(PyExc_TypeError, "parameter error.");
					return NULL;
				}
				buff[i] = (float)PyFloat_AsDouble(item);
			}
		}
		else if (PyFloat_Check(param)) {
			size = 1;
			buff[0] = (float)PyFloat_AsDouble(param);
		}
		if (size == 0) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		switch (size) {
		case 1: type = ParamTypeFloat; break;
		case 2: type = ParamTypeFloat2; break;
		case 3: type = ParamTypeFloat3; break;
		case 4: type = ParamTypeFloat4; break;
		}
		if (!self->editablefigure->SetMaterialParam(materialName, paramName, buff, type)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SetMaterialParamTexture(editablefigure_obj* self, PyObject* args, PyObject* kwargs)
	{
		static char* kwlist[] = { "materialName","samplerName","textureName","pixel","width","height","wrap_s","wrap_t","minfilter","magfilter","mipfilter", NULL };

		char* materialName = nullptr;
		char* samplerName = nullptr;
		char* textureName = nullptr;
		PyObject* pixel = nullptr;
		int w = 0;
		int h = 0;
		int wrap_s = SamplerState::WRAP;
		int wrap_t = SamplerState::WRAP;
		int minfilter = SamplerState::LINEAR_MIPMAP_LINEAR;
		int magfilter = SamplerState::LINEAR;
		int mipfilter = SamplerState::LINEAR_MIPMAP_LINEAR;

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sss|Oiiiiiii", kwlist,
			&materialName, &samplerName, &textureName,
			&pixel, &w, &h,
			&wrap_s, &wrap_t, &minfilter, &magfilter, &mipfilter)) return NULL;

		Sampler sampler;
		if (pixel) {
			if (!PyBytes_Check(pixel) || w == 0 || h == 0) {
				PyErr_SetString(PyExc_TypeError, "parameter error");
				return NULL;
			}
			char* pix = PyBytes_AsString(pixel);
			sampler.tex = pyxieResourceCreator::Instance().NewTexture(textureName, pix,w,h,true);
		}
		else {
			sampler.tex = pyxieResourceCreator::Instance().NewTexture(textureName);
		}
		TextureSource texsrc;
		pyxie_strncpy(texsrc.path, textureName, MAX_PATH);
		texsrc.normal = false;
		texsrc.wrap = false;
		sampler.textureNameIndex = self->editablefigure->SetTextureSource(texsrc);
		sampler.samplerSlot = 0;
		sampler.samplerState.wrap_s = wrap_s;
		sampler.samplerState.wrap_t = wrap_t;
		sampler.samplerState.minfilter = minfilter;
		sampler.samplerState.magfilter = magfilter;
		sampler.samplerState.mipfilter = mipfilter;

		if(!self->editablefigure->SetMaterialParam(materialName, samplerName, &sampler)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_GetMaterialParam(editablefigure_obj* self, PyObject* args)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SetMaterialRenderState(editablefigure_obj* self, PyObject* args, PyObject* kwargs)
	{
		static char* kwlist[] = { "materialName","paramName","value1","value2","value3","value4", NULL };
		char* mateName = nullptr;
		char* paramName = nullptr;
		PyObject* values[4] = {0,0,0,0};
		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssO|OOO", kwlist,
			&mateName, &paramName, &values[0], &values[1], &values[2], &values[3] )) return NULL;

		RenderStateInfo* stateInfo = nullptr;
		const ShaderParameterInfo* paramInfo = pyxieRenderContext::Instance().GetShaderParameterInfoByName(paramName);
		if(paramInfo) stateInfo = pyxieRenderContext::Instance().GetRenderStateInfo((ShaderParameterKey)paramInfo->key);
		if (!stateInfo) {
			PyErr_SetString(PyExc_ValueError, "unsupported render states");
			return NULL;
		}
		uint32_t val[4] = { 0,0,0,0 };
		for (int i = 0; i < 4; i++) {
			if (values) {
				if (stateInfo->valueTypes[i] == Int) val[i] = (uint32_t)PyLong_AsLong(values[i]);
				else if (stateInfo->valueTypes[i] == Float)  ((float*)val)[i] = (float)PyFloat_AsDouble(values[i]);
			}
		}
		self->editablefigure->SetMaterialState(mateName, (ShaderParameterKey)paramInfo->key, val);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject *editablefigure_SaveFigure(editablefigure_obj *self, PyObject *args)
	{
		char* path = nullptr;
		int excludeBaseAnime = 0;
		int excludeSkeleton = 0;

		if (PyArg_ParseTuple(args, "s|ii", &path, &excludeBaseAnime, &excludeSkeleton)){
			bool rv = self->editablefigure->SaveFigure(path, (bool)excludeBaseAnime, (bool)excludeSkeleton);
			return PyBool_FromLong((long)rv);
		}
		return 0;
	}

	static PyObject* editablefigure_GetTextureSource(editablefigure_obj* self)
	{
		const std::vector<TextureSource>& texes = self->editablefigure->GetTextureSources();
		PyObject* texsrces = PyTuple_New(texes.size());

		int idx = 0;
		for (auto it = texes.begin(); it != texes.end(); it++) {
			PyObject* texsrc = Py_BuildValue("{s:s,s:b,s:b}",
				"path", it->path,
				"wrap", it->wrap,
				"normal", it->normal);
			PyTuple_SetItem(texsrces, idx, texsrc);
			idx++;
		}
		return texsrces;
	}


	bool DictToTextureSource(PyObject* dict, TextureSource& texsrc) {
		PyObject* key_obj;
		PyObject* value_obj;
		Py_ssize_t pos = 0;
		while (PyDict_Next(dict, &pos, &key_obj, &value_obj))
		{
			if (!PyUnicode_Check(key_obj)) return false;
			Py_ssize_t data_len;
			const char* key_str = PyUnicode_AsUTF8AndSize(key_obj, &data_len);
			if (strcmp(key_str, "path") == 0) {
				if (!PyUnicode_Check(value_obj)) return false;
				const char* obj_str = PyUnicode_AsUTF8AndSize(value_obj, &data_len);
				pyxie_strncpy(texsrc.path, obj_str, MAX_PATH);
			}
			else if (strcmp(key_str, "normal") == 0) {
				if (!PyLong_Check(value_obj)) return false;
				texsrc.normal = (bool)PyLong_AsLong(value_obj);
			}
			else if (strcmp(key_str, "wrap") == 0) {
				if (!PyLong_Check(value_obj)) return false;
				texsrc.wrap = (bool)PyLong_AsLong(value_obj);
			}
			else {
				return false;
			}
		}
		ReplaceSep(texsrc.path);
		return true;
	}

	static PyObject* editablefigure_ReplaceTextureSource(editablefigure_obj* self, PyObject* args)
	{
		PyObject* oldTex=nullptr;
		PyObject* newTex = nullptr;
		TextureSource oldTexsrc;
		TextureSource newTexsrc;

		if (!PyArg_ParseTuple(args, "OO", &oldTex, &newTex)){
			return NULL;
		}
		if (!PyDict_Check(oldTex)) {

			return NULL;
		}
		if (!PyDict_Check(newTex)) {
			return NULL;
		}
		if (!DictToTextureSource(oldTex, oldTexsrc)) {
			return NULL;
		}
		if (!DictToTextureSource(newTex, newTexsrc)) {
			return NULL;
		}
		self->editablefigure->ReplaceTextureSource(oldTexsrc, newTexsrc);

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_ConvertTextureToPlatform(editablefigure_obj* self, PyObject* args)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_CompressFolder(editablefigure_obj* self, PyObject* args)
	{
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* editablefigure_Clear(editablefigure_obj* self)
	{
		self->editablefigure->ClearAll();
		Py_INCREF(Py_None);
		return Py_None;
	}
	static PyObject* editablefigure_ClearMesh(editablefigure_obj* self)
	{
		self->editablefigure->ClearAllMeshes();
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_setVertexPtr(editablefigure_obj* self, PyObject* args) {

		char* nodename = nullptr;
		PyObject* ptr;
		uint64_t numVerts = 0;
		PyObject* attr= nullptr;
		if (!PyArg_ParseTuple(args, "sOkO", &nodename, &ptr, &numVerts, &attr)) {
			return NULL;
		}
		uint32_t numAttr = 0;
		VertexAttribute attribute[NUM_ATTRIBUTE_ID];
		if (PyTuple_Check(attr)) {
			numAttr = (uint32_t)PyTuple_Size(attr);
			for (uint32_t i = 0; i < numAttr; i++) {
				PyObject* v = PyTuple_GET_ITEM(attr, i);
				if (PyTuple_Check(v)) {
					int n = (int)PyTuple_Size(v);
					if (n != 4) { numAttr = 0; break;}
					PyObject* tmp;
					tmp = PyTuple_GET_ITEM(v, 0);
					if(!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].id = PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 1);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].size = PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 2);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].normalize = PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 3);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].type = PyLong_AsLong(tmp);
				}
				else { numAttr = 0; break; }
			}
		}
		if (numAttr == 0) {
			PyErr_SetString(PyExc_TypeError, "attribute param error ((id,size,nom,type),(),()...)");
			return NULL;
		}
		void* vrts;
		if(sizeof(void*) == 8)
			vrts = (void*)PyLong_AsLongLong(ptr);
		else
			vrts = (void*)PyLong_AsLong(ptr);

		if (!self->editablefigure->SetVertexPointer(nodename, vrts, numVerts, attribute, numAttr)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_setTrianglePtr(editablefigure_obj* self, PyObject* args) {

		char* nodename = nullptr;
		PyObject* ptr;
		uint64_t numTriangles = 0;
		uint32_t size = 2;

		if (!PyArg_ParseTuple(args, "sOk|i", &nodename, &ptr, &numTriangles, &size)) {
			return NULL;
		}

		void* triangles;
		if (sizeof(void*) == 8)
			triangles = (void*)PyLong_AsLongLong(ptr);
		else
			triangles = (void*)PyLong_AsLong(ptr);

		if (!self->editablefigure->SetIndexPointer(nodename, triangles, numTriangles, size)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_addDrawSet(editablefigure_obj* self, PyObject* args) {
		char* nodename = nullptr;
		uint32_t offset = 0;
		uint32_t size = 0;

		if (!PyArg_ParseTuple(args, "sii", &nodename, &offset, &size)) {
			return NULL;
		}
		if (!self->editablefigure->AddDrawSet(nodename, offset, size)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_setDrawSetRenderState(editablefigure_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "nodeName","drawSetNo","paramName","value1","value2","value3","value4", NULL };
		char* nodeName = nullptr;
		char* paramName = nullptr;
		int setNo = 0;
		PyObject* values[4] = { 0,0,0,0 };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "sisO|OOO", kwlist,
			&nodeName, &setNo, &paramName, &values[0], &values[1], &values[2], &values[3])) return NULL;

		RenderStateInfo* stateInfo = nullptr;
		const ShaderParameterInfo* paramInfo = pyxieRenderContext::Instance().GetShaderParameterInfoByName(paramName);
		if (paramInfo) stateInfo = pyxieRenderContext::Instance().GetRenderStateInfo((ShaderParameterKey)paramInfo->key);
		if (!stateInfo) {
			PyErr_SetString(PyExc_ValueError, "unsupported render states");
			return NULL;
		}
		uint32_t val[4] = { 0,0,0,0 };
		for (int i = 0; i < 4; i++) {
			if (values) {
				if (stateInfo->valueTypes[i] == Int) val[i] = (uint32_t)PyLong_AsLong(values[i]);
				else if (stateInfo->valueTypes[i] == Float)  ((float*)val)[i] = (float)PyFloat_AsDouble(values[i]);
			}
		}
		if (!self->editablefigure->AddDrawSetState(nodeName, setNo, (ShaderParameterKey)paramInfo->key, val)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	PyMethodDef editablefigure_methods[] = {
		{ "addMaterial", (PyCFunction)editablefigure_AddMaterial, METH_VARARGS },
		{ "addMesh", (PyCFunction)editablefigure_AddMesh, METH_VARARGS },
		{ "setVertexElements", (PyCFunction)editablefigure_SetVertexElements, METH_VARARGS },
		{ "setTriangles", (PyCFunction)editablefigure_SetTriangles, METH_VARARGS },
		{ "addJoint", (PyCFunction)editablefigure_addJoint, METH_VARARGS | METH_KEYWORDS },
		{ "setVertexPtr", (PyCFunction)editablefigure_setVertexPtr, METH_VARARGS },
		{ "setTrianglePtr", (PyCFunction)editablefigure_setTrianglePtr, METH_VARARGS},
		{ "addDrawSet", (PyCFunction)editablefigure_addDrawSet, METH_VARARGS },
		{ "setDrawSetRenderState", (PyCFunction)editablefigure_setDrawSetRenderState, METH_VARARGS | METH_KEYWORDS},
		{ "setMaterialParam", (PyCFunction)editablefigure_SetMaterialParam, METH_VARARGS },
		{ "setMaterialParamTexture", (PyCFunction)editablefigure_SetMaterialParamTexture, METH_VARARGS | METH_KEYWORDS },
		{ "getMaterialParam", (PyCFunction)editablefigure_GetMaterialParam, METH_VARARGS },
		{ "setMaterialRenderState", (PyCFunction)editablefigure_SetMaterialRenderState, METH_VARARGS | METH_KEYWORDS},
		{ "saveFigure", (PyCFunction)editablefigure_SaveFigure, METH_VARARGS },
		{ "getTextureSource", (PyCFunction)editablefigure_GetTextureSource, METH_NOARGS },
		{ "replaceTextureSource", (PyCFunction)editablefigure_ReplaceTextureSource, METH_VARARGS },
		{ "convertTextureToPlatform", (PyCFunction)editablefigure_ConvertTextureToPlatform, METH_VARARGS },
		{ "compressFolder", (PyCFunction)editablefigure_CompressFolder, METH_VARARGS },
		{ "clear", (PyCFunction)editablefigure_Clear, METH_NOARGS },
		{ "clearMesh", (PyCFunction)editablefigure_ClearMesh, METH_NOARGS },
		{ NULL,	NULL }
	};

	PyGetSetDef editablefigure_getsets[] = {
		{ const_cast<char*>("position"), (getter)editablefigure_getPosition, (setter)editablefigure_setPosition,NULL, NULL },
		{ const_cast<char*>("rotation"), (getter)editablefigure_getRotation, (setter)editablefigure_setRotation,NULL, NULL },
		{ const_cast<char*>("scale"),    (getter)editablefigure_getScale,    (setter)editablefigure_setScale,NULL, NULL },
		{ NULL, NULL }
	};

	PyMemberDef editablefigure_members[] = {
		{"VertexAttribure_POSITION",T_INT, offsetof(editablefigure_obj, VertexAttribure_POSITION), READONLY, NULL},
		{"VertexAttribure_NORMAL",T_INT, offsetof(editablefigure_obj, VertexAttribure_NORMAL), READONLY, NULL},
		{"VertexAttribure_TANGENT",T_INT, offsetof(editablefigure_obj, VertexAttribure_TANGENT), READONLY, NULL},
		{"VertexAttribure_BINORMAL",T_INT, offsetof(editablefigure_obj, VertexAttribure_BINORMAL), READONLY, NULL},
		{"VertexAttribure_UV0",T_INT, offsetof(editablefigure_obj, VertexAttribure_UV0), READONLY, NULL},
		{"VertexAttribure_UV1",T_INT, offsetof(editablefigure_obj, VertexAttribure_UV1), READONLY, NULL},
		{"VertexAttribure_UV2",T_INT, offsetof(editablefigure_obj, VertexAttribure_UV2), READONLY, NULL},
		{"VertexAttribure_UV3",T_INT, offsetof(editablefigure_obj, VertexAttribure_UV3), READONLY, NULL},
		{"VertexAttribure_COLOR",T_INT, offsetof(editablefigure_obj, VertexAttribure_COLOR), READONLY, NULL},
		{"VertexAttribure_BLENDINDICES",T_INT, offsetof(editablefigure_obj, VertexAttribure_BLENDINDICES), READONLY, NULL},
		{"VertexAttribure_BLENDWEIGHT",T_INT, offsetof(editablefigure_obj, VertexAttribure_BLENDWEIGHT), READONLY, NULL},
		{"VertexAttribure_PSIZE",T_INT, offsetof(editablefigure_obj, VertexAttribure_PSIZE), READONLY, NULL},
		{NULL}  /* Sentinel */
	};


	PyTypeObject EditableFigureType = {
		PyVarObject_HEAD_INIT(NULL, 0)
		"pyxie.editableFigure",				/* tp_name */
		sizeof(editablefigure_obj),         /* tp_basicsize */
		0,                                  /* tp_itemsize */
		(destructor)editablefigure_dealloc,	/* tp_dealloc */
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
		(reprfunc)editablefigure_str,       /* tp_str */
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
		editablefigure_methods,				/* tp_methods */
		editablefigure_members,             /* tp_members */
		editablefigure_getsets,             /* tp_getset */
		0,                                  /* tp_base */
		0,                                  /* tp_dict */
		0,                                  /* tp_descr_get */
		0,                                  /* tp_descr_set */
		0,                                  /* tp_dictoffset */
		0,                                  /* tp_init */
		0,                                  /* tp_alloc */
		editablefigure_new,					/* tp_new */
		0,									/* tp_free */
	};

}