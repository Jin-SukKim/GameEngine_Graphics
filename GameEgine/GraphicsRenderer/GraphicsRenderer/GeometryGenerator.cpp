#include "GeometryGenerator.h"
#pragma once


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
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.uv = texcoords[i];

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
    std::vector<Vector2> uv;

    pos.push_back(Vector3(-1.f, -1.f, 0.f));
    pos.push_back(Vector3(0.f, 1.f, 0.f));
    pos.push_back(Vector3(1.f, -1.f, 0.f));

    color.push_back(Vector3(1.f, 0.f, 0.f));
    color.push_back(Vector3(0.f, 1.f, 0.f));
    color.push_back(Vector3(0.f, 0.f, 1.f));

    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));
    normal.push_back(Vector3(0.f, 0.f, -1.f));


    uv.push_back(Vector2(0.f, 1.f));
    uv.push_back(Vector2(0.5f, 0.f));
    uv.push_back(Vector2(1.f, 1.f));

    MeshData triangle;

    for (int i = 0; i < pos.size(); i++)
    {
        Vertex v;

        v.pos = pos[i];
        v.color = color[i];
        v.normal = normal[i];
        v.uv = uv[i];

        triangle.vertices.push_back(v);
    }

    triangle.indices = {
        0, 1, 2
    };

    return triangle;
}

MeshData GeometryGenerator::MakeSqaure()
{
    std::vector<Vector3> pos;
    std::vector<Vector3> color;
    std::vector<Vector3> normal;
    std::vector<Vector2> uv;

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

    uv.push_back(Vector2(0.f, 1.f));
    uv.push_back(Vector2(0.f, 0.f));
    uv.push_back(Vector2(1.f, 0.f));
    uv.push_back(Vector2(1.f, 1.f));

    MeshData square;

    for (int i = 0; i < pos.size(); i++)
    {
        Vertex v;

        v.pos = pos[i];
        v.color = color[i];
        v.normal = normal[i];
        v.uv = uv[i];

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
    v.uv = Vector2(0.5f, 0.5f);

    circle.vertices.push_back(v);

    // vertex를 회전시킬 각도
    const float radian = DirectX::XM_2PI / static_cast<float>(numTriangles);

    for (float angle = 0; angle < DirectX::XM_2PI; angle += radian)
    {
        v.pos = center + radius * Vector3(cos(angle), sin(angle), 0.f);
        v.color = color;
        v.normal = Vector3(0.f, 0.f, -1.f);
        v.uv = Vector2((cos(angle) + 1.f) * 0.5f, (sin(angle) + 1.f) * 0.5f);

        circle.vertices.push_back(v);
    }

    // 마지막 삼각형 제외
    for (size_t i = 0; i < numTriangles - 1; i++)
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
                stack, Matrix::CreateTranslation(Vector3(dx * i, 0.f, 0.f))
            );
            v.normal = v.pos;
            v.normal.z = -1.f;
            v.color = Vector3(1.f, 1.f, 1.f);
            v.uv = Vector2((float)i / numSlices, 1.f - (float)j / numSlices);
            
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
