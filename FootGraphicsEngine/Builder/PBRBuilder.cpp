#include "GraphicsPch.h"
#include "PBRBuilder.h"

#include "Object/PBRObj.h"
#include "GraphicsCore/RasterizerState.h"

#include "Manager/ResourceManager.h"
#include <typeinfo>

namespace GraphicsEngineSpace
{
	PBRBuilder::~PBRBuilder()
	{
	}

	std::shared_ptr<IDXObject> PBRBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// ���� ������Ʈ Ȯ��
		if (std::dynamic_pointer_cast<PBRObj>(pDXObj) == nullptr)
		{
			OutputDebugString(L"Not PB OBJ");

			return nullptr;
		}

		// ���� basicMeshes�� �ִ��� ã�ƺ���.
		auto findIt = resourceManager->basicMeshes.find(objectName);

		if (findIt != resourceManager->basicMeshes.end())
		{
			std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();
			auto mesh = resourceManager->basicMeshes[objectName];
			_tmpObjRes->ObjName = mesh->GetName();
			pDXObj->SetMeshResources(_tmpObjRes);

			return pDXObj;
		}

		// basicMeshes�� ���ٸ� �ܺο��� �ҷ��´�.? �ٵ� PBRObj ��.. ���Ϸ� �����Ҽ�������?
		// ���ʿ� ���ӿ�����Ʈ�� ������ȭ�ؼ� ���Ϸ� ���� �� �ִٸ� ���Ұ�����.. �׷��ٸ�? �ϴ��� ���� �Ѱ��ش�ġ��

		// pbrobj�� ������ �ٵ� �׳� �̰� �Լ����� �����ʳ�? �׷���
		BuildGeometry(pDXObj, objectName);

