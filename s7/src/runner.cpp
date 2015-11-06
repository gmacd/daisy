#include "runner.h"

#include "common.h"
#include "material.h"
#include "mesh.h"

namespace s7 {

	struct PackedVert
	{
		Vec3f _pos;
		Vec4u8 _normal;
	};

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
		auto u_time = bgfx::createUniform("u_time", bgfx::UniformType::Vec4);
		auto program = LoadProgram("vs_mesh", "fs_mesh");

		///////////////////////////////////////////////////////////
		// Meshgen

		bgfx::VertexDecl myDecl;
		myDecl.begin();
		myDecl.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float, false, false);
		myDecl.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true);
		myDecl.end();

		Mesh mesh(myDecl, 1);
		auto& group = mesh.GetGroup(0);

		// Allocate & assign vertices (and normals)
		// TODO Work out normals.
		// TODO Calc accurate points
		// TODO Break out/refactor
		{
			auto numVerts = 5;
			auto vertMem = bgfx::alloc(numVerts * myDecl.getStride());
			auto verts = reinterpret_cast<PackedVert*>(vertMem->data);

			verts[0]._pos.Set(0, 0, 1);
			verts[0]._normal.Set(255, 0, 0, 255);
			verts[1]._pos.Set(-1, 0, 0);
			verts[1]._normal.Set(255, 0, 0, 255);
			verts[2]._pos.Set(1, 0, 0);
			verts[2]._normal.Set(255, 0, 0, 255);
			verts[3]._pos.Set(0, -1, 0);
			verts[3]._normal.Set(255, 0, 0, 255);

			group._vbh = bgfx::createVertexBuffer(vertMem, myDecl);
		}

		// Allocate & assign indices
		{
			auto numIndices = 12;
			auto idxMem = bgfx::alloc(numIndices * 2);
			auto indices = reinterpret_cast<uint16_t*>(idxMem->data);

			indices[0] = 0;  indices[1] = 3;  indices[2] = 1;
			indices[3] = 0;  indices[4] = 2;  indices[5] = 3;
			indices[6] = 0;  indices[7] = 1;  indices[8] = 2;
			indices[9] = 1;  indices[10] = 3;  indices[11] = 2;

			group._ibh = bgfx::createIndexBuffer(idxMem);
		}

		// Create a primitive
		{
			group._prims.resize(1);
			auto& prim = group._prims[0];
			prim.m_startVertex = 0;
			prim.m_numVertices = 4;
			prim.m_startIndex = 0;
			prim.m_numIndices = 12;
		}

		// Calc bounds for prim and group
		mesh.CalcBounds();

		///////////////////////////////////////////////////////////
		// End of Meshgen

		auto timeOffset = bx::getHPCounter();

		uint32_t debug = BGFX_DEBUG_TEXT;
		uint32_t reset = BGFX_RESET_VSYNC;
		bgfx::reset(_width, _height, reset);
		bgfx::setDebug(debug);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

		while (!entry::processEvents(_width, _height, debug, reset))
		{
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
			bgfx::setUniform(u_time, &time);

			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(0, 1, 0x4f, "s7");
			bgfx::dbgTextPrintf(0, 2, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

			float at[3] = { 0.0f, 1.0f,  0.0f };
			float eye[3] = { 0.0f, 1.0f, -2.5f };

			// Set view and projection matrix for view 0.
			const auto* hmd = bgfx::getHMD();
			if (NULL != hmd && 0 != (hmd->flags & BGFX_HMD_RENDERING))
			{
				float view[16];
				bx::mtxQuatTranslationHMD(view, hmd->eye[0].rotation, eye);

				float proj[16];
				bx::mtxProj(proj, hmd->eye[0].fov, 0.1f, 100.0f);

				bgfx::setViewTransform(0, view, proj);

				// Set view 0 default viewport.
				//
				// Use HMD's width/height since HMD's internal frame buffer size
				// might be much larger than window size.
				bgfx::setViewRect(0, 0, 0, hmd->width, hmd->height);
			}
			else
			{
				float view[16];
				bx::mtxLookAt(view, eye, at);

				float proj[16];
				bx::mtxProj(proj, 60.0f, float(_width) / float(_height), 0.1f, 100.0f);
				bgfx::setViewTransform(0, view, proj);

				// Set view 0 default viewport.
				bgfx::setViewRect(0, 0, 0, _width, _height);
			}

			float mtx[16];
			bx::mtxRotateXY(mtx, 0.0f, time*0.37f);

			mesh.Submit(0, program, mtx);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();
		}

		mesh.Unload();

		bgfx::destroyProgram(program);
		bgfx::destroyUniform(u_time);
	}

	void Runner::Shutdown()
	{
		bgfx::shutdown();
	}

}
