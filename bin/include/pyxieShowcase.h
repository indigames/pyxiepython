#pragma once

#include "pyxieResource.h"
#include <unordered_set>

namespace pyxie {
	class pyxieFigure;
	class pyxieEnvironmentSet;

	class PYXIE_EXPORT pyxieShowcase : public pyxieResource {
		std::unordered_set<pyxieFigure*> figures;
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
		void Update(float dt);
		void Render();
	};

}
