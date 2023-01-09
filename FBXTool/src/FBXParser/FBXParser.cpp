#include "pch.h"
#include "FBXParser/FBXParser.h"
#include "ParserData/ParserData.h"
#include "BinarySerializer/BinarySerializer.h"

FBXParser::FBXParser()
{}

FBXParser::~FBXParser()
{
	if (scene)
		scene->Destroy();

	if (manager)
		manager->Destroy();
}

void FBXParser::LoadFbx(const std::string& fbxFilePath, const std::string& saveName)
{
	fbxModel = std::make_shared<FBXModel>();

	Import(fbxFilePath);

	LoadAnimation();

	ProcessBones(scene->GetRootNode(), 0, -1);

	ProcessMesh(scene->GetRootNode(), FbxNodeAttribute::eMesh);

	std::shared_ptr<BinarySerializer> binarySerializer = std::make_shared<BinarySerializer>();

	std::string saveBinaryPath = "BinaryFile/";

	binarySerializer->SaveBinaryFile(fbxModel, saveName, saveBinaryPath);
}

void FBXParser::Import(const std::string& path)
{
	// FBX SDK ������ ��ü ����
	manager = fbxsdk::FbxManager::Create();

	// IOSettings ��ü ���� �� ����
	fbxsdk::FbxIOSettings* settings = fbxsdk::FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(settings);

	// �� ��ü ����
	scene = fbxsdk::FbxScene::Create(manager, "");

	// FbxImporter ��ü ����
	importer = fbxsdk::FbxImporter::Create(manager, "");

	// importer �ʱ�ȭ
	importer->Initialize(path.c_str(), -1, manager->GetIOSettings());

	// fbx ���� ������ scene ���� �����´�.
	importer->Import(scene);

	FbxSystemUnit lFbxFileSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	FbxSystemUnit lFbxOriginSystemUnit = scene->GetGlobalSettings().GetOriginalSystemUnit();
	double factor = lFbxOriginSystemUnit.GetScaleFactor();

	const FbxSystemUnit::ConversionOptions lConversionOptions =
	{
	  true,
	  true,
	  true,
	  true,
	  true,
	  true
	};
	lFbxFileSystemUnit.m.ConvertScene(scene, lConversionOptions);
	lFbxOriginSystemUnit.m.ConvertScene(scene, lConversionOptions);

	// GeometryConverter ��ü ����
	geometryConverter = new FbxGeometryConverter(manager);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��. 
	geometryConverter->Triangulate(scene, true, true);

	// ���� ���͸����� �ϳ��� �޽��� �Ҵ�Ȱ��� �ϳ��� �޽��� �ϳ��� ���͸���� ��������
	geometryConverter->SplitMeshesPerMaterial(scene, true);

	// importer �ı�
	importer->Destroy();
}

/// <summary>
/// Mesh���� �ε��ؼ� FBXModel�� fbxMeshInfoList�� �����Ѵ�.
/// ���ؽ��� ������ ��� ��� bone�� ������ �޴� mesh�� �ִٸ� bone ����ġ�� �־��ְ�
/// bone�� offsetMatrix ������ �߰����ش�
/// <summary>
void FBXParser::ProcessMesh(fbxsdk::FbxNode* node, FbxNodeAttribute::EType attribute)
{
	fbxsdk::FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute && nodeAttribute->GetAttributeType() == attribute)
	{
		// mesh�� �ϳ��� ������ �� mesh�� vertexList�� vertex�� �ٴ�� material�̶� indexBuffer�� ������ �Ѱ���
		std::shared_ptr<FBXMeshInfo> fbxMeshInfo = std::make_shared<FBXMeshInfo>();

		fbxMeshInfo->indices.resize(node->GetNodeAttributeCount());

		for (int meshCnt = 0; meshCnt < node->GetNodeAttributeCount(); meshCnt++)
		{
			fbxsdk::FbxMesh* mesh = (fbxsdk::FbxMesh*)node->GetNodeAttributeByIndex(meshCnt);

			LoadMesh(node, mesh, fbxMeshInfo, meshCnt);

			fbxsdk::FbxLayerElementMaterial* findMatIndex = mesh->GetElementMaterial(0);

			if (findMatIndex != nullptr)
			{
				int index = findMatIndex->GetIndexArray().GetAt(0);

				fbxsdk::FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<fbxsdk::FbxSurfaceMaterial>(index);

				LoadMaterial(surfaceMaterial, fbxMeshInfo);
			}
		}

		fbxModel->fbxMeshInfoList.emplace_back(fbxMeshInfo);
	}

	// Tree ���� ��� ȣ��
	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; ++i)
		ProcessMesh(node->GetChild(i), FbxNodeAttribute::eMesh);
}


