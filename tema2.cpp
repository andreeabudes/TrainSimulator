#include "lab_m1/tema2/tema2.h"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace m1;


Tema2::Tema2() {}

Tema2::~Tema2() {
    delete camera;
}

static void BuildCylinder(float radius, float height, int segments,
    std::vector<VertexFormat>& outVertices,
    std::vector<unsigned int>& outIndices)
{
    outVertices.clear();
    outIndices.clear();

    if (segments < 3) segments = 3;
    float halfH = height * 0.5f;
    const float twoPi = 2.0f * 3.14;

    glm::vec3 lightPink(0.95f, 0.70f, 0.80f);

    // lateral (partea tubulara)
    unsigned int sideStart = (unsigned int)outVertices.size();

    for (int i = 0; i <= segments; i++) {
        // unghi de constructie
        float t = (float)i / (float)segments;
        float ang = t * twoPi;

        // coordonatele de pe marginea cercului
        float x = radius * cos(ang);
        float z = radius * sin(ang);

        glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));

        // pt fiecare pas pe cerc creez 2 puncte: unul jos si unul sus
        outVertices.emplace_back(glm::vec3(x, -halfH, z), normal, lightPink);
        outVertices.emplace_back(glm::vec3(x, halfH, z), normal, lightPink);
    }

    for (int i = 0; i < segments; i++) {
        // fac drepunghiul din 2 triunghuri
        unsigned int i0 = sideStart + (unsigned int)(2 * i);   // stanga jos
        unsigned int i1 = i0 + 1;                              // stanga sus
        unsigned int i2 = i0 + 2;                              // dreapta jos
        unsigned int i3 = i0 + 3;                              // dreapta sus

        // primul triunghi
        outIndices.push_back(i0); 
        outIndices.push_back(i1); 
        outIndices.push_back(i2);
        // al doilea
        outIndices.push_back(i2); 
        outIndices.push_back(i1); 
        outIndices.push_back(i3);
    }

    // partea de sus
    unsigned int topCenter = (unsigned int)outVertices.size();
    outVertices.emplace_back(glm::vec3(0, halfH, 0), glm::vec3(0, 1, 0), lightPink);

    unsigned int topRingStart = (unsigned int)outVertices.size();
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float ang = t * twoPi;
        float x = radius * std::cos(ang);
        float z = radius * std::sin(ang);
       
        outVertices.emplace_back(glm::vec3(x, halfH, z), glm::vec3(0, 1, 0), lightPink);
    }

    for (int i = 0; i < segments; i++) {
        unsigned int v1 = topRingStart + (unsigned int)i;
        unsigned int v2 = topRingStart + (unsigned int)(i + 1);

        //conectez centru cu puncte de pe margine
        outIndices.push_back(topCenter);
        outIndices.push_back(v1);
        outIndices.push_back(v2);
    }

    // partea de jos
    unsigned int bottomCenter = (unsigned int)outVertices.size();
    outVertices.emplace_back(glm::vec3(0, -halfH, 0), glm::vec3(0, -1, 0), lightPink);

    unsigned int bottomRingStart = (unsigned int)outVertices.size();
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float ang = t * twoPi;
        float x = radius * cos(ang);
        float z = radius * sin(ang);

        outVertices.emplace_back(glm::vec3(x, -halfH, z), glm::vec3(0, -1, 0), lightPink);
    }

    // pentru jos, inversez v1/v2 ca sa fie fata orientata corect
    for (int i = 0; i < segments; i++) {
        unsigned int v1 = bottomRingStart + (unsigned int)i;
        unsigned int v2 = bottomRingStart + (unsigned int)(i + 1);

        outIndices.push_back(bottomCenter);
        outIndices.push_back(v2);
        outIndices.push_back(v1);
    }
}

