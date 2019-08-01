#include "pyxie.h"
#include "Backyard.h"
#include "pyxieShowcase.h"
#include "pyxieFigure.h"
#include "pyxieCamera.h"
#include "pyxieRenderContext.h"
#include "pyxieRenderTarget.h"
#include "pyxieTime.h"

#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <iostream>
#include <exception>
#include <python.h>
namespace pyxie {

	struct RenderSet {
		pyxieCamera* camera;
		pyxieShowcase* showcas;
		pyxieRenderTarget* offscreen;
		bool clearColor;
		bool colearDepth;
		Vec4 color;
	};
	std::vector<RenderSet> renderSets;

	Backyard* Backyard::instance;
	Backyard& Backyard::Instance() { return *instance; }


	std::mutex main_mtx;
	std::mutex python_mtx;

	std::condition_variable main_cv;
	std::condition_variable python_cv;

	bool wake_Main = false;

	bool quitPython = false;
	bool quitRequest = false;

	Backyard::~Backyard() {
		quitRequest = true;
		while (quitPython) {
			python_cv.notify_one();
		}

		for (auto itr = renderSets.begin(); itr != renderSets.end(); ++itr) {
			(*itr).camera->DecReference();
			(*itr).showcas->DecReference();
		}
		renderSets.clear();
	}

	void Backyard::Delete() {
		PYXIE_SAFE_DELETE(instance); 
	}

/*
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
*/

	void Backyard::WakeBoth() {
		python_cv.notify_one();
		main_cv.notify_one();
	}

	bool Swapframe = true;
	void Backyard::SyncMain() {
		do{
			std::this_thread::sleep_for(std::chrono::microseconds(10));

			std::unique_lock<std::mutex> main_lk(main_mtx);

			while(wake_Main)
				python_cv.notify_one();

			main_cv.wait(main_lk);
			wake_Main = true;
			Render();
		}while (!Swapframe);
	}

	void Backyard::SyncPython(bool swapframe) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));

		std::unique_lock<std::mutex> python_lk(python_mtx);

		Swapframe = swapframe;

		while (!wake_Main)
			main_cv.notify_one();
		python_cv.wait(python_lk);
		wake_Main = false;

		if (quitRequest) {
			quitPython = true;
			PyRun_SimpleString("import os\nos._exit(0)");
		}

	}


	void Backyard::RenderRequest(pyxieCamera* camera, pyxieShowcase* showcase, pyxieRenderTarget* offscreen, bool clearColor, bool clearDepth, const float* color){
		camera->IncReference();
		showcase->IncReference();

		RenderSet rset;
		rset.camera = camera;
		rset.showcas = showcase;
		rset.offscreen = offscreen;
		rset.clearColor = clearColor;
		rset.colearDepth = clearDepth;
		rset.color = Vec4(color[0], color[1], color[2], color[3]);
		renderSets.push_back(rset);
		SyncPython(false);
	}

	void Backyard::Render() {

		pyxieRenderContext& renderContext = pyxieRenderContext::Instance();

		for (auto itr = renderSets.begin(); itr != renderSets.end(); ++itr) {
			renderContext.BeginScene((*itr).offscreen, (*itr).color,(*itr).clearColor, (*itr).colearDepth);
			(*itr).showcas->Update(0.0f);
			(*itr).camera->Render();
			(*itr).showcas->Render();
			renderContext.EndScene();
			(*itr).camera->DecReference();
			(*itr).showcas->DecReference();
		}
		renderSets.clear();
	}
}
