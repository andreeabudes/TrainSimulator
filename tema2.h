#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"
#include <vector>
#include <memory>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();
        void Init() override;

    protected:
        struct Rail {
            glm::vec3 startPosition;
            glm::vec3 endPosition;
            Rail* next = nullptr;
            std::vector<Rail*> children;
            int type;

            // pt intersectii
            Rail* nextLeft = nullptr;
            Rail* nextRight = nullptr;
            Rail* nextUp = nullptr;
            Rail* nextDown = nullptr;
            bool isIntersection = false;
        };

        struct Train {
            Rail* rail = nullptr;
            float progress = 0.f;
            float speed = 0.3f;
            glm::vec3 position;
        };

        enum class TerrainType {
            Field, 
            Water,
            Mountain
        };

        struct TerrainTile {
            glm::vec3 center;
            glm::vec2 size;
            TerrainType type;
        };

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

      
        void BuildRailRoad();
        void DrawRails(Rail* rail);
        void DrawLocomotive(const glm::mat4& base);
        void DrawWagon(const glm::mat4& base);
        static glm::mat4 BarBetween(const glm::vec3& a, const glm::vec3& b, float thicknessY = 0.08f, float thicknessZ = 0.08f);
        Mesh* CreateColoredCube(const std::string& name, const glm::vec3& color);

        Mesh* CreateColoredPyramid(const std::string& name, const glm::vec3& color);
        Mesh* CreateColoredSphere(const std::string& name, const glm::vec3& color);
        TerrainType GetTerrainAt(float x, float z) const;
        int RailTypeFromTerrain(TerrainType tt) const;

    protected:
        implemented::MyCamera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        bool persp;
        float leftOrtho;
        float rightOrtho;
        float bottomOrtho;
        float topOrtho;

        float zNear;
        float zFar;
        float fov;
        float aspect;

        // culori 
        glm::vec3 darkPink;
        glm::vec3 lightPink;
        glm::vec3 babyBlue;
        glm::vec3 hotPink;

        std::vector<std::unique_ptr<Rail>> railRoad;
        Train train;
        std::vector<TerrainTile> terrain;

        // tot pt intersectii
        enum class TurnDir { None, W, A, S, D };
        TurnDir pendingTurn = TurnDir::None;


    };
}   // namespace m1