// aceeasi functie doar ca am schimbat culoarea
static void BuildPinkCylinder(float radius, float height, int segments,
    std::vector<VertexFormat>& outVertices, std::vector<unsigned int>& outIndices) {
    outVertices.clear();
    outIndices.clear();

    if (segments < 3) segments = 3;
    float halfH = height * 0.5f;
    const float twoPi = 2.0f * 3.14f;

    glm::vec3 pinkColor(1.0f, 0.08f, 0.58f);
    unsigned int sideStart = (unsigned int)outVertices.size();

    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float ang = t * twoPi;
        float x = radius * cos(ang);
        float z = radius * sin(ang);
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));

        outVertices.emplace_back(glm::vec3(x, -halfH, z), normal, pinkColor);
        outVertices.emplace_back(glm::vec3(x, halfH, z), normal, pinkColor);
    }

    for (int i = 0; i < segments; i++) {
        unsigned int i0 = sideStart + (unsigned int)(2 * i);
        unsigned int i1 = i0 + 1;
        unsigned int i2 = i0 + 2;
        unsigned int i3 = i0 + 3;
        outIndices.push_back(i0); outIndices.push_back(i1); outIndices.push_back(i2);
        outIndices.push_back(i2); outIndices.push_back(i1); outIndices.push_back(i3);
    }
    unsigned int topCenter = (unsigned int)outVertices.size();
    outVertices.emplace_back(glm::vec3(0, halfH, 0), glm::vec3(0, 1, 0), pinkColor);

    unsigned int topRingStart = (unsigned int)outVertices.size();
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float ang = t * twoPi;
        float x = radius * std::cos(ang);
        float z = radius * std::sin(ang);
        outVertices.emplace_back(glm::vec3(x, halfH, z), glm::vec3(0, 1, 0), pinkColor);
    }

    for (int i = 0; i < segments; i++) {
        unsigned int v1 = topRingStart + (unsigned int)i;
        unsigned int v2 = topRingStart + (unsigned int)(i + 1);
        outIndices.push_back(topCenter);
        outIndices.push_back(v1);
        outIndices.push_back(v2);
    }

    unsigned int bottomCenter = (unsigned int)outVertices.size();
    outVertices.emplace_back(glm::vec3(0, -halfH, 0), glm::vec3(0, -1, 0), pinkColor);

    unsigned int bottomRingStart = (unsigned int)outVertices.size();
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / (float)segments;
        float ang = t * twoPi;
        float x = radius * cos(ang);
        float z = radius * sin(ang);
        outVertices.emplace_back(glm::vec3(x, -halfH, z), glm::vec3(0, -1, 0), pinkColor);
    }

    for (int i = 0; i < segments; i++) {
        unsigned int v1 = bottomRingStart + (unsigned int)i;
        unsigned int v2 = bottomRingStart + (unsigned int)(i + 1);
        outIndices.push_back(bottomCenter);
        outIndices.push_back(v2);
        outIndices.push_back(v1);
    }
}

Mesh* Tema2::CreateColoredCube(const std::string& name, const glm::vec3& color) {
    std::vector<VertexFormat> vertices = {
        // cubul are 8 colturi
        VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 0, 1), color),
        VertexFormat(glm::vec3(1, -1,  1), glm::vec3(0, 0, 1), color),
        VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(0, 0, 1), color),
        VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 0, 1), color),

        VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(0, 0,-1), color),
        VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 0,-1), color),
        VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(0, 0,-1), color),
        VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0,-1), color),
    };
    // fiecare fata a cubului e formata din 2 triunghiuri
    std::vector<unsigned int> indices = {
        0, 1, 2,   1, 3, 2,   // fata
        2, 3, 7,   2, 7, 6,   // sus
        1, 7, 3,   1, 5, 7,   // dreapta
        6, 7, 4,   7, 5, 4,   // spate
        0, 4, 1,   1, 4, 5,   // jos
        0, 2, 4,   2, 6, 4    // stanga
    };

    return CreateMesh(name.c_str(), vertices, indices);
}


