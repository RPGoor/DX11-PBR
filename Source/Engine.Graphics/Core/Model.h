#include "../DX11/Graphics.h"
#include <ConditionalNoexcept.h>
#include "Mesh.h"
#include "Node.h"
#include <memory>
#include <assimp/scene.h>

class Model
{
public:
    Model(Graphics& gfx, const std::string fileName);
    void Draw(Graphics& gfx) const conexcept;
    void ShowWindow(const char* windowName = nullptr) noexcept;
    ~Model() noexcept;
private:
    static std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterials);
    std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node) noexcept;
private:
    std::unique_ptr<Node> pRoot;
    std::vector<std::unique_ptr<Mesh>> meshPtrs;
    std::unique_ptr<class ModelWindow> pWindow;
};