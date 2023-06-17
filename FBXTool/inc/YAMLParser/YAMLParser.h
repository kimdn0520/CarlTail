#pragma once
#include "YAMLBinaryLayout.h"

class YAMLParser
{
private:
	char line[256] = { 0 };	// �� ��

	// TopNode �������� LoadAll �Ѵ�.
	std::vector<YAML::Node> yamlNodeList;

	// UnityScene ���� ID ���
	std::vector<std::string> fildIDList;

	std::shared_ptr<YAMLBinaryData::Scene> scene;

	std::string prefabPath = "Resources/Prefab/";

public:
	YAMLParser() {};
	~YAMLParser() {};

public:
	void OpenFile(const std::string& path, const std::string& saveName);

	void OpenPrefab(const std::string& prefabName);

	YAMLBinaryData::Float3 QuatToEuler(float qx, float qy, float qz, float qw); 
};

