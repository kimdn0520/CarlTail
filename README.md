# [CockTail]
게임인재원 3기 Final Project

# FBXLoader / ResourceManager 
- **FBX sdk를 활용한 FBXLoader**
  - FBX Static/Skinned Mesh 불러오기
  - 하나의 Mesh에 Material 여러개이면 하나의 Mesh에 하나의 Material로 쪼개는 함수를 쓰면
    Index를 알게 되어서 그 Mesh들을 다시 하나로 합치는 작업 수행
  - Boost Library의 Serialization을 이용해 FBXLoad 수행 후 자체포맷 BinaryFile로 저장 (Mesh, Material, Animation)
  - CreateProcess, WaitForMultipleObjects 로 FBX파일을 나누어서 Load를 빠르게 할 수 있도록 하였음.
  
![image](https://user-images.githubusercontent.com/58064839/226089958-6932760d-8e6f-4495-882f-1b40ecbe21b0.png)
![image](https://user-images.githubusercontent.com/58064839/226090015-f24d20ef-32ab-4396-9268-07b3fe6fc9aa.png)
![image](https://user-images.githubusercontent.com/58064839/226090155-d2dac4ed-1135-4eec-a1df-948dbaa1e032.png)

# Unity Scene Parsing
- **yaml-cpp 를 활용한 유니티 씬 파싱**
  - 현재 GameObject와 Transform, BoxCollider, SphereCollider, Camera, Light 등 컴포넌트 정보를 가져올 수 있다.
  - GameObject와 컴포넌트들의 ID 값을 가져올 수 있다.
![image](https://user-images.githubusercontent.com/58064839/211226390-b9c861ef-34cc-4301-98ce-f6519f0371ff.png)
![image](https://user-images.githubusercontent.com/58064839/221354055-b48cdf9e-7ef6-460c-ba4a-f285fec8f9dd.png)

# Nav Mesh / Nav Agent
- **언리얼 엔진에서 사용하는 Recast & Detour library를 활용한 NavMesh**
  - Recast & Detour Tool를 사용해서 navMesh.bin 을 뽑은뒤에 자체엔진으로 로드
  - 내장된 함수 FindPath 를 사용해서 NavAgent 컴포넌트에 적용
  - IMGUI를 래핑하여 DrawLine, DrawSpot, DrawTriangle로 NavMesh와 NavPath를 DebugDraw 할수 있도록 함 
![ezgif com-video-to-gif](https://user-images.githubusercontent.com/58064839/221354769-85a97b92-f1ab-417d-abe3-198a3d913284.gif)


# Animator State 구조 구성 / Animation Blending
- **유니티의 Animator를 참고하여 State, Transtion, Condition기능을 추가해서 적용**
  - transition duration(0~1) 의 값에 따라 Animation Blending 적용
![ez1gif com-video-to-gif](https://user-images.githubusercontent.com/58064839/221354976-4ee21770-c8f0-401d-b6df-2e35b1fae446.gif)
