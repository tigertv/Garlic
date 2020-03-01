#include "Tunic/Rendering/Renderables/Model.hpp"

#include "Tunic/Rendering/Renderables/Mesh.hpp"

namespace tnc::rnd{
	Model::Model(std::vector<std::shared_ptr<Mesh>> meshes)
		: meshes(std::move(meshes)){
	}

	Model::Model(const Model& other) = default;

	Model::Model(Model&& other) = default;

	Model& Model::operator=(const Model& other) = default;

	Model& Model::operator=(Model&& other) = default;

	Model::~Model() = default;
}