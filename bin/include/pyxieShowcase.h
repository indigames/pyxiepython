#pragma once

#include "pyxieResource.h"
#include <unordered_set>
#include <list>

namespace pyxie {
	class pyxieFigure;
	class pyxieEnvironmentSet;

	class PYXIE_EXPORT pyxieShowcase : public pyxieResource {
		std::list<pyxieFigure*> figures;
		pyxieEnvironmentSet* environmentSet;
	public:
		pyxieShowcase(const char* name);
		pyxieShowcase(pyxieShowcase* org);
		~pyxieShowcase();
		void Build();
		void Initialize();
		void Clone(bool afterFinishBuild);
		RESOURCETYPE ResourceType() { return SHOWCASETYPE; }

		void Add(pyxieResource* res);
		void Remove(pyxieResource* res);
		void Clear();
		void Update(float dt);
		void Render();
	};

}
