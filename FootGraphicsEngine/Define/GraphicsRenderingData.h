#pragma once
//#include "Utils/d3d11Utils.h"
#include <string>
#include "SimpleMath.h"
#include "GraphicsTypedef.h"
#include "Resources/Resource.h"

// Mesh �ҷ������ �ش� �޽õ��� ���ҽ��Ŵ������� ���� ���� �ʱ�ȭ ������Ѵ�.
constexpr size_t CUBE_MESH = 0;
constexpr size_t SPHERE_MESH = 1;
constexpr size_t AXIS_MESH = 2;
constexpr size_t GRID_MESH = 3;
constexpr size_t SCREEN_MESH = 4;
constexpr size_t DEBUG_SCREEN_MESH = 5;
constexpr size_t WIRECUBE_MESH = 6;
constexpr size_t SKYBOX_MESH = 7;
constexpr size_t PLANE_MESH = 8;

// �Լ� �߰��� enum class
enum class TextureMapType
{
	ALBEDO,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AO,
	EMISSIVE,
	CUBE
};

struct ObjectInfo
{
	uint64 objectID;

	DirectX::SimpleMath::Matrix worldTM;

	DirectX::SimpleMath::Vector3 worldPos;

	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];

	uint64 materialName;
};