void FBXParser::LoadMesh(fbxsdk::FbxNode* node, fbxsdk::FbxMesh* mesh, std::shared_ptr<FBXMeshInfo>& meshData, int meshCnt)
{
	// Node TM �ֱ�
	FbxAMatrix nodeTransform = scene->GetAnimationEvaluator()->GetNodeGlobalTransform(node);

	DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

	DirectX::XMVECTOR det = XMMatrixDeterminant(nodeMatrix);

	// ������ �װŸ���~
	if (det.m128_f32[0] < 0)
	{
		// Decompose �ߴٰ� scale -�ְ� �ٽ� ���ľ���..
		DirectX::XMVECTOR scale;
		DirectX::XMVECTOR rotQuat;
		DirectX::XMVECTOR trans;
		DirectX::XMMatrixDecompose(&scale, &rotQuat, &trans, nodeMatrix);
		DirectX::XMVECTOR minusScale = { -scale.m128_f32[0], -scale.m128_f32[1], -scale.m128_f32[2] };
		scale = minusScale;

		// �ٽ� SRT ����
		nodeMatrix = DirectX::XMMatrixScaling(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]) *
			DirectX::XMMatrixRotationQuaternion(rotQuat) *
			DirectX::XMMatrixTranslation(trans.m128_f32[0], trans.m128_f32[1], trans.m128_f32[2]);

		isNegativeScale = true;
	}

	const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

	const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

	DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

	nodeMatrix *= XMMatrixRotationQuaternion(q);

	meshData->nodeTM = nodeMatrix;

	// mesh �̸� �ֱ�
	meshData->meshName = mesh->GetName();

	// Node Parent ã��
	std::string parentName = node->GetParent()->GetName();

	// �θ� �̸� �ֱ�
	meshData->parentName = parentName;

	const int vertexCount = mesh->GetControlPointsCount();

	// �ӽ� ���ؽ�����Ʈ
	std::vector<Vertex> tmpMeshVertexList;
	tmpMeshVertexList.resize(vertexCount);

	// Position������ ������(�� �ٲ㼭 ������)
	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < vertexCount; ++i)
	{
		tmpMeshVertexList[i].position.x = static_cast<float>(controlPoints[i].mData[0]);
		tmpMeshVertexList[i].position.y = static_cast<float>(controlPoints[i].mData[2]);
		tmpMeshVertexList[i].position.z = static_cast<float>(controlPoints[i].mData[1]);
	}

	const int deformerCount = mesh->GetDeformerCount(FbxDeformer::eSkin);

	for (int i = 0; i < deformerCount; i++)
	{
		meshData->isSkinned = true;

		fbxModel->isSkinnedAnimation = true;	// �ϴ� ���⼭..

		FbxSkin* fbxSkin = static_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		if (fbxSkin)
		{
			FbxSkin::EType type = fbxSkin->GetSkinningType();

			if (FbxSkin::eRigid == type || FbxSkin::eLinear)
			{
				// FbxCluster�� Skinning������ �ִ� ������ ������ �ش�.
				const int clusterCount = fbxSkin->GetClusterCount();

				for (int j = 0; j < clusterCount; j++)
				{
					FbxCluster* cluster = fbxSkin->GetCluster(j);
					if (cluster->GetLink() == nullptr)
						continue;

					// �ش� ���� ���� ã�ƿ�
					int boneIdx = FindBoneIndex(cluster->GetLink()->GetName());
					assert(boneIdx >= 0);

					// �ش� ���� ������ �޴� ������ �ε��� ������ ����
					const int indicesCount = cluster->GetControlPointIndicesCount();

					for (int k = 0; k < indicesCount; k++)
					{
						double weight = cluster->GetControlPointWeights()[k];	// �ش� ���������� ����ġ

						int vtxIdx = cluster->GetControlPointIndices()[k];		// �ش� ������ �ε����� ���

						// �ִ� 8���� �Ұž�
						// ���鼭 ����� �ְ� break ��
						for (int weightIdx = 0; weightIdx < 8; weightIdx++)
						{
							if (tmpMeshVertexList[vtxIdx].boneIndices[weightIdx] == -1)
							{
								tmpMeshVertexList[vtxIdx].weights[weightIdx] = weight;

								tmpMeshVertexList[vtxIdx].boneIndices[weightIdx] = boneIdx;

								break;
							}
						}
					}

					// ���������� ���� BoneOffsetMatrix�� ���ؾ��Ѵ�.
					// GetTrasnformLinkMatrix�� �ش� Bone�� ���� Matrix�� ��� GetTransformMatrix�δ� �θ���� ���Ե� Matrix�� ��´�.
					// �ش� �θ� ����� ������� ���ϸ� �θ���ı��� �ڱ� �ڽ��� ������ ���� ��İ��� ���� �Ǽ� ���� ������ ��� ���� ���´�.
					FbxAMatrix matClusterTransformMatrix;
					FbxAMatrix matClusterLinkTransformMatrix;

					cluster->GetTransformMatrix(matClusterTransformMatrix);				// The transformation of the mesh at binding time 
					cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);		// The transformation of the cluster(joint) at binding time from joint space to world space 

					// Bone Matrix ����..
					DirectX::SimpleMath::Matrix clusterMatrix = ConvertMatrix(matClusterTransformMatrix);
					DirectX::SimpleMath::Matrix clusterlinkMatrix = ConvertMatrix(matClusterLinkTransformMatrix);

					// BindPose ����� ������
					FbxAMatrix geometryTransform = GetTransformMatrix(mesh->GetNode());
					DirectX::SimpleMath::Matrix geometryMatrix = ConvertMatrix(geometryTransform);

					// OffsetMatrix�� WorldBindPose�� �����
					DirectX::SimpleMath::Matrix offsetMatrix = clusterMatrix * clusterlinkMatrix.Invert() * geometryMatrix;

					fbxModel->fbxBoneInfoList[boneIdx]->offsetMatrix = offsetMatrix;
				}
			}
		}
	}

	const int polygonSize = mesh->GetPolygonSize(0);

	assert(polygonSize == 3);		// �ﰢ���� �ƴϸ� ����

	int arrIdx[3];
	int vertexCounter = 0;			// ������ ����

	const int triCount = mesh->GetPolygonCount(); // �޽��� �ﰢ�� ������ �����´�

	std::map<std::tuple<unsigned, float, float, float, float, float>, unsigned> indexMap;

	for (int i = 0; i < triCount; i++) // �ﰢ���� ����
	{
		for (int j = 0; j < 3; j++)	   // �ﰢ���� �� ���� �������� ����
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // �������� �ε��� ����

			arrIdx[j] = controlPointIndex;

			DirectX::SimpleMath::Vector3 normal;

			normal = GetNormal(mesh, controlPointIndex, vertexCounter);

			DirectX::SimpleMath::Vector2 uv;

			uv = GetUV(mesh, controlPointIndex, vertexCounter);

			const auto indexPair = std::make_tuple(controlPointIndex, uv.x, uv.y, normal.x, normal.y, normal.z);

			const auto iter = indexMap.find(indexPair);

			// map�� ������ insert �� ���ο� ���ؽ� ����
			if (iter == indexMap.end())
			{
				Vertex vertex;
				vertex.position = tmpMeshVertexList[controlPointIndex].position;	// �������� ����

				// ����ġ ���� ����
				for (int weightIdx = 0; weightIdx < 8; weightIdx++)
				{
					vertex.weights[weightIdx] = tmpMeshVertexList[controlPointIndex].weights[weightIdx];

					vertex.boneIndices[weightIdx] = tmpMeshVertexList[controlPointIndex].boneIndices[weightIdx];
				}

				vertex.uv = uv;

				vertex.normal = normal;

				meshData->meshVertexList.emplace_back(vertex);								// ���ο� ���ؽ� ����

				controlPointIndex = meshData->meshVertexList.size() - 1;				// index ���ο� ���ؽ� ���� �ٲٱ�

				arrIdx[j] = controlPointIndex;

				indexMap.insert({ indexPair, controlPointIndex });
			}
			// map�� �ִ��Ŷ��
			else
			{
				arrIdx[j] = iter->second;
			}

			vertexCounter++;
		}

		meshData->indices[meshCnt].emplace_back(arrIdx[0]);
		meshData->indices[meshCnt].emplace_back(arrIdx[2]);
		meshData->indices[meshCnt].emplace_back(arrIdx[1]);
	}

	// tangent ������ �����´�.
	if (mesh->GetElementNormalCount() >= 1)
		GetTangent(meshData, meshCnt);

	// �װ�Ƽ�� bool �ʱ�ȭ
	isNegativeScale = false;
}