Mesh* Tema2::CreateColoredPyramid(const std::string& name, const glm::vec3& color) {
    std::vector<VertexFormat> vertices = {
        // baza - patrat (normala in jos)
        VertexFormat(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0, -1, 0), color),
        VertexFormat(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0, -1, 0), color),
        VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0, -1, 0), color),
        VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0, -1, 0), color), 

        // restul fetelor sunt triunghuri
        // fata (normala inclinata spre fata-sus)
        VertexFormat(glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(0, 0.7f, 0.7f), color), 
        VertexFormat(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0, 0.7f, 0.7f), color), 
        VertexFormat(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0, 0.7f, 0.7f), color), 

        // dreapta (normala inclinata spre dreapta-sus)
        VertexFormat(glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(0.7f, 0.7f, 0), color), 
        VertexFormat(glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0.7f, 0.7f, 0), color), 
        VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.7f, 0.7f, 0), color), 

        // spate (normala inclinata spre spate-sus)
        VertexFormat(glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(0, 0.7f, -0.7f), color),
        VertexFormat(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0, 0.7f, -0.7f), color), 
        VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0, 0.7f, -0.7f), color), 

        // stanga (normala inclinata spre stanga-sus)
        VertexFormat(glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(-0.7f, 0.7f, 0), color), 
        VertexFormat(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.7f, 0.7f, 0), color), 
        VertexFormat(glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.7f, 0.7f, 0), color),
    };

    std::vector<unsigned int> indices = {
        0, 2, 1,  0, 3, 2, // baza (2 triunghiuri)
        4, 5, 6,           // fata
        7, 8, 9,           // dreapta
        10, 11, 12,        // spate
        13, 14, 15         // stanga
    };

    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* Tema2::CreateColoredSphere(const std::string& name, const glm::vec3& color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    const float radius = 0.5f;
    // linii orizontale
    const int stacks = 20;    
    // linii verticale
    const int sectors = 20;    
    const float PI = 3.1415926f;

    // o iau de sus in jos
    for (int i = 0; i <= stacks; ++i) {
        // calculez cat de jos sunt pe sfera
        float V = (float)i / (float)stacks;
        float phi = V * PI;
        // merg de jur imprejur 
        for (int j = 0; j <= sectors; ++j) {
            float U = (float)j / (float)sectors;
            // cat m-am rotit in jurul axei
            float theta = U * 2.0f * PI;
            // coordonate sferice
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            glm::vec3 pos = glm::vec3(x, y, z) * radius;
            glm::vec3 normal = glm::vec3(x, y, z);

            vertices.emplace_back(pos, normal, color);
        }
    }

    // triunghiurile (indices)
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int first = (i * (sectors + 1)) + j;
            int second = first + sectors + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return CreateMesh(name.c_str(), vertices, indices);
}

