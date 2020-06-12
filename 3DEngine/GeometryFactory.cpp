#include "GeometryFactory.h"

Geometry buildVerticesSmooth(float radius, int sub, int vertexCountPerRow);
Geometry buildVerticesSharp(float radius, int sub, int vertexCountPerRow);
void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3]);
void scaleVertex(float v[3], float scale);
float computeScaleForLength(const float v[3], float length);
std::vector<float> getUnitPositiveX(unsigned int pointsPerRow);

GeometryFactory::~GeometryFactory()
{
}

Geometry GeometryFactory::CreateCube(float size)
{
    int vertexCountPerRow = (unsigned int)pow(2, 0) + 1;
    int vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

    return buildVerticesSharp(size, 0, vertexCountPerRow);
}

Geometry GeometryFactory::CreateSphere(float radius, int sub)
{
    int vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    int vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

    return buildVerticesSmooth(radius, sub, vertexCountPerRow);
}

Geometry buildVerticesSharp(float radius, int sub, int vertexCountPerRow)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // generate unit-length verties in +X face
    std::vector<float> unitVertices = getUnitPositiveX(vertexCountPerRow);

    unsigned int k = 0, k1, k2, i1, i2; // indices
    float v1[3], v2[3], v3[3], v4[3];   // tmp vertices
    float t1[2], t2[2], t3[2], t4[2];   // texture coords
    float n[3];                         // normal vector

    // +X face
    for (unsigned int i = 0; i < vertexCountPerRow - 1; ++i)
    {
        k1 = i * vertexCountPerRow;              // index at curr row
        k2 = k1 + vertexCountPerRow;             // index at next row

        // vertical tex coords
        t1[1] = t3[1] = (float)i / (vertexCountPerRow - 1);
        t2[1] = t4[1] = (float)(i + 1) / (vertexCountPerRow - 1);

        for (unsigned int j = 0; j < vertexCountPerRow - 1; ++j, ++k1, ++k2)
        {
            i1 = k1 * 3;
            i2 = k2 * 3;

            // 4 vertices of a quad
            // v1--v3
            // | / |
            // v2--v4
            v1[0] = unitVertices[i1];
            v1[1] = unitVertices[i1 + 1];
            v1[2] = unitVertices[i1 + 2];
            v2[0] = unitVertices[i2];
            v2[1] = unitVertices[i2 + 1];
            v2[2] = unitVertices[i2 + 2];
            v3[0] = unitVertices[i1 + 3];
            v3[1] = unitVertices[i1 + 4];
            v3[2] = unitVertices[i1 + 5];
            v4[0] = unitVertices[i2 + 3];
            v4[1] = unitVertices[i2 + 4];
            v4[2] = unitVertices[i2 + 5];

            // compute face nornal
            computeFaceNormal(v1, v2, v3, n);

            // resize vertices by sphere radius
            scaleVertex(v1, radius);
            scaleVertex(v2, radius);
            scaleVertex(v3, radius);
            scaleVertex(v4, radius);

            // compute horizontal tex coords
            t1[0] = t2[0] = (float)j / (vertexCountPerRow - 1);
            t3[0] = t4[0] = (float)(j + 1) / (vertexCountPerRow - 1);

            // add 4 vertex attributes
            Vertex vertex0;
            vertex0.Position = glm::vec3(v1[0], v1[1], v1[2]);
            vertex0.Normal = glm::vec3(n[0], n[1], n[2]);
            vertex0.TexCoords = glm::vec2(t1[0], t1[1]);
            vertices.push_back(vertex0);

            Vertex vertex1;
            vertex1.Position = glm::vec3(v2[0], v2[1], v2[2]);
            vertex1.Normal = glm::vec3(n[0], n[1], n[2]);
            vertex1.TexCoords = glm::vec2(t2[0], t2[1]);
            vertices.push_back(vertex1);

            Vertex vertex2;
            vertex2.Position = glm::vec3(v3[0], v3[1], v3[2]);
            vertex2.Normal = glm::vec3(n[0], n[1], n[2]);
            vertex2.TexCoords = glm::vec2(t3[0], t3[1]);
            vertices.push_back(vertex2);

            Vertex vertex3;
            vertex3.Position = glm::vec3(v4[0], v4[1], v4[2]);
            vertex3.Normal = glm::vec3(n[0], n[1], n[2]);
            vertex3.TexCoords = glm::vec2(t4[0], t4[1]);
            vertices.push_back(vertex3);

            // add indices of 2 triangles
            indices.push_back(k1);
            indices.push_back(k + 1);
            indices.push_back(k + 2);

            indices.push_back(k + 2);
            indices.push_back(k + 1);
            indices.push_back(k + 3);

            k += 4;     // next
        }
    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face

    // build -X face by negating x and z values
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.x, vertices[i].Position.y, -vertices[i].Position.z);
        vertex.Normal = glm::vec3(-vertices[i].Normal.x, vertices[i].Normal.y, -vertices[i].Normal.z);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);

        //addVertex(-vertices[i], vertices[i + 1], -vertices[i + 2]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i], normals[i + 1], -normals[i + 2]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, vertices[i].Position.x, -vertices[i].Position.y);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, vertices[i].Normal.x, -vertices[i].Normal.y);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);

        //addVertex(-vertices[i + 2], vertices[i], -vertices[i + 1]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], normals[i], -normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }


    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, -vertices[i].Position.x, vertices[i].Position.y);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, -vertices[i].Normal.x, vertices[i].Normal.y);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);

        //addVertex(-vertices[i + 2], -vertices[i], vertices[i + 1]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], -normals[i], normals[i + 1]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, vertices[i].Position.y, vertices[i].Position.x);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, vertices[i].Normal.y, vertices[i].Normal.x);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);

        //addVertex(-vertices[i + 2], vertices[i + 1], vertices[i]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(-normals[i + 2], normals[i + 1], normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(vertices[i].Position.z, vertices[i].Position.y, -vertices[i].Position.x);
        vertex.Normal = glm::vec3(vertices[i].Normal.z, vertices[i].Normal.y, -vertices[i].Normal.x);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);

        //addVertex(vertices[i + 2], vertices[i + 1], -vertices[i]);
        //addTexCoord(texCoords[j], texCoords[j + 1]);
        //addNormal(normals[i + 2], normals[i + 1], -normals[i]);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    return Geometry(vertices, indices);
};