/// <summary>
/// FBXModel�� fbxBoneInfoList�� bone���� ���� �����Ѵ�.
/// ������ boneName, parentIndex �� ����.
/// </summary>
void FBXParser::ProcessBones(fbxsdk::FbxNode* node, int idx, int parentIdx)
{
	fbxsdk::FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		std::shared_ptr<FBXBoneInfo> fbxBoneInfo = std::make_shared<FBXBoneInfo>();

		fbxBoneInfo->boneName = node->GetName();

		fbxBoneInfo->parentIndex = parentIdx;

		FbxAMatrix nodeTransform = node->EvaluateGlobalTransform(fbxsdk::FbxTime(0));

		DirectX::SimpleMath::Matrix nodeMatrix = ConvertMatrix(nodeTransform);

		const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

		const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

		DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

		nodeMatrix *= XMMatrixRotationQuaternion(q);

		fbxBoneInfo->nodeMatrix = nodeMatrix;

		fbxModel->fbxBoneInfoList.push_back(fbxBoneInfo);

		ProcessAnimationData(node);
	}

	const int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
	{
		if (attribute && attribute->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
		{
			ProcessBones(node->GetChild(i), static_cast<int>(fbxModel->fbxBoneInfoList.size()), idx);
		}
		else
		{
			ProcessBones(node->GetChild(i), static_cast<int>(fbxModel->fbxBoneInfoList.size()), parentIdx);
		}
	}
}