void Tema2::Init() {
    renderCameraTarget = false;
    camera = new implemented::MyCamera();
    camera->Set(glm::vec3(10, 10, 20), glm::vec3(10, 0, 8), glm::vec3(0, 1, 0));
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    leftOrtho = -5.0f;
    rightOrtho = 5.0f;
    bottomOrtho = -5.0f;
    topOrtho = 5.0f;
    zNear = 0.01f;
    zFar = 200.0f;
    persp = true;
    fov = 60;
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, zNear, zFar);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);

    CreateColoredCube("cube_green", glm::vec3(0.6f, 0.9f, 0.6f));   // iarba
    CreateColoredCube("cube_blue", glm::vec3(0.2f, 0.6f, 1.0f));    // apa
    CreateColoredCube("cube_brown", glm::vec3(0.5f, 0.4f, 0.3f));   // munte
    CreateColoredCube("cube_black", glm::vec3(0.1f, 0.1f, 0.1f));   // sine
    CreateColoredCube("cube_gray", glm::vec3(0.6f, 0.6f, 0.6f));    // pod
    CreateColoredCube("cube_orange", glm::vec3(1.0f, 0.6f, 0.0f));  // gara

    CreateColoredCube("cube_darkPink", { 0.85f, 0.35f, 0.55f });
    CreateColoredCube("cube_lightPink", { 0.95f, 0.70f, 0.80f });
    CreateColoredCube("cube_hotPink", { 1.0f, 0.08f, 0.58f });
    CreateColoredCube("cube_babyBlue", { 0.55f, 0.75f, 0.90f });
    CreateColoredCube("cube_darkGray", { 0.25f, 0.25f, 0.25f });
    CreateColoredCube("cube_greenDark", glm::vec3(0.039f, 0.388f, 0.118f));

    CreateColoredPyramid("pyramid_blue", glm::vec3(0.188f, 0.216f, 0.922f));
    CreateColoredSphere("sphere_purple", glm::vec3(0.608f, 0.373f, 0.839f));
    CreateColoredPyramid("pyramid_red", glm::vec3(0.8f, 0.1f, 0.1f));

    // rotile roz
    {
        std::vector<VertexFormat> v;
        std::vector<unsigned int> i;
        BuildPinkCylinder(0.5f, 1.0f, 24, v, i);
        CreateMesh("cylinder_pink", v, i);
    }
    // motor
    {
        std::vector<VertexFormat> v;
        std::vector<unsigned int> i;
        BuildCylinder(0.5f, 1.0f, 24, v, i);
        CreateMesh("cylinder", v, i);
    }

    // teren
    terrain.clear();

    // munte
    terrain.push_back({ glm::vec3(5.0f, 0.0f, 5.0f), glm::vec2(8.0f, 8.0f), TerrainType::Mountain });

    // apa - in forma de L: vertical dreapta
    terrain.push_back({ glm::vec3(17.0f, 0.0f, 12.5f), glm::vec2(2.0f, 11.0f), TerrainType::Water });
    // orizontal sus
    terrain.push_back({ glm::vec3(14.5f, 0.0f, 6.0f), glm::vec2(11.0f, 2.0f), TerrainType::Water });

    // fundal iarba
    terrain.push_back({ glm::vec3(0.0f, -0.1f, 0.0f), glm::vec2(40.0f, 40.0f), TerrainType::Field });

    BuildRailRoad();
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices) {
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // pos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // texcoord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // color
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
        (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    glBindVertexArray(0);
    CheckOpenGLError();

    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
    Shader* shader = shaders["Simple"];
   // construiesc terenul
    for (auto& t : terrain) {
        glm::mat4 M = glm::mat4(1);
        float y = (t.type == TerrainType::Field) ? -0.05f : 0.0f;
        M = glm::translate(M, t.center + glm::vec3(0, y, 0));
        M = glm::scale(M, glm::vec3(t.size.x * 0.5f, 0.01f, t.size.y * 0.5f));

        if (t.type == TerrainType::Field)
            RenderMesh(meshes["cube_green"], shaders["VertexNormal"], M);
        else if (t.type == TerrainType::Water)
            RenderMesh(meshes["cube_blue"], shaders["VertexNormal"], M);
        else if (t.type == TerrainType::Mountain)
            RenderMesh(meshes["cube_brown"], shaders["VertexNormal"], M);
    }

    // logica trenului - mereu am segment curent + progresul pe segment
    train.progress += deltaTimeSeconds * train.speed;
    while (train.progress >= 1.f) {
        train.progress -= 1.f;

        // salvez sina curenta
        Rail* current = train.rail;
        glm::vec3 node = current->endPosition;
        Rail* nextRail = current->next;

        // intersectie
        if (current->isIntersection) {

            if (pendingTurn == TurnDir::A) {
                if (current->nextLeft) {
                    nextRail = current->nextLeft;
                }
            }
            else if (pendingTurn == TurnDir::D) {
                if (current->nextRight) {
                    nextRail = current->nextRight;  
                }
            }
            else if (pendingTurn == TurnDir::W) {
                if (current->nextUp) {
                    nextRail = current->nextUp;
                }
            }
            else if (pendingTurn == TurnDir::S) {
                if (current->nextDown) {
                    nextRail = current->nextDown;
                }
            }
        }
        if (nextRail) {
            train.rail = nextRail;
        }
    }
    // capetele bucatii de sina pe care sunt
    auto start = train.rail->startPosition;
    auto end = train.rail->endPosition;
    train.position = glm::mix(start, end, train.progress);
    glm::vec3 dir = glm::normalize(end - start);
    // unghiul de rotatie
    float unghi = atan2(dir.z, dir.x);

    glm::mat4 base = glm::mat4(1);
    base = glm::translate(base, train.position);
    base = glm::rotate(base, -unghi, glm::vec3(0, 1, 0));

    DrawLocomotive(base);

    // primul vagon
    glm::mat4 wagonBase = base;
    wagonBase = glm::translate(wagonBase, glm::vec3(-2.2f, 0, 0));
    DrawWagon(wagonBase);

    // al 2-lea vagon
    glm::mat4 wagon2Base = wagonBase;
    wagon2Base = glm::translate(wagon2Base, glm::vec3(-2.2f, 0, 0));
    DrawWagon(wagon2Base);

    // bdara dntre locomotiva si primul vagon
    glm::vec3 locoBack = glm::vec3(base * glm::vec4(-0.8f, 0.35f, 0, 1));       // Spate locomotiva
    glm::vec3 wagon1Front = glm::vec3(wagonBase * glm::vec4(0.9f, 0.35f, 0, 1));  // Fata vagon 1
    RenderMesh(meshes["cube_darkPink"], shaders["VertexNormal"], BarBetween(locoBack, wagon1Front));

    // bara dintre primul vagon si al doilea vagon
    glm::vec3 wagon1Back = glm::vec3(wagonBase * glm::vec4(-0.9f, 0.35f, 0, 1)); // Spate vagon 1
    glm::vec3 wagon2Front = glm::vec3(wagon2Base * glm::vec4(0.9f, 0.35f, 0, 1));  // Fata vagon 2
    RenderMesh(meshes["cube_darkPink"], shaders["VertexNormal"], BarBetween(wagon1Back, wagon2Front));

    // sine
    for (auto& rptr : railRoad) {
        DrawRails(rptr.get());
    }

    // gari
    // gara centrala
    {
        glm::vec3 stationPos = glm::vec3(-10.0f, 0.0f, 10.0f); // Coordonate aproximate pentru zona mov

        // cub
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, stationPos);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.75f, 0)); // Ridicam sa fie pe sol (scara Y=0.75 => inaltime 1.5, deci ridicam jumate)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 0.5f, 0.9f)); // Lungime 2.5, Inaltime 1.5 total, Latime 1.5
        RenderMesh(meshes["cube_orange"], shaders["VertexNormal"], modelMatrix);

        // piramida
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, stationPos);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.3f, 0)); // Il punem exact deasupra cubului (inaltime cub = 1.5)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.8f, 1.2f, 1.8f)); // Putin mai mare ca peretii (streasina)
        RenderMesh(meshes["pyramid_red"], shaders["VertexNormal"], modelMatrix);
    }
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1.9f, 0.35f, 10.8f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 1.2f));
        RenderMesh(meshes["cube_greenDark"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10.0f, -0.1f, 19.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f, 2.5f, 1.2f)); 
        RenderMesh(meshes["pyramid_blue"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(14.7f, 0.7f, 2.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f, 2.5f, 2.5f));
        RenderMesh(meshes["sphere_purple"], shaders["VertexNormal"], modelMatrix);
    }
}