		return pDXObj;
	}

	std::shared_ptr<MeshResources> PBRBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject,
		std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// ������Ʈ �̸��� �Ǻ��մϴ�.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".model")
		{
			// ���� �ҷ����� �ϴ� path.. �ӽ�
			std::ifstream ifs("BinaryFile/Model/" + objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			_tmpObjRes.reset();

			// �ִϸ��̼��� ������, ���� �޽� ����
			if (modelData.isSkinnedAnimation != true)
			{
				// �޽ð� �������� ���ɼ��� �ֱ� ������ ���ο��� �� ������ش�.
				BuildStaticMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
			// �ִٸ� skinned Mesh ����
			else
			{
				BuildSkinnedMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}
	}

	void PBRBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// ���ҽ� �Ŵ��� ĳ��
		resourceManager = ResourceManager::GetInstance();
	}

	void PBRBuilder::BuildStaticMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		// ��� �޽��� ���鼭 ���ؽ����ۿ� �ε��� ���۸� ������ݴϴ�.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}
	}

	void PBRBuilder::BuildSkinnedMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		// ��� �޽��� ���鼭 �޽� �̸��� isSkinnedMesh�� �־��ش�.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->isSkinnedMesh = true;

			objRes->setMesh = true;

			objRes->setMaterial = true;

			objRes->ObjName = mesh;

			DXObject->SetMeshResources(objRes);
		}

		// skinned �� ������ bone�� ����
		std::shared_ptr<BoneResources> boneRes = std::make_shared<BoneResources>();

		for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
		{
			SimpleMath::Matrix boneOffset;
			SimpleMath::Matrix node;

			auto& bone = modelData.boneInfoList[i];

			boneOffset.m[0][0] = bone.offsetMatrix.m[0][0];
			boneOffset.m[0][1] = bone.offsetMatrix.m[0][1];
			boneOffset.m[0][2] = bone.offsetMatrix.m[0][2];
			boneOffset.m[0][3] = bone.offsetMatrix.m[0][3];

			boneOffset.m[1][0] = bone.offsetMatrix.m[1][0];
			boneOffset.m[1][1] = bone.offsetMatrix.m[1][1];
			boneOffset.m[1][2] = bone.offsetMatrix.m[1][2];
			boneOffset.m[1][3] = bone.offsetMatrix.m[1][3];

			boneOffset.m[2][0] = bone.offsetMatrix.m[2][0];
			boneOffset.m[2][1] = bone.offsetMatrix.m[2][1];
			boneOffset.m[2][2] = bone.offsetMatrix.m[2][2];
			boneOffset.m[2][3] = bone.offsetMatrix.m[2][3];

			boneOffset.m[3][0] = bone.offsetMatrix.m[3][0];
			boneOffset.m[3][1] = bone.offsetMatrix.m[3][1];
			boneOffset.m[3][2] = bone.offsetMatrix.m[3][2];
			boneOffset.m[3][3] = bone.offsetMatrix.m[3][3];

			node.m[0][0] = bone.nodeMatrix.m[0][0];
			node.m[0][1] = bone.nodeMatrix.m[0][1];
			node.m[0][2] = bone.nodeMatrix.m[0][2];
			node.m[0][3] = bone.nodeMatrix.m[0][3];

			node.m[1][0] = bone.nodeMatrix.m[1][0];
			node.m[1][1] = bone.nodeMatrix.m[1][1];
			node.m[1][2] = bone.nodeMatrix.m[1][2];
			node.m[1][3] = bone.nodeMatrix.m[1][3];

			node.m[2][0] = bone.nodeMatrix.m[2][0];
			node.m[2][1] = bone.nodeMatrix.m[2][1];
			node.m[2][2] = bone.nodeMatrix.m[2][2];
			node.m[2][3] = bone.nodeMatrix.m[2][3];

			node.m[3][0] = bone.nodeMatrix.m[3][0];
			node.m[3][1] = bone.nodeMatrix.m[3][1];
			node.m[3][2] = bone.nodeMatrix.m[3][2];
			node.m[3][3] = bone.nodeMatrix.m[3][3];
			

			// �ʱ� ���� ������ �ٲ��ش�.
			//boneOffset = boneOffset * node;

			// �ش� ���� offest, node, parentIdx�� �־��ش�
			Bone tmpBone;
			tmpBone.offsetTransform = boneOffset;
			tmpBone.nodeTransform = node;
			if(i == 0)
				tmpBone.parentIdx = -1;
			else
				tmpBone.parentIdx = bone.parentIndex;

			boneRes->bones.emplace_back(tmpBone);
		}

		// ��� ���� �־��� �� �ش� resource�� PBRObj�� �־��ش�.
		DXObject->SetBoneResource(boneRes);

		// Animation ����
		std::shared_ptr<AnimationResources> animRes = std::make_shared<AnimationResources>();

		// �� �������� ��� Animation Clip�� �ִ´�..
		for (auto& animClipName : modelData.animationClipList)
		{
			// �ִϸ��̼� ���� �ҷ�����
			FBXBinaryData::AnimationClipData animClip;

			std::ifstream ifs("BinaryFile/Animation/" + animClipName + ".anim", std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive iaAnim(buffer);
			iaAnim >> animClip;

			// �� AnimClip ����
			std::shared_ptr<AnimationClipResource> animClipRes = std::make_shared<AnimationClipResource>();

			// ������ ������ ä���ش�.
			animClipRes->animationName = animClip.animationName;
			animClipRes->frameRate = animClip.frameRate;
			animClipRes->startKeyFrame = animClip.startKeyFrame;
			animClipRes->totalKeyFrame = animClip.totalKeyFrame;
			animClipRes->endKeyFrame = animClip.endKeyFrame;
			animClipRes->tickPerFrame = animClip.tickPerFrame;

			// ������ KeyFrame�� �߰����ش�.
				// ������ Bone�� ������ �ִ� ��� Keyframe �����͸� �־��ش�.
			for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
			{
				std::vector<KeyFrame> boneKeyFrames;

				for (auto& keyFrame : animClip.keyFrameList[i])
				{
					KeyFrame keyFrameRes;
					
					keyFrameRes.framePos.x = keyFrame.localTransform.x;
					keyFrameRes.framePos.y = keyFrame.localTransform.y;
					keyFrameRes.framePos.z = keyFrame.localTransform.z;

					keyFrameRes.frameRot.x = keyFrame.localRotation.x;
					keyFrameRes.frameRot.y = keyFrame.localRotation.y;
					keyFrameRes.frameRot.z = keyFrame.localRotation.z;
					keyFrameRes.frameRot.w = keyFrame.localRotation.w;

					keyFrameRes.frameScale.x = keyFrame.localScale.x;
					keyFrameRes.frameScale.y = keyFrame.localScale.y;
					keyFrameRes.frameScale.z = keyFrame.localScale.z;

					keyFrameRes.time = keyFrame.time;

					// ������ keyFrame ����� ���ͷ� ����� �ְ�
					boneKeyFrames.emplace_back(keyFrameRes);
				}

				// ��� keyFrame�� ���ͷ� ������. �ش� ���͸� �־��ش�.
				animClipRes->keyFrames.emplace_back(boneKeyFrames);
			}

			animRes->animationClipList.insert(std::make_pair(animClipRes->animationName, animClipRes));
		}

		// ��� �ִϸ��̼��� ������ �� �ش� ���ҽ��� PBRObj�� �־��ش�.
		DXObject->SetAnimationResource(animRes);
	}
}

