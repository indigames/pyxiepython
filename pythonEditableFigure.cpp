#include "pyxie.h"
#include "pythonResource.h"
#include "pyxieResourceCreator.h"
#include "pyxieTime.h"
#include "structmember.h"
#include "pyVectorMath.h"
#include "pythonEditableFigure_doc_en.h"

namespace pyxie
{
	////////////////////////////////////////////////////////
	//Local funcs
	////////////////////////////////////////////////////////
	int pyObjToIntArray(PyObject* obj, uint32_t* idx) {

		int totalCount = 0;
		int type = -1;
		if (PyTuple_Check(obj)) type = 0;
		else if (PyList_Check(obj))  type = 1;
		if (type == -1) return 0;

		int elementCount = 0;
		int numElem = (type == 0) ? PyTuple_Size(obj) : PyList_Size(obj);
		for (int i = 0; i < numElem; i++) {
			PyObject* element = (type == 0) ? PyTuple_GET_ITEM(obj, i) : PyList_GET_ITEM(obj, i);
			if (PyLong_Check(element)) {
				if (idx)idx[totalCount] = PyLong_AsLong(element);
				totalCount++;
				elementCount++;
				if (elementCount >= 3) elementCount = 0;
			}
			else if (PyTuple_Check(element)) {
				int d = (int)PyTuple_Size(element);
				for (int j = 0; j < d; j++) {
					PyObject* val = PyTuple_GET_ITEM(element, j);
					if (idx)idx[totalCount] = PyLong_AsLong(val);
					totalCount++;
					elementCount++;
					if (elementCount >= 3) break;
				}
				elementCount = 0;
			}
			else if (PyList_Check(element)) {
				int d = (int)PyList_Size(element);
				for (int j = 0; j < d; j++) {
					PyObject* val = PyList_GET_ITEM(element, j);
					if (idx)idx[totalCount] = PyLong_AsLong(val);
					totalCount++;
					elementCount++;
					if (elementCount >= 3) break;
				}
				elementCount = 0;
			}
		}
		return totalCount;
	}

	int GetMeshIndex(editablefigure_obj* self, PyObject* arg) {
		int index = -1;
		if (PyLong_Check(arg)) {
			index = PyLong_AsLong(arg);
		}
		else if (PyUnicode_Check(arg)) {
			Py_ssize_t data_len;
			const char* key_str = PyUnicode_AsUTF8AndSize(arg, &data_len);
			index = self->editablefigure->GetMeshIndex(GenerateNameHash(key_str));
		}
		if (index == -1) {
			PyErr_SetString(PyExc_TypeError, "mesh not found.");
		}
		return index;
	}

