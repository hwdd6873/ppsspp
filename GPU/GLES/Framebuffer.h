// Copyright (c) 2012- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.

#pragma once

#include <list>

#include "gfx_es2/fbo.h"
// Keeps track of allocated FBOs.
// Also provides facilities for drawing and later converting raw
// pixel data.


#include "../Globals.h"

struct GLSLProgram;

enum PspDisplayPixelFormat {
	PSP_DISPLAY_PIXEL_FORMAT_565 = 0,
	PSP_DISPLAY_PIXEL_FORMAT_5551 = 1,
	PSP_DISPLAY_PIXEL_FORMAT_4444 = 2,
	PSP_DISPLAY_PIXEL_FORMAT_8888 = 3,
};

class FramebufferManager {
public:
	FramebufferManager();
	~FramebufferManager();

	struct VirtualFramebuffer {
		int last_frame_used;

		u32 fb_address;
		u32 z_address;
		int fb_stride;
		int z_stride;

		// There's also a top left of the drawing region, but meh...
		int width;
		int height;

		int format;  // virtual, right now they are all RGBA8888
		FBOColorDepth colorDepth;
		FBO *fbo;
	};

	void DrawPixels(const u8 *framebuf, int pixelFormat, int linesize);
	void DrawActiveTexture(float w, float h, bool flip = false);

	void DestroyAllFBOs();
	void DecimateFBOs();

	void BeginFrame();
	void Resized();
	void CopyDisplayToOutput();
	void SetRenderFrameBuffer();  // Uses parameters computed from gstate
	// TODO: Break out into some form of FBO manager
	VirtualFramebuffer *GetDisplayFBO();
	void SetDisplayFramebuffer(u32 framebuf, u32 stride, int format);
	size_t NumVFBs() const { return vfbs_.size(); }

private:
	// Deletes old FBOs.

	u32 displayFramebufPtr_;
	u32 displayStride_;
	int displayFormat_;

	VirtualFramebuffer *displayFramebuf_;
	VirtualFramebuffer *prevDisplayFramebuf_;
	VirtualFramebuffer *prevPrevDisplayFramebuf_;

	std::list<VirtualFramebuffer *> vfbs_;

	VirtualFramebuffer *currentRenderVfb_;

	// Used by DrawPixels
	unsigned int backbufTex;

	u8 *convBuf;
	GLSLProgram *draw2dprogram;
	bool resized_;
};
