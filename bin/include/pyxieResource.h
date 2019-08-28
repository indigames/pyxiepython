///////////////////////////////////////////////////////////////
//Pyxie game engine
//
//  Copyright Kiharu Shishikura 2019. All rights reserved.
///////////////////////////////////////////////////////////////
#pragma once
#include "pyxieObject.h"
#include "pyxieDebug.h"
#include <thread>

namespace pyxie
{
	class PYXIE_EXPORT pyxieResource : public pyxieObject
	{
	protected:
		enum
		{
			state_buildEnd = 1,
			state_initializeEnd = (state_buildEnd << 1),
			state_buildSuccess = (state_initializeEnd << 1),
			state_initializeSuccess = (state_buildSuccess << 1),
			state_resource_last = state_initializeSuccess
		};
		uint32_t state;

		struct InstanceInfo
		{
			uint32_t totalCount;
			uint32_t instanceCount;
			pyxieResource* original;
			char* resName;
		};
		InstanceInfo* instanceInfo;
		uint32_t instanceID;
		int referenceCount;
		ErrorCode errorCode;

	public:
		pyxieResource();
		pyxieResource(const pyxieResource* org);
		virtual ~pyxieResource();

		virtual void Build()=0;
		virtual void Initialize()=0;
		virtual void Clone(bool afterFinishBuild) = 0;
		virtual RESOURCETYPE ResourceType() = 0;


		inline void SetState(uint32_t stateBit) { state |= stateBit; }
		inline void ResetState(uint32_t stateBit) { state &= (~stateBit); }
		inline bool CheckState(uint32_t stateBit) { return (state & stateBit)?true:false; }

		inline bool IsBuidEnd() { return CheckState(state_buildEnd); }
		inline bool IsBuidSuccess() { return CheckState(state_buildSuccess); }
		inline bool IsInitializeEnd() { return CheckState(state_initializeEnd); }
		inline bool IsInitializeSuccess() { return CheckState(state_initializeSuccess); }

		inline bool WaitBuild() { 
			if (CheckState(state_buildSuccess)) return true; 
			_WaitUntilBuildIsEnd();
			return CheckState(state_buildSuccess);
		}
		inline bool WaitInitialize() {
			if (CheckState(state_initializeSuccess)) return true;
			_WaitUntilInitializeIsEnd();
			return CheckState(state_initializeSuccess);
		}
		inline void Uninitialize() { ResetState(state_initializeEnd | state_initializeSuccess); }
		inline uint16_t GetErrorCode() { return errorCode; }

		inline void IncReference() { referenceCount++; }
		inline void DecReference() {
			pyxie_assert(referenceCount > 0);
			referenceCount--;
		}
		inline int ReferenceCount() { return referenceCount; }
		inline int InstanceCount() { return instanceInfo->instanceCount; }
		inline const char* ResourceName() { return instanceInfo->resName; }
		inline int InstanceID() { return instanceID; }
		inline pyxieResource* Original() { return instanceInfo->original; }
		inline bool IsOriginal() { return (instanceInfo->original == this); }
		void SetResourceName(const char* name);


#ifdef __ENABLE_SUSPEND_RECOVER__
		virtual bool Restore() { return true; }
		virtual bool Release() { return true; }
#endif
	protected:
		inline void BuildSuccess() { SetState(state_buildEnd | state_buildSuccess); }
		inline void InitializeSuccess() { SetState(state_initializeEnd | state_initializeSuccess); }
		void BuildError(ErrorCode code);
		void InitializeError(ErrorCode code);
		void InitializeError(const char* errmsg);
		void ShaderError();
		void _WaitUntilBuildIsEnd();
		void _WaitUntilInitializeIsEnd();

	};


	class PYXIE_EXPORT pyxieResourcePath {
		char name[MAX_PATH];
		RESOURCETYPE type;
		uint32_t instanceID;
		char outbuf[MAX_PATH];
	public:
		pyxieResourcePath(const char* filePath, RESOURCETYPE _type, uint32_t _instanceID=0);
		pyxieResourcePath(pyxieResource* res);
		void Set(const char* filePath, RESOURCETYPE _type, uint32_t _instanceID);

		const char* ResourceName();// "dir/dir/file.ext:999"
		const char* FilePath(bool ext=true);
		const char* Directory();

		uint32_t InstanceID() { return instanceID; }
		RESOURCETYPE ResourceType() { return type; }
	};

}