	////////////////////////////////////////////////////////
	//python funcs
	////////////////////////////////////////////////////////
	PyObject *editablefigure_new(PyTypeObject *type, PyObject *args, PyObject *kw) {

		auto t = &EditableFigureType;

		char* path;
		editablefigure_obj * self = NULL;
		if (PyArg_ParseTuple(args, "s", &path)) {
			self = (editablefigure_obj*)type->tp_alloc(type, 0);
			//self = PyObject_New(editablefigure_obj, &EditableFigureType);
			self->editablefigure = pyxieResourceCreator::Instance().NewEditableFigure(path);
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

	PyObject* editablefigure_numJoints(editablefigure_obj* self)
	{
		return PyLong_FromLong(self->editablefigure->NumJoints());
	}

	PyObject* editablefigure_numMeshes(editablefigure_obj* self)
	{
		return PyLong_FromLong(self->editablefigure->NumMeshes());
	}

	PyObject* editablefigure_numMaterials(editablefigure_obj* self)
	{
		return PyLong_FromLong(self->editablefigure->NumMaterials());
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

	static PyObject* editablefigure_SetVertexElements(editablefigure_obj* self, PyObject* args)
	{
		//efig.setVertexElemetns("mesh01", efig.ElementType_POSITION, 0, 4, points)
		PyObject* arg;
		int vertAttr;
		int offset = 0;
		PyObject* points;

		if (PyArg_ParseTuple(args, "OiO|i", &arg, &vertAttr, &points, &offset)) {
			if (vertAttr < 1 || vertAttr >= NUM_ATTRIBUTE_ID) {
				PyErr_SetString(PyExc_TypeError, "The value of vertexAttribure is incorrect.");
				return NULL;
			}
			const VertexAttribute& attr = pyxieEditableFigure::GetVertexAttribute((AttributeID)vertAttr);
			int inputVertexSize = attr.size;
			int bufferSize = pyObjToFloatArray(points, nullptr, inputVertexSize);
			if (bufferSize == 0) {
				PyErr_SetString(PyExc_TypeError, "Parameter error.");
				return NULL;
			}

			int index = GetMeshIndex(self, arg);
			if (index == -1) return NULL;

			float* buffer = (float*)PYXIE_MALLOC(bufferSize * sizeof(float));
			MemoryCleaner creaner(buffer);
			pyObjToFloatArray(points, buffer, inputVertexSize);
			self->editablefigure->SetMeshVertexValues(index, buffer, bufferSize / inputVertexSize, (AttributeID)vertAttr, offset);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_GetVertexElements(editablefigure_obj* self, PyObject* args) {

		PyObject* arg;
		int vertAttr;
		int offset = 0;
		int size = 100000000;

		if (!PyArg_ParseTuple(args, "Oi|ii", &arg, &vertAttr, &offset, &size)) return NULL;

		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;
		EditableMesh* emesh = self->editablefigure->GetMesh(index);

		uint32_t offs = 0;
		uint32_t number = 0;
		uint32_t datasize = 0;

		for (uint32_t i = 0; i < emesh->numVertexAttributes; i++) {
			if (emesh->attributes[i].id == vertAttr) {
				offs = emesh->attributes[i].offset;
				number = emesh->attributes[i].size;
				break;
			}
		}
		if (size + offset > emesh->numVertices) size = emesh->numVertices - offset;

		if (size <= 0) {
			Py_INCREF(Py_None);
			return Py_None;
		}

		PyTypeObject* type;
		switch (number) {
		case 2: type = _Vec2Type; break;
		case 3: type = _Vec3Type; break;
		case 4: type = _Vec4Type; break;
		}
		PyObject* verts = PyTuple_New(size);
		if (!verts) return NULL;

		for (int i = 0; i < size; i++) {
			float* location = emesh->vertices + ((i + offset) * emesh->numFloatsPerVertex) + offs;
			vec_obj* vrobj = PyObject_New(vec_obj, type);
			vrobj->d = number;
			for (int j = 0; j < number; j++) {
				vrobj->v[j] = location[j];
			}
			PyTuple_SetItem(verts, i, (PyObject*)vrobj);
		}

		return verts;
	}

	static PyObject* editablefigure_SetTriangles(editablefigure_obj* self, PyObject* args)
	{
		PyObject* arg;
		int offset=0;
		PyObject* tris;
		if (PyArg_ParseTuple(args, "OO|i", &arg, &tris, &offset)) {



			int bufferSize = pyObjToIntArray(tris, nullptr);
			if (bufferSize == 0) {
				PyErr_SetString(PyExc_TypeError, "Parameter error.");
				return NULL;
			}
			uint32_t* buffer = (uint32_t*)PYXIE_MALLOC(bufferSize * sizeof(int));
			MemoryCleaner creaner(buffer);
			pyObjToIntArray(tris, buffer);

			int index = GetMeshIndex(self, arg);
			if (index == -1) return NULL;

			self->editablefigure->SetMeshIndices(index, offset, buffer, bufferSize / 3, 4);
		}
		Py_INCREF(Py_None);
		return Py_None;
	}


	static PyObject* editablefigure_setVertexPtr(editablefigure_obj* self, PyObject* args) {

		PyObject* arg = nullptr;
		PyObject* ptr;
		uint64_t numVerts = 0;
		PyObject* attr = nullptr;
		if (!PyArg_ParseTuple(args, "OOkO", &arg, &ptr, &numVerts, &attr)) {
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
					if (n != 4) { numAttr = 0; break; }
					PyObject* tmp;
					tmp = PyTuple_GET_ITEM(v, 0);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].id = (uint8_t)PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 1);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].size = (uint16_t)PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 2);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].normalize = (uint8_t)PyLong_AsLong(tmp);
					tmp = PyTuple_GET_ITEM(v, 3);
					if (!PyLong_Check(tmp)) { numAttr = 0; break; }
					attribute[i].type = (uint16_t)PyLong_AsLong(tmp);
				}
				else { numAttr = 0; break; }
			}
		}
		if (numAttr == 0) {
			PyErr_SetString(PyExc_TypeError, "attribute param error ((id,size,nom,type),(),()...)");
			return NULL;
		}


		void* vrts;
		if (PyLong_Check(ptr)) {
			if (sizeof(void*) == 8)
				vrts = (void*)PyLong_AsLongLong(ptr);
			else
				vrts = (void*)PyLong_AsLong(ptr);
		}


		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;

		if (!self->editablefigure->SetMeshVertexAttributes(index, attribute, numAttr)) {
			PyErr_SetString(PyExc_TypeError, "faild to set vertex attributes.");
			return NULL;
		}
		if (!self->editablefigure->SetMeshVertices(index, vrts, (uint32_t)numVerts)) {
			PyErr_SetString(PyExc_TypeError, "faild to set vertex.");
			return NULL;
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_setTrianglePtr(editablefigure_obj* self, PyObject* args) {

		PyObject* arg = nullptr;
		PyObject* ptr;
		uint64_t numTriangles = 0;
		uint32_t size = 2;

		if (!PyArg_ParseTuple(args, "OOk|i", &arg, &ptr, &numTriangles, &size)) {
			return NULL;
		}

		void* triangles;
		if (sizeof(void*) == 8)
			triangles = (void*)PyLong_AsLongLong(ptr);
		else
			triangles = (void*)PyLong_AsLong(ptr);

		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;

		if (!self->editablefigure->SetMeshIndices(index, 0, (uint32_t*)triangles, numTriangles, size)) {
			PyErr_SetString(PyExc_TypeError, "failed to set triangles.");
			return NULL;
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
		if (parentJointName) parentIndex = self->editablefigure->GetJointIndex(GenerateNameHash(parentJointName));
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
				if (!(PyFloat_Check(item) | PyLong_Check(item))) {
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
				if (!(PyFloat_Check(item) | PyLong_Check(item))) {
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

		PyObject* textureName = nullptr;

		PyObject* pixel = nullptr;
		int w = 0;
		int h = 0;
		int wrap_s = SamplerState::WRAP;
		int wrap_t = SamplerState::WRAP;
		int minfilter = SamplerState::LINEAR_MIPMAP_LINEAR;
		int magfilter = SamplerState::LINEAR;
		int mipfilter = SamplerState::LINEAR_MIPMAP_LINEAR;

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssO|Oiiiiiii", kwlist,
			&materialName, &samplerName, &textureName,
			&pixel, &w, &h,
			&wrap_s, &wrap_t, &minfilter, &magfilter, &mipfilter)) return NULL;

		Sampler sampler;
		if (textureName->ob_type == &TextureType) {
			((texture_obj*)textureName)->colortexture->IncReference();
			sampler.tex = ((texture_obj*)textureName)->colortexture;
		}
		else {
			const char* nameText = PyUnicode_AsUTF8(textureName);
			if (pixel) {
				if (!PyBytes_Check(pixel) || w == 0 || h == 0) {
					PyErr_SetString(PyExc_TypeError, "parameter error");
					return NULL;
				}
				char* pix = PyBytes_AsString(pixel);
				sampler.tex = pyxieResourceCreator::Instance().NewTexture(nameText, pix, w, h, true);
			}
			else {
				sampler.tex = pyxieResourceCreator::Instance().NewTexture(nameText);
			}
			TextureSource texsrc;
			pyxie_strncpy(texsrc.path, nameText, MAX_PATH);
			texsrc.normal = false;
			texsrc.wrap = false;
			sampler.textureNameIndex = self->editablefigure->SetTextureSource(texsrc);
		}
		sampler.samplerSlot = 0;
		sampler.samplerState.wrap_s = wrap_s;
		sampler.samplerState.wrap_t = wrap_t;
		sampler.samplerState.minfilter = minfilter;
		sampler.samplerState.magfilter = magfilter;
		sampler.samplerState.mipfilter = mipfilter;

		if(!self->editablefigure->SetMaterialParam(materialName, samplerName, &sampler)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			if (sampler.tex)sampler.tex->DecReference();
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

	static PyObject* editablefigure_SaveSkeleton(editablefigure_obj* self, PyObject* args) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SaveAnimation(editablefigure_obj* self, PyObject* args) {
		Py_INCREF(Py_None);
		return Py_None;
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

	static PyObject* editablefigure_addDrawSet(editablefigure_obj* self, PyObject* args) {
		PyObject* arg = nullptr;
		uint32_t offset = 0;
		uint32_t size = 0;

		if (!PyArg_ParseTuple(args, "Oii", &arg, &offset, &size)) {
			return NULL;
		}
		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;

		if (!self->editablefigure->AddMeshDrawSet(index, offset, size)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_setDrawSetRenderState(editablefigure_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "meshNameOrIndex","drawSetNo","paramName","value1","value2","value3","value4", NULL };
		PyObject* arg = nullptr;
		char* paramName = nullptr;
		int setNo = 0;
		PyObject* values[4] = { 0,0,0,0 };

		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OisO|OOO", kwlist,
			&arg, &setNo, &paramName, &values[0], &values[1], &values[2], &values[3])) return NULL;

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

		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;

		if (!self->editablefigure->AddMeshDrawSetState(index, setNo, (ShaderParameterKey)paramInfo->key, val)) {
			PyErr_SetString(PyExc_TypeError, "parameter error.");
			return NULL;
		}

		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_SetParentJoint(editablefigure_obj* self, PyObject* args) {

		PyObject* parent;
		char* jointName;
		if (!PyArg_ParseTuple(args, "Os", &parent, &jointName)) return NULL;

		auto drawable = ((pyxieDrawable*)(((resource_obj*)parent)->res));
		uint32_t hash = GenerateNameHash(jointName);
		int idx = drawable->GetJointIndex(hash);
		if (idx != -1) {
			self->editablefigure->SetParentJoint(drawable, drawable->GetJointMatrix(idx));
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_getJoint(editablefigure_obj* self, PyObject* args) {

		PyObject* arg;
		if (!PyArg_ParseTuple(args, "O", &arg))return NULL;

		int index = -1;
		if (PyLong_Check(arg)) {
			index = PyLong_AsLong(arg);
		}
		else if (PyUnicode_Check(arg)) {
			Py_ssize_t data_len;
			const char* key_str = PyUnicode_AsUTF8AndSize(arg, &data_len);
			index = self->editablefigure->GetJointIndex(GenerateNameHash(key_str));
		}

		Joint joint;
		if (index == -1) {
			pyxie_printf("joint is not found.");
		}
		else
		{
			joint = self->editablefigure->GetJoint(index);
		}
		PyObject* joint_obj = PyTuple_New(3);

		vec_obj* pos = (vec_obj*)PyObject_New(vec_obj, _Vec3Type);
		vec_obj* rot = (vec_obj*)PyObject_New(vec_obj, _QuatType);
		vec_obj* scale = (vec_obj*)PyObject_New(vec_obj, _Vec3Type);
		for (int i = 0; i < 4; i++) {
			pos->v[i] = joint.translation[i];
			rot->v[i] = joint.rotation[i];
			scale->v[i] = joint.scale[i];
		}
		pos->d = 3;
		rot->d = 4;
		scale->d = 3;
		PyTuple_SetItem(joint_obj, 0, (PyObject*)pos);
		PyTuple_SetItem(joint_obj, 1, (PyObject*)rot);
		PyTuple_SetItem(joint_obj, 2, (PyObject*)scale);
		return joint_obj;
	}

	static PyObject* editablefigure_setJoint(editablefigure_obj* self, PyObject* args, PyObject* kwargs) {

		static char* kwlist[] = { "jointName","position","rotation","scale", NULL };

		PyObject* arg;
		PyObject* arg1 = nullptr;
		PyObject* arg2 = nullptr;
		PyObject* arg3 = nullptr;
		if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|OOO", kwlist, &arg, &arg1, &arg2, &arg3))return NULL;

		int index = -1;
		if (PyLong_Check(arg)) {
			index = PyLong_AsLong(arg);
		}
		else if (PyUnicode_Check(arg)) {
			Py_ssize_t data_len;
			const char* key_str = PyUnicode_AsUTF8AndSize(arg, &data_len);
			index = self->editablefigure->GetJointIndex(GenerateNameHash(key_str));
		}
		if (index != -1) {
			Joint joint = self->editablefigure->GetJoint(index);
			float* v;
			int d;
			float buff[4];
			if (arg1) {
				v = pyObjToFloat(arg1, buff, d);
				for (int i = 0; i < d; i++) joint.translation[i] = v[i];
			}
			if (arg2) {
				v = pyObjToFloat(arg2, buff, d);
				for (int i = 0; i < d; i++) joint.rotation[i] = v[i];
			}
			if (arg3) {
				v = pyObjToFloat(arg3, buff, d);
				for (int i = 0; i < d; i++) joint.scale[i] = v[i];
			}
			self->editablefigure->SetJoint(index, joint);
		}
		else {
			pyxie_printf("joint not found.");
		}
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_mergeMesh(editablefigure_obj* self)
	{
		self->editablefigure->MergeMesh();
		Py_INCREF(Py_None);
		return Py_None;
	}

	static PyObject* editablefigure_getMeshAABB(editablefigure_obj* self, PyObject* args) {
		PyObject* arg;
		if (!PyArg_ParseTuple(args, "O", &arg)) return NULL;
		int index = GetMeshIndex(self, arg);
		if (index == -1) return NULL;

		vec_obj* min = PyObject_New(vec_obj, _Vec3Type);
		vec_obj* max = PyObject_New(vec_obj, _Vec3Type);
		self->editablefigure->CalcMeshAABBox(index, min->v, max->v);
		min->d = 3;
		max->d = 3;

		PyObject* tuple = PyTuple_New(2);
		PyTuple_SetItem(tuple, 0, (PyObject*)min);
		PyTuple_SetItem(tuple, 1, (PyObject*)max);

		return tuple;
	}


	PyMethodDef editablefigure_methods[] = {
	{ "addMaterial", (PyCFunction)editablefigure_AddMaterial, METH_VARARGS,addMaterial_doc  },
	{ "addMesh", (PyCFunction)editablefigure_AddMesh, METH_VARARGS, addMesh_doc},
	{ "addJoint", (PyCFunction)editablefigure_addJoint, METH_VARARGS | METH_KEYWORDS, addJoint_doc},
	{ "setTriangles", (PyCFunction)editablefigure_SetTriangles, METH_VARARGS, setTriangles_doc},
	{ "setTrianglePtr", (PyCFunction)editablefigure_setTrianglePtr, METH_VARARGS,setTrianglePtr_doc},
	{ "setVertexElements", (PyCFunction)editablefigure_SetVertexElements, METH_VARARGS, setVertexElements_doc},
	{ "getVertexElements", (PyCFunction)editablefigure_GetVertexElements, METH_VARARGS, getVertexElements_doc},
	{ "setVertexPtr", (PyCFunction)editablefigure_setVertexPtr, METH_VARARGS, setVertexPtr_doc},
	{ "addDrawSet", (PyCFunction)editablefigure_addDrawSet, METH_VARARGS,addDrawSet_doc },
	{ "setDrawSetRenderState", (PyCFunction)editablefigure_setDrawSetRenderState, METH_VARARGS | METH_KEYWORDS,setDrawSetRenderState_doc},
	{ "setMaterialParam", (PyCFunction)editablefigure_SetMaterialParam, METH_VARARGS,setMaterialParam_doc },
	{ "setMaterialParamTexture", (PyCFunction)editablefigure_SetMaterialParamTexture, METH_VARARGS | METH_KEYWORDS, setMaterialParamTexture_doc},
	{ "getMaterialParam", (PyCFunction)editablefigure_GetMaterialParam, METH_VARARGS, getMaterialParam_doc},
	{ "setMaterialRenderState", (PyCFunction)editablefigure_SetMaterialRenderState, METH_VARARGS | METH_KEYWORDS,setMaterialRenderState_doc},
	{ "saveFigure", (PyCFunction)editablefigure_SaveFigure, METH_VARARGS,saveFigure_doc },
	{ "saveSkeleton", (PyCFunction)editablefigure_SaveSkeleton, METH_VARARGS,saveSkeleton_doc },
	{ "saveAnimation", (PyCFunction)editablefigure_SaveAnimation, METH_VARARGS,saveAnimation_doc },
	{ "getTextureSource", (PyCFunction)editablefigure_GetTextureSource, METH_NOARGS, getTextureSource_doc},
	{ "replaceTextureSource", (PyCFunction)editablefigure_ReplaceTextureSource, METH_VARARGS, replaceTextureSource_doc},
	{ "clear", (PyCFunction)editablefigure_Clear, METH_NOARGS, clear_doc},
	{ "clearMesh", (PyCFunction)editablefigure_ClearMesh, METH_NOARGS, clearMesh_doc},
	{ "setParentJoint", (PyCFunction)editablefigure_SetParentJoint, METH_VARARGS, setParentJoint_doc},
	{ "getJoint", (PyCFunction)editablefigure_getJoint, METH_VARARGS, getJoint_doc},
	{ "setJoint", (PyCFunction)editablefigure_setJoint, METH_VARARGS | METH_KEYWORDS, setJoint_doc},
	{ "mergeMesh", (PyCFunction)editablefigure_mergeMesh, METH_VARARGS, mergeMesh_doc},
	{ "getMeshAABB", (PyCFunction)editablefigure_getMeshAABB, METH_VARARGS, getMeshAABB_doc},
	
	{ NULL,	NULL }
	};

	PyGetSetDef editablefigure_getsets[] = {
		{ const_cast<char*>("position"), (getter)editablefigure_getPosition, (setter)editablefigure_setPosition,position_doc, NULL },
		{ const_cast<char*>("rotation"), (getter)editablefigure_getRotation, (setter)editablefigure_setRotation,rotation_doc, NULL },
		{ const_cast<char*>("scale"),    (getter)editablefigure_getScale,    (setter)editablefigure_setScale,scale_doc, NULL },
		{ const_cast<char*>("numJoints"),(getter)editablefigure_numJoints,    (setter)NULL,numJoints_doc, NULL },
		{ const_cast<char*>("numMeshes"),(getter)editablefigure_numMeshes,    (setter)NULL,numMeshes_doc, NULL },
		{ const_cast<char*>("numMaterials"),(getter)editablefigure_numMaterials,    (setter)NULL,numMaterials_doc, NULL },

	{ NULL, NULL }
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
		0,									/* tp_members */
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