void Tema2::FrameEnd() {
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) {
    if (!mesh || !shader || !shader->program) return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::DrawRails(Rail* rail) {
    // imi fac o matrice care sa reprezinte unde se afla segmentul de sina si cum e orientat
    glm::vec3 a = rail->startPosition;
    glm::vec3 b = rail->endPosition;
    int type = rail->type;

    glm::vec3 dir = b - a;
    float len = glm::length(dir);
    if (len < 0.01f) return;

    glm::vec3 forward = glm::normalize(dir);
    float angle = atan2(forward.z, forward.x);

    // matricea de baza plasata in centrul segmentului
    glm::mat4 baseM(1);
    baseM = glm::translate(baseM, (a + b) * 0.5f);
    baseM = glm::rotate(baseM, -angle, glm::vec3(0, 1, 0));

    float scaleLen = len / 2 + 0.1f;

    // tipuri de sine
    if (type == 0) {
        // pt campie am sina simpla neagra
        glm::mat4 M = baseM;
        M = glm::scale(M, glm::vec3(scaleLen, 0.1f, 0.5f));
        RenderMesh(meshes["cube_black"], shaders["VertexNormal"], M);
    }
    else if (type == 1) {
        // pod: o bucata neagra intre 2 bucati gri
        for (float sign : {-1.0f, 1.0f}) {
            glm::mat4 M = baseM;
            M = glm::translate(M, glm::vec3(0, 0.1f, sign * 0.4f));
            M = glm::scale(M, glm::vec3(scaleLen, 0.2f, 0.2f));
            RenderMesh(meshes["cube_gray"], shaders["VertexNormal"], M);
        }
        RenderMesh(meshes["cube_black"], shaders["VertexNormal"],
            glm::scale(baseM, glm::vec3(scaleLen, 0.1f, 0.6f)));
    }
    else if (type == 2) {
        // tunel: 4 paralelipipede
        float tieThickness = 0.3f;
        float spacing = 0.2f;
        float unit = tieThickness + spacing;
        int numSegments = (int)(len / unit);

        for (int i = 0; i < numSegments; i++) {
            // pozitia pe X pt fiecare segment
            float offset = -len / 2.0f + i * unit + unit / 2.0f;
            glm::mat4 M = baseM;
            M = glm::translate(M, glm::vec3(offset, 0.05f, 0));
            M = glm::scale(M, glm::vec3(tieThickness * 0.5f, 0.15f, 0.4f));

            // alternare culori
            if (i % 2 == 0) {
                RenderMesh(meshes["cube_darkGray"], shaders["VertexNormal"], M);
            }
            else {
                RenderMesh(meshes["cube_black"], shaders["VertexNormal"], M);
            }
        }
    }
}

void Tema2::DrawLocomotive(const glm::mat4& base) {
    // platforma
    {
        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(0, 0.4f, 0));
        M = glm::scale(M, glm::vec3(0.8f, 0.1f, 0.4f));
        RenderMesh(meshes["cube_darkPink"], shaders["VertexNormal"], M);
    }

    // cabina (paralelipiped vertical)
    {
        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(-0.5f, 0.8f, 0));
        M = glm::scale(M, glm::vec3(0.3f, 0.4f, 0.35f));
        RenderMesh(meshes["cube_hotPink"], shaders["VertexNormal"], M);
    }
    // motor
    const float motorCenterX = 0.15f;
    const float motorCenterY = 0.65f;
    const float motorLen = 0.90f;
    const float motorR = 0.50f;

    {
        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(motorCenterX, motorCenterY, 0.0f));
        M = glm::rotate(M, RADIANS(90.f), glm::vec3(0, 0, 1));   // pe X
        M = glm::scale(M, glm::vec3(motorR, motorLen, motorR));
        RenderMesh(meshes["cylinder"], shaders["VertexNormal"], M);
    }

    // cilindrul mic din fata motorului
    const float cLen = 0.25f;
    const float cRad = 0.18f;
    {
        const float motorFrontX = motorCenterX + motorLen * 0.5f;

        // centrul trebuie sa fie cu jumatate din lungimea lui in fata
        const float cilCenterX = motorFrontX + cLen * 0.5f;

        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(cilCenterX, motorCenterY, 0.0f));
        M = glm::rotate(M, RADIANS(90.f), glm::vec3(0, 0, 1));   // pe X
        M = glm::scale(M, glm::vec3(cRad, cLen, cRad));

        RenderMesh(meshes["cylinder_pink"], shaders["VertexNormal"], M);
    }


    // roti (mai multi cilindri)
    for (int i = -3; i <= 3; i += 2) {
        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(i * 0.25f, 0.2f, 0.45f));
        M = glm::rotate(M, RADIANS(90.f), glm::vec3(1, 0, 0)); // cilindru pe Z
        M = glm::scale(M, glm::vec3(0.25f, 0.25f, 0.25f));
        RenderMesh(meshes["cylinder_pink"], shaders["VertexNormal"], M);

        M = base;
        M = glm::translate(M, glm::vec3(i * 0.25f, 0.2f, -0.45f));
        M = glm::rotate(M, RADIANS(90.f), glm::vec3(1, 0, 0));
        M = glm::scale(M, glm::vec3(0.25f, 0.25f, 0.25f));
        RenderMesh(meshes["cylinder_pink"], shaders["VertexNormal"], M);
    }
}

