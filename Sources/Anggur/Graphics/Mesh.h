#pragma once

#include "Anggur/Graphics/Texture2D.h"
#include "Anggur/Math/Vector2.h"
#include "Anggur/Math/Vector3.h"
#include "Anggur/Math/Vector4.h"

namespace Anggur
{
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector4 color;
		Vector2 uv;
		float slot;

		Vertex() = default;
		Vertex(
			const Vector3& newPosition, const Vector3& newNormal, const Vector4& newColor, const Vector2& newUV,
			float newSlot = 0.0f
		)
			: position(newPosition), normal(newNormal), color(newColor), uv(newUV), slot(newSlot)
		{
		}

		std::string ToString()
		{
			std::string temp;

			temp += "position: " + position.ToString() + "\n";
			temp += "normal: " + normal.ToString() + "\n";
			temp += "color: " + color.ToString() + "\n";
			temp += "textureCoord: " + uv.ToString() + "\n";
			temp += "textureSlot: " + std::to_string(slot) + "\n";

			return temp;
		}
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<std::shared_ptr<Texture2D>> textures;
	};
}