void FBXParser::LoadMaterial(fbxsdk::FbxSurfaceMaterial* surfaceMaterial, std::shared_ptr<FBXMeshInfo>& meshData)
{
	std::string matName = surfaceMaterial->GetName();

	std::shared_ptr<FBXMeshInfo> meshInfo = meshData;

	// �޽ÿ��� ���͸��� �̸���
	meshInfo->materials.emplace_back(matName);

	auto it = find_if(fbxModel->materialList.begin(), fbxModel->materialList.end(), [&name = matName](const std::shared_ptr<FBXMaterialInfo>& s)->bool {return (s->materialName == name); });

	// �̹� �ִ� material�̸� return
	if (it != fbxModel->materialList.end())
		return;

	std::shared_ptr<FBXMaterialInfo> material = std::make_shared<FBXMaterialInfo>();

	material->materialName = surfaceMaterial->GetName();

	if (surfaceMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[0]) * 10.0f;
		material->material_Ambient.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[1]) * 10.0f;
		material->material_Ambient.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Ambient.Get()[2]) * 10.0f;
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Specular Data
		material->material_Specular.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[0]);
		material->material_Specular.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[1]);
		material->material_Specular.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Specular.Get()[2]);
		material->material_Specular.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Emissive Strength
		material->emissionFactor = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->EmissiveFactor);

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->TransparencyFactor.Get());

		// Shininess Data
		material->roughness = material->roughness = 1.0f - float(sqrt(fmax(static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->Shininess.Get()), 0.0)) / 10.0);

		// metallic ���ε���?
		material->metallic = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->ReflectionFactor.Get());

		// Reflectivity Data �ϴ�...
		material->material_Reflectivity = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->ReflectionFactor.Get());
	}
	else if (surfaceMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		// Ambient Data
		material->material_Ambient.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[0]);
		material->material_Ambient.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[1]);
		material->material_Ambient.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Ambient.Get()[2]);
		material->material_Ambient.w = 1.0f;

		// Diffuse Data
		material->material_Diffuse.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[0]);
		material->material_Diffuse.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[1]);
		material->material_Diffuse.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Diffuse.Get()[2]);
		material->material_Diffuse.w = 1.0f;

		// Emissive Data
		material->material_Emissive.x = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[0]);
		material->material_Emissive.y = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[1]);
		material->material_Emissive.z = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->Emissive.Get()[2]);
		material->material_Emissive.w = 1.0f;

		// Emissive Strength
		material->emissionFactor = static_cast<float>(((FbxSurfacePhong*)surfaceMaterial)->EmissiveFactor);

		// Transparecy Data
		material->material_Transparency = static_cast<float>(((FbxSurfaceLambert*)surfaceMaterial)->TransparencyFactor.Get());
	}

	material->albedoMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sDiffuse);
	material->normalMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sNormalMap);
	material->roughnessMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sShininess);
	material->emissiveMap = GetTextureRelativeName(surfaceMaterial, fbxsdk::FbxSurfaceMaterial::sEmissive);

	// ���͸��� ����Ʈ�� �߰�
	fbxModel->materialList.push_back(material);
}