void Tema2::DrawWagon(const glm::mat4& base)
{
    // corp vagon
    {
        glm::mat4 M = base;
        M = glm::translate(M, glm::vec3(0, 0.6f, 0));
        M = glm::scale(M, glm::vec3(0.8f, 0.35f, 0.4f));
        RenderMesh(meshes["cube_lightPink"], shaders["VertexNormal"], M);
    }

    // 4 roti
    const glm::vec3 wheelOffsets[4] = {
        {-0.7f, 0.25f,  0.5f},
        { 0.7f, 0.25f,  0.5f},
        {-0.7f, 0.25f, -0.5f},
        { 0.7f, 0.25f, -0.5f}
    };

    for (auto off : wheelOffsets) {
        glm::mat4 M = base;
        M = glm::translate(M, off);
        M = glm::rotate(M, RADIANS(90.f), glm::vec3(1, 0, 0)); // cilindru pe Z
        M = glm::scale(M, glm::vec3(0.25f));
        RenderMesh(meshes["cylinder_pink"], shaders["VertexNormal"], M);
    }
}

// fac o functie care sa vada ce tip de teren am
Tema2::TerrainType Tema2::GetTerrainAt(float x, float z) const {
    for (const auto& t : terrain) {
        float hx = t.size.x * 0.5f;
        float hz = t.size.y * 0.5f;
        // verific exact in ce zona ma aflu dupa coordonate
        if (x >= t.center.x - hx && x < t.center.x + hx && z >= t.center.z - hz && z < t.center.z + hz) {
            return t.type;
        }
    }
    return Tema2::TerrainType::Field;
}