Geometry buildVerticesSmooth(float radius, int sub, int vertexCountPerRow)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    // generate unit-length verties in +X face
    std::vector<float> unitVertices = getUnitPositiveX(vertexCountPerRow);

    float x, y, z, s, t;
    int k = 0, k1, k2;

    // build +X face
    for (unsigned int i = 0; i < vertexCountPerRow; ++i)
    {
        k1 = i * vertexCountPerRow;     // index for curr row
        k2 = k1 + vertexCountPerRow;    // index for next row
        t = (float)i / (vertexCountPerRow - 1);

        for (unsigned int j = 0; j < vertexCountPerRow; ++j, k += 3, ++k1, ++k2)
        {
            x = unitVertices[k];
            y = unitVertices[k + 1];
            z = unitVertices[k + 2];
            s = (float)j / (vertexCountPerRow - 1);
            Vertex vertex;
            vertex.Position = glm::vec3(x*radius, y*radius, z*radius);
            vertex.Normal = glm::vec3(x, y, z);
            vertex.TexCoords = glm::vec2(s, t);
            vertices.push_back(vertex);

            // add indices
            if (i < (vertexCountPerRow - 1) && j < (vertexCountPerRow - 1))
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    // array size and index for building next face
    unsigned int startIndex;                    // starting index for next face
    int vertexSize = (int)vertices.size();      // vertex array size of +X face
    int indexSize = (int)indices.size();        // index array size of +X face

    // build -X face by negating x and z
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.x, vertices[i].Position.y, -vertices[i].Position.z);
        vertex.Normal = glm::vec3(-vertices[i].Normal.x, vertices[i].Normal.y, -vertices[i].Normal.z);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Y face by swapping x=>y, y=>-z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, vertices[i].Position.x, -vertices[i].Position.y);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, vertices[i].Normal.x, -vertices[i].Normal.y);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Y face by swapping x=>-y, y=>z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, -vertices[i].Position.x, vertices[i].Position.y);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, -vertices[i].Normal.x, vertices[i].Normal.y);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build +Z face by swapping x=>z, z=>-x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(-vertices[i].Position.z, vertices[i].Position.y, vertices[i].Position.x);
        vertex.Normal = glm::vec3(-vertices[i].Normal.z, vertices[i].Normal.y, vertices[i].Normal.x);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    // build -Z face by swapping x=>-z, z=>x
    startIndex = vertices.size();
    for (int i = 0, j = 0; i < vertexSize; ++i)
    {
        Vertex vertex;
        vertex.Position = glm::vec3(vertices[i].Position.z, vertices[i].Position.y, -vertices[i].Position.x);
        vertex.Normal = glm::vec3(vertices[i].Normal.z, vertices[i].Normal.y, -vertices[i].Normal.x);
        vertex.TexCoords = glm::vec2(vertices[i].TexCoords.x, vertices[i].TexCoords.y);
        vertices.push_back(vertex);
    }
    for (int i = 0; i < indexSize; ++i)
    {
        indices.push_back(startIndex + indices[i]);
    }

    return Geometry(vertices, indices);
}

float computeScaleForLength(const float v[3], float length)
{
    // and normalize the vector then re-scale to new radius
    return length / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void scaleVertex(float v[3], float scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

void computeFaceNormal(const float v1[3], const float v2[3], const float v3[3], float n[3])
{
    const float EPSILON = 0.000001f;

    // default return value (0, 0, 0)
    n[0] = n[1] = n[2] = 0;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = v2[0] - v1[0];
    float ey1 = v2[1] - v1[1];
    float ez1 = v2[2] - v1[2];
    float ex2 = v3[0] - v1[0];
    float ey2 = v3[1] - v1[1];
    float ez2 = v3[2] - v1[2];

    // cross product: e1 x e2
    float nx, ny, nz;
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        n[0] = nx * lengthInv;
        n[1] = ny * lengthInv;
        n[2] = nz * lengthInv;
    }
}

std::vector<float> getUnitPositiveX(unsigned int pointsPerRow)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along y-axis
    float a2;           // latitudinal angle
    float scale;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis
    for (unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis
        for (unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            scale = computeScaleForLength(v, 1);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // DEBUG
            //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
            //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
        }
    }

    return vertices;
}