void FBXParser::LoadAnimation()
{
	// animNames Array�ȿ� ��� �ִϸ��̼� �̸��� ����.
	scene->FillAnimStackNameArray(OUT animNames);

	const int animCount = animNames.GetCount();

	for (int i = 0; i < animCount; i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[i]->Buffer());

		if (animStack == nullptr)
			continue;

		std::shared_ptr<FBXAnimClipInfo> animClip = std::make_shared<FBXAnimClipInfo>();
		animClip->animationName = animStack->GetName();						// �ִϸ��̼� �̸�

		// ���۽ð�, ����ð�, �ʴ� �����ӿ� ���� ����
		FbxTakeInfo* takeInfo = scene->GetTakeInfo(animStack->GetName());
		double startTime = takeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
		double endTime = takeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
		double frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

		animClip->frameRate = frameRate;

		if (startTime < endTime)
		{
			animClip->totalKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->endKeyFrame = (int)((endTime - startTime) * (double)frameRate);
			animClip->tickPerFrame = (endTime - startTime) / animClip->totalKeyFrame;
			animClip->startKeyFrame = (int)(startTime)*animClip->totalKeyFrame;
		}

		fbxModel->animationClipList.push_back(animClip);
	}
}

void FBXParser::ProcessAnimationData(FbxNode* node)
{
	const int animCount = fbxModel->animationClipList.size();

	for (int animIdx = 0; animIdx < animCount; animIdx++)
	{
		std::vector<std::shared_ptr<FBXKeyFrameInfo>> keyFrameList;
		
		std::shared_ptr<FBXKeyFrameInfo> fbxKeyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

		FbxTime::EMode timeMode = scene->GetGlobalSettings().GetTimeMode();

		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animNames[animIdx]->Buffer());
		scene->SetCurrentAnimationStack(OUT animStack);

		for (FbxLongLong frame = 0; frame < fbxModel->animationClipList[animIdx]->totalKeyFrame; frame++)
		{
			std::shared_ptr<FBXKeyFrameInfo> keyFrameInfo = std::make_shared<FBXKeyFrameInfo>();

			FbxTime fbxTime = 0;

			fbxTime.SetFrame(frame, timeMode);

			// Local Transform = �θ� Bone�� Global Transform�� inverse Transform * �ڽ� Bone�� Global Transform;
			FbxAMatrix localTransform = node->EvaluateGlobalTransform(fbxTime);

			DirectX::SimpleMath::Matrix localTM;

			if (FbxNode* parent = node->GetParent())
			{
				FbxNodeAttribute* ParentAttribute = parent->GetNodeAttribute();

				if (ParentAttribute && ParentAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
				{
					FbxAMatrix GlobalParentTransform = parent->EvaluateGlobalTransform(fbxTime);

					localTransform = GlobalParentTransform.Inverse() * localTransform;

					localTM = ConvertMatrix(localTransform);
				}
				else
				{
					localTM = ConvertMatrix(localTransform);

					const auto roll = -90.0f * DirectX::XM_PI / 180.0f;

					const auto pitch = 180.0f * DirectX::XM_PI / 180.0f;

					DirectX::SimpleMath::Quaternion q = DirectX::XMQuaternionRotationRollPitchYaw(roll, pitch, 0.0f);

					localTM *= XMMatrixRotationQuaternion(q);
				}
			}

			DirectX::XMVECTOR localScale;
			DirectX::XMVECTOR localRot;
			DirectX::XMVECTOR localPos;
			XMMatrixDecompose(&localScale, &localRot, &localPos, localTM);

			keyFrameInfo->time = fbxTime.GetSecondDouble();

			keyFrameInfo->localTransform = DirectX::SimpleMath::Vector3(localPos);
			keyFrameInfo->localRotation = DirectX::SimpleMath::Quaternion(localRot);
			keyFrameInfo->localScale = DirectX::SimpleMath::Vector3(localScale);

			keyFrameList.push_back(keyFrameInfo);
		}

		fbxModel->animationClipList[animIdx]->keyFrameList.push_back(keyFrameList);
	}
}