// functie pt a pune tipul de sina corect pe un anumit teren
int Tema2::RailTypeFromTerrain(TerrainType tt) const {
    if (tt == TerrainType::Mountain) return 2; // tunel
    if (tt == TerrainType::Water)    return 1; // pod
    return 0;                                  // normal
}

void Tema2::BuildRailRoad() {
    railRoad.clear();

    // aici am coordonatele pt primul set de sine
    const float xL = 3.0f;
    const float xR = 13.0f;
    const float zBottom = 2.0f;
    const float zTop = 18.0f;

    // al doilea set de sine in stanga si in sus
    float xOutL = -8.0f;
    float zOutT = -12.0f;

    const float xMountainStart = 1.0f;
    const float zMountainStart = 1.0f;
    const float xMountainEdge = 9.0f;
    const float zMountainEnd = 9.0f;

    // rau orizontal
    const float zRiverLow = 5.0f;
    const float zRiverHigh = 7.0f;

    // fac segment uc segment
    auto addRail = [&](glm::vec3 a, glm::vec3 b, int type) -> Rail* {
        auto r = std::make_unique<Rail>();
        r->startPosition = a;
        r->endPosition = b;
        r->type = type;

        // pointer cu care le leg
        Rail* ptr = r.get();
        railRoad.push_back(std::move(r));
        return ptr;
        };

    // tunel stanga
    Rail* r1 = addRail({ xL, 0, zBottom }, { xL, 0, zMountainEnd }, 2);

    // iesire din tunel la stanga
    Rail* r2 = addRail({ xL, 0, zMountainEnd }, { xL, 0, zTop }, 0);

    // sus de la stanga la dreapta
    Rail* r3 = addRail({ xL, 0, zTop }, { xR, 0, zTop }, 0);

    // dreapta pana la rau 
    Rail* r4 = addRail({ xR, 0, zTop }, { xR, 0, zRiverHigh }, 0);

    // podul peste rau
    Rail* r5 = addRail({ xR, 0, zRiverHigh }, { xR, 0, zRiverLow }, 1);

    // cobor pe dreapta
    Rail* r6 = addRail({ xR, 0, zRiverLow }, { xR, 0, zBottom }, 0);

    // marginea muntelui
    Rail* r7_iarba = addRail({ xR, 0, zBottom }, { xMountainEdge, 0, zBottom }, 0);

    // merg prin munte spre stanga
    Rail* r7_tunel = addRail({ xMountainEdge, 0, zBottom }, { xL, 0, zBottom }, 2);

    // continuare sine stanga si sus
    Rail* r_ext_L1 = addRail({ xL, 0, zTop }, { xOutL, 0, zTop }, 0);
    Rail* r_ext_L2 = addRail({ xOutL, 0, zTop }, { xOutL, 0, zBottom }, 0);
    Rail* r_ext_L3_iarba = addRail({ xOutL, 0, zBottom }, { xMountainStart, 0, zBottom }, 0);
    Rail* r_ext_L3_tunel = addRail({ xMountainStart, 0, zBottom }, { xL, 0, zBottom }, 2);

    Rail* r_ext_T1 = addRail({ xR, 0, zTop }, { xR, 0, zOutT }, 0);
    Rail* r_ext_T2 = addRail({ xR, 0, zOutT }, { xL, 0, zOutT }, 0);
    Rail* r_ext_T3_iarba = addRail({ xL, 0, zOutT }, { xL, 0, zMountainStart }, 0);
    Rail* r_ext_T3_tunel = addRail({ xL, 0, zMountainStart }, { xL, 0, zBottom }, 2);
    Rail* r_ext_BR1 = addRail({ xR, 0, zBottom }, { xR, 0, zOutT }, 0);

    // INTERSECTII
    
    // incercare pt intersectia din munte
    // din dreapta
    Rail* r_hub_W = addRail({ 3, 0, 2 }, { 1, 0, 2 }, 0);
    Rail* r_hub_A = addRail({ 3, 0, 2 }, { 3, 0, 1 }, 0);
    r_hub_W->next = r_ext_L2;
    r_hub_A->next = r_ext_T2;
    r7_tunel->isIntersection = false;
    r7_tunel->next = r_hub_W;       // W (inainte) 
    r7_tunel->nextLeft = r_hub_A;   // A (stanga)
    r7_tunel->next = r1;  // D (dreapta) 

    // venind din stang
    r_ext_L3_tunel->isIntersection = true;
    r_ext_L3_tunel->next = r1;
    r_ext_L3_tunel->nextLeft = r1;
    r_ext_L3_tunel->nextRight = r1;

    // venind de sus 
    r_ext_T3_tunel->isIntersection = true;
    r_ext_T3_tunel->next = r1;
    r_ext_T3_tunel->nextLeft = r1;
    r_ext_T3_tunel->nextRight = r1;

    r1->isIntersection = false;
    r1->next = r2;

    // intersectia din stanga sus
    r2->isIntersection = true;
    r2->next = r3;           // W (Default)
    r2->nextLeft = r_ext_L1; // A (Stanga)
    r2->nextRight = r3;      // D (Dreapta)

    // interesctia de dreapta sus
    r3->isIntersection = true;
    r3->next = r4;        
    r3->nextLeft = r_ext_T1;  
    r3->nextRight = r_ext_T1; 

    r_ext_BR1->next = r_ext_T2;

    r6->isIntersection = true;
    r6->next = r7_iarba;       
    r6->nextLeft = r7_iarba;   
    r6->nextRight = r_ext_BR1;

    // LEGATURI
    r1->next = r2;
    r4->next = r5;
    r5->next = r6;
    r6->next = r7_iarba;
    r7_iarba->next = r7_tunel;
    r7_tunel->next = r1;

    r_ext_L1->next = r_ext_L2;
    r_ext_L2->next = r_ext_L3_iarba;
    r_ext_L3_iarba->next = r_ext_L3_tunel;

    r_ext_T1->next = r_ext_T2;
    r_ext_T2->next = r_ext_T3_iarba;
    r_ext_T3_iarba->next = r_ext_T3_tunel;

    // Tren
    train.rail = r1;
    train.position = r1->startPosition;
    train.progress = 0.f;
    train.speed = 0.3f;
}

