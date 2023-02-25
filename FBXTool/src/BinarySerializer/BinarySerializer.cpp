#include "pch.h"
#include "BinarySerializer/BinarySerializer.h"
#include "ParserData/ParserData.h"

void BinarySerializer::SaveBinaryFile(std::shared_ptr<FBXModel> fbxModel, std::string name, std::string path)
{
	// MaterialList.json�� �ҷ��ͼ� Materials�� �̸����� ���� �ִ��� Ȯ��
	rapidjson::Document document;
	
	std::ifstream file("BinaryFile/Material/MaterialList.json");
	rapidjson::IStreamWrapper isw { file };
	
	document.ParseStream(isw);

	std::string modelPathStr = "Model/";
	std::string materialPathStr = "Material/";
	std::string meshPathStr = "Mesh/";
	std::string animationPathStr = "Animation/";

	try {
		// ���͸������ �ø��������
		std::vector<std::string> materialList;

		rapidjson::Document::AllocatorType allocator = document.GetAllocator();

		for (auto& mat : fbxModel->materialList)
		{
			bool isMat = false;

			for (int i = 0; i < document["Materials"].Size(); i++)
			{
				std::string matName = document["Materials"][i].GetString();

				// �̸��� ���� ���͸����� �ִٸ� ���� ��Ҹ� �����ش�.
				// �̸��� ������ ���� ��� ���е��� ���� �ٸ��ٸ�..! �̸��� �ٲ��ְ� �߰����ش�.
				// �̸��� ������ ���� ��� ���е��� ���� ���ٸ� �߰��� �ϸ� �ȵȴ�.
				// "red" + name �ش� ���͸����� �����ϰ��ִ� �޽��� ���͸��� �̸��� �ٲ�����Ѵ�.
				if (matName == mat->materialName)
				{
					// �̸��� ���Ҵ� ���͸����� �����´�.
					std::ifstream ifs(path + materialPathStr + matName + ".mat", std::ios_base::binary);
					boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
					buffer.push(boost::iostreams::zlib_decompressor());
					buffer.push(ifs);
					boost::archive::binary_iarchive ia(buffer);

					FBXBinaryData::MaterialData materialData;

					ia >> materialData;

					// ������ �� ���ٸ� ����� �ȵ�
					if (mat->material_Ambient.x == materialData.material_Ambient.x && mat->material_Ambient.y == materialData.material_Ambient.y && mat->material_Ambient.z == materialData.material_Ambient.z &&
						mat->material_Diffuse.x == materialData.material_Diffuse.x && mat->material_Diffuse.y == materialData.material_Diffuse.y && mat->material_Diffuse.z == materialData.material_Diffuse.z &&
						mat->material_Specular.x == materialData.material_Specular.x && mat->material_Specular.y == materialData.material_Specular.y && mat->material_Specular.z == materialData.material_Specular.z &&
						mat->material_Emissive.x == materialData.material_Emissive.x && mat->material_Emissive.y == materialData.material_Emissive.y && mat->material_Emissive.z == materialData.material_Emissive.z &&
						mat->emissionFactor == materialData.emissionFactor && mat->material_Transparency == materialData.material_Transparency && mat->material_Reflectivity == materialData.material_Reflectivity &&
						mat->metallic == materialData.metallic && mat->roughness == materialData.roughness && mat->albedoMap == materialData.albedoMap && mat->normalMap == materialData.normalMap &&
						mat->metallicMap == materialData.metallicMap && mat->AOMap == materialData.AOMap && mat->emissiveMap == materialData.emissiveMap)
					{
						isMat = true;
					}
					// ������ �ٸ��� �ִٸ� name �� �ٿ��ְ� �޽��� ���͸��� �̸��� �ٲ��ش�.
					else
					{
						// fbxModel�� Mesh�� ���͸��� �̸��ٲٱ�
						for (auto& mesh : fbxModel->fbxMeshInfoList)
						{
							for (auto& meshMat : mesh->materials)
							{
								if(meshMat == mat->materialName)
									meshMat = meshMat + "_" + name;
							}
						}

						mat->materialName = mat->materialName + "_" + name;
					}

					break;
				}
			}

			if (!isMat)
			{
				FBXBinaryData::Float4 material_Ambient(mat->material_Ambient.x, mat->material_Ambient.y, mat->material_Ambient.z, mat->material_Ambient.w);
				FBXBinaryData::Float4 material_Diffuse(mat->material_Diffuse.x, mat->material_Diffuse.y, mat->material_Diffuse.z, mat->material_Diffuse.w);
				FBXBinaryData::Float4 material_Specular(mat->material_Specular.x, mat->material_Specular.y, mat->material_Specular.z, mat->material_Specular.w);
				FBXBinaryData::Float4 material_Emissive(mat->material_Emissive.x, mat->material_Emissive.y, mat->material_Emissive.z, mat->material_Emissive.w);

				FBXBinaryData::MaterialData binaryMaterial(
					mat->materialName, mat->albedoMap, mat->normalMap, mat->metallicMap, mat->roughnessMap, mat->AOMap, mat->emissiveMap,
					material_Ambient, material_Diffuse, material_Specular, material_Emissive, mat->emissionFactor, mat->material_Transparency, mat->material_Reflectivity,
					mat->metallic, mat->roughness);

				// �� �����Ϳ� �߰��� ���͸��� �̸�
				materialList.emplace_back(binaryMaterial.materialName);

				// .mat BinaryFile �߰�
				std::ofstream mat_ofs(path + materialPathStr + binaryMaterial.materialName + ".mat", std::ios_base::binary);
				boost::iostreams::filtering_stream<boost::iostreams::output> matBuffer;
				matBuffer.push(boost::iostreams::zlib_compressor());
				matBuffer.push(mat_ofs);
				boost::archive::binary_oarchive oaMat(matBuffer);
				oaMat << binaryMaterial;

				// MaterialList.json ���� �̸��� �÷��ش�.
				rapidjson::Value stringVal;
				std::string matjsonName = binaryMaterial.materialName;
				stringVal.SetString(matjsonName.c_str(), allocator);
				document["Materials"].PushBack(stringVal, allocator);

				std::ofstream ofs{ R"(BinaryFile/Material/MaterialList.json)" };
				rapidjson::OStreamWrapper osw{ ofs };
				rapidjson::Writer<rapidjson::OStreamWrapper> writer{ osw };
				document.Accept(writer);
			}
			else
			{
				isMat = false;
			}
		}

		// �޽� �ø��������
		std::vector<std::string> meshInfoList;

		for (auto& mesh : fbxModel->fbxMeshInfoList)
		{
			mesh->meshName = mesh->meshName + "_" + name;

			std::vector<FBXBinaryData::VertexData> vertexDataList;

			for (auto& vertex : mesh->meshVertexList)
			{
				FBXBinaryData::Float3 position(vertex.position.x, vertex.position.y, vertex.position.z);
				FBXBinaryData::Float4 color(vertex.color.x, vertex.color.y, vertex.color.z, vertex.color.w);
				FBXBinaryData::Float2 uv(vertex.uv.x, vertex.uv.y);
				FBXBinaryData::Float3 normal(vertex.normal.x, vertex.normal.y, vertex.normal.z);
				FBXBinaryData::Float3 tangent(vertex.tangent.x, vertex.tangent.y, vertex.tangent.z);

				FBXBinaryData::VertexData vertexData(position, color, uv, normal, tangent);

				for (int i = 0; i < 8; i++)
				{
					vertexData.weights[i] = vertex.weights[i];
					vertexData.boneIndices[i] = vertex.boneIndices[i];
				}

				vertexDataList.emplace_back(vertexData);
			}

			FBXBinaryData::MeshData meshData(mesh->meshName, mesh->parentName, mesh->materials, vertexDataList, mesh->indices, ConvertFloat4x4(mesh->nodeTM), mesh->isSkinned);

			std::ofstream mesh_ofs(path + meshPathStr + mesh->meshName + ".mesh", std::ios_base::binary);
			boost::iostreams::filtering_stream<boost::iostreams::output> meshBuffer;
			meshBuffer.push(boost::iostreams::zlib_compressor());
			meshBuffer.push(mesh_ofs);

			boost::archive::binary_oarchive oaMesh(meshBuffer);
			oaMesh << meshData;

			meshInfoList.emplace_back(meshData.meshName);
		}

		// Bone
		std::vector<FBXBinaryData::BoneData> boneInfoList;

		for (auto& bone : fbxModel->fbxBoneInfoList)
		{
			FBXBinaryData::BoneData boneData(bone->boneName, bone->parentIndex, ConvertFloat4x4(bone->offsetMatrix), ConvertFloat4x4(bone->nodeMatrix));

			boneInfoList.emplace_back(boneData);
		}


		// Animation Serialize
		std::vector<std::string> animationClipList;

		for (auto& anim : fbxModel->animationClipList)
		{
			std::vector<std::vector<FBXBinaryData::KeyFrameInfoData>> keyFrameInfoList;

			for (auto& keyList : anim->keyFrameList)
			{
				std::vector<FBXBinaryData::KeyFrameInfoData> keyFrames;

				for (auto& key : keyList)
				{
					FBXBinaryData::Float3 localTransform(key->localTransform.x, key->localTransform.y, key->localTransform.z);
					FBXBinaryData::Float4 localRotation(key->localRotation.x, key->localRotation.y, key->localRotation.z, key->localRotation.w);
					FBXBinaryData::Float3 localScale(key->localScale.x, key->localScale.y, key->localScale.z);

					FBXBinaryData::KeyFrameInfoData keyFrameData(key->time, localTransform, localRotation, localScale);

					keyFrames.emplace_back(keyFrameData);
				}

				keyFrameInfoList.emplace_back(keyFrames);
			}

			// �ִϸ��̼� �̸��� �������� Idle �̷��� �ٲ�µ� �Ƹ� �ε��ϰ� �Ǹ� Armature|Idle �̷������� ����������
			// �׷��� |�� �ִٸ� |�� �����ؼ� �տ� �� �������
			ReconstructionAnimName(anim->animationName);

			// �� ���� animation �տ� modelName_�� �ٿ��ش�.
			anim->animationName = name + "_" + anim->animationName;

			FBXBinaryData::AnimationClipData animData(
				anim->animationName, anim->frameRate, anim->tickPerFrame, anim->totalKeyFrame, anim->startKeyFrame, anim->endKeyFrame,
				keyFrameInfoList);

			std::ofstream anim_ofs(path + animationPathStr + animData.animationName + ".anim", std::ios_base::binary);
			boost::iostreams::filtering_stream<boost::iostreams::output> animBuffer;
			animBuffer.push(boost::iostreams::zlib_compressor());
			animBuffer.push(anim_ofs);

			boost::archive::binary_oarchive oaAnim(animBuffer);
			oaAnim << animData;

			animationClipList.emplace_back(animData.animationName);
		}

		FBXBinaryData::ModelData modelData(meshInfoList, materialList, animationClipList, boneInfoList, fbxModel->isSkinnedAnimation);

		std::ofstream model_ofs(path + modelPathStr + name + ".model", std::ios_base::binary);
		boost::iostreams::filtering_stream<boost::iostreams::output> modelBuffer;
		modelBuffer.push(boost::iostreams::zlib_compressor());
		modelBuffer.push(model_ofs);
		boost::archive::binary_oarchive oaModel(modelBuffer);	// �𵨿��� Mesh, Material, Animation���� �̸��� ����
		
		oaModel << modelData;
	}
	catch (const boost::archive::archive_exception e) {
		std::cout << "boost::archive exception code is " << e.code << std::endl;
	}
}

void BinarySerializer::ReconstructionAnimName(std::string& name)
{
	if (name.find("|") != std::string::npos)
	{
		int idx = name.find("|") + 1;

		name.erase(0, idx);

		ReconstructionAnimName(name);
	}
}
