//
//  Client/Graphics/PixelBufferRenderer.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 22/04/09.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#ifndef _DREAM_CLIENT_GRAPHICS_PIXELBUFFERRENDERER_H
#define _DREAM_CLIENT_GRAPHICS_PIXELBUFFERRENDERER_H

#include "MeshBuffer.h"
#include "TextureManager.h"

namespace Dream {
	namespace Client {
		namespace Graphics {

			using Euclid::Numerics::Vec2;
			using Euclid::Numerics::Vec2b;

			/// Efficiently render pixel buffers as textured quads. Rendering pixel buffers is a common operation especially for user interfaces, text, certain graphical effects, etc. The PixelBufferRenderer provides an efficient implementation of this operation that avoids uploading pixel buffers to textures if they haven't changed.
			class PixelBufferRenderer : public Object {
			protected:
				Ref<TextureManager> _texture_manager;

				// A cache of pixel-buffer -> texture mappings.
				// typedef std::map<Ref<IPixelBuffer>, Ref<Texture>> TextureCacheT;
				typedef std::map<Ptr<IPixelBuffer>, Ref<Texture>> TextureCacheT;
				TextureCacheT _texture_cache;
				std::vector<Ref<Texture>> _available_textures;

				Ref<Texture> fetch(Ptr<IPixelBuffer> pixel_buffer, bool invalidate = false);
				TextureParameters _texture_parameters;

				struct Vertex {
					Vec2 position;
					Vec2 mapping;
				};

				VertexArray _vertex_array;
				VertexBuffer<Vertex> _vertex_buffer;

			public:
				typedef int RotationT;

				enum Attributes {
					POSITION = 0,
					MAPPING = 1
				};

				enum Textures {
					DIFFUSE_TEXTURE = 0
				};

				PixelBufferRenderer(Ptr<TextureManager> texture_manager);
				virtual ~PixelBufferRenderer();

				TextureParameters & texture_parameters() { return _texture_parameters; }
				const TextureParameters & texture_parameters() const { return _texture_parameters; }

				void render(const AlignedBox2 & box, Ptr<IPixelBuffer> pixel_buffer);
				void render(const AlignedBox2 & box, Ptr<IPixelBuffer> pixel_buffer, Vec2b flip, RotationT rotation);

				void invalidate(Ptr<IPixelBuffer> pixel_buffer);
			};
		}
	}
}

#endif