int FBXParser::FindBoneIndex(std::string boneName)
{
	for (int i = 0; i < fbxModel->fbxBoneInfoList.size(); ++i)
	{
		if (fbxModel->fbxBoneInfoList[i]->boneName == boneName)
			return i;
	}

	return -1;
}

DirectX::SimpleMath::Matrix FBXParser::GetNodeTM(fbxsdk::FbxNode* node)
{
	FbxMatrix localpos = scene->GetAnimationEvaluator()->GetNodeLocalTransform(node);

	DirectX::SimpleMath::Matrix localTM = ConvertMatrix(localpos);

	return localTM;
}

DirectX::SimpleMath::Vector3 FBXParser::GetNormal(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	DirectX::SimpleMath::Vector3 fbxNormal;

	if (mesh->GetElementNormalCount() >= 1)
	{
		FbxGeometryElementNormal* normal = mesh->GetElementNormal();
		int normalIdx = 0;

		// �ε����� �������� ��� ���� ����
		// ���ؽ� ���ø��� �ʿ��ϴ�.
		if (normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
				normalIdx = vertexCounter;
			else
				normalIdx = normal->GetIndexArray().GetAt(vertexCounter);
		}
		// ������ �������� ��� ���� ����.
		else if (normal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (normal->GetReferenceMode() == FbxGeometryElement::eDirect)
				normalIdx = controlPointIndex;
			else
				normalIdx = normal->GetIndexArray().GetAt(controlPointIndex);
		}

		FbxVector4 vec = normal->GetDirectArray().GetAt(normalIdx);

		if (isNegativeScale)
		{
			fbxNormal.x = static_cast<float>(-vec.mData[0]);
			fbxNormal.y = static_cast<float>(-vec.mData[2]);
			fbxNormal.z = static_cast<float>(-vec.mData[1]);
		}
		else
		{
			fbxNormal.x = static_cast<float>(vec.mData[0]);
			fbxNormal.y = static_cast<float>(vec.mData[2]);
			fbxNormal.z = static_cast<float>(vec.mData[1]);
		}

		return fbxNormal;
	}

	fbxNormal = { -1.f, -1.f, -1.f };

	return fbxNormal;
}

