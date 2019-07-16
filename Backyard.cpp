#include "pyxie.h"
#include "Backyard.h"
#include "pyxieShowcase.h"
#include "pyxieFigure.h"
#include "pyxieCamera.h"
#include "pyxieRenderContext.h"

#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>

namespace pyxie {

	struct RenderSet {
		pyxieCamera* camera;
		pyxieShowcase* showcas;
	};
	std::vector<RenderSet> renderSets;


	Backyard* Backyard::instance;
	Backyard& Backyard::Instance() { return *instance; }
	
	Backyard::~Backyard() {
		for (auto itr = renderSets.begin(); itr != renderSets.end(); ++itr) {
			(*itr).camera->DecReference();
			(*itr).showcas->DecReference();
		}
		renderSets.clear();
	}

	void Backyard::Delete() {
		PYXIE_SAFE_DELETE(instance); 
	}


	std::mutex python_mtx;
	std::condition_variable python_cv;
	bool is_readyPython = false;
	bool wake_Python = true;

	void Backyard::SyncPython() {
		std::unique_lock<std::mutex> python_lk(python_mtx);
		is_readyPython = false;
		wake_Python = false;
		python_cv.wait(python_lk, [] {return is_readyPython; });
		wake_Python = true;
	}
	void Backyard::SyncMain() {
		if (!wake_Python) {
			Render();
			is_readyPython = true;
			python_cv.notify_one();
		}
	}
	void Backyard::WakeBoth() {
		is_readyPython = true;
		python_cv.notify_one();
	}


/*
	std::mutex main_mtx;
	std::mutex python_mtx;

	std::condition_variable main_cv;
	std::condition_variable python_cv;

	bool is_readyMain = false;
	bool is_readyPython = false;

	bool wake_Main = true;
	bool wake_Python = true;

	void Backyard::WakeBoth() {
		is_readyMain = true;
		is_readyPython = true;
		python_cv.notify_one();
		main_cv.notify_one();
	}

	void Backyard::SyncMain() {
		std::unique_lock<std::mutex> main_lk(main_mtx);
		while (!wake_Python) {
			is_readyPython = true;
			python_cv.notify_one();
		}
		is_readyMain = false;
		wake_Main = false;
		main_cv.wait(main_lk, [] {
			return is_readyMain; 
			});
		wake_Main = true;
	}

	void Backyard::SyncPython() {
		std::unique_lock<std::mutex> python_lk(python_mtx);

		while (!wake_Main) {
			is_readyMain = true;
			main_cv.notify_one();
		}
		is_readyPython = false;
		wake_Python = false;
		python_cv.wait(python_lk, [] { 
			if (is_readyPython) return true;
			if (is_readyMain) return false;
			is_readyPython = true;
			return true;
			});
		wake_Python = true;
	}
*/

	void Backyard::RenderRequest(pyxieCamera* camera, pyxieShowcase* showcase){
		camera->IncReference();
		showcase->IncReference();

		RenderSet rset;
		rset.camera = camera;
		rset.showcas = showcase;
		renderSets.push_back(rset);
	}

	void Backyard::Render() {

		pyxieRenderContext& renderContext = pyxieRenderContext::Instance();

		bool clearColor = true;
		for (auto itr = renderSets.begin(); itr != renderSets.end(); ++itr) {
			renderContext.BeginScene(NULL, Vec4(0.2f, 0.6f, 0.8f), clearColor);
			(*itr).showcas->Update(0.0f);
			(*itr).camera->Render();
			(*itr).showcas->Render();
			renderContext.EndScene();
			(*itr).camera->DecReference();
			(*itr).showcas->DecReference();
			clearColor = false;
		}
		//imguiRender();
		renderSets.clear();
	}
}
