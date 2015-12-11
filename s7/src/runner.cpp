#include "runner.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"
#include "material.h"
#include "genmeshconverter.h"
#include "primitives.h"
#include "entity.h"
#include "updatectx.h"


namespace s7 {

	Runner::Runner()
	{
		_width = 1280;
		_height = 720;
	}

	void Runner::Init()
	{
		bgfx::init();
	}

	void Runner::Run()
	{
        UpdateContext ctx;
        ctx.Init();
        
        Entity octomesh;
        octomesh.Init();
        
		auto u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
		auto u_lightIntensity = bgfx::createUniform("u_lightIntensity", bgfx::UniformType::Vec4);
		auto u_normalMat = bgfx::createUniform("u_normalMat", bgfx::UniformType::Mat4);

		auto program = LoadProgram("mesh.vert", "mesh.frag");
        
        auto genmesh = CreateOctahedronMesh();
        auto mesh = Convert(*genmesh);

		auto timeOffset = bx::getHPCounter();

		uint32_t debug = BGFX_DEBUG_TEXT;
		uint32_t reset = BGFX_RESET_VSYNC;
		bgfx::reset(_width, _height, reset);
		bgfx::setDebug(debug);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		while (!entry::processEvents(_width, _height, debug, reset))
		{
            ctx.Update();
            
			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, _width, _height);

			// This dummy draw call is here to make sure that view 0 is cleared
			// if no other draw calls are submitted to view 0.
			bgfx::touch(0);

			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency());
			const double toMs = 1000.0 / freq;
			float time = (float)((bx::getHPCounter() - timeOffset) / double(bx::getHPFrequency()));

			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(0, 1, 0x4f, "s7");
			bgfx::dbgTextPrintf(0, 2, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

            glm::vec3 target(0, 0, 0);
            glm::vec3 eye(0, 1, -2.5f);
            glm::vec3 up(0, 1, 0);
            
            glm::mat4 viewMat = glm::lookAt(eye, target, up);
            glm::mat4 projMat = glm::perspective(toRad(60.0f), float(_width) / float(_height), 0.1f, 100.0f);
            bgfx::setViewTransform(0, &viewMat, &projMat);

            // Set view 0 default viewport.
            bgfx::setViewRect(0, 0, 0, _width, _height);

            // Scene
            Mat44 modelMat;
            modelMat.RotateXY(0.0f, time*0.37f);

            // Prep shader
            Vec4f lightPos(0, 0, 0, 1);
			bgfx::setUniform(u_lightPos, &lightPos);

            Vec4f lightIntensity(0, 1, -2.5f, 1);
			bgfx::setUniform(u_lightIntensity, &lightIntensity);
            
            //auto normalMat = (viewMat * modelMat).Inverse().Transpose();
			//bgfx::setUniform(u_normalMat, &normalMat);

			mesh.Submit(0, program, modelMat.Data());

            glm::mat4 mtx;
            mtx = glm::rotate(mtx, time*0.37f, up);
			mesh.Submit(0, program, (float*)&mtx);

			bgfx::frame();
		}

		mesh.Unload();

		bgfx::destroyProgram(program);
		bgfx::destroyUniform(u_lightPos);
		bgfx::destroyUniform(u_lightIntensity);
		bgfx::destroyUniform(u_normalMat);
	}

	void Runner::Shutdown()
	{
		bgfx::shutdown();
	}

}