// constructia barei dintre vagoane
glm::mat4 Tema2::BarBetween(const glm::vec3& a, const glm::vec3& b, float thicknessY, float thicknessZ) {
    glm::vec3 d = b - a;
    float len = glm::length(d);
    glm::vec3 dir = d / len;
    float rot_angle = atan2(dir.z, dir.x); // rotatie pe OY 

    glm::mat4 M(1);
    M = glm::translate(M, (a + b) * 0.5f);
    M = glm::rotate(M, -rot_angle, glm::vec3(0, 1, 0));

    M = glm::scale(M, glm::vec3(len + 0.4f, thicknessY, thicknessZ));
    return M;
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->MoveForward(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->MoveForward(-cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }

        float fovSpeed = 20 * deltaTime;
        if (window->KeyHold(GLFW_KEY_L)) {
            fov -= fovSpeed;
            if (fov < 10.0f) fov = 10.0f;
            projectionMatrix = glm::perspective(glm::radians(fov), window->props.aspectRatio, 0.01f, 200.0f);

        }
        if (window->KeyHold(GLFW_KEY_M)) {
            fov += fovSpeed;
            if (fov < 10.0f) fov = 10.0f;
            projectionMatrix = glm::perspective(glm::radians(fov), window->props.aspectRatio, 0.01f, 200.0f);
        }
    }
}

void Tema2::OnKeyPress(int key, int mods) {
    if (key == GLFW_KEY_A) {
        pendingTurn = TurnDir::A;
    }
    if (key == GLFW_KEY_D) {
        pendingTurn = TurnDir::D;
    }
    if (key == GLFW_KEY_W) {
        pendingTurn = TurnDir::W;
    }

    if (key == GLFW_KEY_T) {
        renderCameraTarget = !renderCameraTarget;
    }
    if (key == GLFW_KEY_O) {
        persp = false;
        projectionMatrix = glm::ortho(leftOrtho, rightOrtho, bottomOrtho, topOrtho, zNear, zFar);
    }

    if (key == GLFW_KEY_P) {
        persp = true;
        projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
    }
}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}
