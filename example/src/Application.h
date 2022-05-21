#pragma once

#include <Anggur/Window/WindowManager.h>
#include <Anggur/Graphics/Function.h>
#include <Anggur/Graphics/Shader.h>
#include <Anggur/Graphics/VertexArray.h>
#include <Anggur/Graphics/Texture2D.h>
#include <Anggur/Utility/Log.h>
#include <Anggur/System/File.h>
#include "Renderer2D.h"
#include "ScreenVP.h"
#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <filesystem>

using namespace std::chrono_literals;

struct Application 
{
	struct Rectangle 
	{
		Vector2 position;
		Vector2 size;
	};

	bool GenerateAtlas(const char *fontFilename, const std::string& outPath) 
	{
		using namespace msdf_atlas;

		bool success = false;
		// Initialize instance  FreeType library
		if (msdfgen::FreetypeHandle *ft = msdfgen::initializeFreetype()) {
			// Load font file
			if (msdfgen::FontHandle *font = msdfgen::loadFont(ft, fontFilename)) {
				// Storage for glyph geometry and their coordinates in the atlas
				std::vector<GlyphGeometry> glyphs;
				// FontGeometry is a helper class that loads a set of glyphs from a single font.
				// It can also be used to get additional font metrics, kerning information, etc.
				FontGeometry fontGeometry(&glyphs);
				// Load a set of character glyphs:
				// The second argument can be ignored unless you mix different font sizes in one atlas.
				// In the last argument, you can specify a charset other than ASCII.
				// To load specific glyph indices, use loadGlyphs instead.
				fontGeometry.loadCharset(font, 1.0, Charset::ASCII);
				// Apply MSDF edge coloring. See edge-coloring.h for other coloring strategies.
				const double maxCornerAngle = 3.0;
				for (GlyphGeometry &glyph : glyphs)
					glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
				// TightAtlasPacker class computes the layout of the atlas.
				TightAtlasPacker packer;
				// Set atlas parameters:
				// setDimensions or setDimensionsConstraint to find the best value
				packer.setDimensionsConstraint(TightAtlasPacker::DimensionsConstraint::SQUARE);
				// setScale for a fixed size or setMinimumScale to use the largest that fits
				packer.setMinimumScale(24.0);
				// setPixelRange or setUnitRange
				packer.setPixelRange(2.0);
				packer.setMiterLimit(1.0);
				// Compute atlas layout - pack glyphs
				packer.pack(glyphs.data(), glyphs.size());
				// Get final atlas dimensions
				int width = 0, height = 0;
				packer.getDimensions(width, height);
				// The ImmediateAtlasGenerator class facilitates the generation of the atlas bitmap.
				ImmediateAtlasGenerator<
					float, // pixel type of buffer for individual glyphs depends on generator function
					3, // number of atlas color channels
					&msdfGenerator, // function to generate bitmaps for individual glyphs
					BitmapAtlasStorage<byte, 3> // class that stores the atlas bitmap
						// For example, a custom atlas storage class that stores it in VRAM can be used.
						> generator(width, height);
				// GeneratorAttributes can be modified to change the generator's default settings.
				GeneratorAttributes attributes;
				generator.setAttributes(attributes);
				generator.setThreadCount(4);
				// Generate atlas bitmap
				generator.generate(glyphs.data(), glyphs.size());
				// The atlas bitmap can now be retrieved via atlasStorage as a BitmapConstRef.
				// The glyphs array (or fontGeometry) contains positioning data for typesetting text.
				// success = msdfgen::submitAtlasBitmapAndLayout(generator.atlasStorage(), glyphs);
				auto bitmap = (msdfgen::BitmapConstRef<byte, 3>) generator.atlasStorage();
				for (auto& glyph: glyphs) {
					ANGGUR_LOG("");
				}
				if (saveImage(bitmap, ImageFormat::PNG, outPath.c_str(), YDirection::TOP_DOWN)) {
					puts("Atlas image file saved.");
				} else {
					success = false;
					puts("Failed to save the atlas as an image file.");
				}
				// Cleanup
				msdfgen::destroyFont(font);
			} else {
				ANGGUR_LOG("FAILED TO LOAD FONT");
			}
			msdfgen::deinitializeFreetype(ft);
		} else {
			ANGGUR_LOG("FAILED TO INITIALIZE FREE TYPE");
		}
		return success;
	}


	Application() 
	{
		WindowManager windowManager;
		Window window(Vector2(400, 300), "Example Window");
		Renderer2D renderer;

		auto texture = std::make_shared<Texture2D>("resources/images/wall.jpeg"); 

		while (window.IsOpen())
		{
			renderer.SetViewProjection(CreateScreenVP(window.GetSize()));
			renderer.SetClearColor(Vector4::charcoal);

			renderer.Begin();

				renderer.Clear();
				renderer.RenderRectangle(Vector2(0.0f, 0.0f), Vector2(200.0f, 200.0f), texture, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f));

			renderer.End();
			
			windowManager.PollEvents();
			window.SwapBuffers();
		}
	}
};
