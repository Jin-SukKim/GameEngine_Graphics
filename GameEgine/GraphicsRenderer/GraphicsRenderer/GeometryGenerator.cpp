#include "GeometryGenerator.h"
#pragma once

#include <iostream>

MeshData GeometryGenerator::MakeCube()
{
    std::vector<Vector3> positions;
    std::vector<Vector3> colors;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords; // 텍스춰 좌표

    // 윗면
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, 1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 아랫면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    normals.push_back(Vector3(0.0f, -1.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 앞면
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, -1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 뒷면
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(0.0f, 1.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 왼쪽
    positions.push_back(Vector3(-1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, 1.0f));
    positions.push_back(Vector3(-1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(-1.0f, -1.0f, -1.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    colors.push_back(Vector3(1.0f, 1.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(-1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));

    // 오른쪽
    positions.push_back(Vector3(1.0f, -1.0f, 1.0f));
    positions.push_back(Vector3(1.0f, -1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, -1.0f));
    positions.push_back(Vector3(1.0f, 1.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    colors.push_back(Vector3(1.0f, 0.0f, 1.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    normals.push_back(Vector3(1.0f, 0.0f, 0.0f));
    texcoords.push_back(Vector2(0.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 0.0f));
    texcoords.push_back(Vector2(1.0f, 1.0f));
    texcoords.push_back(Vector2(0.0f, 1.0f));
    
    MeshData Cube;

    // Vertex 형식이 정해져 있다.
    for (int i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.texcoord = texcoords[i];

        Cube.vertices.push_back(v);
    }

    // index의 값을 unsigned int16으로 받고 있다. (16bit)
    // 큰 모델을 사용할 때는 uint32_t도 사용하고 작으면 uint8_t도 사용한다.
    Cube.indices = {
        0,  1,  2,  0,  2,  3,  // 윗면
        4,  5,  6,  4,  6,  7,  // 아랫면
        8,  9,  10, 8,  10, 11, // 앞면
        12, 13, 14, 12, 14, 15, // 뒷면
        16, 17, 18, 16, 18, 19, // 왼쪽
        20, 21, 22, 20, 22, 23  // 오른쪽
    };

    return Cube;
}

MeshData GeometryGenerator::MakeTriangle()
{
    std::vector<Vector3> pos;
    std::vector<Vector3> color;
    std::vector<Vector3> normal;
    std::vector<Vector2> texcoord;

    pos.push_back(Vector3(-1.f, -1.f, 0.f));
    pos.push_back(Vector3(0.f, 1.f, 0.f));
    pos.push_back(Vector3(1.f, -1.f, 0.f));

    color.push_back(Vector3(1.f, 0.f, 0.f));
    color.push_back(Vector3(0.f, 1.f, 0.f));
    color.push_back(Vector3(0.f, 0.f, 1.f));

    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));


    texcoord.push_back(Vector2(0.f, 1.f));
    texcoord.push_back(Vector2(0.5f, 0.f));
    texcoord.push_back(Vector2(1.f, 1.f));

    MeshData triangle;

    for (int i = 0; i < pos.size(); i++)
    {
        Vertex v;

        v.pos = pos[i];
        v.color = color[i];
        v.normal = normal[i];
        v.texcoord = texcoord[i];

        triangle.vertices.push_back(v);
    }

    triangle.indices = {
        0, 1, 2
    };

    return triangle;
}

MeshData GeometryGenerator::MakeSquare()
{
    std::vector<Vector3> pos;
    std::vector<Vector3> color;
    std::vector<Vector3> normal;
    std::vector<Vector2> texcoord;

    pos.push_back(Vector3(-1.f, -1.f, 0.f));
    pos.push_back(Vector3(-1.f, 1.f, 0.f));
    pos.push_back(Vector3(1.f, 1.f, 0.f));
    pos.push_back(Vector3(1.f, -1.f, 0.f));

    color.push_back(Vector3(1.f, 0.f, 0.f));
    color.push_back(Vector3(0.f, 1.f, 0.f));
    color.push_back(Vector3(0.f, 0.f, 1.f));
    color.push_back(Vector3(0.f, 1.f, 0.f));

    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));

    texcoord.push_back(Vector2(0.f, 1.f));
    texcoord.push_back(Vector2(0.f, 0.f));
    texcoord.push_back(Vector2(1.f, 0.f));
    texcoord.push_back(Vector2(1.f, 1.f));

    MeshData square;

    for (int i = 0; i < pos.size(); i++)
    {
        Vertex v;

        v.pos = pos[i];
        v.color = color[i];
        v.normal = normal[i];
        v.texcoord = texcoord[i];

        square.vertices.push_back(v);
    }

    square.indices = {
        0, 1, 2, 
        0, 2, 3
    };

    return square;
}

MeshData GeometryGenerator::MakeCircle(const float& radius, const int& numTriangles, const Vector3& color)
{
    // 원의 중심
    Vector3 center = { 0.f, 0.f, 0.f };

    MeshData circle;
    
    Vertex v;
    v.pos = center;
    v.color = color;
    v.normal = Vector3(0.f, 0.f, -1.f);
    v.texcoord = Vector2(0.5f, 0.5f);

    circle.vertices.push_back(v);

    // vertex를 회전시킬 각도
    const float radian = DirectX::XM_2PI / static_cast<float>(numTriangles);

    for (float angle = 0; angle < DirectX::XM_2PI; angle += radian)
    {
        v.pos = center + radius * Vector3(cos(angle), sin(angle), 0.f);
        v.color = color;
        v.normal = Vector3(0.f, 0.f, -1.f);
        v.texcoord = Vector2((cos(angle) + 1.f) * 0.5f, (sin(angle) + 1.f) * 0.5f);

        circle.vertices.push_back(v);
    }

    // 마지막 삼각형 제외
    for (int i = 0; i < numTriangles - 1; i++)
    {
        // clock-wise
        circle.indices.push_back(0);
        circle.indices.push_back(i + 2);
        circle.indices.push_back(i + 1);
    }
    // 맨 마지막 삼각형은 첫 vertex를 재활용
    circle.indices.push_back(0);
    circle.indices.push_back(1);
    circle.indices.push_back(numTriangles);

    return circle;
}

MeshData GeometryGenerator::MakeGrid(const float& width, const float& height, const int& numSlices, const int& numStacks)
{
    using DirectX::SimpleMath::Matrix;

    // 좌우로 몇조각으로 쪼개 다음 x의 위치는 previous x + dx
    const float dx = width / numSlices;
    // 위아래 몇 층으로 쪼개 다음 y의 위치 previous y + dy
    const float dy = height / numStacks;

    MeshData grid;

    Vector3 leftBottom = Vector3(-0.5f * width, -0.5f * height, 0.f);

    for (int j = 0; j <= numStacks; j++)
    {
        Vector3 stack = Vector3::Transform(
            // dy * j만큼 이동시킨 vertex의 위치
            leftBottom, Matrix::CreateTranslation(Vector3(0.f, dy * j, 0.f))
        );
        for (int i = 0; i <= numSlices; i++)
        {
            Vertex v;

            v.pos = Vector3::Transform(
                // dx * i만큼 이동시킨 vertex의 위치
                stack, Matrix::CreateTranslation(Vector3(dx * (float)i, 0.f, 0.f))
            );
            v.normal = v.pos;
            v.normal.z = -1.f;
            v.color = Vector3(1.f, 1.f, 1.f);
            v.texcoord = Vector2((float)i / numSlices, 1.f - (float)j / numSlices);
            grid.vertices.push_back(v);
        }
    }

    for (int j = 0; j < numStacks; j++)
    {
        // 1d array를 2d array처럼 사용하기 위해 offset 설정
        const int offset = (numSlices + 1) * j;
        for (int i = 0; i < numSlices; i++)
        {
            // 삼각형 1
            grid.indices.push_back(offset + i);
            grid.indices.push_back(offset + i + numSlices + 1); // 1d array를 2d array처럼 사용
            grid.indices.push_back(offset + i + numSlices + 2);

            // 삼각형 2
            grid.indices.push_back(offset + i);
            grid.indices.push_back(offset + i + numSlices + 2);
            grid.indices.push_back(offset + i + 1);
        }
    }

    return grid;
}

MeshData GeometryGenerator::MakeCylinder(const float& topRadius, const float& bottomRadius, const float& height, const int& numSlices)
{
    using DirectX::SimpleMath::Matrix;

    const float angle = -DirectX::XM_2PI / (float)numSlices;

    MeshData cylinder;

    // 옆면의 아래 Vertex들
    for (int i = 0; i <= numSlices; i++)
    {
        Vertex v;

        v.pos = Vector3::Transform(
            Vector3(bottomRadius, -0.5f * height, 0.f),
            Matrix::CreateRotationY(angle * (float)i));

        // normal vector = vertex - 원의 중심
        v.normal = v.pos - Vector3(0.f, -0.5f * height, 0.f);
        v.normal.Normalize();

        // Texture 좌표계는 [0, 1] 범위
        // 시작점과 끝점이 같은 위치에 있지만 Texcoord는 0과 1로 다르다.
        v.texcoord = Vector2((float)i / numSlices, 1.f);

        v.color = Vector3(1.f, 1.f, 1.f);

        cylinder.vertices.push_back(v);
    }

    // 옆면의 위 vertex들
    for (int i = 0; i <= numSlices; i++)
    {
        Vertex v;

        v.pos = Vector3::Transform(Vector3(topRadius, 0.5f * height, 0.f),
            Matrix::CreateRotationY(angle * (float)i));

        v.normal = v.pos - Vector3(0.f, 0.5f * height, 0.f);
        v.normal.Normalize();

        v.texcoord = Vector2((float)i / numSlices, 0.f);
        
        v.color = Vector3(1.f, 1.f, 1.f);

        cylinder.vertices.push_back(v);
    }

    // index들
    for (int i = 0; i < numSlices; i++)
    {
        // 삼각형 1
        cylinder.indices.push_back(i);
        cylinder.indices.push_back(i + numSlices + 1);
        cylinder.indices.push_back(i + numSlices + 2);

        // 삼각형 2
        cylinder.indices.push_back(i);
        cylinder.indices.push_back(i + numSlices + 2);
        cylinder.indices.push_back(i + 1);
    }

    return cylinder;
}

MeshData GeometryGenerator::MakeSphere(const float& radius, const int& numSlices, const int& numStacks)
{
    using DirectX::SimpleMath::Matrix;

    // 구의 360도 (Column 각도)
    const float angle = -DirectX::XM_2PI / (float)numSlices;
    // 각 row 줄의 각도
    const float phi = -DirectX::XM_PI / (float)numStacks; 

    MeshData sphere;

    for (int j = 0; j <= numStacks; j++)
    {
        // 맨 아래에서 위로 올라가면 vertex 생성 (index 계산이 좀 더 편하다)
        Vector3 startPoint = Vector3::Transform(
            Vector3(0.0f, -radius, 0.f),
            Matrix::CreateRotationZ(phi * float(j)));

        for (int i = 0; i <= numSlices; i++)
        {
            Vertex v;

            v.pos = Vector3::Transform(
                startPoint, Matrix::CreateRotationY(angle * (float)i));

            // 구의 중심은 원점(0.f, 0.f, 0.f)
            // v.pos는 원점에서 시작하므로 normalize한 값이 normal 벡터와 같다.
            v.normal = v.pos;
            v.normal.Normalize();

            v.texcoord = Vector2((float)i / numSlices, 1.f - (float)j / numStacks);

            v.color = Vector3(1.f, 1.f, 1.f);
            sphere.vertices.push_back(v);
        }
    }

    for (int j = 0; j < numStacks; j++)
    {
        const int offset = (numSlices + 1) * j;

        for (int i = 0; i < numSlices; i++)
        {
            sphere.indices.push_back(offset + i);
            sphere.indices.push_back(offset + i + numSlices + 1);
            sphere.indices.push_back(offset + i + numSlices + 2);

            sphere.indices.push_back(offset + i);
            sphere.indices.push_back(offset + i + numSlices + 2);
            sphere.indices.push_back(offset + i + 1);
        }
    }

    return sphere;
}

MeshData GeometryGenerator::MakeIcosahedron()
{
    const float X = 0.525731f;
    const float Z = 0.850651f;

    MeshData icosahedron;

    std::vector<Vector3> pos = {
        Vector3(-X, 0.0f, Z), Vector3(X, 0.0f, Z),   Vector3(-X, 0.0f, -Z),
        Vector3(X, 0.0f, -Z), Vector3(0.0f, Z, X),   Vector3(0.0f, Z, -X),
        Vector3(0.0f, -Z, X), Vector3(0.0f, -Z, -X), Vector3(Z, X, 0.0f),
        Vector3(-Z, X, 0.0f), Vector3(Z, -X, 0.0f),  Vector3(-Z, -X, 0.0f) };

    for (size_t i = 0; i < pos.size(); i++) {
        Vertex v;
        v.pos = pos[i];
        v.normal = v.pos;
        v.normal.Normalize();
        v.color = Vector3(1.f, 1.f, 1.f);

        icosahedron.vertices.push_back(v);
    }

    icosahedron.indices = { 1,  4,  0, 4,  9, 0, 4, 5,  9, 8, 5, 4,  1,  8, 4,
                       1,  10, 8, 10, 3, 8, 8, 3,  5, 3, 2, 5,  3,  7, 2,
                       3,  10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6,  1, 0,
                       10, 1,  6, 11, 0, 9, 2, 11, 9, 5, 2, 9,  11, 2, 7 };

    return icosahedron;
}


// Regular Tetrahedron
// https://mathworld.wolfram.com/RegularTetrahedron.html
MeshData GeometryGenerator::MakeTetrahedron()
{
    const float a = 1.0f;
    const float x = sqrt(3.0f) / 3.0f * a;
    const float d = sqrt(3.0f) / 6.0f * a; // = x / 2
    const float h = sqrt(6.0f) / 3.0f * a;

    std::vector<Vector3> points = { {0.0f, x, 0.0f},
                              {-0.5f * a, -d, 0.0f},
                              {+0.5f * a, -d, 0.0f},
                              {0.0f, 0.0f, h} };

    Vector3 center = Vector3(0.0f);

    for (int i = 0; i < 4; i++) {
        center += points[i];
    }
    center /= 4.0f;

    for (int i = 0; i < 4; i++) {
        points[i] -= center;
    }

    MeshData meshData;

    for (int i = 0; i < points.size(); i++) {

        Vertex v;
        v.pos = points[i];
        v.normal = v.pos; // 중심이 원점
        v.normal.Normalize();
        v.color = Vector3(1.f, 1.f, 1.f);

        meshData.vertices.push_back(v);
    }

    meshData.indices = { 0, 1, 2, 3, 2, 1, 0, 3, 1, 0, 2, 3 };

    return meshData;
}

// 상세도가 낮은 모델의 상세도를 삼각형을 여러개로 쪼개 높여준다.
// GPU에서 계산할 수 있다. (Geometry Shader, Tessellation)
MeshData GeometryGenerator::SubdivideToSphere(const float& radius, MeshData sphere)
{
    using DirectX::SimpleMath::Matrix;

    // 구 모델의 반지름 조절
    for (auto& v : sphere.vertices)
    {
        v.normal = v.pos;
        v.normal.Normalize();
        // normal은 원점에서 vertex 방향 벡터 * radius(반지름) = 새로운 구 모델의 vertex pos
        v.pos = v.normal * radius;
    }

    // 한 삼각형의 각 정점들을 평균내서 새로운 vertex를 생성해
    // 여러 삼각형으로 쪼개면 삼각형 위에 새로운 vertex들이 생기는데
    // 이 vertex들을 다시 구의 표면으로 옮겨준다.
    // function pointer = void(*ProjectVertex)(Vertex&) = auto
    auto ProjectVertex = [&](Vertex& v) {
        v.normal = v.pos; 
        v.normal.Normalize();
        v.pos = v.normal * radius;
    };

    // 같은 위치의 vertex들이 같은 normal vector를 가진 경우와 (더 각져있다)
    // 각각의 face normal vector를 가진 경우 결과가 달라진다. (더 부드러워진다)
    auto UpdateFaceNormal = [](Vertex& v0, Vertex& v1, Vertex& v2) {
        auto faceNormal = (v1.pos - v0.pos).Cross(v2.pos - v0.pos);
        faceNormal.Normalize();

        // 같은 normal vector를 가지도록 설정
        v0.normal = faceNormal;
        v1.normal = faceNormal;
        v2.normal = faceNormal;
    };

    MeshData newSphere;
    uint16_t count = 0;

    // 삼각형 1개를 4개로 쪼개준다.
    for (size_t i = 0; i < sphere.indices.size(); i += 3)
    {
        size_t i0 = sphere.indices[i];
        size_t i1 = sphere.indices[i + 1];
        size_t i2 = sphere.indices[i + 2];

        // 각 삼각형의 vertex는 index를 이용해 구해준다.
        Vertex v0 = sphere.vertices[i0];
        Vertex v1 = sphere.vertices[i1];
        Vertex v2 = sphere.vertices[i2];

        // 삼각형을 4등분 하기 위한 새로운 vertex 3개
        Vertex v3;
        v3.pos = (v0.pos + v2.pos) * 0.5f;
        v3.texcoord = (v0.texcoord + v2.texcoord) * 0.5f;
        v3.color = Vector3(1.f, 1.f, 1.f);
        // 새로운 삼각형은 원래 삼각형의 평면 위에 있기에
        // 구의 평면 위로 옮겨준다. (여기서 normal도 설정하고 있다.)
        ProjectVertex(v3);

        Vertex v4;
        v4.pos = (v0.pos + v1.pos) * 0.5f;
        v4.texcoord = (v0.texcoord + v1.texcoord) * 0.5f;
        v4.color = Vector3(1.f, 1.f, 1.f);
        ProjectVertex(v4);

        Vertex v5;
        v5.pos = (v1.pos + v2.pos) * 0.5f;
        v5.texcoord = (v1.texcoord + v2.texcoord) * 0.5f;
        v5.color = Vector3(1.f, 1.f, 1.f);
        ProjectVertex(v5);

        // 파라미터를 참조로 받고 있기에 각각의 vertex에 영향을 준다.
        UpdateFaceNormal(v4, v1, v5);

        // 모든 vertex 새로 추가 (삼각형 순서대로) - 삼각형 4개
        // copy constructor로 복사되기에 원본이 한 번 더 바뀌기 전에 넣어준다.
        newSphere.vertices.push_back(v4);
        newSphere.vertices.push_back(v1);
        newSphere.vertices.push_back(v5);

        UpdateFaceNormal(v0, v4, v3);

        newSphere.vertices.push_back(v0);
        newSphere.vertices.push_back(v4);
        newSphere.vertices.push_back(v3);

        UpdateFaceNormal(v3, v4, v5);

        newSphere.vertices.push_back(v3);
        newSphere.vertices.push_back(v4);
        newSphere.vertices.push_back(v5);

        UpdateFaceNormal(v3, v5, v2);

        newSphere.vertices.push_back(v3);
        newSphere.vertices.push_back(v5);
        newSphere.vertices.push_back(v2);

        // index 업데이트
        // 현재 vertex를 삼각형의 순서대로 넣어주고 있어서
        // index는 순차적으로 넣어줘도 된다.
        for (int j = 0; j < 12; j++) { // 삼각형 4개 vertex는 12개
            newSphere.indices.push_back(j + count);
        }
        // 삼각형 1개당 3개의 index를 필요하므로 3 * 4(삼각형 4개)
        count += 12;
    }

    return newSphere;
}