void FBXParser::GetTangent(std::shared_ptr<FBXMeshInfo>& meshInfo, int meshCnt)
{
	for (int i = 0; i < meshInfo->indices[meshCnt].size(); i += 3) // �ﰢ���� ����
	{
		int i0 = meshInfo->indices[meshCnt][i];
		int i1 = meshInfo->indices[meshCnt][i + 1];
		int i2 = meshInfo->indices[meshCnt][i + 2];

		// e1 = p1 - p0, e2 = p2 - p0
		DirectX::SimpleMath::Vector3 e1 = meshInfo->meshVertexList[i1].position - meshInfo->meshVertexList[i0].position;
		DirectX::SimpleMath::Vector3 e2 = meshInfo->meshVertexList[i2].position - meshInfo->meshVertexList[i0].position;

		float x1 = meshInfo->meshVertexList[i1].uv.x - meshInfo->meshVertexList[i0].uv.x; // u1 - u0
		float y1 = meshInfo->meshVertexList[i1].uv.y - meshInfo->meshVertexList[i0].uv.y; // v1 - v0
		float x2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.x; // u2 - u0
		float y2 = meshInfo->meshVertexList[i2].uv.y - meshInfo->meshVertexList[i0].uv.y; // v2 - v0

		float det = (x1 * y2) - (x2 * y1); // ad - bc
		if (det == 0.f) { det = 0.00001f; }
		float r = 1.0f / det;

		DirectX::SimpleMath::Vector3 tangent = (e1 * y2 - e2 * y1) * r;

		meshInfo->meshVertexList[i0].tangent += tangent;
		meshInfo->meshVertexList[i1].tangent += tangent;
		meshInfo->meshVertexList[i2].tangent += tangent;
	}
}

DirectX::SimpleMath::Vector2 FBXParser::GetUV(fbxsdk::FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	DirectX::SimpleMath::Vector2 fbxUV;

	if (mesh->GetElementUVCount() >= 1)
	{
		int uvIndex;

		FbxGeometryElementUV* uv = mesh->GetLayer(0)->GetUVs();

		if (uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
				uvIndex = vertexCounter;
			else
				uvIndex = uv->GetIndexArray().GetAt(vertexCounter);
		}
		else if (uv->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (uv->GetReferenceMode() == FbxGeometryElement::eDirect)
				uvIndex = controlPointIndex;
			else
				uvIndex = uv->GetIndexArray().GetAt(controlPointIndex);
		}

		fbxUV.x = static_cast<float>(uv->GetDirectArray().GetAt(uvIndex).mData[0]);
		fbxUV.y = 1.f - static_cast<float>(uv->GetDirectArray().GetAt(uvIndex).mData[1]);

		if (fbxUV.x < 0)
			fbxUV.x += 1;

		return fbxUV;
	}

	fbxUV = { 0.f, 0.f };

	return fbxUV;
}

std::string FBXParser::GetTextureRelativeName(fbxsdk::FbxSurfaceMaterial* surface, const char* materialProperty)
{
	std::string name;

	fbxsdk::FbxProperty textureProperty = surface->FindProperty(materialProperty);

	if (textureProperty.IsValid())
	{
		int count = textureProperty.GetSrcObjectCount();

		if (1 <= count)
		{
			FbxFileTexture* texture = textureProperty.GetSrcObject<FbxFileTexture>(0);
			if (texture)
				name = texture->GetRelativeFileName();
		}
	}

	std::wstring wstr = L"";

	wstr.assign(name.begin(), name.end());

	std::wstring tempName = fs::path(name).filename();

	const std::string fileName(tempName.begin(), tempName.end());

	return fileName;
}

FbxAMatrix FBXParser::GetTransformMatrix(FbxNode* node)
{
	const FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	return FbxAMatrix(translation, rotation, scaling);
}