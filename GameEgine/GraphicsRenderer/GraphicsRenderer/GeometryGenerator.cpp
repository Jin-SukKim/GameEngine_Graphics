#include "GeometryGenerator.h"
#pragma once

#include <iostream>

MeshData GeometryGenerator::MakeCube()
{
    std::vector<Vector3> positions;
    std::vector<Vector3> colors;
    std::vector<Vector3> normals;
    std::vector<Vector2> texcoords; // �ؽ��� ��ǥ

    // ����
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

    // �Ʒ���
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

    // �ո�
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

    // �޸�
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

    // ����
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

    // ������
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

    // Vertex ������ ������ �ִ�.
    for (int i = 0; i < positions.size(); i++) {
        Vertex v;
        v.pos = positions[i];
        v.color = colors[i];
        v.texcoord = texcoords[i];

        Cube.vertices.push_back(v);
    }

    // index�� ���� unsigned int16���� �ް� �ִ�. (16bit)
    // ū ���� ����� ���� uint32_t�� ����ϰ� ������ uint8_t�� ����Ѵ�.
    Cube.indices = {
        0,  1,  2,  0,  2,  3,  // ����
        4,  5,  6,  4,  6,  7,  // �Ʒ���
        8,  9,  10, 8,  10, 11, // �ո�
        12, 13, 14, 12, 14, 15, // �޸�
        16, 17, 18, 16, 18, 19, // ����
        20, 21, 22, 20, 22, 23  // ������
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
    // ���� �߽�
    Vector3 center = { 0.f, 0.f, 0.f };

    MeshData circle;
    
    Vertex v;
    v.pos = center;
    v.color = color;
    v.normal = Vector3(0.f, 0.f, -1.f);
    v.texcoord = Vector2(0.5f, 0.5f);

    circle.vertices.push_back(v);

    // vertex�� ȸ����ų ����
    const float radian = DirectX::XM_2PI / static_cast<float>(numTriangles);

    for (float angle = 0; angle < DirectX::XM_2PI; angle += radian)
    {
        v.pos = center + radius * Vector3(cos(angle), sin(angle), 0.f);
        v.color = color;
        v.normal = Vector3(0.f, 0.f, -1.f);
        v.texcoord = Vector2((cos(angle) + 1.f) * 0.5f, (sin(angle) + 1.f) * 0.5f);

        circle.vertices.push_back(v);
    }

    // ������ �ﰢ�� ����
    for (int i = 0; i < numTriangles - 1; i++)
    {
        // clock-wise
        circle.indices.push_back(0);
        circle.indices.push_back(i + 2);
        circle.indices.push_back(i + 1);
    }
    // �� ������ �ﰢ���� ù vertex�� ��Ȱ��
    circle.indices.push_back(0);
    circle.indices.push_back(1);
    circle.indices.push_back(numTriangles);

    return circle;
}

MeshData GeometryGenerator::MakeGrid(const float& width, const float& height, const int& numSlices, const int& numStacks)
{
    using DirectX::SimpleMath::Matrix;

    // �¿�� ���������� �ɰ� ���� x�� ��ġ�� previous x + dx
    const float dx = width / numSlices;
    // ���Ʒ� �� ������ �ɰ� ���� y�� ��ġ previous y + dy
    const float dy = height / numStacks;

    MeshData grid;

    Vector3 leftBottom = Vector3(-0.5f * width, -0.5f * height, 0.f);

    for (int j = 0; j <= numStacks; j++)
    {
        Vector3 stack = Vector3::Transform(
            // dy * j��ŭ �̵���Ų vertex�� ��ġ
            leftBottom, Matrix::CreateTranslation(Vector3(0.f, dy * j, 0.f))
        );
        for (int i = 0; i <= numSlices; i++)
        {
            Vertex v;

            v.pos = Vector3::Transform(
                // dx * i��ŭ �̵���Ų vertex�� ��ġ
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
        // 1d array�� 2d arrayó�� ����ϱ� ���� offset ����
        const int offset = (numSlices + 1) * j;
        for (int i = 0; i < numSlices; i++)
        {
            // �ﰢ�� 1
            grid.indices.push_back(offset + i);
            grid.indices.push_back(offset + i + numSlices + 1); // 1d array�� 2d arrayó�� ���
            grid.indices.push_back(offset + i + numSlices + 2);

            // �ﰢ�� 2
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

    // ������ �Ʒ� Vertex��
    for (int i = 0; i <= numSlices; i++)
    {
        Vertex v;

        v.pos = Vector3::Transform(
            Vector3(bottomRadius, -0.5f * height, 0.f),
            Matrix::CreateRotationY(angle * (float)i));

        // normal vector = vertex - ���� �߽�
        v.normal = v.pos - Vector3(0.f, -0.5f * height, 0.f);
        v.normal.Normalize();

        // Texture ��ǥ��� [0, 1] ����
        // �������� ������ ���� ��ġ�� ������ Texcoord�� 0�� 1�� �ٸ���.
        v.texcoord = Vector2((float)i / numSlices, 1.f);

        v.color = Vector3(1.f, 1.f, 1.f);

        cylinder.vertices.push_back(v);
    }

    // ������ �� vertex��
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

    // index��
    for (int i = 0; i < numSlices; i++)
    {
        // �ﰢ�� 1
        cylinder.indices.push_back(i);
        cylinder.indices.push_back(i + numSlices + 1);
        cylinder.indices.push_back(i + numSlices + 2);

        // �ﰢ�� 2
        cylinder.indices.push_back(i);
        cylinder.indices.push_back(i + numSlices + 2);
        cylinder.indices.push_back(i + 1);
    }

    return cylinder;
}

MeshData GeometryGenerator::MakeSphere(const float& radius, const int& numSlices, const int& numStacks)
{
    using DirectX::SimpleMath::Matrix;

    // ���� 360�� (Column ����)
    const float angle = -DirectX::XM_2PI / (float)numSlices;
    // �� row ���� ����
    const float phi = -DirectX::XM_PI / (float)numStacks; 

    MeshData sphere;

    for (int j = 0; j <= numStacks; j++)
    {
        // �� �Ʒ����� ���� �ö󰡸� vertex ���� (index ����� �� �� ���ϴ�)
        Vector3 startPoint = Vector3::Transform(
            Vector3(0.0f, -radius, 0.f),
            Matrix::CreateRotationZ(phi * float(j)));

        for (int i = 0; i <= numSlices; i++)
        {
            Vertex v;

            v.pos = Vector3::Transform(
                startPoint, Matrix::CreateRotationY(angle * (float)i));

            // ���� �߽��� ����(0.f, 0.f, 0.f)
            // v.pos�� �������� �����ϹǷ� normalize�� ���� normal ���Ϳ� ����.
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
        v.normal = v.pos; // �߽��� ����
        v.normal.Normalize();
        v.color = Vector3(1.f, 1.f, 1.f);

        meshData.vertices.push_back(v);
    }

    meshData.indices = { 0, 1, 2, 3, 2, 1, 0, 3, 1, 0, 2, 3 };

    return meshData;
}

// �󼼵��� ���� ���� �󼼵��� �ﰢ���� �������� �ɰ� �����ش�.
// GPU���� ����� �� �ִ�. (Geometry Shader, Tessellation)
MeshData GeometryGenerator::SubdivideToSphere(const float& radius, MeshData sphere)
{
    using DirectX::SimpleMath::Matrix;

    // �� ���� ������ ����
    for (auto& v : sphere.vertices)
    {
        v.normal = v.pos;
        v.normal.Normalize();
        // normal�� �������� vertex ���� ���� * radius(������) = ���ο� �� ���� vertex pos
        v.pos = v.normal * radius;
    }

    // �� �ﰢ���� �� �������� ��ճ��� ���ο� vertex�� ������
    // ���� �ﰢ������ �ɰ��� �ﰢ�� ���� ���ο� vertex���� ����µ�
    // �� vertex���� �ٽ� ���� ǥ������ �Ű��ش�.
    // function pointer = void(*ProjectVertex)(Vertex&) = auto
    auto ProjectVertex = [&](Vertex& v) {
        v.normal = v.pos; 
        v.normal.Normalize();
        v.pos = v.normal * radius;
    };

    // ���� ��ġ�� vertex���� ���� normal vector�� ���� ���� (�� �����ִ�)
    // ������ face normal vector�� ���� ��� ����� �޶�����. (�� �ε巯������)
    auto UpdateFaceNormal = [](Vertex& v0, Vertex& v1, Vertex& v2) {
        auto faceNormal = (v1.pos - v0.pos).Cross(v2.pos - v0.pos);
        faceNormal.Normalize();

        // ���� normal vector�� �������� ����
        v0.normal = faceNormal;
        v1.normal = faceNormal;
        v2.normal = faceNormal;
    };

    MeshData newSphere;
    uint16_t count = 0;

    // �ﰢ�� 1���� 4���� �ɰ��ش�.
    for (size_t i = 0; i < sphere.indices.size(); i += 3)
    {
        size_t i0 = sphere.indices[i];
        size_t i1 = sphere.indices[i + 1];
        size_t i2 = sphere.indices[i + 2];

        // �� �ﰢ���� vertex�� index�� �̿��� �����ش�.
        Vertex v0 = sphere.vertices[i0];
        Vertex v1 = sphere.vertices[i1];
        Vertex v2 = sphere.vertices[i2];

        // �ﰢ���� 4��� �ϱ� ���� ���ο� vertex 3��
        Vertex v3;
        v3.pos = (v0.pos + v2.pos) * 0.5f;
        v3.texcoord = (v0.texcoord + v2.texcoord) * 0.5f;
        v3.color = Vector3(1.f, 1.f, 1.f);
        // ���ο� �ﰢ���� ���� �ﰢ���� ��� ���� �ֱ⿡
        // ���� ��� ���� �Ű��ش�. (���⼭ normal�� �����ϰ� �ִ�.)
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

        // �Ķ���͸� ������ �ް� �ֱ⿡ ������ vertex�� ������ �ش�.
        UpdateFaceNormal(v4, v1, v5);

        // ��� vertex ���� �߰� (�ﰢ�� �������) - �ﰢ�� 4��
        // copy constructor�� ����Ǳ⿡ ������ �� �� �� �ٲ�� ���� �־��ش�.
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

        // index ������Ʈ
        // ���� vertex�� �ﰢ���� ������� �־��ְ� �־
        // index�� ���������� �־��൵ �ȴ�.
        for (int j = 0; j < 12; j++) { // �ﰢ�� 4�� vertex�� 12��
            newSphere.indices.push_back(j + count);
        }
        // �ﰢ�� 1���� 3���� index�� �ʿ��ϹǷ� 3 * 4(�ﰢ�� 4��)
        count += 12;
    }

    return newSphere;
}
